#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "language.hpp"
#include "config.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>

#define OLD_LANGUAGE_FILE "players2/default/language"

namespace language
{
	game::language_values languages[game::LANGUAGE_COUNT + 1]{};

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
			{game::LANGUAGE_TURKISH},
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
			if (languages[i].name == lower)
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
			if (languages[language].name == name || languages[language].shortname == name)
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

	bool is_custom()
	{
		return custom_languages.contains(current());
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

		const auto language = languages[index];
		set(language.name);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::call(0x14060AFFB, get_loc_language_string);

			std::memcpy(languages, game::languages.get(), sizeof(game::language_values) * game::LANGUAGE_COUNT_ORIGINAL);
			languages[game::LANGUAGE_TURKISH].name = "turkish";
			languages[game::LANGUAGE_TURKISH].shortname = "tur";
			languages[game::LANGUAGE_TURKISH].prefix1 = "tr";
			languages[game::LANGUAGE_TURKISH].prefix2 = "tr";
			languages[game::LANGUAGE_TURKISH].prefix3 = "tr";
			languages[game::LANGUAGE_TURKISH].is_supported = 1;

			utils::hook::inject(0x1405E3B67 + 3, &languages[game::LANGUAGE_ENGLISH].name);
			utils::hook::inject(0x1405E5217 + 3, &languages[game::LANGUAGE_ENGLISH].name);
			utils::hook::inject(0x1405E5223 + 3, &languages[game::LANGUAGE_ENGLISH].name);
			utils::hook::inject(0x1405E524D + 3, &languages[game::LANGUAGE_ENGLISH].name);
			utils::hook::inject(0x1405E5398 + 3, &languages[game::LANGUAGE_ENGLISH].name);
			utils::hook::inject(0x1405E53AB + 3, &languages[game::LANGUAGE_ENGLISH].name);
			utils::hook::inject(0x1405E53E8 + 3, &languages[game::LANGUAGE_ENGLISH].name);
			utils::hook::inject(0x1405E6273 + 3, &languages[game::LANGUAGE_ENGLISH].name);

			utils::hook::inject(0x1405E5197 + 3, &languages[game::LANGUAGE_ENGLISH].shortname);
			utils::hook::inject(0x1405E51A3 + 3, &languages[game::LANGUAGE_ENGLISH].shortname);
			utils::hook::inject(0x1405E58B2 + 3, &languages[game::LANGUAGE_ENGLISH].shortname);
			utils::hook::inject(0x1405E58BF + 3, &languages[game::LANGUAGE_ENGLISH].shortname);
			utils::hook::inject(0x1405E6144 + 3, &languages[game::LANGUAGE_ENGLISH].shortname);
			utils::hook::inject(0x1405E6189 + 3, &languages[game::LANGUAGE_ENGLISH].shortname);

			utils::hook::inject(0x1405E51E7 + 3, &languages[game::LANGUAGE_ENGLISH].prefix2);
			utils::hook::inject(0x1405E51F3 + 3, &languages[game::LANGUAGE_ENGLISH].prefix2);

			utils::hook::inject(0x1405E5157 + 3, &languages[game::LANGUAGE_ENGLISH].prefix3);
			utils::hook::inject(0x1405E5163 + 3, &languages[game::LANGUAGE_ENGLISH].prefix3);

			utils::hook::inject(0x1405E5470 + 3, &languages[game::LANGUAGE_ENGLISH].is_supported);
			utils::hook::inject(0x1405E57FA + 3, &languages[game::LANGUAGE_ENGLISH].is_supported);
			utils::hook::inject(0x1405E612F + 3, &languages[game::LANGUAGE_ENGLISH].is_supported);

			utils::hook::inject(0x1405E5D79 + 3, &languages[game::LANGUAGE_GERMAN].is_supported);

			utils::hook::inject(0x1405E5E3B + 3, &languages[game::LANGUAGE_ENGLISH_SAFE].is_supported);

			utils::hook::inject(0x1405E5257 + 3, &languages[game::LANGUAGE_COUNT].name);

			utils::hook::set<std::uint8_t>(0x140342B59 + 2, game::LANGUAGE_COUNT);
			utils::hook::set<std::uint8_t>(0x1405E5390 + 2, game::LANGUAGE_COUNT - 1);
			utils::hook::set<std::uint8_t>(0x1405E518B + 2, game::LANGUAGE_COUNT - 1);
			utils::hook::set<std::uint8_t>(0x1405E520B + 2, game::LANGUAGE_COUNT - 1);

			utils::hook::set<std::uint32_t>(0x1405E556C + 4, game::LANGUAGE_COUNT - 1);
			utils::hook::set<std::uint32_t>(0x1403D7480 + 4, game::LANGUAGE_COUNT - 1);

			for (const auto& language : custom_languages)
			{
				languages[language].is_supported = 1;
			}
		}
	};
}

REGISTER_COMPONENT(language::component)
