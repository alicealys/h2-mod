#include <stdinc.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "chat.hpp"
#include "scheduler.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

#define chat_font game::R_RegisterFont("fonts/fira_mono_regular.ttf", 25)

namespace chat
{
	namespace
	{
		struct message
		{
			std::string text;
			std::chrono::steady_clock::time_point time;
		};

		std::deque<message> history;

		float color_white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		float screen_max[2];

		void check_resize()
		{
			screen_max[0] = game::ScrPlace_GetViewPlacement()->realViewportSize[0];
			screen_max[1] = game::ScrPlace_GetViewPlacement()->realViewportSize[1];
		}

		float relative(float value)
		{
			const auto ratio = screen_max[0] / 2560.f;

			return value * ratio;
		}

		void draw_chat()
		{
			check_resize();

			const auto now = std::chrono::high_resolution_clock::now();

			for (auto i = 0; i < std::min(15, (int)history.size()); i++)
			{
				if (now - history[i].time > 11s)
				{
					return;
				}

				const auto diff = now - history[i].time;

				float color[4] = { color_white[0], color_white[1], color_white[2], 1.f };

				if (diff > 10.5s)
				{
					const auto milliseconds = (float)(11000 - std::chrono::duration_cast<std::chrono::milliseconds>(diff).count());

					color[3] = (float)(milliseconds / 500.f);
				}

				game::R_AddCmdDrawText(history[i].text.data(), 0x7FFFFFFF, chat_font, relative(15.f), relative(600.f + i * 25), 1.f, 1.f, 0.f, color, 0);
			}
		}
	}

	void print(const std::string& msg)
	{
		message m;
		m.text = msg;
		m.time = std::chrono::high_resolution_clock::now();

		history.push_front(m);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			scheduler::loop([]()
			{
				draw_chat();
			}, scheduler::pipeline::renderer);
		}
	};
}

REGISTER_COMPONENT(chat::component)
