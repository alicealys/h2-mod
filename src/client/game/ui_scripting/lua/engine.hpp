#pragma once

#include "../event.hpp"

namespace ui_scripting::lua::engine
{
	void start();
	void stop();

	void ui_event(const std::string&, const std::vector<int>&);
	void notify(const event& e);
	void run_frame();
}
