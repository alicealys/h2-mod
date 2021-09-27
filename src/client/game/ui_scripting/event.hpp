#pragma once
#include "value.hpp"

namespace ui_scripting
{
	struct event
	{
		std::string name;
		const void* element{};
		arguments arguments;
	};
}
