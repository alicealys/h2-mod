#pragma once

#include "../event.hpp"
#include "../menu.hpp"

#pragma warning(push)
#pragma warning(disable: 4702)

#define SOL_ALL_SAFETIES_ON 1
#define SOL_PRINT_ERRORS 0
#include <sol/sol.hpp>

#include "scheduler.hpp"
#include "event_handler.hpp"

namespace ui_scripting::lua
{
	enum script_type
	{
		file,
		code
	};

	extern std::unordered_map<std::string, menu> menus;
	extern std::vector<element*> elements;
	extern element ui_element;
	extern int mouse[2];

	class context
	{
	public:
		context(std::string data, script_type);
		~context();

		context(context&&) noexcept = delete;
		context& operator=(context&&) noexcept = delete;

		context(const context&) = delete;
		context& operator=(const context&) = delete;

		void run_frame();
		void notify(const event& e);

	private:
		sol::state state_{};
		std::string folder_;
		std::unordered_set<std::string> loaded_scripts_;

		scheduler scheduler_;
		event_handler event_handler_;

		void load_script(const std::string& script);
	};
}
