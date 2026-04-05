#pragma once

namespace gui
{
	struct notification_t
	{
		std::string title;
		std::string text;
		std::chrono::milliseconds duration{};
		std::chrono::high_resolution_clock::time_point creation_time{};
	};

	extern std::unordered_map<std::string, bool> enabled_menus;

	bool gui_key_event(const int local_client_num, const int key, const int down);
	bool gui_char_event(const int local_client_num, const int key);
	bool gui_mouse_event(const int local_client_num, int x, int y);

	void on_frame(const std::function<void()>& callback, bool always = false);
	bool is_menu_open(const std::string& name);
	void notification(const std::string& title, const std::string& text, const std::chrono::milliseconds duration = 3s);
	void copy_to_clipboard(const std::string& text);

	void register_menu(const std::string& name, const std::string& title, 
		const std::function<void()>& callback, bool always = false);

	void register_callback(const std::function<void()>& callback, bool always = false);

	void shutdown_gui();
	void set_device(ID3D11Device* device, ID3D11DeviceContext* context);

	void input_flags8(std::uint8_t* flags, const std::vector<const char*>& flag_names);
	void input_flags(std::uint32_t* flags, const std::vector<const char*>& flag_names);
	void input_flags(int* flags, const std::vector<const char*>& flag_names);

	bool input_u8(const char* label, unsigned char* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);
	bool input_u16(const char* label, unsigned short* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);
}