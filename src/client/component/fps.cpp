#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

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
		game::dvar_t* cg_speedGraphIncludeZAxis;

		game::dvar_t* cg_drawGameTime;

		float fps_color_good[4] = {0.6f, 1.0f, 0.0f, 1.0f};
		float fps_color_ok[4] = {1.0f, 0.7f, 0.3f, 1.0f};
		float fps_color_bad[4] = {1.0f, 0.3f, 0.3f, 1.0f};

		float color_blue[4] = {0.12f, 0.77f, 1.f, 1.f};
		float color_white[4] = {1.f, 1.f, 1.f, 1.f};
		float color_red[4] = {1.f, 0.f, 0.f, 1.f};
		float color_black[4] = {0.f, 0.f, 0.f, 1.f};

		float screen_max[2];

		std::deque<float> speed_history;

		utils::hook::detour sub_7C55D0_hook;

		struct cg_perf_data
		{
			std::chrono::time_point<std::chrono::steady_clock> perf_start;
			std::int32_t current_ms;
			std::int32_t previous_ms;
			std::int32_t frame_ms;
			std::int32_t history[32];
			std::int32_t count;
			std::int32_t index;
			std::int32_t instant;
			std::int32_t total;
			float average;
			float variance;
			std::int32_t min;
			std::int32_t max;
		};

		cg_perf_data cg_perf{};

		void perf_calc_fps(cg_perf_data* data, const std::int32_t value)
		{
			data->history[data->index % 32] = value;
			data->instant = value;
			data->min = 0x7FFFFFFF;
			data->max = 0;
			data->average = 0.0f;
			data->variance = 0.0f;
			data->total = 0;

			for (auto i = 0; i < data->count; ++i)
			{
				const std::int32_t idx = (data->index - i) % 32;

				if (idx < 0)
				{
					break;
				}

				data->total += data->history[idx];

				if (data->min > data->history[idx])
				{
					data->min = data->history[idx];
				}

				if (data->max < data->history[idx])
				{
					data->max = data->history[idx];
				}
			}

			data->average = static_cast<float>(data->total) / static_cast<float>(data->count);
			++data->index;
		}

		void perf_update()
		{
			cg_perf.count = 32;

			cg_perf.current_ms = static_cast<std::int32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now() - cg_perf.perf_start).count());
			cg_perf.frame_ms = cg_perf.current_ms - cg_perf.previous_ms;
			cg_perf.previous_ms = cg_perf.current_ms;

			perf_calc_fps(&cg_perf, cg_perf.frame_ms);

			sub_7C55D0_hook.invoke<void>();
		}

		void check_resize()
		{
			screen_max[0] = game::ScrPlace_GetViewPlacement()->realViewportSize[0];
			screen_max[1] = game::ScrPlace_GetViewPlacement()->realViewportSize[1];
		}
		
		float relative(int x)
		{
			return (static_cast<float>(x) / 1000) * screen_max[0];
		}

		void draw_line(float x, float y, float width, float height)
		{
			game::R_AddCmdDrawStretchPic(x, y, width, height, 0.0f, 0.0f, 0.0f, 0.0f, 
				cg_speedGraphColor->current.vector, material_white);
		}

		std::chrono::high_resolution_clock::time_point last_speed_sample;

		void draw_speed()
		{
			if (!cg_drawSpeed->current.enabled)
			{
				return;
			}

			const auto speed = static_cast<float>(sqrt(
				pow(game::g_clients[0].velocity[0], 2) +
				pow(game::g_clients[0].velocity[1], 2) +
				pow(game::g_clients[0].velocity[2], 2)
			));

			const auto font = speed_font;
			const auto speed_string = utils::string::va("%i\n", static_cast<int>(speed));

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
			game::vec4_t dark_color{};

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
			if (!cg_speedGraph->current.enabled)
			{
				return;
			}

			const auto speed = static_cast<float>(sqrt(
				pow(game::g_clients[0].velocity[0], 2) +
				pow(game::g_clients[0].velocity[1], 2) +
				(cg_speedGraphIncludeZAxis->current.enabled ? pow(game::g_clients[0].velocity[2], 2) : 0)
			));

			const auto base_width = relative(cg_speedGraphWidth->current.integer);
			const auto base_height = relative(cg_speedGraphHeight->current.integer);
			const auto max = static_cast<int>(base_width);

			if (static_cast<int>(speed_history.size()) > max)
			{
				speed_history.pop_front();
			}

			const auto now = std::chrono::high_resolution_clock::now();
			if (now - last_speed_sample >= 10ms)
			{
				last_speed_sample = now;
				speed_history.push_back(speed);
			}

			auto max_speed = 0.f;
			for (const auto& value : speed_history)
			{
				if (value > max_speed)
				{
					max_speed = value;
				}
			}

			const auto base_x = relative(cg_speedGraphX->current.integer);
			const auto base_y = screen_max[1] - relative(cg_speedGraphY->current.integer);
			const auto width = 1.f;

			draw_box(base_x, base_y - base_height - 4.f, base_width + 5.f, 
				base_height + 4.f, cg_speedGraphBackgroundColor->current.vector);

			const auto diff = max - static_cast<int>(speed_history.size());

			for (auto i = 0; i < speed_history.size(); i++)
			{
				const auto percentage = std::min(speed_history[i] / std::max(500.f, max_speed), 1.f);
				const auto height = percentage * base_height;

				const auto x = base_x + static_cast<float>(diff + i) * width + 2.f;
				const auto y = base_y - height - 2.f;

				draw_line(x + 1.f, y, width, height);
			}

			const auto speed_string = utils::string::va("%i (%i)\n", 
				static_cast<int>(speed), static_cast<int>(max_speed));

			const auto font_height = relative(15);
			const auto font = game::R_RegisterFont("fonts/default.otf", static_cast<int>(font_height));

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

			const auto fps = static_cast<std::int32_t>(static_cast<float>(1000.0f / static_cast<float>(cg_perf.
				average))
				+ 9.313225746154785e-10);

			const auto font = fps_font;
			const auto fps_string = utils::string::va("%i", fps);
			const auto x = screen_max[0] - 15.f - game::R_TextWidth(fps_string, 0x7FFFFFFF, font);
			const auto color = fps >= 60 ? fps_color_good : (fps >= 30 ? fps_color_ok : fps_color_bad);

			game::R_AddCmdDrawText(fps_string, 0x7FFFFFFF, font, x, 35.f, 1.0f, 1.0f, 0.0f, color, 1);
		}

		void draw_pos()
		{
			if (cg_drawFps->current.integer < 2)
			{
				return;
			}

			const auto font = fps_font;
			const auto pos_string = utils::string::va("%f %f %f",
				game::refdef->org[0],
				game::refdef->org[1],
				game::refdef->org[2]);

			const auto x = screen_max[0] - 15.f - game::R_TextWidth(pos_string, 0x7FFFFFFF, font);

			game::R_AddCmdDrawText(pos_string, 0x7FFFFFFF, font, x,
				60.f, 1.0f, 1.0f, 0.0f, fps_color_ok, 0);
		}

		void draw_game_time()
		{
			if (!cg_drawGameTime->current.enabled)
			{
				return;
			}

			auto msec = *game::gameTime;

			// remove first frames
			if (msec >= 300)
			{
				msec -= 300;
			}

			const auto ms = static_cast<int>((msec % 1000) / 10);
			const auto s = static_cast<int>(msec / 1000) % 60;
			const auto m = static_cast<int>(msec / 1000 / 60);
			const auto text = utils::string::va("%d:%02d.%02d", m, s, ms);

			const auto height = relative(24);
			const auto base_width = relative(cg_speedGraphWidth->current.integer);
			const auto base_height = cg_speedGraph->current.enabled ? relative(cg_speedGraphHeight->current.integer) : 0;

			const auto base_x = relative(cg_speedGraphX->current.integer);
			const auto base_y = screen_max[1] - relative(cg_speedGraphY->current.integer) - base_height - height
				- (cg_speedGraph->current.enabled ? 2.f : 0.f);

			const auto font_height = relative(20);
			const auto font = game::R_RegisterFont("fonts/default.otf", static_cast<int>(font_height));
			const auto text_width = game::R_TextWidth(text, 0x7FFFFFFF, font);

			draw_box(base_x, base_y, base_width + 5.f,
				height, cg_speedGraphBackgroundColor->current.vector);

			const auto text_y = base_y + (height / 2.f) + ((font_height + relative(5)) / 2.f);

			game::R_AddCmdDrawText(text, 0x7FFFFFFF, font, base_x + base_width - text_width,
				text_y, 1.0f, 1.0f, 0.0f, cg_speedGraphFontColor->current.vector, 0);
		}

		void draw()
		{
			if (*dvars::cg_draw_2d && !(*dvars::cg_draw_2d)->current.enabled)
			{
				return;
			}

			check_resize();
			draw_fps();

			if (!game::CL_IsCgameInitialized() || game::g_entities[0].client == nullptr)
			{
				return;
			}

			draw_pos();
			draw_speed();
			draw_speed_graph();
			draw_game_time();
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			scheduler::loop(draw, scheduler::pipeline::renderer);

			sub_7C55D0_hook.create(0x1407C55D0, perf_update);

			cg_drawSpeed = dvars::register_bool("cg_drawSpeed", 0, game::DVAR_FLAG_SAVED, "Draw speed");
			cg_drawFps = dvars::register_int("cg_drawFPS", 0, 0, 4, game::DVAR_FLAG_SAVED, "Draw fps");

			cg_speedGraph = dvars::register_bool("cg_speedGraph", 0, game::DVAR_FLAG_SAVED, "Enable speed graph");

			cg_speedGraphColor = dvars::register_vec4("cg_speedGraphColor", 
				1.f, 0.f, 0.f, 1.0f, 0.f, 1.f, game::DVAR_FLAG_SAVED, "Speed graph color");
			cg_speedGraphFontColor = dvars::register_vec4("cg_speedGraphFontColor", 
				1.f, 1.f, 1.f, 1.f, 0.f, 1.f, game::DVAR_FLAG_SAVED, "Speed graph font color");
			cg_speedGraphBackgroundColor = dvars::register_vec4("cg_speedGraphBackgroundColor", 
				0.f, 0.f, 0.f, 0.8f, 0.f, 1.f, game::DVAR_FLAG_SAVED, "Speed graph background color");

			cg_speedGraphX = dvars::register_int("cg_speedGraphX", 15, 0, 1000, game::DVAR_FLAG_SAVED, "Speed graph x position");
			cg_speedGraphY = dvars::register_int("cg_speedGraphY", 15, 0, 1000, game::DVAR_FLAG_SAVED, "Speed graph y position");

			cg_speedGraphWidth = dvars::register_int("cg_speedGraphWidth", 200, 0, 1000, game::DVAR_FLAG_SAVED, "Speed graph width");
			cg_speedGraphHeight = dvars::register_int("cg_speedGraphHeight", 80, 0, 1000, game::DVAR_FLAG_SAVED, "Speed graph height");

			cg_speedGraphIncludeZAxis = dvars::register_bool("cg_speedGraphIncludeZAxis", false, game::DVAR_FLAG_SAVED, 
				"Include velocity on the z axis when calculating the speed");

			cg_drawGameTime = dvars::register_bool("cg_drawGameTime", false, game::DVAR_FLAG_SAVED, "Draw game time");
		}
	};
}

REGISTER_COMPONENT(fps::component)
