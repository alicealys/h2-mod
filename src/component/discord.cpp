#include <stdinc.hpp>
#include "loader/component_loader.hpp"
#include "scheduler.hpp"
#include "game/game.hpp"

#include <utils/string.hpp>

#include <discord_rpc.h>

namespace discord
{
	namespace
	{
		DiscordRichPresence discord_presence;

		void update_discord()
		{
			Discord_RunCallbacks();

			if (!game::CL_IsCgameInitialized())
			{
				discord_presence.details = "Main Menu";
				discord_presence.state = "";

				discord_presence.startTimestamp = 0;

				discord_presence.largeImageKey = "h2";
			}
			else
			{
				const auto map = game::Dvar_FindVar("mapname")->current.string;
				const auto mapname = game::UI_SafeTranslateString(utils::string::va("PRESENCE_SP_%s", map));

				discord_presence.details = mapname;
				discord_presence.state = "";

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
		}

	private:
		bool initialized_ = false;

		static void ready(const DiscordUser* /*request*/)
		{
			ZeroMemory(&discord_presence, sizeof(discord_presence));

			discord_presence.instance = 1;

			Discord_UpdatePresence(&discord_presence);
		}

		static void errored(const int error_code, const char* message)
		{
			printf("Discord: (%i) %s", error_code, message);
		}
	};
}

REGISTER_COMPONENT(discord::component)