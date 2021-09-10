#pragma once

namespace ui_scripting::lua::engine
{
	void start();
	void stop();

	void ui_event(const std::string&, const std::vector<int>&);
	void run_frame();
}
