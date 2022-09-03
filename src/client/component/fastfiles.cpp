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

		bool try_load_zone(std::string name, bool localized, bool game = false)
		{
			if (localized)
			{
				const auto language = game::SEH_GetCurrentLanguageCode();
				if (!try_load_zone(language + "_"s + name, false) && language != "eng"s)
				{
					try_load_zone("eng_" + name, false);
				}
			}

			if (!fastfiles::exists(name))
			{
				return false;
			}

			game::XZoneInfo info{};
			info.name = name.data();
			info.allocFlags = (game ? game::DB_ZONE_GAME : game::DB_ZONE_COMMON) | game::DB_ZONE_CUSTOM;
			info.freeFlags = 0;
			game::DB_LoadXAssets(&info, 1u, game::DBSyncMode::DB_LOAD_ASYNC);
			return true;
		}

		void load_post_gfx_and_ui_and_common_zones(game::XZoneInfo* zoneInfo, 
			unsigned int zoneCount, game::DBSyncMode syncMode)
		{
			// code_post_gfx_mp
			// ui_mp
			// common_mp

			try_load_zone("h2_mod_ui", true);
			try_load_zone("h2_mod_common", true);

			game::DB_LoadXAssets(zoneInfo, zoneCount, syncMode);

			try_load_zone("mod", true);
		}

		constexpr unsigned int get_asset_type_size(const game::XAssetType type)
		{
			constexpr int asset_type_sizes[] =
			{
				96, 88, 128, 56, 40, 216, 56, 696,
				624, 32, 32, 32, 32, 32, 2112, 2032,
				104, 32, 24, 1520, 152, 152, 16, 64,
				640, 40, 16, 136, 24, 296, 176, 2896,
				48, 0, 24, 200, 88, 16, 144, 3848,
				56, 72, 16, 16, 0, 0, 0, 0, 24,
				40, 24, 48, 40, 24, 16, 80, 128,
				2280, 136, 32, 72, 24, 64, 88, 48,
				32, 96, 120, 64, 32, 32
			};

			return asset_type_sizes[type];
		}

		constexpr unsigned int get_pool_type_size(const game::XAssetType type)
		{
			constexpr int asset_pool_sizes[] =
			{
				150, 1024, 16, 1, 128, 7000, 5248, 2560, 
				10624, 256, 49152, 12288, 12288, 72864, 
				512, 3072, 12000, 16000, 256, 64, 64,
				64, 64, 10000, 1, 1, 1, 1, 1, 2, 1, 
				1, 32, 0, 128, 400, 0, 11500, 128, 
				360, 1, 2048, 4, 6, 0, 0, 0, 0, 1024, 
				768, 400, 128, 128, 24, 24, 24, 32, 
				128, 2, 0, 64, 384, 128, 1, 128, 64, 
				32, 32, 16, 32, 16
			};

			return asset_pool_sizes[type];
		}

		template <game::XAssetType Type, size_t Size>
		char* reallocate_asset_pool()
		{
			constexpr auto element_size = get_asset_type_size(Type);
			static char new_pool[element_size * Size] = {0};
			static_assert(element_size != 0);
			assert(element_size == game::DB_GetXAssetTypeSize(Type));

			std::memmove(new_pool, game::g_assetPool[Type], game::g_poolSize[Type] * element_size);

			game::g_assetPool[Type] = new_pool;
			game::g_poolSize[Type] = Size;

			return new_pool;
		}

		template <game::XAssetType Type, size_t Multiplier>
		char* reallocate_asset_pool_multiplier()
		{
			constexpr auto pool_size = get_pool_type_size(Type);
			return reallocate_asset_pool<Type, pool_size * Multiplier>();
		}

