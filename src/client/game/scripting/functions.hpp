#pragma once
#include "game/game.hpp"

namespace scripting
{
	using script_function = void(*)(game::scr_entref_t);

	std::vector<std::string> find_token(unsigned int id);
	std::string find_token_single(unsigned int id);
	unsigned int find_token_id(const std::string& name);

	script_function find_function(const std::string& name, const bool prefer_global);
}
