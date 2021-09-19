#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "chat.hpp"
#include "scheduler.hpp"
#include "command.hpp"

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

		game::dvar_t* cg_drawFps;
		game::dvar_t* cg_drawSpeed;

		game::dvar_t* cg_speedGraph;
		game::dvar_t* cg_speedGraphColor;
		game::dvar_t* cg_speedGraphFontColor;
		game::dvar_t* cg_speedGraphBackgroundColor;
		game::dvar_t* cg_speedGraphX;
		game::dvar_t* cg_speedGraphY;
		game::dvar_t* cg_speedGraphWidth;
		game::dvar_t* cg_speedGraphHeight;

		float fps_color_good[4] = {0.6f, 1.0f, 0.0f, 1.0f};
		float fps_color_ok[4] = {1.0f, 0.7f, 0.3f, 1.0f};
		float fps_color_bad[4] = {1.0f, 0.3f, 0.3f, 1.0f};

		float color_blue[4] = {0.12f, 0.77f, 1.f, 1.f};
		float color_white[4] = {1.f, 1.f, 1.f, 1.f};
		float color_red[4] = {1.f, 0.f, 0.f, 1.f};
		float color_black[4] = {0.f, 0.f, 0.f, 1.f};

		float screen_max[2];

		int history_count = 20;
		int history[20] = { 0 };
		int index;

		void check_resize()
		{
			screen_max[0] = game::ScrPlace_GetViewPlacement()->realViewportSize[0];
			screen_max[1] = game::ScrPlace_GetViewPlacement()->realViewportSize[1];
		}
		
		float relative(int x)
		{
			return ((float)x / 1000) * screen_max[0];
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
			game::R_AddCmdDrawStretchPic(x, y, width, height, 0.0f, 0.0f, 0.0f, 0.0f, 
				cg_speedGraphColor->current.vector, material_white);
		}

		std::chrono::high_resolution_clock::time_point last_speed_sample;

		void draw_speed()
		{
			if (cg_drawSpeed->current.integer < 1)
			{
				return;
			}

			const auto speed = (float)sqrt(pow(game::g_entities[0].client->velocity[0], 2) +
										   pow(game::g_entities[0].client->velocity[1], 2) +
										   pow(game::g_entities[0].client->velocity[2], 2));

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

		void draw_box(const float x, const float y, const float w, const float h, float* color)
		{
			game::vec4_t dark_color;

			dark_color[0] = color[0] * 0.5f;
			dark_color[1] = color[1] * 0.5f;
			dark_color[2] = color[2] * 0.5f;
			dark_color[3] = color[3];

			game::R_AddCmdDrawStretchPic(x, y, w, h, 0.0f, 0.0f, 0.0f, 0.0f, color, material_white);
			game::R_AddCmdDrawStretchPic(x, y, 2.0f, h, 0.0f, 0.0f, 0.0f, 0.0f, dark_color, material_white);
			game::R_AddCmdDrawStretchPic((x + w) - 2.0f, y, 2.0f, h, 0.0f, 0.0f, 0.0f, 0.0f, dark_color,
				material_white);
			game::R_AddCmdDrawStretchPic(x, y, w, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, dark_color, material_white);
			game::R_AddCmdDrawStretchPic(x, (y + h) - 2.0f, w, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, dark_color,
				material_white);
		}

		void draw_speed_graph()
		{
			if (cg_speedGraph->current.integer < 1)
			{
				return;
			}

			const auto speed = (float)sqrt(pow(game::g_entities[0].client->velocity[0], 2) +
										   pow(game::g_entities[0].client->velocity[1], 2) +
										   pow(game::g_entities[0].client->velocity[2], 2));

			const auto base_width = relative(cg_speedGraphWidth->current.integer);
			const auto base_height = relative(cg_speedGraphHeight->current.integer);

			const auto max = (int)base_width;

			if (speed_history.size() > max)
			{
				speed_history.pop_front();
			}

			const auto now = std::chrono::high_resolution_clock::now();
			if (now - last_speed_sample >= 10ms)
			{
				last_speed_sample = now;
				speed_history.push_back(speed);
			}

			const auto base_x = relative(cg_speedGraphX->current.integer);
			const auto base_y = screen_max[1] - relative(cg_speedGraphY->current.integer);

			const auto width = 1.f;

			draw_box(base_x, base_y - base_height - 4.f, base_width + 4.f, base_height + 4.f, cg_speedGraphBackgroundColor->current.vector);

			const auto diff = max - speed_history.size();

			const auto start = diff >= 0
				? 0
				: diff * - 1;

			const auto offset = diff > 0
				? diff
				: 0;

			for (auto i = start; i < speed_history.size(); i++)
			{
				const auto percentage = std::min(speed_history[i] / 1000.f, 1.f);
				const auto height = percentage * base_height;

				const auto x = base_x + (float)(offset + i) * width + 2.f;
				const auto y = base_y - height - 2.f;

				draw_line(x, y, width, height);
			}

			const auto speed_string = utils::string::va("%i\n", (int)speed);

			const auto font_height = relative(20);
			const auto font = game::R_RegisterFont("fonts/fira_mono_regular.ttf", (int)font_height);

			const auto text_x = base_x + relative(5);
			const auto text_y = base_y - (base_height / 2.f) + (font_height / 2.f);

			game::R_AddCmdDrawText(speed_string, 0x7FFFFFFF, font, text_x,
				text_y, 1.0f, 1.0f, 0.0f, cg_speedGraphFontColor->current.vector, 0);
		}

		void draw_fps()
		{
			if (cg_drawFps->current.integer < 1)
			{
				return;
			}

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
				fps >= 60 ? fps_color_good : (fps >= 30 ? fps_color_ok : fps_color_bad), 1);
		}

		void draw_pos()
		{
			if (cg_drawFps->current.integer < 2)
			{
				return;
			}

			const auto font = fps_font;

			const auto pos_string = utils::string::va("%f %f %f",
													  game::g_entities[0].origin[0],
													  game::g_entities[0].origin[1],
													  game::g_entities[0].origin[2]);

			const auto x = screen_max[0] - 15.f - game::R_TextWidth(pos_string, 0x7FFFFFFF, font);

			game::R_AddCmdDrawText(pos_string, 0x7FFFFFFF, font, x,
				60.f, 1.0f, 1.0f, 0.0f, fps_color_ok, 0);
		}
	}

	void draw()
	{
		check_resize();
		draw_fps();

		if (!game::CL_IsCgameInitialized() || !game::g_entities[0].client)
		{
			return;
		}

		draw_pos();
		draw_speed();
		draw_speed_graph();
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			cg_drawSpeed = dvars::register_int("cg_drawSpeed", 0, 0, 2, game::DVAR_FLAG_SAVED);
			cg_drawFps = dvars::register_int("cg_drawFPS", 0, 0, 4, game::DVAR_FLAG_SAVED, false);

			cg_speedGraph = dvars::register_bool("cg_speedGraph", false, game::DVAR_FLAG_SAVED);

			cg_speedGraphColor = dvars::register_vec4("cg_speedGraphColor", 1.f, 0.f, 0.f, 1.0f, 0.f, 1.f, game::DVAR_FLAG_SAVED);
			cg_speedGraphFontColor = dvars::register_vec4("cg_speedGraphFontColor", 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, game::DVAR_FLAG_SAVED);
			cg_speedGraphBackgroundColor = dvars::register_vec4("cg_speedGraphBackgroundColor", 0.f, 0.f, 0.f, 0.8f, 0.f, 1.f, game::DVAR_FLAG_SAVED);

			cg_speedGraphX = dvars::register_int("cg_speedGraphX", 15, 0, 1000, game::DVAR_FLAG_SAVED);
			cg_speedGraphY = dvars::register_int("cg_speedGraphY", 15, 0, 1000, game::DVAR_FLAG_SAVED);

			cg_speedGraphWidth = dvars::register_int("cg_speedGraphWidth", 200, 0, 1000, game::DVAR_FLAG_SAVED);
			cg_speedGraphHeight = dvars::register_int("cg_speedGraphHeight", 80, 0, 1000, game::DVAR_FLAG_SAVED);
		}
	};
}

REGISTER_COMPONENT(fps::component)
