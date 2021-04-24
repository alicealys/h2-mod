#include <stdinc.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "chat.hpp"
#include "scheduler.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

#define fps_font game::R_RegisterFont("fonts/fira_mono_regular.ttf", 25)

namespace fps
{
	namespace
	{
		std::chrono::nanoseconds frametime;
		auto lastframe = std::chrono::high_resolution_clock::now();

		float fps_color_good[4] = { 0.6f, 1.0f, 0.0f, 1.0f };
		float fps_color_ok[4] = { 1.0f, 0.7f, 0.3f, 1.0f };
		float fps_color_bad[4] = { 1.0f, 0.3f, 0.3f, 1.0f };

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

		void draw_fps()
		{
			check_resize();

			const auto now = std::chrono::high_resolution_clock::now();
			const auto frametime = now - lastframe;
			lastframe = now;

			const int fps = 1000000000 / frametime.count();

			if (index >= history_count)
			{
				index = 0;
			}

			history[index++] = fps;

			const auto fps_string = utils::string::va("%i", average_fps());
			const auto x = screen_max[0] - 15.f - game::R_TextWidth(fps_string, 0x7FFFFFFF, fps_font);

			game::R_AddCmdDrawText(fps_string, 0x7FFFFFFF, fps_font, x, 25.f, 1.0f, 1.0f, 0.0f,
				fps >= 60 ? fps_color_good : (fps >= 30 ? fps_color_ok : fps_color_bad), 0);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			scheduler::loop(draw_fps, scheduler::pipeline::renderer);
		}
	};
}

REGISTER_COMPONENT(fps::component)
