#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "command.hpp"
#include "console.hpp"

#include <utils/hook.hpp>

namespace camera
{
	namespace
	{
		game::dvar_t* cl_free_move_scale = nullptr;

		float angle_normalize(const float angle)
		{
			const auto value = ((angle * 0.0027777778f) - std::floorf(angle * 0.0027777778f)) * 360.f;
			auto result = value - 360.f;

			if ((value - 360.f) < 0.f)
			{
				result = value;
			}

			return result;
		}

		void vec_cross(float* a1, float* a2, float* a3)
		{
			a3[0] = (a2[2] * a1[1]) - (a2[1] * a1[2]);
			a3[1] = (a1[2] * a2[0]) - (a2[2] * a1[0]);
			a3[2] = (a2[1] * a1[0]) - (a1[1] * a2[0]);
		}

		void paused_free_move()
		{
			const auto cg_paused = *reinterpret_cast<game::dvar_t**>(0x141E39FC0);
			if (cg_paused->current.integer != 2)
			{
				return;
			}

			constexpr auto local_client_num = 0;
			const auto ps = game::CG_GetPredictedPlayerState(local_client_num);
			const auto cmd_number = game::CL_GetCurrentCmdNumber(local_client_num);

			game::usercmd_s cmd{};
			if (!game::CL_GetUserCmd(local_client_num, cmd_number, &cmd))
			{
				return;
			}

			float viewangles[3]{};
			viewangles[0] = angle_normalize((cmd.angles[0] * 0.000021457672f) + ps->delta_angles[0]);
			viewangles[1] = angle_normalize((cmd.angles[1] * 0.000021457672f) + ps->delta_angles[1]);
			viewangles[2] = angle_normalize((cmd.angles[2] * 0.000021457672f) + ps->delta_angles[2]);

			const auto game_time = game::CG_GetGameTime(local_client_num);

			static auto saved_game_time = 0;
			static auto saved_ms = 0;

			if (saved_game_time != game_time)
			{
				saved_game_time = game_time;
				saved_ms = game::Sys_Milliseconds();
			}

			const auto now = game::Sys_Milliseconds();
			const auto diff = now - saved_ms;
			saved_ms = game::Sys_Milliseconds();

			auto scale = 1.f;
			if ((cmd.buttons & 0x6) != 0)
			{
				scale = 10.f;
			}

			scale *= cl_free_move_scale->current.value;

			game::AnglesToAxis(viewangles, game::refdef->axis);

			float v1[3] = {0.f, 0.f, 1.f};
			float v2[3] = {game::refdef->axis[0][0], game::refdef->axis[0][1], game::refdef->axis[0][2]};
			float v3[3]{};

			vec_cross(v1, v2, v3);
			vec_cross(v3, v1, v2);

			if (cmd.forwardmove)
			{
				ps->origin[0] += scale * (game::refdef->axis[0][0] * (static_cast<float>(cmd.forwardmove) * 0.2f) * (static_cast<float>(diff) * 0.05f));
				ps->origin[1] += scale * (game::refdef->axis[0][1] * (static_cast<float>(cmd.forwardmove) * 0.2f) * (static_cast<float>(diff) * 0.05f));
				ps->origin[2] += scale * (game::refdef->axis[0][2] * (static_cast<float>(cmd.forwardmove) * 0.2f) * (static_cast<float>(diff) * 0.05f));
			}

			if (cmd.rightmove)
			{
				ps->origin[0] += scale * (v3[0] * (static_cast<float>(-1 * cmd.rightmove) * 0.2f) * (static_cast<float>(diff) * 0.05f));
				ps->origin[1] += scale * (v3[1] * (static_cast<float>(-1 * cmd.rightmove) * 0.2f) * (static_cast<float>(diff) * 0.05f));
				ps->origin[2] += scale * (v3[2] * (static_cast<float>(-1 * cmd.rightmove) * 0.2f) * (static_cast<float>(diff) * 0.05f));
			}

			if ((cmd.buttons & 0x1) != 0)
			{
				ps->origin[2] += scale * 10.f;
			}

			if ((cmd.buttons & 0x800) != 0)
			{
				ps->origin[2] -= scale * 10.f;
			}

			game::refdef->org[0] = ps->origin[0];
			game::refdef->org[1] = ps->origin[1];
			game::refdef->org[2] = ps->origin[2] + ps->viewHeightCurrent;
		}

		void set_viewpos_now_stub(void* a1)
		{
			paused_free_move();
			utils::hook::invoke<void>(0x1403B07C0, a1);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			cl_free_move_scale = dvars::register_float("cl_freemoveScale", 1.f, 0.01f, 100.f,
				game::DVAR_FLAG_SAVED, "Scale how fast you move in cl_freemove mode");

			utils::hook::call(0x1403ACB2D, set_viewpos_now_stub);
		}
	};
}

REGISTER_COMPONENT(camera::component)
