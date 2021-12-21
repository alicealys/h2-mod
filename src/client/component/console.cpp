#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "command.hpp"
#include "game_console.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/thread.hpp>
#include <utils/hook.hpp>

namespace console
{
	namespace
	{
		std::thread console_thread;
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			ShowWindow(GetConsoleWindow(), SW_SHOW);
			SetConsoleTitle("H2-Mod");

			console_thread = utils::thread::create_named_thread("Console", []()
			{
				std::string cmd;
				while (true)
				{
					std::getline(std::cin, cmd);
					game_console::add(cmd.data(), false);
				}
			});
		}
	};
}

REGISTER_COMPONENT(console::component)
