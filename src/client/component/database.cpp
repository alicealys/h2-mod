#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/dvars.hpp"
#include "game/game.hpp"

#include "filesystem.hpp"
#include "console.hpp"
#include "command.hpp"
#include "sound.hpp"

#include <utils/io.hpp>
#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/flags.hpp>

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

		std::unordered_map<std::string, std::string> file_search_folders =
		{
			{".flac", "sound/"},
			{".bik", "video/"},
			{".ff", "zone/"},
		};

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

		game::DB_IFileSysFile* bnet_fs_open_file_stub(game::DB_FileSysInterface* this_, int folder, const char* file)
		{
			std::string name = file;

			const auto search_path = [&](const std::string& ext, const std::string& path)
			{
				if (name.ends_with(ext) && !filesystem::exists(name))
				{
					name = path + name;
				}
			};

			for (const auto& [ext, path] : file_search_folders)
			{
				search_path(ext, path);
			}

			std::string path{};
			if (!filesystem::find_file(name, &path))
			{
				return bnet_fs_open_file_hook.invoke<game::DB_IFileSysFile*>(this_, folder, file);
			}

			const auto handle = handle_allocator.allocate<game::DB_IFileSysFile>();

			try
			{
#ifdef DEBUG
				console::info("[Database] Opening file %s\n", path.data());
#endif

				auto stream = std::make_unique<std::ifstream>();
				stream->open(path, std::ios::binary);

				bnet_file_handle_t bnet_handle{};
				bnet_handle.stream = std::move(stream);
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
			std::string name = filename;
			const auto search_path = [&](const std::string& ext, const std::string& path)
			{
				if (!name.ends_with(ext))
				{
					return false;
				}

				return filesystem::exists(name) || filesystem::exists(path + name);
			};

			if (filesystem::exists(filename))
			{
				return true;
			}

			for (const auto& [ext, path] : file_search_folders)
			{
				if (search_path(ext, path))
				{
					return true;
				}
			}

			return bnet_fs_exists_hook.invoke<bool>(this_, handle, filename);
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
		}
	};
}

REGISTER_COMPONENT(database::component)
