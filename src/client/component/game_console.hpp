#pragma once

#include "game/dvars.hpp"

namespace game_console
{
	void print(int type, const char* fmt, ...);

	bool console_char_event(int local_client_num, int key);
	bool console_key_event(int local_client_num, int key, int down);

	void find_matches(std::string input, std::vector<dvars::dvar_info>& suggestions, const bool exact);
	void execute(const char* cmd);
	void clear_console();
	void add(const std::string& cmd);

	std::deque<std::string>& get_output();
	std::deque<std::string>& get_history();
}