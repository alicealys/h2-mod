#pragma once

namespace mods
{
	struct mod_zone
	{
		std::string name;
		unsigned int alloc_flags;
	};

	bool mod_requires_restart(const std::string& path);
	void set_mod(const std::string& path);
	std::optional<std::string> get_mod();
	std::vector<mod_zone> get_mod_zones();

	std::vector<std::string> get_mod_list();
	std::optional<nlohmann::json> get_mod_info(const std::string& mod);

	nlohmann::json& get_current_stats();
	void write_mod_stats();
}