#pragma once
#include "script_value.hpp"

namespace ui_scripting
{
	struct event
	{
		std::string name;
		const void* element{};
		arguments arguments;
	};
}
