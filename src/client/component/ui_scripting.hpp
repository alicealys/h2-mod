#pragma once
#include "game/ui_scripting/menu.hpp"

namespace ui_scripting
{
	extern std::unordered_map<std::string, std::unordered_map<std::string, game::hks::lua_function>> libs;

	void enable_error_hook();
	void disable_error_hook();

	game::hks::lua_function find_function(const std::string& name);
}
