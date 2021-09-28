#pragma once
#include "game/ui_scripting/menu.hpp"
#include "game/ui_scripting/event.hpp"

namespace ui_scripting
{
	void enable_error_hook();
	void disable_error_hook();

	game::hks::lua_function find_function(const std::string& name);
	game::hks::lua_function find_method(const std::string& name);

	void notify(const event& e);
}
