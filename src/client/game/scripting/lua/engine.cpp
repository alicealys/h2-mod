#include <std_include.hpp>
#include "engine.hpp"
#include "context.hpp"

#include "../../../component/notifies.hpp"
#include "../execution.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>

namespace scripting::lua::engine
{
	namespace
	{
		auto& get_scripts()
		{
			static std::vector<std::unique_ptr<context>> scripts{};
			return scripts;
		}

		void load_generic_script()
		{
			get_scripts().push_back(std::make_unique<context>());
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
					get_scripts().push_back(std::make_unique<context>(script));
				}
			}
		}
	}

	void start()
	{
		clear_custom_fields();
		get_scripts().clear();

		load_generic_script();

		load_scripts("scripts/");
		load_scripts("h2-mod/scripts/");
		load_scripts("data/scripts/");

		if (!game::mod_folder.empty())
		{
			load_scripts(utils::string::va("%s/scripts/", game::mod_folder.data()));
		}
	}

	void stop()
	{
		notifies::clear_callbacks();
		get_scripts().clear();
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

	std::optional<std::string> load(const std::string& code)
	{
		if (get_scripts().size() == 0)
		{
			return {};
		}

		const auto& script = get_scripts()[0];
		return {script->load(code)};
	}
}
