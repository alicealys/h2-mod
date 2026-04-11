#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"
#include "dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace patches
{
	namespace
	{
		utils::hook::detour gscr_set_save_dvar_hook;
		utils::hook::detour dvar_register_float_hook;

		utils::hook::detour com_frame_hook;

		game::dvar_t* com_wait_end_frame_mode = nullptr;

		DECLSPEC_NORETURN void quit_stub()
		{
			utils::hook::invoke<void>(0x1408B1BA0);
		}

		void gscr_set_save_dvar_stub()
		{
			const auto dvar = game::Scr_GetString(0);
			if (dvar == nullptr)
			{
				return;
			}

			const auto string = utils::string::to_lower(dvar);
			if (string == "cg_fov" || string == "cg_fovscale")
			{
				return;
			}

			gscr_set_save_dvar_hook.invoke<void>();
		}

		void vid_restart_stub()
		{
			// free stuff
			*reinterpret_cast<void**>(0x141E584D8) = nullptr; // free material
			utils::hook::invoke<void>(0x1406926B0); // free scripted viewmodel xanim stuff
			utils::hook::invoke<void>(0x14032A540); // properly free lui memory

			utils::hook::invoke<void>(0x1405A6480);
		}

		void exec_config_stub(void* a1)
		{
			dvars::register_string("name", "Unknown Soldier", game::DVAR_FLAG_SAVED, "Player name");
			utils::hook::invoke<void>(0x1405A35E0, a1);
		}

		const char* dvar_get_hash_stub(game::dvar_t* dvar)
		{
			const auto info = dvars::get_dvar_info_from_hash(dvar->hash);
			if (info.has_value())
			{
				const auto& value = info.value();
				return utils::string::va("%s", value.name.data());
			}
			else
			{
				return utils::hook::invoke<const char*>(0x140619240, dvar);
			}
		}

		void r_process_workers_with_timeout_stub(void* a1, void* a2)
		{
			if (com_wait_end_frame_mode->current.enabled)
			{
				return;
			}

			utils::hook::invoke<void>(0x140793DE0, a1, a2);
		}

		void com_frame_stub()
		{
			const auto value = com_wait_end_frame_mode->current.integer;
			if (value == 0)
			{
				return com_frame_hook.invoke<void>();
			}

			const auto start = std::chrono::high_resolution_clock::now();
			com_frame_hook.invoke<void>();

			auto max_fps = (*dvars::com_max_fps)->current.integer;
			if (max_fps == 0)
			{
				max_fps = 1000;
			}

			constexpr auto nano_secs = std::chrono::duration_cast<std::chrono::nanoseconds>(1s);
			const auto frame_time = nano_secs / max_fps;

			if (value == 1)
			{
				const auto diff = (std::chrono::high_resolution_clock::now() - start);
				if (diff > frame_time)
				{
					return;
				}

				const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(frame_time - diff);
				std::this_thread::sleep_for(ms);
			}
			else if (value == 2)
			{
				while (std::chrono::high_resolution_clock::now() - start < frame_time)
				{
					std::this_thread::sleep_for(0ms);
				}
			}
		}

		void set_timer_resolution()
		{
			ULONG data{};
			const utils::nt::library ntdll("ntdll.dll");
			ntdll.invoke_pascal<void>("NtSetTimerResolution", 5000, TRUE, &data);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Fix shutdown crash
			utils::hook::jump(0x1408B1CD0, 0x1408B1BA0);

			// Disable splash screen
			utils::hook::nop(0x14064F546, 5);

			// Unlock fps in main menu
			utils::hook::set<BYTE>(0x1403D8E1B, 0xEB);

			// Allow kbam input when gamepad is enabled
			utils::hook::nop(0x1403D2F8E, 2);
			utils::hook::nop(0x1403D0C9C, 6);

			// Prevent game from overriding cg_fov and cg_fovscale values
			gscr_set_save_dvar_hook.create(0x140504C60, &gscr_set_save_dvar_stub);

			// fix vid_restart crashing
			utils::hook::call(0x1403D7413, vid_restart_stub);

			// cinematicingameloopresident -> cinematicingameloop (fix ingame cinematics)
			utils::hook::jump(0x140502140, 0x1405020C0);

			// override dvar flags
			dvars::override::register_float("cg_fovScale", 1.f, 0.1f, 2.f, game::DVAR_FLAG_SAVED);
			dvars::override::register_float("cg_fov", 65.f, 40.f, 200.f, game::DVAR_FLAG_SAVED);
			dvars::override::register_bool("snd_musicDisabledForCustomSoundtrack", false, game::DVAR_FLAG_SAVED);

			// make "name" saved
			utils::hook::call(0x1405A4960, exec_config_stub);
			dvars::override::register_string("name", "Unknown Soldier", game::DVAR_FLAG_SAVED);

			utils::hook::call(0x1405A7CB7, dvar_get_hash_stub);

			// fix vehicle hud compass color
			utils::hook::set<float>(0x140948F40, 1.f);
			utils::hook::set<float>(0x140948F44, 1.f);
			utils::hook::set<float>(0x140948F48, 1.f);

			utils::hook::nop(0x14037B8AE, 7);

			dvars::register_bool("g_soldierPronouns", false, game::DVAR_FLAG_SAVED, "Show soldier pronouns");

			// unlock safeArea_*
			utils::hook::jump(0x1403DEEEA, 0x1403DEEF8);
			utils::hook::jump(0x1403DEF11, 0x1403DEF3C);
			dvars::override::register_float("safeArea_horizontal", 1.f, 0.f, 1.f, game::DVAR_FLAG_SAVED);
			dvars::override::register_float("safeArea_vertical", 1.f, 0.f, 1.f, game::DVAR_FLAG_SAVED);
			dvars::override::register_float("safeArea_adjusted_horizontal", 1.f, 0.f, 1.f, game::DVAR_FLAG_SAVED);
			dvars::override::register_float("safeArea_adjusted_vertical", 1.f, 0.f, 1.f, game::DVAR_FLAG_SAVED);

			// Make fps capping accurate
			com_wait_end_frame_mode = dvars::register_int("com_waitEndFrameMode", 0, 0, 2, game::DVAR_FLAG_SAVED, "Wait end frame mode (0 = default, 1 = sleep(n), 2 = loop sleep(0)");
			utils::hook::call(0x1405A38B9, r_process_workers_with_timeout_stub);
			com_frame_hook.create(0x1405A3740, com_frame_stub);

			set_timer_resolution();
		}
	};
}

REGISTER_COMPONENT(patches::component)
