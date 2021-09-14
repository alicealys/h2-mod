#pragma once

#include "context.hpp"

namespace ui_scripting::lua
{
	struct event
	{
		std::string name;
		const void* element{};
		std::vector<std::variant<int, std::string>> arguments;
	};
}
