#pragma once

namespace gui
{
	extern std::unordered_map<std::string, bool> enabled_menus;

	bool gui_key_event(const int local_client_num, const int key, const int down);
	bool gui_char_event(const int local_client_num, const int key);
	bool gui_mouse_event(const int local_client_num, int x, int y);

	void on_frame(const std::function<void()>& callback);
	bool is_menu_open(const std::string& name);
}