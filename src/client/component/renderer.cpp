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
			case 4:
				return 53; // wireframe
			case 3:
				return 3; // debug
			case 2:
				return 13;
			default:
				return game::TECHNIQUE_UNLIT;
			}
		}

		void gfxdrawmethod()
		{
			game::gfxDrawMethod->drawScene = game::GFX_DRAW_SCENE_STANDARD;
			game::gfxDrawMethod->baseTechType = dvars::r_fullbright->current.integer ? get_fullbright_technique() : game::TECHNIQUE_LIT;
			game::gfxDrawMethod->emissiveTechType = dvars::r_fullbright->current.integer ? get_fullbright_technique() : game::TECHNIQUE_EMISSIVE;
			game::gfxDrawMethod->forceTechType = dvars::r_fullbright->current.integer ? get_fullbright_technique() : 254;
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

		utils::hook::detour r_filter_things_into_cells_r_hook;
		void r_filter_things_into_cells_r_stub(void* node, unsigned int dyn_ent_index, game::Bounds* bounds, unsigned int* cell_bits, unsigned int word_count)
		{
			const auto gfx_world = *reinterpret_cast<size_t*>(0x14EE49000);
			const auto bsp_version = *reinterpret_cast<unsigned int*>(gfx_world + 16);
			if (bsp_version == 115) // h2 bsp version
			{
				return r_filter_things_into_cells_r_hook.invoke<void>(node, dyn_ent_index, bounds, cell_bits, word_count);
			}

			constexpr auto cell_index = 0;
			const auto index = (dyn_ent_index >> 5) + word_count * cell_index;
			cell_bits[index] |= 0x80000000 >> (dyn_ent_index & 0x1F);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			dvars::r_fullbright = dvars::register_int("r_fullbright", 0, 0, 4, game::DVAR_FLAG_SAVED, "Fullbright method");

			r_init_draw_method_hook.create(0x14072F950, &r_init_draw_method_stub);
			r_update_front_end_dvar_options_hook.create(0x14076EE70, &r_update_front_end_dvar_options_stub);

			// workaround for zonetool issue
			r_filter_things_into_cells_r_hook.create(0x140724720, r_filter_things_into_cells_r_stub);
		}
	};
}

REGISTER_COMPONENT(renderer::component)
