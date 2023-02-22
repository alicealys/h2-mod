#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "config.hpp"
#include "console.hpp"
#include "language.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>

#define CONFIG_FILE "players2/default/h2_mod.json"

namespace config
{
	namespace
	{
		using validate_type_callback_t = std::function<bool(const field_value&)>;
		using validate_callback_t = std::function<bool(const field_value&)>;

		struct field_definition_t
		{
			field_type type;
			field_value default_value;
			std::optional<validate_callback_t> validate_value = {};
		};

		template <typename... Args>
		std::pair<std::string, field_definition_t> define_field(const std::string& name, Args&&... args)
		{
			return std::make_pair(name, field_definition_t{std::forward<Args>(args)...});
		}

		std::unordered_map<std::string, field_definition_t> field_definitions =
		{
			{define_field("language", config::field_type::string, language::get_default_language(), language::is_valid_language)},
		};
	}
	
	nlohmann::json validate_config_field(const std::string& key, const nlohmann::json& value)
	{
		const auto iter = field_definitions.find(key);
		if (iter == field_definitions.end())
		{
			return value;
		}

		if (value.type() != iter->second.type)
		{
			return iter->second.default_value;
		}

		if (iter->second.validate_value.has_value())
		{
			const auto& validate_value = iter->second.validate_value.value();
			if (!validate_value(value))
			{
				iter->second.default_value;
			}
		}

		return value;
	}

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
