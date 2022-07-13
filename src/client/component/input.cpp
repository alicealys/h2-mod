#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "game_console.hpp"
#include "gui.hpp"
#include "game/ui_scripting/execution.hpp"

#include <utils/hook.hpp>

namespace input
{
	namespace
	{
		struct point
		{
			short x;
			short y;
		};

		utils::hook::detour cl_char_event_hook;
		utils::hook::detour cl_key_event_hook;
		utils::hook::detour cl_mouse_move_hook;
		utils::hook::detour lui_cod_key_event_hook;

		void cl_char_event_stub(const int local_client_num, const int key)
		{
			if (!game_console::console_char_event(local_client_num, key))
			{
				return;
			}

			if (!gui::gui_char_event(local_client_num, key))
			{
				return;
			}

			cl_char_event_hook.invoke<void>(local_client_num, key);
		}

		void cl_key_event_stub(const int local_client_num, const int key, const int down)
		{
			if (!game_console::console_key_event(local_client_num, key, down))
			{
				return;
			}

			if (!gui::gui_key_event(local_client_num, key, down))
			{
				return;
			}

			cl_key_event_hook.invoke<void>(local_client_num, key, down);
		}

		void lui_cod_key_event_stub(const int local_client_num, const int a2, const int key, const int down, void* a5, void* a6)
		{
			const auto state = *game::hks::lua_state;
			if (game::LUI_BeginCachedEvent(local_client_num, down ? 3 : 4, state))
			{
				const auto key_str = game::Key_KeynumToString(key, 0, 1);
				game::LUI_SetTableInt("keynum", key, state);
				game::LUI_SetTableString("key", key_str, state);
				game::LUI_SetTableString("name", down ? "keydown" : "keyup", state);
				game::LUI_EndEvent(state);
			}

			lui_cod_key_event_hook.invoke<void>(local_client_num, a2, key, down, a5, a6);
		}

		void cl_mouse_move_stub(const int local_client_num, int x, int y)
		{
			if (!gui::gui_mouse_event(local_client_num, x, y))
			{
				return;
			}

			cl_mouse_move_hook.invoke<void>(local_client_num, x, y);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			static const char* lui_cached_events[5] = 
			{
				"process_events",
				"gamepad_button",
				"transition_complete",
				"keydown",
				"keyup",
			};

			utils::hook::inject(0x14031EB8B, lui_cached_events);

			cl_char_event_hook.create(0x1403D27B0, cl_char_event_stub);
			cl_key_event_hook.create(0x1403D2AE0, cl_key_event_stub);
			cl_mouse_move_hook.create(0x1403296F0, cl_mouse_move_stub);
			lui_cod_key_event_hook.create(0x140328F50, lui_cod_key_event_stub);
		}
	};
}

REGISTER_COMPONENT(input::component)
