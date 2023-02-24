#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "language.hpp"
#include "config.hpp"

#include "localized_strings.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>

#define OLD_LANGUAGE_FILE "players2/default/language"

namespace language
{
	namespace
	{
		std::unordered_set<game::language_t> non_latin_languages =
		{
			{game::LANGUAGE_RUSSIAN},
			{game::LANGUAGE_POLISH},
			{game::LANGUAGE_JAPANESE_FULL},
			{game::LANGUAGE_JAPANESE_PARTIAL},
			{game::LANGUAGE_TRADITIONAL_CHINESE},
			{game::LANGUAGE_SIMPLIFIED_CHINESE},
			{game::LANGUAGE_ARABIC},
			{game::LANGUAGE_CZECH},
			{game::LANGUAGE_KOREAN},
			{game::LANGUAGE_RUSSIAN_PARTIAL},
		};

		std::unordered_set<game::language_t> slavic_languages =
		{
			{game::LANGUAGE_RUSSIAN},
			{game::LANGUAGE_POLISH},
			{game::LANGUAGE_CZECH},
			{game::LANGUAGE_RUSSIAN_PARTIAL},
		};

		std::unordered_set<game::language_t> asian_languages =
		{
			{game::LANGUAGE_JAPANESE_FULL},
			{game::LANGUAGE_JAPANESE_PARTIAL},
			{game::LANGUAGE_TRADITIONAL_CHINESE},
			{game::LANGUAGE_SIMPLIFIED_CHINESE},
			{game::LANGUAGE_KOREAN},
		};

		std::unordered_set<game::language_t> custom_languages =
		{
			{game::LANGUAGE_CZECH},
		};

		const char* get_loc_language_string()
		{
			const auto data = config::get<std::string>("language");
			if (!data.has_value())
			{
				return nullptr;
			}

			static char language[0x200] = {0};
			const auto& value = data.value();
			strcpy_s(language, value.data());
			return language;
		}
	}

	std::string get_default_language()
	{
		return "english";
	}

	bool is_valid_language(const std::string& name)
	{
		const auto lower = utils::string::to_lower(name);
		for (auto i = 0; i < game::LANGUAGE_COUNT; i++)
		{
			if (game::languages[i].name == lower)
			{
				return true;
			}
		}

		return false;
	}

	bool is_custom_language(const std::string& name)
	{
		for (const auto& language : custom_languages)
		{
			if (game::languages[language].name == name || game::languages[language].shortname == name)
			{
				return true;
			}
		}

		return false;
	}

	game::language_t current()
	{
		static auto* loc_language = game::Dvar_FindVar("loc_language");
		return static_cast<game::language_t>(loc_language->current.integer);
	}

	bool is_non_latin()
	{
		return non_latin_languages.contains(current());
	}

	bool is_slavic()
	{
		return slavic_languages.contains(current());
	}

	bool is_arabic()
	{
		return current() == game::LANGUAGE_ARABIC;
	}

	bool is_asian()
	{
		return asian_languages.contains(current());
	}

	void set(const std::string& lang)
	{
		config::set("language", lang);
	}

	void set_from_index(const int index)
	{
		if (index < 0 || index >= game::LANGUAGE_COUNT)
		{
			return;
		}

		const auto language = game::languages[index];
		set(language.name);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::call(0x14060AFFB, get_loc_language_string);

			if (utils::io::file_exists(OLD_LANGUAGE_FILE))
			{
				const auto lang = utils::io::read_file(OLD_LANGUAGE_FILE);
				config::set("language", lang);
				utils::io::remove_file(OLD_LANGUAGE_FILE);
			}

			for (const auto& language : custom_languages)
			{
				game::languages[language].is_supported = 1;
			}
		}
	};
}

REGISTER_COMPONENT(language::component)
