#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "localized_strings.hpp"
#include "scheduler.hpp"
#include "command.hpp"
#include "game/game.hpp"

#include "game/ui_scripting/execution.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <version.h>

namespace branding
{
	namespace
	{
		float color[4] = {0.9f, 0.9f, 0.5f, 1.f};

		int get_build_number_stub(game::hks::lua_State* s)
		{
			ui_scripting::push_value(VERSION);
			return 1;
		}

		void draw()
		{
			const auto font = game::R_RegisterFont("fonts/defaultBold.otf", 22);
			game::R_AddCmdDrawText("h2-mod", 0x7FFFFFFF, font, 15.f,
				15.f + static_cast<float>(font->pixelHeight),
				1.f, 1.f, 0.f, color, 0);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			scheduler::loop(draw, scheduler::pipeline::renderer);

			localized_strings::override("MENU_SP_CAMPAIGN", "H2-MOD");

			utils::hook::jump(0x14033D550, get_build_number_stub, true);
		}
	};
}

REGISTER_COMPONENT(branding::component) 