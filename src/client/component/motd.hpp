#pragma once

namespace motd
{
	using links_map_t = std::unordered_map<std::string, std::string>;
	links_map_t get_links();

	int get_num_featured_tabs();
	nlohmann::json get_motd();
	nlohmann::json get_featured_tab(const int index);
	bool has_motd();
}
