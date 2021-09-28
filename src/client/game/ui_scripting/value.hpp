#pragma once
#include "game/game.hpp"

namespace ui_scripting
{
	struct lightuserdata
	{
		void* ptr;
		bool operator==(const lightuserdata other) const noexcept
		{
			return this->ptr == other.ptr;
		}
	};

	using value = std::variant<std::monostate, bool, int, float, std::string, lightuserdata>;
	using arguments = std::vector<value>;
}
