#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "loadscreen.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>

namespace loadscreen
{
	namespace
	{
		game::dvar_t* cl_disable_map_movies = nullptr;
		game::dvar_t* cl_loadscreen_image = nullptr;
		game::dvar_t* cl_loadscreen_title = nullptr;
		game::dvar_t* cl_loadscreen_desc = nullptr;
		game::dvar_t* cl_loadscreen_obj = nullptr;
		game::dvar_t* cl_loadscreen_obj_icon = nullptr;
		game::dvar_t* cl_enable_custom_loadscreen = nullptr;

		utils::hook::detour ui_draw_loadbar_hook;

		float white_color[4] = {0.8f, 0.8f, 0.8f, 1.f};
		float text_color[4] = {0.65f, 0.65f, 0.65f, 1.f};
		float gray_color[4] = {0.2f, 0.2f, 0.2f, 1.f};
		float icon_yellow_color[4] = {0.86f, 0.81f, 0.34f, 1.f};
		float icon_grey_color[4] = {0.6f, 0.6f, 0.6f, 1.f};

		void draw_loadscreen_gradient()
		{
			const auto placement = game::ScrPlace_GetViewPlacement();
			const auto gradient = game::Material_RegisterHandle("gradient");
			const auto black = game::Material_RegisterHandle("black");

			game::CL_DrawStretchPic(placement, -750, -200, 600, 800, 2, 1, 0.0f, 0.0f, 1.0f, 1.0f, white_color, black);
			game::CL_DrawStretchPic(placement, -150, -200, 300, 800, 2, 1, 0.0f, 0.0f, 1.0f, 1.0f, white_color, gradient);
		}

		void draw_image(const float x, const float y, const float w, const float h, 
			game::Material* mat)
		{
			game::rectangle rect{};
			rect.p0.x = x;
			rect.p0.y = 0;
			rect.p0.f2 = 0.f;
			rect.p0.f3 = 1.f;

			rect.p1.x = x + w;
			rect.p1.y = 0;
			rect.p1.f2 = 0.f;
			rect.p1.f3 = 1.f;

			rect.p2.x = x + w;
			rect.p2.y = 0 + h;
			rect.p2.f2 = 0.f;
			rect.p2.f3 = 1.f;

			rect.p3.x = x;
			rect.p3.y = 0 + h;
			rect.p3.f2 = 0.f;
			rect.p3.f3 = 1.f;

			game::R_DrawRectangle(&rect, 0.f, 0.f, 1.f, 1.f,
				white_color, mat);
		}

		void draw_loadscreen_image()
		{
			if (*cl_loadscreen_image->current.string == 0)
			{
				return;
			}

			const auto material = game::Material_RegisterHandle(cl_loadscreen_image->current.string);
			const auto black = game::Material_RegisterHandle("black");
			const auto placement = game::ScrPlace_GetViewPlacement();

			const auto h = placement->realViewportSize[1];
			const auto real_w = placement->realViewportSize[0];
			const auto w = h * 16 / 9;
			const auto x = real_w / 2 - w / 2;

			draw_image(0, 0, real_w, h, black);
			draw_image(x, 0, w, h, material);
		}

		void draw_loadscreen_progress_bar()
		{
			const auto fraction = utils::hook::invoke<float>(0x140287E30);

			const auto w = 290.f;
			const auto w_progress = w * fraction;

			const auto material = game::Material_RegisterHandle("white");
			const auto placement = game::ScrPlace_GetViewPlacement();

			game::CL_DrawStretchPic(placement, -20, 290, w, 3, 0, 0, 0.0f, 0.0f, 1.0f, 1.0f, gray_color, material);
			game::CL_DrawStretchPic(placement, -20, 290, w_progress, 3, 0, 0, 0.0f, 0.0f, 1.0f, 1.0f, white_color, material);
		}

		void draw_loadscreen_title()
		{
			auto x = -20.f;
			auto y = 288.f;
			auto h = 24.f;
			auto w = 0.f;

			const auto placement = game::ScrPlace_GetViewPlacement();
			game::ScrPlace_ApplyRect(placement, &x, &y, &w, &h, 0, 0);

			const auto font = game::R_RegisterFont("fonts/default.otf", static_cast<int>(h));
			game::R_AddCmdDrawText(cl_loadscreen_title->current.string, 0x7FFFFFFF, font, x, y, 1.f, 1.f, 0.f, text_color, 0);
		}

		void draw_loadscreen_desc()
		{
			const auto font = game::R_RegisterFont("fonts/default.otf", 20);
			const auto placement = game::ScrPlace_GetViewPlacement();
			const auto text = cl_loadscreen_desc->current.string;

			game::rectDef_s rect{};
			rect.x = 0;
			rect.y = 0;
			rect.w = 290;
			rect.horzAlign = 0;
			rect.vertAlign = 0;

			game::rectDef_s text_rect{};

			game::UI_DrawWrappedText(placement, text, &rect, font, -20, 310, 0.25f, text_color, 0, 0, &text_rect, 0);
		}

