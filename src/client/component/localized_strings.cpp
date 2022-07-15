#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "localized_strings.hpp"
#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/concurrency.hpp>
#include "game/game.hpp"

#include "game_console.hpp"

namespace localized_strings
{
	namespace
	{
		utils::hook::detour seh_string_ed_get_string_hook;

		using localized_map = std::unordered_map<std::string, std::string>;
		utils::concurrency::container<localized_map> localized_overrides;

		const char* seh_string_ed_get_string(const char* reference)
		{
			return localized_overrides.access<const char*>([&](const localized_map& map)
			{
				const auto entry = map.find(reference);
				if (entry != map.end())
				{
					return utils::string::va("%s", entry->second.data());
				}

				return seh_string_ed_get_string_hook.invoke<const char*>(reference);
			});
		}

		game::XAssetHeader db_find_localize_entry_stub(game::XAssetType type, const char* name, int allow_create_default)
		{
			const auto value = localized_overrides.access<const char*>([&](const localized_map& map)
				-> const char*
			{
				const auto entry = map.find(name);
				if (entry != map.end())
				{
					return utils::string::va("%s", entry->second.data());
				}

				return nullptr;
			});

			if (value == nullptr)
			{
				return game::DB_FindXAssetHeader(type, name, allow_create_default);
			}

			static game::LocalizeEntry entry{};
			entry.value = value;
			entry.name = name;

			return static_cast<game::XAssetHeader>(&entry);
		}
	}

	void override(const std::string& key, const std::string& value)
	{
		localized_overrides.access([&](localized_map& map)
		{
			map[key] = value;
		});
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Change some localized strings
			seh_string_ed_get_string_hook.create(0x1405E5FD0, &seh_string_ed_get_string);
			utils::hook::call(0x1405E5AB9, db_find_localize_entry_stub);
		}
	};
}

REGISTER_COMPONENT(localized_strings::component) 