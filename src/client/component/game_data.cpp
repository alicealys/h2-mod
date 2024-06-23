#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "console.hpp"
#include "filesystem.hpp"
#include "mods.hpp"
#include "mod_stats.hpp"
#include "command.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>
#include <utils/concurrency.hpp>
#include <utils/thread.hpp>
#include <utils/properties.hpp>

#define PLAYERS_FOLDER "players2/"
#define DEFAULT_PLAYERS_FOLDER PLAYERS_FOLDER "default"
#define H2_MOD_PLAYERS_FOLDER PLAYERS_FOLDER "h2-mod"

namespace game_data
{
	namespace
	{
		std::optional<std::string> find_bnet_player_folder()
		{
			const auto dirs = utils::io::list_files(PLAYERS_FOLDER);
			for (const auto& dir : dirs)
			{
				if (dir == DEFAULT_PLAYERS_FOLDER || dir == H2_MOD_PLAYERS_FOLDER)
				{
					continue;
				}

				const auto cfg_file = std::format("{}\\config.cfg", dir);
				if (utils::io::file_exists(cfg_file))
				{
					return dir;
				}
			}

			return {};
		}

		void initialize_players_folder()
		{
			if (utils::io::directory_exists(H2_MOD_PLAYERS_FOLDER))
			{
				return;
			}

			if (utils::io::directory_exists(DEFAULT_PLAYERS_FOLDER))
			{
				utils::io::copy_folder(DEFAULT_PLAYERS_FOLDER, H2_MOD_PLAYERS_FOLDER);
				return;
			}

			const auto bnet_folder = find_bnet_player_folder();
			if (!bnet_folder.has_value())
			{
				return;
			}

			utils::io::copy_folder(bnet_folder.value(), H2_MOD_PLAYERS_FOLDER);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			initialize_players_folder();
			utils::hook::inject(0x14059D64E + 3, H2_MOD_PLAYERS_FOLDER);
		}
	};
}

REGISTER_COMPONENT(game_data::component)
