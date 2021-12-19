#pragma once

namespace gui
{
	bool gui_key_event(const int local_client_num, const int key, const int down);
	bool gui_char_event(const int local_client_num, const int key);
	bool gui_mouse_event(const int local_client_num, int x, int y);
}