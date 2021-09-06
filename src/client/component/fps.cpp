#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "chat.hpp"
#include "scheduler.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

#define fps_font game::R_RegisterFont("fonts/fira_mono_regular.ttf", 25)
#define speed_font game::R_RegisterFont("fonts/fira_mono_regular.ttf", 50)
#define material_white game::Material_RegisterHandle("white")

namespace fps
{
	namespace
	{
		auto lastframe = std::chrono::high_resolution_clock::now();

		game::dvar_t* cg_drawfps;
		game::dvar_t* cg_drawspeed;

		float fps_color_good[4] = {0.6f, 1.0f, 0.0f, 1.0f};
		float fps_color_ok[4] = {1.0f, 0.7f, 0.3f, 1.0f};
		float fps_color_bad[4] = {1.0f, 0.3f, 0.3f, 1.0f};

		float color_blue[4] = {0.12f, 0.77f, 1.f, 1.f};

		float color_white[4] = {1.f, 1.f, 1.f, 1.f};

		float screen_max[2];

		int history_count = 20;
		int history[20] = { 0 };
		int index;

		void check_resize()
		{
			screen_max[0] = game::ScrPlace_GetViewPlacement()->realViewportSize[0];
			screen_max[1] = game::ScrPlace_GetViewPlacement()->realViewportSize[1];
		}

		int average_fps()
		{
			auto total = 0;

			for (auto i = 0; i < history_count; i++)
			{
				total += history[i];
			}

			return total / history_count;
		}

		std::deque<float> speed_history;

		void draw_line(float x, float y, float width, float height)
		{
			game::R_AddCmdDrawStretchPic(x, y, width, height, 0.0f, 0.0f, 0.0f, 0.0f, color_white, material_white);
		}

		void draw_speed()
		{
			if (!game::CL_IsCgameInitialized() || !game::g_entities[0].client)
			{
				return;
			}

			const auto speed = (float)sqrt(pow(game::g_entities[0].client->velocity[0], 2) +
										   pow(game::g_entities[0].client->velocity[1], 2) +
									       pow(game::g_entities[0].client->velocity[2], 2));

			if (cg_drawspeed->current.integer >= 1)
			{
				const auto font = speed_font;
				const auto speed_string = utils::string::va("%i\n", (int)speed);

				const auto width = game::R_TextWidth(speed_string, 0x7FFFFFFF, font);

				const auto x = (screen_max[0] / 2) - (width / 2);
				const auto y = screen_max[1] - 400.f;

				const auto color = speed >= 300.f
					? color_blue
					: fps_color_bad;

				game::R_AddCmdDrawText(speed_string, 0x7FFFFFFF, font, x,
					y, 1.0f, 1.0f, 0.0f, color, 0);
			}

			if (cg_drawspeed->current.integer >= 2)
			{
				if (speed_history.size() > 400)
				{
					speed_history.pop_front();
				}

				speed_history.push_back(speed);

				const auto base_x = 80.f;
				const auto base_y = screen_max[1] - 400.f;

				const auto width = 1.f;

				for (auto i = 0; i < speed_history.size(); i++)
				{
					const auto height = std::min(std::max(speed_history[i] / 5.f, 1.f), 1000.f);

					const auto x = base_x + (float)i * width;
					const auto y = base_y - height;

					draw_line(x, y, width, height);
				}
			}
		}

		void draw()
		{
			check_resize();

			if (cg_drawfps->current.integer >= 1)
			{
				const auto now = std::chrono::high_resolution_clock::now();
				const auto frametime = now - lastframe;
				lastframe = now;

				const int fps = (int)(1000000000 / (int)frametime.count());

				if (index >= history_count)
				{
					index = 0;
				}

				history[index++] = fps;

				const auto fps_string = utils::string::va("%i", average_fps());
				const auto x = screen_max[0] - 15.f - game::R_TextWidth(fps_string, 0x7FFFFFFF, fps_font);

				game::R_AddCmdDrawText(fps_string, 0x7FFFFFFF, fps_font, x, 35.f, 1.0f, 1.0f, 0.0f,
					fps >= 60 ? fps_color_good : (fps >= 30 ? fps_color_ok : fps_color_bad), 0);
			}

			if (!game::CL_IsCgameInitialized() || !game::g_entities[0].client)
			{
				return;
			}

			if (cg_drawfps->current.integer >= 2)
			{
				const auto font = fps_font;

				const auto pos_string = utils::string::va("%f %f %f",
														  game::g_entities[0].origin[0],
														  game::g_entities[0].origin[1],
														  game::g_entities[0].origin[2]);

				const auto x = screen_max[0] - 15.f - game::R_TextWidth(pos_string, 0x7FFFFFFF, font);

				game::R_AddCmdDrawText(pos_string, 0x7FFFFFFF, font, x,
					60.f, 1.0f, 1.0f, 0.0f, fps_color_ok, 0);
			}

			draw_speed();
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			cg_drawspeed = dvars::register_int("cg_drawSpeed", 0, 0, 2, game::DVAR_FLAG_SAVED);
			cg_drawfps = dvars::register_int("cg_drawFPS", 0, 0, 4, game::DVAR_FLAG_SAVED, false);

			scheduler::loop(draw, scheduler::pipeline::renderer);
		}
	};
}

REGISTER_COMPONENT(fps::component)
