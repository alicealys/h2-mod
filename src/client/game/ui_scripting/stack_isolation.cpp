#include <std_include.hpp>
#include "stack_isolation.hpp"

namespace ui_scripting
{
	stack_isolation::stack_isolation()
	{
		const auto state = *game::hks::lua_state;
		state->m_apistack.top = state->m_apistack.base;
	}
}
