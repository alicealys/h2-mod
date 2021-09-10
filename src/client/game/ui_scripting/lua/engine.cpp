#include <std_include.hpp>
#include "engine.hpp"
#include "context.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>

namespace ui_scripting::lua::engine
{
	namespace
	{
		auto& get_scripts()
		{
			static std::vector<std::unique_ptr<context>> scripts{};
			return scripts;
		}

		void load_scripts()
		{
			const auto script_dir = "ui_scripts/"s;

			if (!utils::io::directory_exists(script_dir))
			{
				return;
			}

			printf("here\n");

			const auto scripts = utils::io::list_files(script_dir);

			for (const auto& script : scripts)
			{
				if (std::filesystem::is_directory(script) && utils::io::file_exists(script + "/__init__.lua"))
				{
					get_scripts().push_back(std::make_unique<context>(script));
				}
			}
		}
	}

	void start()
	{
		get_scripts().clear();
		load_scripts();
	}

	void stop()
	{
		get_scripts().clear();
	}

	void ui_event(const std::string& type, const std::vector<int>& arguments)
	{
		for (auto& script : get_scripts())
		{
			script->ui_event(type, arguments);
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
