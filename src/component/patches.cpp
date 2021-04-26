#include <stdinc.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>

namespace patches
{
	namespace
	{
		utils::hook::detour pm_crashland_hook;

		void pm_crashland_stub(void* ps, void* pm)
		{
			if (dvars::jump_enableFallDamage->current.enabled)
			{
				pm_crashland_hook.invoke<void>(ps, pm);
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Unlock fps in main menu
			utils::hook::set<BYTE>(game::base_address + 0x3D8E1B, 0xEB);

			// Disable battle net popup
			utils::hook::set(game::base_address + 0xBE7F83C, true);

			pm_crashland_hook.create(game::base_address + 0x688A20, pm_crashland_stub);
			dvars::jump_enableFallDamage = dvars::register_bool("jump_enableFallDamage", 1, game::DVAR_FLAG_REPLICATED);

			dvars::register_float("jump_height", 39, 0, 1000, game::DVAR_FLAG_REPLICATED);
			dvars::register_float("g_gravity", 800, 1, 1000, game::DVAR_FLAG_REPLICATED);
			dvars::register_int("g_speed", 190, 0, 1000, game::DVAR_FLAG_REPLICATED);
		}
	};
}

REGISTER_COMPONENT(patches::component)
