#pragma once

namespace game_console
{
	enum console_type
	{
		con_type_error = 1,
		con_type_warning = 3,
		con_type_info = 7
	};

	void draw_console();

	void print(int type, const char* fmt, ...);

	bool console_char_event(int local_client_num, int key);
	bool console_key_event(int local_client_num, int key, int down);

	void find_matches(std::string input, std::vector<std::string>& suggestions, const bool exact);
	void execute(const char* cmd);
	void clear_console();
	void add(const std::string& cmd, bool print_ = true);

	std::deque<std::string>& get_output();
	std::deque<std::string>& get_history();
}