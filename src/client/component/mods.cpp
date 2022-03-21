#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "command.hpp"
#include "game_console.hpp"
#include "scheduler.hpp"
#include "filesystem.hpp"
#include "materials.hpp"
#include "fonts.hpp"
#include "mods.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>

namespace mods
{
	std::string mod_path{};

	namespace
	{
		utils::hook::detour db_release_xassets_hook;
		bool release_assets = false;

		void db_release_xassets_stub()
		{
			if (release_assets)
			{
				materials::clear();
				fonts::clear();
			}

			db_release_xassets_hook.invoke<void>();
		}

		void restart()
		{
			scheduler::once([]()
			{
				release_assets = true;
				game::Com_Shutdown("");
				release_assets = false;
			}, scheduler::pipeline::main);
		}
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
					game_console::print(game_console::con_type_info, "Usage: loadmod mods/<modname>");
					return;
				}

				if (!game::Com_InFrontend())
				{
					game_console::print(game_console::con_type_error, "Cannot load mod while in-game!\n");
					game::CG_GameMessage(0, "^1Cannot unload mod while in-game!");
					return;
				}

				const auto path = params.get(1);
				if (!utils::io::directory_exists(path))
				{
					game_console::print(game_console::con_type_error, "Mod %s not found!\n", path);
					return;
				}

				game_console::print(game_console::con_type_info, "Loading mod %s\n", path);
				filesystem::get_search_paths().erase(mod_path);
				filesystem::get_search_paths().insert(path);
				mod_path = path;
				restart();
			});

			command::add("unloadmod", [](const command::params& params)
			{
				if (mod_path.empty())
				{
					game_console::print(game_console::con_type_info, "No mod loaded\n");
					return;
				}

				if (!game::Com_InFrontend())
				{
					game_console::print(game_console::con_type_error, "Cannot unload mod while in-game!\n");
					game::CG_GameMessage(0, "^1Cannot unload mod while in-game!");
					return;
				}

				game_console::print(game_console::con_type_info, "Unloading mod %s\n", mod_path.data());
				filesystem::get_search_paths().erase(mod_path);
				mod_path.clear();
				restart();
			});
		}
	};
}

REGISTER_COMPONENT(mods::component)
