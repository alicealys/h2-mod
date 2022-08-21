#pragma once

namespace mods
{
	extern std::string mod_path;

	bool mod_requires_restart(const std::string& path);
}