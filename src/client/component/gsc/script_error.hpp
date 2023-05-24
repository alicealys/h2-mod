#pragma once

namespace gsc
{
	extern std::array<const char*, 27> var_typename;

	std::optional<std::pair<std::string, std::string>> find_function(const char* pos);
}
