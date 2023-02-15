#pragma once
#include "game/game.hpp"
#include "game/dvars.hpp"

namespace language
{
	void set(const std::string& language);
	void set_from_index(const int index);

	game::language_t current();
	bool is_non_latin();
	bool is_polrus();
	bool is_arabic();
	bool is_asian();
}
