#pragma once

#include "../event.hpp"

namespace scripting::lua::engine
{
	void start();
	void stop();
	void notify(const event& e);
	void handle_endon_conditions(const event& e);
	void run_frame();

	std::optional<std::string> load(const std::string& code);
}