		void draw_loadscreen_objective_icons()
		{
			if (*cl_loadscreen_obj_icon->current.string == 0)
			{
				return;
			}

			const auto material = game::Material_RegisterHandle(cl_loadscreen_obj_icon->current.string);
			const auto placement = game::ScrPlace_GetViewPlacement();

			const auto w = 15.f;
			const auto base_y = 365.f;
			const auto base_x = -20.f;

			for (auto row = 0; row < 3; row++)
			{
				for (auto column = 0; column < 3; column++)
				{
					auto x = base_x + column * w;
					auto y = base_y + row * w + 2;

					const auto color = column <= row ? icon_yellow_color : icon_grey_color;
					game::CL_DrawStretchPic(placement, x, y, w, w, 0, 0, 0.f, 0.f, 1.f, 1.f, color, material);
				}
			}
		}

		void draw_loadscreen_objective()
		{
			if (*cl_loadscreen_obj->current.string == 0)
			{
				return;
			}

			draw_loadscreen_objective_icons();

			const auto font = game::R_RegisterFont("fonts/default.otf", 20);
			const auto placement = game::ScrPlace_GetViewPlacement();
			const auto text = cl_loadscreen_obj->current.string;

			game::rectDef_s rect{};
			rect.x = 0;
			rect.y = 0;
			rect.w = 290.f;
			rect.horzAlign = 0;
			rect.vertAlign = 0;

			game::rectDef_s text_rect{};

			game::UI_DrawWrappedText(placement, text, &rect, font, 30.f, 365.f + 17.5f, 0.25f, text_color, 0, 0, &text_rect, 0);
		}

		void draw_loadscreen()
		{
			if (cl_loadscreen_image == nullptr || cl_loadscreen_title == nullptr ||
				cl_loadscreen_desc == nullptr || !cl_enable_custom_loadscreen->current.enabled)
			{
				return;
			}

			draw_loadscreen_image();
			draw_loadscreen_gradient();
			draw_loadscreen_progress_bar();
			draw_loadscreen_title();
			draw_loadscreen_desc();
			draw_loadscreen_objective();
		}

		bool in_loadscreen()
		{
			return *reinterpret_cast<int*>(0x14203F3C4) == 4;
		}

		void ui_set_active_menu_stub(utils::hook::assembler& a)
		{
			const auto player_start = a.newLabel();

			a.mov(rax, qword_ptr(reinterpret_cast<uint64_t>(&cl_disable_map_movies)));
			a.mov(al, byte_ptr(rax, 0x10));
			a.cmp(al, 1);
			a.jz(player_start);

			a.mov(rax, qword_ptr(static_cast<uint64_t>(0x14BE6EA10)));
			a.mov(al, byte_ptr(rax, 0x10));
			a.cmp(al, 1);
			a.jz(player_start);

			a.jmp(0x1405F4701);

			a.bind(player_start);
			a.call(0x1405F1B00);
			a.jmp(0x1405F44F2);
		}
	}

	void clear()
	{
		game::Dvar_Reset(cl_enable_custom_loadscreen, game::DVAR_SOURCE_INTERNAL);
		game::Dvar_Reset(cl_disable_map_movies, game::DVAR_SOURCE_INTERNAL);
		game::Dvar_Reset(cl_loadscreen_image, game::DVAR_SOURCE_INTERNAL);
		game::Dvar_Reset(cl_loadscreen_title, game::DVAR_SOURCE_INTERNAL);
		game::Dvar_Reset(cl_loadscreen_desc, game::DVAR_SOURCE_INTERNAL);
	}

	class component final : public component_interface
	{
	public:
		void post_start() override
		{
			scheduler::loop([]()
			{
				if (in_loadscreen())
				{
					draw_loadscreen();
				}
			}, scheduler::pipeline::renderer);
		}

		void post_unpack() override
		{
			// not registered, used in CL_StartLoading
			cl_disable_map_movies = dvars::register_bool("cl_disableMapMovies", false, 0, "Disable map loading videos");

			cl_enable_custom_loadscreen = dvars::register_bool("cl_enableCustomLoadscreen", false, 0, "Use custom loadscreen dvars");

			// auto start the game if cl_disableMapMovies is enabled
			utils::hook::jump(0x1405F46EA, utils::hook::assemble(ui_set_active_menu_stub), true);

			scheduler::once([]()
			{
				cl_loadscreen_image = dvars::register_string("cl_loadscreenImage", "", 0, "Loadscreen background image");
				cl_loadscreen_title = dvars::register_string("cl_loadscreenTitle", "", 0, "Loadscreen mission title");
				cl_loadscreen_desc = dvars::register_string("cl_loadscreenDesc", "", 0, "Loadscreen mission description");
				cl_loadscreen_obj = dvars::register_string("cl_loadscreenObj", "", 0, "Loadscreen mission objective");
				cl_loadscreen_obj_icon = dvars::register_string("cl_loadscreenObjIcon", "", 0, "Loadscreen mission objective icon");
			}, scheduler::pipeline::main);
		}
	};
}

REGISTER_COMPONENT(loadscreen::component)
