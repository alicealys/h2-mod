#pragma once
#include "game/game.hpp"

namespace ui_scripting
{
	class stack_isolation final
	{
	public:
		stack_isolation();
		stack_isolation(stack_isolation&&) = delete;
		stack_isolation(const stack_isolation&) = delete;
		stack_isolation& operator=(stack_isolation&&) = delete;
		stack_isolation& operator=(const stack_isolation&) = delete;

	};
}
