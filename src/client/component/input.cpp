#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "game_console.hpp"
#include "gui.hpp"
#include "game/ui_scripting/lua/engine.hpp"

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

		void cl_char_event_stub(const int local_client_num, const int key)
		{
			ui_scripting::lua::engine::ui_event("char", {key});

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
			ui_scripting::lua::engine::ui_event("key", {key, down});

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

		void cl_mouse_move_stub(const int local_client_num, int x, int y)
		{
			if (!gui::gui_mouse_event(local_client_num, x, y))
			{
				return;
			}

			ui_scripting::lua::engine::ui_event("mousemove", {x, y});
			cl_mouse_move_hook.invoke<void>(local_client_num, x, y);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			cl_char_event_hook.create(0x3D27B0_b, cl_char_event_stub);
			cl_key_event_hook.create(0x3D2AE0_b, cl_key_event_stub);
			cl_mouse_move_hook.create(0x3296F0_b, cl_mouse_move_stub);
		}
	};
}

REGISTER_COMPONENT(input::component)
