#pragma once

namespace motd
{
	using links_map_t = std::unordered_map<std::string, std::string>;
	links_map_t get_links();

	int get_num_featured_tabs();
	nlohmann::json get_motd();
	nlohmann::json get_featured_tab(const int index);
	bool has_motd();

	bool has_wordle();
	std::string get_wordle_solution();
	std::uint32_t get_wordle_id();
	bool has_solved_wordle();
	std::uint32_t get_wordle_score();
	void solve_wordle(bool success);
	bool check_worlde_word(const std::string& word);
}
