#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "config.hpp"
#include "console.hpp"
#include "language.hpp"
#include "updater.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <utils/properties.hpp>

#define OLD_CONFIG_FILE "players2/default/h2_mod.json"

namespace config
{
	namespace
	{
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
			{define_field("disable_custom_fonts", field_type::boolean, false)},
			{define_field("language", field_type::string, language::get_default_language(), language::is_valid_language)},
			{define_field("motd_last_seen", field_type::number_unsigned, 0)},
			{define_field("branch", field_type::string, updater::get_git_branch(), updater::is_valid_git_branch)},
		};

		std::string get_config_file_path()
		{
			return (utils::properties::get_appdata_path() / "config.json").generic_string();
		}
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

	std::optional<nlohmann::json> get_default_value(const std::string& key)
	{
		const auto iter = field_definitions.find(key);
		if (iter == field_definitions.end())
		{
			return {};
		}

		return {iter->second.default_value};
	}

	nlohmann::json get_raw(const std::string& key)
	{
		const auto cfg = read_config();
		if (!cfg.is_object() || !cfg.contains(key))
		{
			const auto default_value = get_default_value(key);
			if (default_value.has_value())
			{
				return default_value.value();
			}

			return {};
		}

		return validate_config_field(key, cfg[key]);
	}

	void write_config(const nlohmann::json& json)
	{
		try
		{
			const auto path = get_config_file_path();
			const auto str = json.dump(4);
			utils::io::write_file(path, str, false);
		}
		catch (const std::exception& e)
		{
			console::error("Failed to write config file: %s\n", e.what());
		}
	}

	nlohmann::json read_config()
	{
		const auto path = get_config_file_path();
		if (!utils::io::file_exists(path))
		{
			return {};
		}

		try
		{
			const auto data = utils::io::read_file(path);
			return nlohmann::json::parse(data);
		}
		catch (const std::exception& e)
		{
			console::error("Failed to parse config file: %s\n", e.what());
			utils::io::write_file(path, "{}", false);
		}

		return {};
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			if (utils::io::file_exists(OLD_CONFIG_FILE))
			{
				const auto data = utils::io::read_file(OLD_CONFIG_FILE);
				utils::io::write_file(get_config_file_path(), data);
				utils::io::remove_file(OLD_CONFIG_FILE);
			}
		}
	};
}

REGISTER_COMPONENT(config::component)
