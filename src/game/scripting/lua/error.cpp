#include <stdinc.hpp>
#include "error.hpp"
#include "../execution.hpp"

#include "component/game_console.hpp"

namespace scripting::lua
{
	namespace
	{
		void notify_error()
		{
			try
			{
				call("iprintln", {"^1Script execution error!"});
			}
			catch (...)
			{
			}
		}
	}

	void handle_error(const sol::protected_function_result& result)
	{
		if (!result.valid())
		{
			try
			{
				game_console::print(game_console::con_type_error, "************** Script execution error **************\n");

				const sol::error err = result;
				game_console::print(game_console::con_type_error, "%s\n", err.what());

				game_console::print(game_console::con_type_error, "****************************************************\n");

				notify_error();
			}
			catch (...)
			{
			}
		}
	}
}
