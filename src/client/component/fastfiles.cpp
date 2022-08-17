#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "fastfiles.hpp"

#include "command.hpp"
#include "console.hpp"
#include "localized_strings.hpp"
#include "sound.hpp"

#include <utils/hook.hpp>
#include <utils/concurrency.hpp>
#include <utils/string.hpp>

namespace fastfiles
{
	static utils::concurrency::container<std::string> current_fastfile;

	namespace
	{
		utils::hook::detour db_try_load_x_file_internal_hook;
		utils::hook::detour db_find_xasset_header;

		void db_try_load_x_file_internal(const char* zone_name, const int flags)
		{
			console::info("Loading fastfile %s\n", zone_name);
			current_fastfile.access([&](std::string& fastfile)
			{
				fastfile = zone_name;
			});
			return db_try_load_x_file_internal_hook.invoke<void>(zone_name, flags);
		}

		game::XAssetHeader db_find_xasset_header_stub(game::XAssetType type, const char* name, int allow_create_default)
		{
			if (type == game::ASSET_TYPE_SOUND)
			{
				const auto res = sound::find_sound(name);
				if (res.sound != nullptr)
				{
					return res;
				}
			}

			const auto start = game::Sys_Milliseconds();
			const auto result = db_find_xasset_header.invoke<game::XAssetHeader>(type, name, allow_create_default);
			const auto diff = game::Sys_Milliseconds() - start;

			if (diff > 100)
			{
				console::print(
					result.data == nullptr
						? console::con_type_error
						: console::con_type_warning,
					"Waited %i msec for %sasset \"%s\", of type \"%s\"\n", 
					diff, 
					result.data == nullptr 
						? "missing " 
						: "", 
					name, 
					game::g_assetNames[type]
				);
			}

			return result;
		}

		void add_missing_localized_strings()
		{
			for (auto map = &game::maps[0]; map->unk; ++map)
			{
				const auto str = utils::string::va("LUA_MENU_SP_LOCATION_%s",
					utils::string::to_upper(map->name).data());
				localized_strings::override(str, str);
			}
		}

		utils::hook::detour db_read_stream_file_hook;
		void db_read_stream_file_stub(int a1, int a2)
		{
			// always use lz4 compressor type when reading stream files
			*game::g_compressor = 4;
			return db_read_stream_file_hook.invoke<void>(a1, a2);
		}

		bool exists(const std::string& zone)
		{
			const auto is_localized = game::DB_IsLocalized(zone.data());
			const auto db_fs = game::DB_FSInitialize();

			auto handle = db_fs->vftbl->OpenFile(db_fs, 
				(is_localized ? game::SF_ZONE_LOC : game::SF_ZONE), utils::string::va("%s.ff", zone.data()));
			const auto _0 = gsl::finally([&]
			{
				if (handle != nullptr)
				{
					db_fs->vftbl->Close(db_fs, handle);
				}

			});

			return handle != nullptr;
		}

		void skip_extra_zones_stub(utils::hook::assembler& a)
		{
			const auto skip = a.newLabel();
			const auto original = a.newLabel();

			a.pushad64();
			a.test(r15d, game::DB_ZONE_CUSTOM); // allocFlags
			a.jnz(skip);

			a.bind(original);
			a.popad64();
			a.mov(r8d, 9);
			a.mov(rdx, 0x140933528);
			a.jmp(0x140415E09);

			a.bind(skip);
			a.popad64();
			a.mov(r14d, game::DB_ZONE_CUSTOM);
			a.not_(r14d);
			a.and_(r15d, r14d);
			a.jmp(0x140415E29);
		}
	}

	void enum_assets(const game::XAssetType type, const std::function<void(game::XAssetHeader)>& callback, const bool includeOverride)
	{
		game::DB_EnumXAssets_Internal(type, static_cast<void(*)(game::XAssetHeader, void*)>([](game::XAssetHeader header, void* data)
		{
			const auto& cb = *static_cast<const std::function<void(game::XAssetHeader)>*>(data);
			cb(header);
		}), &callback, includeOverride);
	}

	std::string get_current_fastfile()
	{
		std::string fastfile_copy;
		return current_fastfile.access<std::string>([&](std::string& fastfile)
		{
			return fastfile;
		});
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			db_try_load_x_file_internal_hook.create(0x1404173B0, db_try_load_x_file_internal);
			db_find_xasset_header.create(game::DB_FindXAssetHeader, db_find_xasset_header_stub);

			add_missing_localized_strings();

			// Allow loading of mixed compressor types
			utils::hook::nop(0x1403E66A7, 2);

			// Fix compressor type on streamed file load
			db_read_stream_file_hook.create(0x14041D710, db_read_stream_file_stub);

			// Don't load extra zones with loadzone
			utils::hook::nop(0x140415DFC, 13);
			utils::hook::jump(0x140415DFC, utils::hook::assemble(skip_extra_zones_stub), true);

			command::add("loadzone", [](const command::params& params)
			{
				if (params.size() < 2)
				{
					console::info("usage: loadzone <zone>\n");
					return;
				}

				const auto name = params.get(1);

				if (!fastfiles::exists(name))
				{
					console::warn("loadzone: zone \"%s\" could not be found!\n", name);
					return;
				}

				game::XZoneInfo info{};
				info.name = name;
				info.allocFlags = game::DB_ZONE_GAME | game::DB_ZONE_CUSTOM;
				info.freeFlags = 0;
				game::DB_LoadXAssets(&info, 1u, game::DBSyncMode::DB_LOAD_SYNC);
			});

			command::add("g_poolSizes", []()
			{
				for (auto i = 0; i < game::ASSET_TYPE_COUNT; i++)
				{
					console::info("g_poolSize[%i]: %i // %s\n", i, game::g_poolSize[i], game::g_assetNames[i]);
				}
			});
		}
	};
}

REGISTER_COMPONENT(fastfiles::component)
