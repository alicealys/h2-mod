#pragma once

#pragma warning(push)
#pragma warning(disable: 4702)

#define SOL_ALL_SAFETIES_ON 1
#define SOL_PRINT_ERRORS 0
#include <sol/sol.hpp>

#include "../menu.hpp"
#include "event.hpp"
#include "scheduler.hpp"
#include "event_handler.hpp"

namespace ui_scripting::lua
{
	void open_menu(const std::string& name);
	void close_menu(const std::string& name);

	class context
	{
	public:
		context(std::string folder);
		~context();

		context(context&&) noexcept = delete;
		context& operator=(context&&) noexcept = delete;

		context(const context&) = delete;
		context& operator=(const context&) = delete;

		void run_frame();
		void ui_event(const std::string&, const std::vector<int>&);

	private:
		sol::state state_{};
		std::string folder_;
		std::unordered_set<std::string> loaded_scripts_;

		scheduler scheduler_;
		event_handler event_handler_;

		void load_script(const std::string& script);
	};
}
