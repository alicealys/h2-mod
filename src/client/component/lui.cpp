#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "command.hpp"
#include "game_console.hpp"

#include <utils/hook.hpp>

namespace lui
{
	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			command::add("lui_open", [](const command::params& params)
			{
				if (params.size() <= 1)
				{
					game_console::print(game_console::con_type_info, "usage: lui_open <name>\n");
					return;
				}

				game::LUI_OpenMenu(0, params[1], 0, 0, 0);
			});

			command::add("lui_open_popup", [](const command::params& params)
			{
				if (params.size() <= 1)
				{
					game_console::print(game_console::con_type_info, "usage: lui_open_popup <name>\n");
					return;
				}

				game::LUI_OpenMenu(0, params[1], 1, 0, 0);
			});

			command::add("lui_restart", []()
			{
				utils::hook::invoke<void>(0x1403203B0);
				utils::hook::invoke<void>(0x14032D370);
			});
		}
	};
}

REGISTER_COMPONENT(lui::component)
