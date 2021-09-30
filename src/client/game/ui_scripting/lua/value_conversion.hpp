#pragma once

#include "context.hpp"

namespace ui_scripting::lua
{
	value convert(const sol::lua_value& value);
	sol::lua_value convert(lua_State* state, const value& value);
}
