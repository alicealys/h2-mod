#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "command.hpp"
#include "console.hpp"
#include "scheduler.hpp"
#include "filesystem.hpp"
#include "materials.hpp"
#include "fonts.hpp"
#include "mods.hpp"
#include "mapents.hpp"
#include "localized_strings.hpp"
#include "loadscreen.hpp"
#include "sound.hpp"

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
				mapents::clear_dvars();
				loadscreen::clear();
			}

			mapents::clear();
			localized_strings::clear();
			sound::clear();

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
					game::CG_GameMessage(0, "^1Cannot unload mod while in-game!");
					return;
				}

				const auto path = params.get(1);
				if (!utils::io::directory_exists(path))
				{
					console::error("Mod %s not found!\n", path);
					return;
				}

				console::info("Loading mod %s\n", path);
				filesystem::unregister_path(mod_path);
				filesystem::register_path(path);
				mod_path = path;
				restart();
			});

			command::add("unloadmod", [](const command::params& params)
			{
				if (mod_path.empty())
				{
					console::info("No mod loaded\n");
					return;
				}

				if (!game::Com_InFrontend())
				{
					console::error("Cannot unload mod while in-game!\n");
					game::CG_GameMessage(0, "^1Cannot unload mod while in-game!");
					return;
				}

				console::info("Unloading mod %s\n", mod_path.data());
				filesystem::unregister_path(mod_path);
				mod_path.clear();
				restart();
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
