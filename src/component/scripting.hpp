#pragma once

namespace scripting
{
	extern std::unordered_map<int, std::unordered_map<std::string, int>> fields_table;

	void init();
}