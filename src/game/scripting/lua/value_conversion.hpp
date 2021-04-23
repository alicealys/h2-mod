#pragma once

#include "context.hpp"

namespace scripting::lua
{
	script_value convert(const sol::lua_value& value);
	sol::lua_value convert(lua_State* state, const script_value& value);
}
