#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "scheduler.hpp"
#include "command.hpp"
#include "console.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

#include <discord_rpc.h>

namespace discord
{
	namespace
	{
		DiscordRichPresence discord_presence;

		struct
		{
			std::optional<std::string> state{};
			std::optional<std::string> details{};
		} custom_strings;

		void update_discord_frontend()
		{
			custom_strings.state.reset();
			custom_strings.details.reset();

			discord_presence.details = game::UI_SafeTranslateString("MENU_MAIN_MENU");
			discord_presence.state = "";

			discord_presence.startTimestamp = 0;

			const auto background_index = static_cast<int>(game::Sys_Milliseconds() / 300000) % 10;
			const auto background_image = std::format("bg_{}", background_index);
			discord_presence.largeImageKey = background_image.data();

			Discord_UpdatePresence(&discord_presence);
		}

		void update_discord_ingame()
		{
			static const auto mapname_dvar = game::Dvar_FindVar("mapname");
			static const auto museum_mode_dvar = game::Dvar_FindVar("ui_char_museum_mode");

			auto mapname = mapname_dvar->current.string;
			auto map_image = mapname;
			const auto museum_mode = museum_mode_dvar->current.string;

			const char* base_mapname = nullptr;
			if (game::Com_IsAddonMap(mapname, &base_mapname))
			{
				map_image = base_mapname;
			}

			if (museum_mode == "free"s)
			{
				map_image = "museum";
				mapname = "MUSEUM";
			}

			const auto key = std::format("PRESENCE_SP_{}", mapname);
			if (game::DB_XAssetExists(game::ASSET_TYPE_LOCALIZE_ENTRY, key.data()) && 
				!game::DB_IsXAssetDefault(game::ASSET_TYPE_LOCALIZE_ENTRY, key.data()))
			{
				mapname = game::UI_SafeTranslateString(key.data());
			}

			discord_presence.largeImageKey = map_image;

			if (custom_strings.details.has_value())
			{
				const auto& details_value = custom_strings.details.value();
				if (details_value.starts_with("@") && details_value.size() > 1)
				{
					const auto loc_string = details_value.substr(1);
					const auto value = game::UI_SafeTranslateString(loc_string.data());
					discord_presence.details = value;
				}
				else
				{
					discord_presence.details = details_value.data();
				}
			}
			else
			{
				discord_presence.details = mapname;
			}

			const auto state_str = custom_strings.state.value_or("");
			discord_presence.state = state_str.data();

			if (discord_presence.startTimestamp == 0)
			{
				discord_presence.startTimestamp = std::chrono::duration_cast<std::chrono::seconds>(
					std::chrono::system_clock::now().time_since_epoch()).count();
			}

			Discord_UpdatePresence(&discord_presence);
		}

		void update_discord()
		{
			Discord_RunCallbacks();

			if (!game::CL_IsCgameInitialized())
			{
				update_discord_frontend();
			}
			else
			{
				update_discord_ingame();
			}
		}

		void ready(const DiscordUser* /*request*/)
		{
			DiscordRichPresence presence{};
			presence.instance = 1;
			presence.state = "";

			Discord_UpdatePresence(&presence);
		}

		void errored(const int error_code, const char* message)
		{
			console::error("Discord: (%i) %s", error_code, message);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			DiscordEventHandlers handlers{};
			handlers.ready = ready;
			handlers.errored = errored;
			handlers.disconnected = errored;

			Discord_Initialize("835690302583996416", &handlers, 1, nullptr);

			scheduler::loop(update_discord, scheduler::pipeline::async, 5s);

			command::add("setdiscordstate", [](const command::params& params)
			{
				const std::string state = params.join(1);
				scheduler::once([=]()
				{
					custom_strings.state.emplace(state);
					update_discord();
				}, scheduler::pipeline::async);
			});

			command::add("setdiscorddetails", [](const command::params& params)
			{
				const std::string details = params.join(1);
				scheduler::once([=]()
				{
					custom_strings.details.emplace(details);
					update_discord();
				}, scheduler::pipeline::async);
			});
		}

	};
}

REGISTER_COMPONENT(discord::component)