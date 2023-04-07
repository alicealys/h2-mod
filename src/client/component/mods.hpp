#pragma once

namespace mods
{
	enum zone_priority
	{
		none,

		// common
		pre_gfx,
		post_gfx,
		post_common,

		// game
		pre_map,
		post_map,
	};

	struct mod_zone
	{
		std::string name;
		unsigned int alloc_flags;
		zone_priority priority;
	};

	bool mod_requires_restart(const std::string& path);
	void set_mod(const std::string& path);
	std::optional<std::string> get_mod();
	std::vector<mod_zone> get_mod_zones();

	void load(const std::string& path);
	void unload();

	std::vector<std::string> get_mod_list();
	std::optional<nlohmann::json> get_mod_info(const std::string& mod);
	bool mod_exists(const std::string& folder);

	nlohmann::json& get_current_stats();
	void write_mod_stats();
}