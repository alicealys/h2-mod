#pragma once

namespace gsc
{
	extern std::array<const char*, 27> var_typename;

	struct script_info_t
	{
		const char* script_start;
		std::string file;
		std::string function;
	};

	std::optional<script_info_t> find_function(const char* pos);
}
