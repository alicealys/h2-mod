#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "language.hpp"

#include "localized_strings.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>

#define LANGUAGE_FILE "players2/default/language"

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

		std::unordered_set<game::language_t> cyrillic_languages =
		{
			game::LANGUAGE_RUSSIAN,
			game::LANGUAGE_POLISH,
			game::LANGUAGE_CZECH,
			game::LANGUAGE_RUSSIAN_PARTIAL,
		};


		std::unordered_set<game::language_t> asian_languages =
		{
			game::LANGUAGE_JAPANESE_FULL,
			game::LANGUAGE_JAPANESE_PARTIAL,
			game::LANGUAGE_TRADITIONAL_CHINESE,
			game::LANGUAGE_SIMPLIFIED_CHINESE,
		};

		const char* get_loc_language_string()
		{
			if (!utils::io::file_exists(LANGUAGE_FILE))
			{
				return nullptr;
			}

			static char language[0x200] = {0};
			const auto data = utils::io::read_file(LANGUAGE_FILE);
			strcpy_s(language, data.data());
			return language;
		}
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

	bool is_polrus()
	{
		return cyrillic_languages.contains(current());
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
		utils::io::write_file(LANGUAGE_FILE, lang, false);
	}

	void set_from_index(const int index)
	{
		if (index < 0 || index > 17)
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
		}
	};
}

REGISTER_COMPONENT(language::component)
