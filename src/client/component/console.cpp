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
		bool kill = false;
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
				while (!kill)
				{
					// to do: get input without blocking the thread
					std::this_thread::sleep_for(1ms);
				}

				std::this_thread::yield();
			});
		}

		void pre_destroy() override
		{
			kill = true;

			if (console_thread.joinable())
			{
				console_thread.join();
			}
		}
	};
}

REGISTER_COMPONENT(console::component)
