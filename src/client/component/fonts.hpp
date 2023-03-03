#pragma once

#include "game/game.hpp"
#include <utils/memory.hpp>

namespace fonts
{
	void add(const std::string& name, const std::string& data);
	void clear();
	void load_font_zones();
}
