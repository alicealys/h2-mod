#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "command.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <version.h>

namespace branding
{
	namespace
	{
		float color[4] = {0.5f, 0.5f, 0.5f, 0.9f};

		utils::hook::detour ui_get_formatted_build_number_hook;

		game::dvar_t* cg_draw_branding = nullptr;

		const char* ui_get_formatted_build_number_stub()
		{
			const auto* const build_num = ui_get_formatted_build_number_hook.invoke<const char*>();
			return utils::string::va("%s (%s)", VERSION, build_num);
		}

		void draw()
		{
			const auto font = game::R_RegisterFont("fonts/fira_mono_regular.ttf", 22);
			if (!font || !cg_draw_branding->current.enabled)
			{
				return;
			}

			const auto placement = game::ScrPlace_GetViewPlacement();

#ifdef DEBUG
			const auto text = "h2-mod " GIT_BRANCH ": " VERSION " (" __DATE__ " " __TIME__ ")";
#else
			const auto text = "h2-mod " GIT_BRANCH ": " VERSION;
#endif

			game::rectDef_s rect{};
			rect.x = 0;
			rect.y = 0;
			rect.w = 500;
			rect.horzAlign = 1;
			rect.vertAlign = 0;

			game::rectDef_s text_rect{};

			game::UI_DrawWrappedText(placement, text, &rect, font,
				5.f, 10.f, 0.12f, color, 0, 0, &text_rect, 0);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			scheduler::loop(draw, scheduler::pipeline::renderer);

			cg_draw_branding = dvars::register_bool("cg_drawBranding", true, game::DVAR_FLAG_SAVED, "Draw h2-mod branding");

			ui_get_formatted_build_number_hook.create(0x1406057D0, ui_get_formatted_build_number_stub);
		}
	};
}

REGISTER_COMPONENT(branding::component) 