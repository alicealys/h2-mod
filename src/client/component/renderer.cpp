#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "component/command.hpp"
#include "component/scheduler.hpp"

#include <utils/hook.hpp>

namespace renderer
{
	namespace
	{
		utils::hook::detour r_init_draw_method_hook;
		utils::hook::detour r_update_front_end_dvar_options_hook;

		int get_fullbright_technique()
		{
			switch (dvars::r_fullbright->current.integer)
			{
			case 3:
				return 3;
			case 2:
				return 13;
			default:
				return game::TECHNIQUE_UNLIT;
			}
		}

		void gfxdrawmethod()
		{
			game::gfxDrawMethod->drawScene = game::GFX_DRAW_SCENE_STANDARD;
			game::gfxDrawMethod->baseTechType = dvars::r_fullbright->current.enabled ? get_fullbright_technique() : game::TECHNIQUE_LIT;
			game::gfxDrawMethod->emissiveTechType = dvars::r_fullbright->current.enabled ? get_fullbright_technique() : game::TECHNIQUE_EMISSIVE;
			game::gfxDrawMethod->forceTechType = dvars::r_fullbright->current.enabled ? get_fullbright_technique() : 254;
		}

		void r_init_draw_method_stub()
		{
			gfxdrawmethod();
		}

		bool r_update_front_end_dvar_options_stub()
		{
			if (dvars::r_fullbright->modified)
			{
				dvars::r_fullbright->modified = false;
				game::R_SyncRenderThread();

				gfxdrawmethod();
			}

			return r_update_front_end_dvar_options_hook.invoke<bool>();
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			dvars::r_fullbright = dvars::register_int("r_fullbright", 0, 0, 3, game::DVAR_FLAG_SAVED);

			r_init_draw_method_hook.create(0x14072F950, &r_init_draw_method_stub);
			r_update_front_end_dvar_options_hook.create(0x14076EE70, &r_update_front_end_dvar_options_stub);
		}
	};
}

REGISTER_COMPONENT(renderer::component)
