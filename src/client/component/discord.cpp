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

		std::optional<std::string> state{};
		std::optional<std::string> details{};

		void update_discord()
		{
			static char details_buf[128] = {0};
			static char state_buf[128] = {0};
			static char image_key_buf[32] = {0};

			Discord_RunCallbacks();

			if (!game::CL_IsCgameInitialized())
			{
				state.reset();
				details.reset();

				discord_presence.details = game::UI_SafeTranslateString("MENU_MAIN_MENU");
				discord_presence.state = "";

				discord_presence.startTimestamp = 0;

				const auto background_index = static_cast<int>(game::Sys_Milliseconds() / 300000) % 10;
				strcpy_s(image_key_buf, sizeof(image_key_buf), utils::string::va("bg_%i", background_index));
				discord_presence.largeImageKey = image_key_buf;
			}
			else
			{
				const char* base_mapname = nullptr;
				auto* map = game::Dvar_FindVar("mapname")->current.string;
				auto* museum_mode = game::Dvar_FindVar("ui_char_museum_mode")->current.string;

				if (game::Com_IsAddonMap(map, &base_mapname))
				{
					map = base_mapname;
				}

				if (museum_mode == "free"s)
				{
					map = "museum";
				}
				
				const auto mapname = game::UI_SafeTranslateString(utils::string::va("PRESENCE_SP_%s", map));
				discord_presence.largeImageKey = map;

				if (details.has_value())
				{
					const auto& details_ = details.value();
					if (details_.starts_with("@") && details_.size() > 1)
					{
						const auto value = game::UI_SafeTranslateString(details_.substr(1).data());
						discord_presence.details = value;
					}
					else
					{
						strcpy_s(details_buf, sizeof(details_buf), utils::string::va("%s", details_.data()));
						discord_presence.details = details_buf;
					}
				}
				else
				{
					discord_presence.details = mapname;
				}

				if (state.has_value())
				{
					strcpy_s(state_buf, sizeof(state_buf), state.value().data());
					discord_presence.state = state_buf;
				}
				else
				{
					discord_presence.state = "";
				}


				if (!discord_presence.startTimestamp)
				{
					discord_presence.startTimestamp = std::chrono::duration_cast<std::chrono::seconds>(
						std::chrono::system_clock::now().time_since_epoch()).count();
				}
			}

			Discord_UpdatePresence(&discord_presence);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			DiscordEventHandlers handlers;
			ZeroMemory(&handlers, sizeof(handlers));
			handlers.ready = ready;
			handlers.errored = errored;
			handlers.disconnected = errored;
			handlers.joinGame = nullptr;
			handlers.spectateGame = nullptr;
			handlers.joinRequest = nullptr;

			Discord_Initialize("835690302583996416", &handlers, 1, nullptr);

			scheduler::loop(update_discord, scheduler::pipeline::async, 5s);

			initialized_ = true;

			command::add("setdiscordstate", [](const command::params& params)
			{
				const std::string _state = params.join(1);
				scheduler::once([=]()
				{
					state = _state;
					update_discord();
				}, scheduler::pipeline::async);
			});

			command::add("setdiscorddetails", [](const command::params& params)
			{
				const std::string details_ = params.join(1);
				scheduler::once([=]()
				{
					details = details_;
					update_discord();
				}, scheduler::pipeline::async);
			});
		}

	private:
		bool initialized_ = false;

		static void ready(const DiscordUser* /*request*/)
		{
			ZeroMemory(&discord_presence, sizeof(discord_presence));

			discord_presence.instance = 1;
			discord_presence.state = "";

			Discord_UpdatePresence(&discord_presence);
		}

		static void errored(const int error_code, const char* message)
		{
			console::error("Discord: (%i) %s", error_code, message);
		}
	};
}

REGISTER_COMPONENT(discord::component)