#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"
#include "game/ui_scripting/execution.hpp"

#include "command.hpp"
#include "console.hpp"
#include "camera.hpp"
#include "gui/gui.hpp"

#include <utils/hook.hpp>
#include <utils/concurrency.hpp>

namespace camera
{
	namespace
	{
		utils::hook::detour sub_140781090_hook;

		game::dvar_t* cl_free_move_scale = nullptr;
		game::dvar_t** cg_paused = nullptr;
		
		using cb_type = std::optional<std::function<void()>>;
		utils::concurrency::container<cb_type> callback;

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

		float camera_origin[3]{};

		auto using_camera_override = false;
		auto using_camera_angles_override = false;

		float camera_origin_override[3]{};
		float camera_angles_override[3]{};

		void paused_free_move()
		{
			if ((*cg_paused)->current.integer != 2)
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

			callback.access([](cb_type& cb)
			{
				if (cb.has_value())
				{
					auto& fn = cb.value();
					fn();
				}
			});

			if (using_camera_angles_override)
			{
				game::cgs->refdefViewAngles[0] = camera_angles_override[0];
				game::cgs->refdefViewAngles[1] = camera_angles_override[1];
				game::cgs->refdefViewAngles[2] = camera_angles_override[2];
			}
			else
			{
				game::cgs->refdefViewAngles[0] = angle_normalize((cmd.angles[0] * 0.000021457672f) + ps->delta_angles[0]);
				game::cgs->refdefViewAngles[1] = angle_normalize((cmd.angles[1] * 0.000021457672f) + ps->delta_angles[1]);
				game::cgs->refdefViewAngles[2] = 0;
			}

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

			game::AnglesToAxis(game::cgs->refdefViewAngles, game::refdef->axis);

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

			if (using_camera_override)
			{
				game::refdef->org[0] = camera_origin_override[0];
				game::refdef->org[1] = camera_origin_override[1];
				game::refdef->org[2] = camera_origin_override[2];
			}
			else
			{
				game::refdef->org[0] = ps->origin[0];
				game::refdef->org[1] = ps->origin[1];
				game::refdef->org[2] = ps->origin[2] + ps->viewHeightCurrent;
			}

			camera_origin[0] = game::refdef->org[0];
			camera_origin[1] = game::refdef->org[1];
			camera_origin[2] = game::refdef->org[2];
		}

		void set_viewpos_now_stub(void* a1)
		{
			paused_free_move();
			utils::hook::invoke<void>(0x1403B07C0, a1);
		}

		void sub_140781090_stub(void* a1, void* a2, float a3)
		{
			sub_140781090_hook.invoke<void>(a1, a2, a3);

			if ((*cg_paused)->current.integer == 2)
			{
				const auto lod_origin = reinterpret_cast<float*>(0x14EEE0890);
				lod_origin[0] = camera_origin[0];
				lod_origin[1] = camera_origin[1];
				lod_origin[2] = camera_origin[2];
			}
		}
	}

	void enable_free_move()
	{
		(*cg_paused)->current.integer = 2;
	}

	void disable_free_move()
	{
		(*cg_paused)->current.integer = 0;
	}

	bool is_free_move_enabled()
	{
		return *cg_paused && (*cg_paused)->current.integer == 2;
	}

	void set_camera_position(const float x, const float y, const float z)
	{
		camera_origin_override[0] = x;
		camera_origin_override[1] = y;
		camera_origin_override[2] = z;
	}

	void set_camera_angles(const float x, const float y, const float z)
	{
		camera_angles_override[0] = x;
		camera_angles_override[1] = y;
		camera_angles_override[2] = z;
	}

	void set_using_origin_override(bool value)
	{
		using_camera_override = value;
	}

	void set_using_angles_override(bool value)
	{
		using_camera_angles_override = value;
	}

	ui_scripting::table get_camera_position()
	{
		ui_scripting::table res;
		res["x"] = camera_origin[0];
		res["y"] = camera_origin[1];
		res["z"] = camera_origin[2];
		return res;
	}

	void set_callback(const ui_scripting::script_value& callback_)
	{
		callback.access([=](cb_type& cb)
		{
			cb = [=]()
			{
				try
				{
					const auto state = *game::hks::lua_state;
					const ui_scripting::table globals = state->globals.v.table;
					const auto pcall = globals["pcall"];
					const auto results = pcall(callback_);

					if (!results[0].as<bool>())
					{
						const auto error = results[1].as<std::string>();
						console::error("Error executing camera callback: %s\n", error.data());
					}
				}
				catch (const std::exception& e)
				{
					console::error("Error executing camera callback: %s\n", e.what());
				}
			};
		});
	}

	void clear_lua()
	{
		callback.access([=](cb_type& cb)
		{
			cb.reset();
		});
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			cl_free_move_scale = dvars::register_float("cl_freemoveScale", 1.f, 0.0001f, 100.f,
				game::DVAR_FLAG_SAVED, "Scale how fast you move in cl_freemove mode");

			cg_paused = reinterpret_cast<game::dvar_t**>(0x141E39FC0);

			utils::hook::call(0x1403ACB2D, set_viewpos_now_stub);
			sub_140781090_hook.create(0x140781090, sub_140781090_stub);
		}
	};
}

REGISTER_COMPONENT(camera::component)
