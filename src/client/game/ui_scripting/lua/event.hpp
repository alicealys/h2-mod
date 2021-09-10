#pragma once

#include "context.hpp"

namespace ui_scripting::lua
{
	struct event
	{
		std::string name;
		element* element{};
		std::vector<sol::lua_value> arguments;
	};
}
