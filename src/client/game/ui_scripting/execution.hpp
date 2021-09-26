#pragma once
#include "game/game.hpp"

namespace ui_scripting
{
	using value = std::variant<std::monostate, bool, int, float, std::string>;
	using arguments = std::vector<value>;
	value call(const std::string& name, const arguments& arguments);
}
