#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game_console.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>

namespace console
{
	namespace
	{
		DWORD WINAPI console(LPVOID)
		{
			ShowWindow(GetConsoleWindow(), SW_SHOW);

			std::string cmd;

			while (true)
			{
				std::getline(std::cin, cmd);
				game_console::execute(cmd.data());
			}

			return 0;
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			CreateThread(0, 0, console, 0, 0, 0);
		}
	};
}

REGISTER_COMPONENT(console::component)
