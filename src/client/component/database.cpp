#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "filesystem.hpp"
#include "console.hpp"
#include "command.hpp"

#include "game/dvars.hpp"
#include "game/game.hpp"

#include <utils/io.hpp>
#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace database
{
	namespace
	{
		struct bnet_file_handle_t
		{
			std::unique_ptr<std::ifstream> stream;
			uint64_t offset{};
		};

		std::unordered_map<game::DB_IFileSysFile*, bnet_file_handle_t> bnet_file_handles{};
		utils::hook::detour bnet_fs_open_file_hook;
		utils::hook::detour bnet_fs_read_hook;
		utils::hook::detour bnet_fs_tell_hook;
		utils::hook::detour bnet_fs_size_hook;
		utils::hook::detour bnet_fs_close_hook;
		utils::hook::detour bnet_fs_exists_hook;
		utils::hook::detour bink_io_read_hook;
		utils::hook::detour bink_io_seek_hook;

		utils::hook::detour open_sound_handle_hook;

		utils::memory::allocator handle_allocator;

		using sound_file_t = std::unordered_map<uint64_t, std::string>;
		std::unordered_map<unsigned short, sound_file_t> sound_files = {};
		std::unordered_map<std::string, uint64_t> sound_sizes = {};

		game::dvar_t* db_filesysImpl = nullptr;
		utils::hook::detour db_fs_initialize_hook;

		game::DB_FileSysInterface* db_fs_initialize_stub()
		{
			switch (db_filesysImpl->current.integer)
			{
			case 0:
				return reinterpret_cast<game::DB_FileSysInterface*>(0x140BE82F8); // ptr to vtable of BnetTACTVFSManager (implements DB_FileSysInterface)
			case 1:
				return reinterpret_cast<game::DB_FileSysInterface*>(0x140BEFDC0); // ptr to vtable of DiskFS (implements DB_FileSysInterface)
			default:
				return nullptr; // this should not happen
			}
		}

		std::string get_sound_file_name(unsigned short file_index, uint64_t packed_file_offset)
		{
			if (sound_files.find(file_index) != sound_files.end() &&
				sound_files[file_index].find(packed_file_offset) != sound_files[file_index].end())
			{
				return sound_files[file_index][packed_file_offset];
			}

			return utils::string::va("soundfile_%i_%llX.flac", file_index, packed_file_offset);
		}

		std::string get_sound_file_name(game::StreamedSound* sound)
		{
			const auto file_index = sound->filename.fileIndex;
			const auto packed_file_offset = sound->filename.info.packed.offset;
			return get_sound_file_name(file_index, packed_file_offset);
		}

		void* disk_fs_sound_file_open_stub(game::StreamFile* file, game::StreamedSound* sound)
		{
			const auto name = "sound/" + get_sound_file_name(sound);
			std::string path{};

			if (!filesystem::find_file(name, &path))
			{
				return open_sound_handle_hook.invoke<void*>(file, sound);
			}

			const auto disk_fs = reinterpret_cast<game::DB_FileSysInterface*>(0x140BEFDC0);
			const auto handle = disk_fs->vftbl->OpenFile(disk_fs, game::SF_ZONE_REGION, path.data());
			if (handle)
			{
				const auto size = disk_fs->vftbl->Size(disk_fs, handle);
				file->isPacked = 0;
				file->length = size;
				file->handle = handle;
			}
			else
			{
				file->handle = nullptr;
				file->length = 0;
			}

			file->startOffset = 0;
			return 0;
		}

		thread_local game::StreamedSound current_sound{};

		void* sound_file_open_stub(game::StreamFile* file, game::StreamedSound* sound)
		{
			if (db_filesysImpl->current.integer == 1)
			{
				return disk_fs_sound_file_open_stub(file, sound);
			}
			else
			{
				current_sound = *sound;
				return open_sound_handle_hook.invoke<void*>(file, sound);
			}
		}

		game::DB_IFileSysFile* bnet_fs_open_file_stub(game::DB_FileSysInterface* this_, int folder, const char* file)
		{
			const auto _0 = gsl::finally([]
			{
				current_sound = {};
			});

			std::string name = file;

			bool is_sound = false;
			if (name.starts_with("soundfile") && name.ends_with(".pak"))
			{
				is_sound = true;
				name = "sound/" + get_sound_file_name(&current_sound);
			}

			if (name.ends_with(".bik"))
			{
				name = "videos/" + name;
			}

			std::string path{};
			if (!filesystem::find_file(name, &path))
			{
				return bnet_fs_open_file_hook.invoke<game::DB_IFileSysFile*>(this_, folder, file);
			}

			const auto handle = handle_allocator.allocate<game::DB_IFileSysFile>();
			std::memset(handle, 0, sizeof(handle));

			try
			{
#ifdef DEBUG
				console::info("[Database] Opening file %s\n", path.data());
#endif

				auto stream = std::make_unique<std::ifstream>();
				stream->open(path, std::ios::binary);

				bnet_file_handle_t bnet_handle{};
				bnet_handle.stream = std::move(stream);
				bnet_handle.offset = current_sound.filename.info.packed.offset;

				bnet_file_handles[handle] = std::move(bnet_handle);
				return handle;
			}
			catch (const std::exception& e)
			{
				console::error("[Database] Error opening file %s: %s\n", path.data(), e.what());
			}

			return handle;
		}

		game::FileSysResult bnet_fs_read_stub(game::DB_FileSysInterface* this_, game::DB_IFileSysFile* handle,
			unsigned __int64 offset, unsigned __int64 size, void* dest)
		{
			if (bnet_file_handles.find(handle) == bnet_file_handles.end())
			{
				return bnet_fs_read_hook.invoke<game::FileSysResult>(this_, handle, offset, size, dest);
			}
			else
			{
				auto& handle_ = bnet_file_handles[handle];
				if (!handle_.stream->is_open())
				{
					return game::FILESYSRESULT_ERROR;
				}

				try
				{
					const auto start_pos = offset - handle_.offset;
					handle_.stream->seekg(0, std::ios::end);
					const auto end_pos = static_cast<uint64_t>(handle_.stream->tellg());
					handle_.stream->seekg(start_pos);

					const auto len = end_pos - start_pos;
					const auto bytes_to_read = len <= size ? len : size;

					const auto& res = handle_.stream->read(reinterpret_cast<char*>(dest), bytes_to_read);
					if (res.bad())
					{
						return game::FILESYSRESULT_ERROR;
					}

					const auto bytes_read = static_cast<uint64_t>(res.gcount());
					handle->bytes_read += bytes_read;
					handle->last_read = bytes_read;

					return game::FILESYSRESULT_SUCCESS;
				}
				catch (const std::exception& e)
				{
					console::error("[Database] bnet_fs_read_stub: %s\n", e.what());
					return game::FILESYSRESULT_ERROR;
				}
			}
		}

		game::FileSysResult bnet_fs_tell_stub(game::DB_FileSysInterface* this_, game::DB_IFileSysFile* handle, uint64_t* bytes_read)
		{
			if (bnet_file_handles.find(handle) == bnet_file_handles.end())
			{
				return bnet_fs_tell_hook.invoke<game::FileSysResult>(this_, handle, bytes_read);
			}
			else
			{
				auto& handle_ = bnet_file_handles[handle];
				if (!handle_.stream->is_open())
				{
					return game::FILESYSRESULT_ERROR;
				}

				try
				{
					*bytes_read = handle->last_read;
					return game::FILESYSRESULT_SUCCESS;
				}
				catch (const std::exception& e)
				{
					console::error("[Database] bnet_fs_tell_stub: %s\n", e.what());
					return game::FILESYSRESULT_ERROR;
				}
			}
		}

		uint64_t bnet_fs_size_stub(game::DB_FileSysInterface* this_, game::DB_IFileSysFile* handle)
		{
			if (bnet_file_handles.find(handle) == bnet_file_handles.end())
			{
				return bnet_fs_size_hook.invoke<uint64_t>(this_, handle);
			}
			else
			{
				auto& handle_ = bnet_file_handles[handle];
				try
				{
					handle_.stream->seekg(0, std::ios::end);
					const std::streamsize size = handle_.stream->tellg();
					handle_.stream->seekg(0, std::ios::beg);
					return static_cast<uint64_t>(size);
				}
				catch (const std::exception& e)
				{
					console::error("[Database] bnet_fs_size_stub: %s\n", e.what());
					return 0;
				}
			}
		}

		void bnet_fs_close_stub(game::DB_FileSysInterface* this_, game::DB_IFileSysFile* handle)
		{
			if (bnet_file_handles.find(handle) == bnet_file_handles.end())
			{
				bnet_fs_close_hook.invoke<uint64_t>(this_, handle);
			}
			else
			{
				handle_allocator.free(handle);
				bnet_file_handles.erase(handle);
			}
		}

		bool bnet_fs_exists_stub(game::DB_FileSysInterface* this_, game::DB_IFileSysFile* handle, const char* filename)
		{
			std::string path{};
			return filesystem::find_file(filename, &path) || bnet_fs_exists_hook.invoke<bool>(this_, handle, filename);
		}

		uint64_t bink_io_read_stub(game::DB_IFileSysFile** handle, void* dest, uint64_t bytes)
		{
			const auto handle_ptr = *handle;
			if (bnet_file_handles.find(handle_ptr) == bnet_file_handles.end())
			{
				return bink_io_read_hook.invoke<uint64_t>(handle, dest, bytes);
			}
			else
			{
				auto& handle_ = bnet_file_handles[handle_ptr];
				if (!handle_.stream->is_open())
				{
					return 0;
				}

				try
				{
					const auto& res = handle_.stream->read(reinterpret_cast<char*>(dest), bytes);
					return static_cast<uint64_t>(res.gcount());
				}
				catch (const std::exception& e)
				{
					console::error("[Database] bink_io_read_stub: %s\n", e.what());
					return 0;
				}
			}
		}

		bool bink_io_seek_stub(game::DB_IFileSysFile** handle, uint64_t pos)
		{
			const auto handle_ptr = *handle;
			if (bnet_file_handles.find(handle_ptr) == bnet_file_handles.end())
			{
				return bink_io_seek_hook.invoke<bool>(handle, pos);
			}
			else
			{
				auto& handle_ = bnet_file_handles[handle_ptr];
				if (!handle_.stream->is_open())
				{
					return false;
				}

				try
				{
					const auto& res = handle_.stream->seekg(pos);
					return !(res.fail() || res.bad());
				}
				catch (const std::exception& e)
				{
					console::error("[Database] bink_io_seek_stub: %s\n", e.what());
					return 0;
				}
			}
		}

		utils::hook::detour db_link_xasset_entry1_hook;
		game::XAssetEntry* db_link_xasset_entry1_stub(game::XAssetType type, game::XAssetHeader* header)
		{
			const auto result = db_link_xasset_entry1_hook.invoke<game::XAssetEntry*>(type, header);
			if (result->asset.type == game::ASSET_TYPE_SOUND)
			{
				const auto sound = result->asset.header.sound;
				for (auto i = 0; i < sound->count; i++)
				{
					const auto alias = &sound->head[i];
					if (alias->soundFile != nullptr && alias->soundFile->type == 2)
					{
						const auto file_index = alias->soundFile->u.streamSnd.filename.fileIndex;
						const auto length = alias->soundFile->u.streamSnd.filename.info.packed.length;
						const auto offset = alias->soundFile->u.streamSnd.filename.info.packed.offset;
						const auto name = utils::string::va("%s.flac", alias->aliasName);

						sound_files[file_index][offset] = name;
						sound_sizes[name] = length;
					}
				}
			}

			return result;
		}

		void dump_flac_sound(const std::string& sound_name, unsigned short file_index, uint64_t start_offset, uint64_t size)
		{
			const auto name = utils::string::va("soundfile%i.pak", file_index);
			const auto fs_interface = db_fs_initialize_stub();

			const auto handle = fs_interface->vftbl->OpenFile(fs_interface, game::Sys_Folder::SF_PAKFILE, name);
			if (handle == nullptr)
			{
				console::error("Sound file %s not found\n", name);
				return;
			}

			const auto buffer = utils::memory::get_allocator()->allocate_array<char>(size);
			const auto _0 = gsl::finally([&]()
			{
				utils::memory::get_allocator()->free(buffer);
			});

			const auto result = fs_interface->vftbl->Read(fs_interface, handle, start_offset, size, buffer);
			if (result != game::FILESYSRESULT_SUCCESS)
			{
				console::error("Error reading file %s\n", name);
			}

			const auto path = utils::string::va("dumps/sound/%s", sound_name.data());
			utils::io::write_file(path, std::string(buffer, size), false);
			console::info("Sound dumped to %s\n", path);
		}

		void dump_sound_file(unsigned short file_index)
		{
			const auto name = utils::string::va("soundfile%i.pak", file_index);
			const auto fs_interface = db_fs_initialize_stub();

			const auto handle = fs_interface->vftbl->OpenFile(fs_interface, game::Sys_Folder::SF_PAKFILE, name);
			if (handle == nullptr)
			{
				console::error("Sound file %s not found\n", name);
				return;
			}

			const auto size = fs_interface->vftbl->Size(fs_interface, handle);
			const auto buffer = utils::memory::get_allocator()->allocate_array<char>(size);
			const auto _0 = gsl::finally([&]()
			{
				utils::memory::get_allocator()->free(buffer);
			});

			const auto result = fs_interface->vftbl->Read(fs_interface, handle, 0, size, buffer);
			if (result != game::FILESYSRESULT_SUCCESS)
			{
				console::error("Error reading file %s\n", name);
			}

			std::vector<std::uint8_t> signature = {0x66, 0x4C, 0x61, 0x43, 0x00, 0x00, 0x00, 0x22};

			const auto check_signature = [&](char* start)
			{
				for (auto i = 0; i < signature.size(); i++)
				{
					if (start[i] != signature[i])
					{
						return false;
					}
				}

				return true;
			};

			const auto end = buffer + size - signature.size() - 1;
			for (auto pos = buffer; pos < end;)
			{
				const auto start_pos = pos;
				if (check_signature(start_pos))
				{
					++pos;

					while (pos < end && !check_signature(pos))
					{
						++pos;
					}

					const auto flac_size = static_cast<size_t>(pos - start_pos);
					std::string data{start_pos, flac_size};

					const auto progress = static_cast<int>(100 * (static_cast<float>(start_pos - buffer) / static_cast<float>(end - buffer)));
					const auto sound_name = get_sound_file_name(file_index, static_cast<uint64_t>(start_pos - buffer));
					const auto path = utils::string::va("dumps/sound/%s", sound_name.data());

					utils::io::write_file(path, data, false);
					console::info("Sound dumped: %s (%i%%)\n", path, progress);
				}
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			static const char* values[] = 
			{
				"BnetTACTVFSManager", // (load files from CASC)
				"DiskFS", // (load files from disk)
				nullptr
			};

			const auto default_value = static_cast<int>(!utils::io::directory_exists("Data/data")
				|| !utils::io::directory_exists("Data/config")
				|| !utils::io::directory_exists("Data/indices"));

			db_filesysImpl = dvars::register_enum("db_filesysImpl", values, default_value, game::DVAR_FLAG_READ, "Filesystem implementation");

			if (default_value == 1)
			{
				utils::hook::nop(0x1405A4868, 22); // TACT related stuff that's pointless if we're using DiskFS
				utils::hook::nop(0x14071AF83, 45); // Skip setting Bink file OS callbacks (not necessary since we're loading from disk)
			}
			
			db_fs_initialize_hook.create(game::DB_FSInitialize, db_fs_initialize_stub);

			open_sound_handle_hook.create(0x1406233B0, sound_file_open_stub);
			db_link_xasset_entry1_hook.create(0x140414900, db_link_xasset_entry1_stub);

			// Allow bnet filesystem to also load files from disk
			if (db_filesysImpl->current.integer == 0)
			{
				const auto bnet_interface = reinterpret_cast<game::DB_FileSysInterface*>(0x140BE82F8);

				bnet_fs_open_file_hook.create(bnet_interface->vftbl->OpenFile, bnet_fs_open_file_stub);
				bnet_fs_read_hook.create(bnet_interface->vftbl->Read, bnet_fs_read_stub);
				bnet_fs_tell_hook.create(bnet_interface->vftbl->Tell, bnet_fs_tell_stub);
				bnet_fs_size_hook.create(bnet_interface->vftbl->Size, bnet_fs_size_stub);
				bnet_fs_close_hook.create(bnet_interface->vftbl->Close, bnet_fs_close_stub);
				bnet_fs_exists_hook.create(bnet_interface->vftbl->Exists, bnet_fs_exists_stub);

				bink_io_read_hook.create(0x1407191B0, bink_io_read_stub);
				bink_io_seek_hook.create(0x140719200, bink_io_seek_stub);
			}

			command::add("listSoundFiles", []()
			{
				for (const auto& packed : sound_files)
				{
					for (const auto& sound : packed.second)
					{
						console::info("soundfile%i.pak %s %llX", packed.first, sound.second.data(), sound.first);
					}
				}
			});

			command::add("dumpSoundFile", [](const command::params& params)
			{
				if (params.size() < 2)
				{
					console::info("Usage: dumpSoundFile <index>\n");
					return;
				}

				const auto index = static_cast<unsigned short>(atoi(params.get(1)));
				dump_sound_file(index);
			});

			command::add("dumpSound", [](const command::params& params)
			{
				if (params.size() < 2)
				{
					console::info("Usage: dumpSound <name>\n");
					return;
				}

				const auto name = params.get(1);
				for (const auto& packed : sound_files)
				{
					for (auto i = packed.second.begin(); i != packed.second.end();)
					{
						if (i->second != name)
						{
							++i;
							continue;
						}

						const auto& sound_name = i->second;
						const auto start_offset = i->first;
						dump_flac_sound(sound_name, packed.first, start_offset, sound_sizes[sound_name]);
						break;
					}
				}
			});
		}
	};
}

REGISTER_COMPONENT(database::component)
