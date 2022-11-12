#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "command.hpp"
#include "console.hpp"
#include "scheduler.hpp"
#include "filesystem.hpp"
#include "fonts.hpp"
#include "mods.hpp"
#include "localized_strings.hpp"
#include "loadscreen.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>

namespace mods
{
	namespace
	{
		struct mod_zone_info
		{
			bool has_common_zones;
			std::vector<mod_zone> zones;
		};

		struct
		{
			std::optional<std::string> path;
			mod_zone_info zone_info;
		} mod_info;

		std::unordered_map<std::string, game::DBAllocFlags> alloc_flags_map =
		{
			{"common", game::DB_ZONE_COMMON},
			{"game", game::DB_ZONE_GAME},
		};

		unsigned int get_alloc_flag(const std::string& name)
		{
			const auto lower = utils::string::to_lower(name);
			if (alloc_flags_map.find(lower) != alloc_flags_map.end())
			{
				return alloc_flags_map[lower];
			}

			return game::DB_ZONE_COMMON;
		}

		utils::hook::detour db_release_xassets_hook;
		bool release_assets = false;

		void db_release_xassets_stub()
		{
			if (release_assets)
			{
				fonts::clear();
				loadscreen::clear();
			}

			localized_strings::clear();

			db_release_xassets_hook.invoke<void>();
		}

		void restart()
		{
			scheduler::once([]()
			{
				release_assets = true;
				const auto _0 = gsl::finally([]()
				{
					release_assets = false;
				});

				game::Com_Shutdown("");
			}, scheduler::pipeline::main);
		}

		void full_restart(const std::string& arg)
		{
			utils::nt::relaunch_self(" -singleplayer "s.append(arg), true);
			utils::nt::terminate();
		}

		void clear_mod_zones()
		{
			mod_info.zone_info = {};
		}

		void parse_mod_zones()
		{
			clear_mod_zones();
			if (!mod_info.path.has_value())
			{
				return;
			}

			const auto path = mod_info.path.value() + "/zones.csv";
			std::string data{};
			if (!utils::io::read_file(path, &data))
			{
				return;
			}

			const auto lines = utils::string::split_lines(data);
			for (const auto& line : lines)
			{
				const auto values = utils::string::split(line, ',');
				if (values.size() < 2)
				{
					continue;
				}

				const auto alloc_flags = get_alloc_flag(values[0]) | game::DB_ZONE_CUSTOM;
				if (alloc_flags & game::DB_ZONE_COMMON)
				{
					mod_info.zone_info.has_common_zones = true;
				}

				mod_info.zone_info.zones.emplace_back(values[1], alloc_flags);
			}
		}
	}

	bool mod_requires_restart(const std::string& path)
	{
		return mod_info.zone_info.has_common_zones || 
			utils::io::file_exists(path + "/mod.ff") || 
			utils::io::file_exists(path + "/zone/mod.ff");
	}

	void set_mod(const std::string& path)
	{
		if (mod_info.path.has_value())
		{
			filesystem::unregister_path(mod_info.path.value());
		}

		mod_info.path = path;
		filesystem::register_path(path);
		parse_mod_zones();
	}

	void clear_mod()
	{
		if (mod_info.path.has_value())
		{
			filesystem::unregister_path(mod_info.path.value());
		}

		mod_info.path.reset();
		clear_mod_zones();
	}

	std::vector<mod_zone> get_mod_zones()
	{
		return mod_info.zone_info.zones;
	}

	std::optional<std::string> get_mod()
	{
		return mod_info.path;
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			if (!utils::io::directory_exists("mods"))
			{
				utils::io::create_directory("mods");
			}

			db_release_xassets_hook.create(0x140416A80, db_release_xassets_stub);

			command::add("loadmod", [](const command::params& params)
			{
				if (params.size() < 2)
				{
					console::info("Usage: loadmod mods/<modname>");
					return;
				}

				if (!game::Com_InFrontend())
				{
					console::error("Cannot load mod while in-game!\n");
					game::CG_GameMessage(0, "^1Cannot unload mod while in-game!", 0);
					return;
				}

				const auto path = params.get(1);
				if (!utils::io::directory_exists(path))
				{
					console::error("Mod %s not found!\n", path);
					return;
				}

				console::info("Loading mod %s\n", path);
				set_mod(path);

				if ((mod_info.path.has_value() && mod_requires_restart(mod_info.path.value())) || 
					mod_requires_restart(path))
				{
					// vid_restart is still broken :(
					console::info("Restarting...\n");
					full_restart("-mod "s + path);
				}
				else
				{
					restart();
				}
			});

			command::add("unloadmod", [](const command::params& params)
			{
				if (!mod_info.path.has_value())
				{
					console::info("No mod loaded\n");
					return;
				}

				if (!game::Com_InFrontend())
				{
					console::error("Cannot unload mod while in-game!\n");
					game::CG_GameMessage(0, "^1Cannot unload mod while in-game!", 0);
					return;
				}

				console::info("Unloading mod %s\n", mod_info.path.value().data());

				if (mod_requires_restart(mod_info.path.value()))
				{
					console::info("Restarting...\n");
					full_restart("");
				}
				else
				{
					clear_mod();
					restart();
				}
			});

			command::add("com_restart", []()
			{
				if (!game::Com_InFrontend())
				{
					return;
				}

				restart();
			});
		}
	};
}

REGISTER_COMPONENT(mods::component)
