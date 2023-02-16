#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "config.hpp"
#include "console.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>

#define CONFIG_FILE "players2/default/h2_mod.json"

namespace config
{
	void write_config(const nlohmann::json& json)
	{
		try
		{
			const auto str = json.dump(4);
			utils::io::write_file(CONFIG_FILE, str, false);
		}
		catch (const std::exception& e)
		{
			console::error("Failed to write config file: %s\n", e.what());
		}
	}

	nlohmann::json read_config()
	{
		if (!utils::io::file_exists(CONFIG_FILE))
		{
			return {};
		}

		try
		{
			const auto data = utils::io::read_file(CONFIG_FILE);
			return nlohmann::json::parse(data);
		}
		catch (const std::exception& e)
		{
			console::error("Failed to parse config file: %s\n", e.what());
			utils::io::write_file(CONFIG_FILE, "{}", false);
		}

		return {};
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{

		}
	};
}

REGISTER_COMPONENT(config::component)
