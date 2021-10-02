#pragma once
#include "game/ui_scripting/lua/value_conversion.hpp"
#include "game/ui_scripting/menu.hpp"
#include "game/ui_scripting/event.hpp"

namespace ui_scripting
{
	extern std::unordered_map<std::string, game::hks::lua_function> functions;
	extern std::unordered_map<std::string, game::hks::lua_function> methods;

	int main_function_handler(game::hks::lua_State* state);
	void add_converted_function(game::hks::cclosure* closure, const sol::protected_function& function);
	void clear_converted_functions();

	void enable_error_hook();
	void disable_error_hook();

	game::hks::lua_function find_function(const std::string& name);
	game::hks::lua_function find_method(const std::string& name);

	void notify(const event& e);
}
