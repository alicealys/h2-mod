#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "localized_strings.hpp"
#include "scheduler.hpp"
#include "command.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <version.h>

namespace branding
{
	namespace
	{
		float color[4] = {0.9f, 0.9f, 0.5f, 1.f};

		utils::hook::detour ui_get_formatted_build_number_hook;

		const char* ui_get_formatted_build_number_stub()
		{
			const auto* const build_num = ui_get_formatted_build_number_hook.invoke<const char*>();
			return utils::string::va("%s (%s)", VERSION, build_num);
		}

		void draw()
		{
			const auto font = game::R_RegisterFont("fonts/fira_mono_bold.ttf", 22);
			if (!font)
			{
				return;
			}

			const auto placement = game::ScrPlace_GetViewPlacement();

			game::rectDef_s rect{};
			rect.x = 0;
			rect.y = 0;
			rect.w = 500;
			rect.horzAlign = 1;
			rect.vertAlign = 0;

			game::rectDef_s text_rect{};

			game::UI_DrawWrappedText(placement, "h2-mod", &rect, font,
				5.f, 12.f, 0.17f, color, 0, 0, &text_rect, 0);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			scheduler::loop(draw, scheduler::pipeline::renderer);

			localized_strings::override("MENU_SP_CAMPAIGN", "H2-MOD");

			ui_get_formatted_build_number_hook.create(0x1406057D0, ui_get_formatted_build_number_stub);
		}
	};
}

REGISTER_COMPONENT(branding::component) 