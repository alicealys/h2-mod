#pragma once
#include "game/game.hpp"

namespace scripting
{
	extern std::unordered_map<std::string, unsigned> method_map;
	extern std::unordered_map<std::string, unsigned> function_map;

	using script_function = void(*)(game::scr_entref_t);

	script_function find_function(const std::string& name, const bool prefer_global);
}
