#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "scheduler.hpp"
#include "command.hpp"

#include <utils/string.hpp>

#include <discord_rpc.h>

namespace discord
{
	namespace
	{
		DiscordRichPresence discord_presence;
		std::string state;
		std::optional<std::string> details{};

		void update_discord()
		{
			Discord_RunCallbacks();

			if (!game::CL_IsCgameInitialized())
			{
				state = {};
				details.reset();

				discord_presence.details = game::UI_SafeTranslateString("MENU_MAIN_MENU");
				discord_presence.state = "";

				discord_presence.startTimestamp = 0;

				const auto background_index = static_cast<int>(game::Sys_Milliseconds() / 300000) % 10;
				discord_presence.largeImageKey = utils::string::va("bg_%i", background_index);
			}
			else
			{
				const auto map = game::Dvar_FindVar("mapname")->current.string;
				const auto mapname = game::UI_SafeTranslateString(utils::string::va("PRESENCE_SP_%s", map));

				discord_presence.largeImageKey = map;

				if (details.has_value())
				{
					discord_presence.details = utils::string::va("%s", details.value().data());
				}
				else
				{
					discord_presence.details = mapname;
				}

				discord_presence.state = state.data();

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

				scheduler::once([_state]()
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
			printf("Discord: (%i) %s", error_code, message);
		}
	};
}

REGISTER_COMPONENT(discord::component)