#include <std_include.hpp>
#include "engine.hpp"
#include "context.hpp"

#include "../../../component/scheduler.hpp"
#include "../../../component/ui_scripting.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>
#include <utils/nt.hpp>

namespace ui_scripting::lua::engine
{
	namespace
	{
		const auto updater_script = utils::nt::load_resource(LUI_UPDATER_MENU);

		void handle_key_event(const int key, const int down)
		{
			event event;
			event.name = down
				? "keydown"
				: "keyup";
			event.arguments = {key};

			engine::notify(event);
		}

		void handle_char_event(const int key)
		{
			std::string key_str = {(char)key};
			event event;
			event.name = "keypress";
			event.arguments = {key_str};

			engine::notify(event);
		}

		auto& get_scripts()
		{
			static std::vector<std::unique_ptr<context>> scripts{};
			return scripts;
		}

		void load_scripts(const std::string& script_dir)
		{
			if (!utils::io::directory_exists(script_dir))
			{
				return;
			}

			const auto scripts = utils::io::list_files(script_dir);

			for (const auto& script : scripts)
			{
				if (std::filesystem::is_directory(script) && utils::io::file_exists(script + "/__init__.lua"))
				{
					get_scripts().push_back(std::make_unique<context>(script, script_type::file));
				}
			}
		}

		void load_code(const std::string& code)
		{
			get_scripts().push_back(std::make_unique<context>(code, script_type::code));
		}
	}

	void start()
	{
		clear_converted_functions();
		get_scripts().clear();

		load_code(updater_script);

		load_scripts("ui_scripts/");
		load_scripts("h2-mod/ui_scripts/");
		load_scripts("data/ui_scripts/");

		if (!game::mod_folder.empty())
		{
			load_scripts(utils::string::va("%s/ui_scripts/", game::mod_folder.data()));
		}
	}

	void stop()
	{
		clear_converted_functions();
		get_scripts().clear();
	}

	void ui_event(const std::string& type, const std::vector<int>& arguments)
	{
		if (type == "key")
		{
			handle_key_event(arguments[0], arguments[1]);
		}

		if (type == "char")
		{
			handle_char_event(arguments[0]);
		}
	}

	void notify(const event& e)
	{
		for (auto& script : get_scripts())
		{
			script->notify(e);
		}
	}

	void run_frame()
	{
		for (auto& script : get_scripts())
		{
			script->run_frame();
		}
	}
}