#define RVA(ptr) static_cast<uint32_t>(reinterpret_cast<size_t>(ptr) - 0x140000000)

		void reallocate_xmodel_pool()
		{
			// array used for DB_GetAllXAssetOfType, not big enough if many assets are added
			static game::XAssetHeader assets[0x100000]{};
			utils::hook::inject(0x1403E2AB7, &assets);
			utils::hook::inject(0x1403E2AC3, &assets);
			utils::hook::inject(0x1403E2ACF, &assets);

			constexpr auto xmodel_pool_size = get_pool_type_size(game::ASSET_TYPE_XMODEL) * 2;
			const auto xmodel_pool = reallocate_asset_pool<game::ASSET_TYPE_XMODEL, xmodel_pool_size>();
			utils::hook::inject(0x140413D93, xmodel_pool + 8);

			// table 1
			static int xmodel_table_1[xmodel_pool_size]{};
			utils::hook::set<uint32_t>(0x14041E0C2 + 4, RVA(&xmodel_table_1[0]));
			utils::hook::inject(0x14041E7F6 + 3, &xmodel_table_1[0]);
			utils::hook::set<uint32_t>(0x140420797 + 4, RVA(&xmodel_table_1[0]));
			utils::hook::inject(0x1404228F6 + 3, &xmodel_table_1[0]);
			utils::hook::inject(0x14042290E + 3, &xmodel_table_1[0]);
			utils::hook::set<uint32_t>(0x140710280 + 4, RVA(&xmodel_table_1[0]));

			// everything below doesnt seem to change anything
			// table 2
			static int xmodel_table_2[xmodel_pool_size]{};
			utils::hook::set<uint32_t>(0x14041E2FA + 4, RVA(&xmodel_table_2[0]));
			utils::hook::set<uint32_t>(0x1404207BC + 4, RVA(&xmodel_table_2[0]));
			utils::hook::inject(0x140422AE1 + 3, &xmodel_table_2[0]);
			utils::hook::set<uint32_t>(0x140422B20 + 4, RVA(&xmodel_table_2[0]));
			utils::hook::inject(0x140422B8D + 3, &xmodel_table_2[0]);
			utils::hook::set<uint32_t>(0x140422BC7 + 4, RVA(&xmodel_table_2[0]));
			utils::hook::inject(0x140422C41 + 3, &xmodel_table_2[0]);
			utils::hook::set<uint32_t>(0x140422CE0 + 4, RVA(&xmodel_table_2[0]));
			utils::hook::set<uint32_t>(0x140422D16 + 4, RVA(&xmodel_table_2[0]));
			utils::hook::set<uint32_t>(0x140723BAE + 5, RVA(&xmodel_table_2[0]));
			utils::hook::set<uint32_t>(0x140723BCC + 5, RVA(&xmodel_table_2[0]));
			utils::hook::inject(0x140728332 + 3, &xmodel_table_2[0]);

			// table 3
			static int xmodel_table_3[xmodel_pool_size]{};
			utils::hook::set<uint32_t>(0x1404207D4 + 4, RVA(&xmodel_table_3[0]));
			utils::hook::set<uint32_t>(0x140724BA3 + 5, RVA(&xmodel_table_3[0]));
			utils::hook::set<uint32_t>(0x140724BC1 + 5, RVA(&xmodel_table_3[0]));

			// table 4
			static int xmodel_table_4[xmodel_pool_size]{};
			utils::hook::set<uint32_t>(0x1404207C8 + 4, RVA(&xmodel_table_4[0]));
			utils::hook::inject(0x140422888 + 3, &xmodel_table_4[0]);
			utils::hook::inject(0x14041EAC0 + 3, reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_4[0]) + 0x10));

			// table 5
			static int xmodel_table_5[xmodel_pool_size]{};
			utils::hook::set<uint32_t>(0x1404205BC + 4, RVA(&xmodel_table_5[0]));
			utils::hook::set<uint32_t>(0x14042062D + 4, RVA(&xmodel_table_5[0]));
			utils::hook::inject(0x140420A35 + 3, &xmodel_table_5[0]);

			// hash table 1
			static int xmodel_hash_table_1[xmodel_pool_size]{};
			utils::hook::set<uint32_t>(0x1404207AA + 4, RVA(&xmodel_hash_table_1[0]));
			utils::hook::inject(0x1404208DE + 3, &xmodel_hash_table_1[0]);
			utils::hook::inject(0x140422535 + 2, &xmodel_hash_table_1[0]);

			// hash table 2
			static int xmodel_hash_table_2[xmodel_pool_size]{};
			utils::hook::inject(0x1403E2A8E + 3, &xmodel_hash_table_2[0]);
			utils::hook::inject(0x1403E2FD9 + 3, &xmodel_hash_table_2[0]);
			utils::hook::inject(0x1403E37C5 + 3, &xmodel_hash_table_2[0]);

			// hash table 2
			static int xmodel_hash_table_3[xmodel_pool_size]{};
			utils::hook::inject(0x1403E2A8E + 3, &xmodel_hash_table_3[0]);
			utils::hook::inject(0x1403E2FD9 + 3, &xmodel_hash_table_3[0]);
			utils::hook::inject(0x1403E37C5 + 3, &xmodel_hash_table_3[0]);

			// hash table 3
			static int xmodel_hash_table_4[xmodel_pool_size]{};
			utils::hook::set<uint32_t>(0x1404207B4 + 4, RVA(&xmodel_hash_table_4[0]));
			utils::hook::inject(0x140422AD7 + 3, &xmodel_hash_table_4[0]);
			utils::hook::set<uint32_t>(0x140422B18 + 4, RVA(&xmodel_hash_table_4[0]));
			utils::hook::inject(0x140422B83 + 3, &xmodel_hash_table_4[0]);
			utils::hook::set<uint32_t>(0x140422BBF + 4, RVA(&xmodel_hash_table_4[0]));
			utils::hook::inject(0x140422C37 + 3, &xmodel_hash_table_4[0]);
			utils::hook::inject(0x140422C97 + 3, &xmodel_hash_table_4[0]);
			utils::hook::set<uint32_t>(0x140422CD8 + 4, RVA(&xmodel_hash_table_4[0]));
			utils::hook::set<uint32_t>(0x140422D0E + 4, RVA(&xmodel_hash_table_4[0]));

			// seems to fix some issues but causes models to load slowly, probably not done correctly
			static int xmodel_table_7[xmodel_pool_size * 6]{};
			utils::hook::set<uint32_t>(0x1404205AF + 3, RVA(reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_7) + 0)));
			utils::hook::set<uint32_t>(0x140420752 + 4, RVA(reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_7) + 0)));

			utils::hook::set<uint32_t>(0x1404205A7 + 4, RVA(reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_7) + 8)));
			utils::hook::set<uint32_t>(0x14042065B + 4, RVA(reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_7) + 8)));
			utils::hook::set<uint32_t>(0x14042068D + 4, RVA(reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_7) + 8)));
			utils::hook::set<uint32_t>(0x1404206AF + 4, RVA(reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_7) + 8)));
			utils::hook::set<uint32_t>(0x1404206F0 + 4, RVA(reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_7) + 8)));
			utils::hook::set<uint32_t>(0x140420720 + 4, RVA(reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_7) + 8)));
			utils::hook::set<uint32_t>(0x14042075F + 4, RVA(reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_7) + 8)));

			utils::hook::set<uint32_t>(0x14041EE9B + 4, RVA(reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_7) + 0x10)));
			utils::hook::set<uint32_t>(0x1404205A0 + 3, RVA(reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_7) + 0x10)));
			utils::hook::set<uint32_t>(0x14042060D + 5, RVA(reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_7) + 0x10)));
			utils::hook::set<uint32_t>(0x140420618 + 3, RVA(reinterpret_cast<void*>(reinterpret_cast<size_t>(&xmodel_table_7) + 0x10)));
		}

		void reallocate_asset_pools()
		{
			//reallocate_xmodel_pool();
			//reallocate_asset_pool_multiplier<game::ASSET_TYPE_XMODELSURFS, 2>();
			reallocate_asset_pool_multiplier<game::ASSET_TYPE_WEAPON, 2>();
			reallocate_asset_pool_multiplier<game::ASSET_TYPE_SOUND, 2>();
			reallocate_asset_pool_multiplier<game::ASSET_TYPE_LOADED_SOUND, 2>();
			reallocate_asset_pool_multiplier<game::ASSET_TYPE_XANIM, 2>();
			reallocate_asset_pool_multiplier<game::ASSET_TYPE_LOCALIZE, 2>();
		}

		void add_custom_level_load_zone(void* load, const char* name, bool localized, const size_t size_est)
		{
			if (localized)
			{
				const auto language = game::SEH_GetCurrentLanguageCode();
				const auto lang_name = language +  "_"s + name;

				if (fastfiles::exists(lang_name))
				{
					add_custom_level_load_zone(load, lang_name.data(), false, size_est);
				}
			}

			game::DB_LevelLoadAddZone(load, name, game::DB_ZONE_GAME | game::DB_ZONE_CUSTOM, size_est);
		}

		void db_load_level_add_custom_zone_stub(void* load, const char* name, const unsigned int alloc_flags,
			const size_t size_est)
		{
			//add_custom_level_load_zone(load, name, true, size_est);
			try_load_zone(name, true, true);
		}

		void db_load_level_add_map_zone_stub(void* load, const char* name, const unsigned int alloc_flags,
			const size_t size_est)
		{
			auto is_builtin_map = false;
			for (auto map = &game::maps[0]; map->unk; ++map)
			{
				if (!std::strcmp(map->name, name))
				{
					is_builtin_map = true;
					break;
				}
			}

			if (is_builtin_map)
			{
				const auto name_ = "h2_mod_patch_"s + name;
				if (fastfiles::exists(name_))
				{
					add_custom_level_load_zone(load, name_.data(), true, size_est);
				}

				game::DB_LevelLoadAddZone(load, name, alloc_flags, size_est);
			}
			else
			{
				add_custom_level_load_zone(load, name, true, size_est);
			}
		}
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

			// load our custom ui and common zones
			utils::hook::call(0x14074E22A, load_post_gfx_and_ui_and_common_zones);

			reallocate_asset_pools();

			// only load extra zones with addon maps & common_specialops & common_survival & custom maps if the exist
			utils::hook::call(0x1404128B0, db_load_level_add_map_zone_stub);
			utils::hook::call(0x140412854, db_load_level_add_custom_zone_stub);
			utils::hook::call(0x14041282D, db_load_level_add_custom_zone_stub);
			utils::hook::call(0x14041287C, db_load_level_add_custom_zone_stub);

			command::add("loadzone", [](const command::params& params)
			{
				if (params.size() < 2)
				{
					console::info("usage: loadzone <zone>\n");
					return;
				}

				const auto name = params.get(1);
				if (!try_load_zone(name, false))
				{
					console::warn("loadzone: zone \"%s\" could not be found!\n", name);
				}
			});

			command::add("g_poolSizes", []()
			{
				for (auto i = 0; i < game::ASSET_TYPE_COUNT; i++)
				{
					console::info("g_poolSize[%i]: %i // %s\n", i, game::g_poolSize[i], game::g_assetNames[i]);
				}
			});

			command::add("poolUsages", []()
			{
				for (auto i = 0; i < game::ASSET_TYPE_COUNT; i++)
				{
					auto count = 0;
					enum_assets(static_cast<game::XAssetType>(i), [&](game::XAssetHeader header)
					{
						count++;
					}, true);

					console::info("%i %s: %i / %i\n", i, game::g_assetNames[i], count, game::g_poolSize[i]);
				}
			});

			command::add("poolUsage", [](const command::params& params)
			{
				if (params.size() < 2)
				{
					console::info("Usage: poolUsage <type>\n");
					return;
				}

				const auto type = static_cast<game::XAssetType>(std::atoi(params.get(1)));

				auto count = 0;
				enum_assets(type, [&](game::XAssetHeader header)
				{
					count++;
				}, true);

				console::info("%i %s: %i / %i\n", type, game::g_assetNames[type], count, game::g_poolSize[type]);
			});

			command::add("assetCount", [](const command::params& params)
			{
				auto count = 0;
				for (auto i = 0; i < game::ASSET_TYPE_COUNT; i++)
				{
					enum_assets(static_cast<game::XAssetType>(i), [&](game::XAssetHeader header)
					{
						count++;
					}, true);
				}

				console::info("assets: %i / %i\n", count, 155000);
			});
		}
	};
}

REGISTER_COMPONENT(fastfiles::component)
