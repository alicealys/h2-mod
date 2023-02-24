#pragma once
#include "game/game.hpp"
#include "game/dvars.hpp"

namespace language
{
	std::string get_default_language();
	bool is_valid_language(const std::string& name);
	bool is_custom_language(const std::string& name);

	void set(const std::string& language);
	void set_from_index(const int index);

	game::language_t current();
	bool is_non_latin();
	bool is_slavic();
	bool is_arabic();
	bool is_asian();
}
