#include <std_include.hpp>
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

		void* sub_46148()
		{
			static uint64_t off_11C52460 = 0xAD0C58_b;
			return &off_11C52460;
		}

		DECLSPEC_NORETURN void quit_stub(const int code)
		{
			component_loader::pre_destroy();
			exit(0);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Not sure but it works
			utils::hook::set(0x633080_b, 0xC301B0);
			utils::hook::set(0x272F70_b, 0xC301B0);
			utils::hook::jump(0x46148_b, sub_46148, true);

			utils::hook::jump(0x64EF10_b, quit_stub, true);

			// Unlock fps in main menu
			utils::hook::set<BYTE>(0x3D8E1B_b, 0xEB);

			// Disable battle net popup
			utils::hook::nop(0x5F4496_b, 5);

			pm_crashland_hook.create(0x688A20_b, pm_crashland_stub);
			dvars::jump_enableFallDamage = dvars::register_bool("jump_enableFallDamage", 1, game::DVAR_FLAG_REPLICATED);

			dvars::register_float("jump_height", 39, 0, 1000, game::DVAR_FLAG_REPLICATED);
			dvars::register_float("g_gravity", 800, 1, 1000, game::DVAR_FLAG_REPLICATED);
			dvars::register_int("g_speed", 190, 0, 1000, game::DVAR_FLAG_REPLICATED);
		}
	};
}

REGISTER_COMPONENT(patches::component)
