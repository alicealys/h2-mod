#pragma once

#include "../event.hpp"

namespace ui_scripting::lua::engine
{
	void start();
	void stop();

	void close_menu(const std::string& name);
	void open_menu(const std::string& name);

	void ui_event(const std::string&, const std::vector<int>&);
	void notify(const event& e);
	void run_frame();
}
