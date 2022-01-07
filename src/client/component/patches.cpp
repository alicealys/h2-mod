#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace patches
{
	namespace
	{
		utils::hook::detour gscr_set_save_dvar_hook;
		utils::hook::detour dvar_register_float_hook;

		void* sub_46148()
		{
			static uint64_t off_11C52460 = 0xAD0C58_b;
			return &off_11C52460;
		}

		DECLSPEC_NORETURN void quit_stub()
		{
			component_loader::pre_destroy();
			exit(0);
		}

		void gscr_set_save_dvar_stub()
		{
			const auto string = utils::string::to_lower(utils::hook::invoke<const char*>(0x5C7C20_b, 0));
			if (string == "cg_fov" || string == "cg_fovscale")
			{
				return;
			}

			gscr_set_save_dvar_hook.invoke<void>();
		}

		game::dvar_t* dvar_register_float_stub(int hash, const char* dvarName, float value, float min, float max, unsigned int flags)
		{
			static const auto cg_fov_hash = game::generateHashValue("cg_fov");
			static const auto cg_fov_scale_hash = game::generateHashValue("cg_fovscale");

			if (hash == cg_fov_hash || hash == cg_fov_scale_hash)
			{
				flags |= game::DvarFlags::DVAR_FLAG_SAVED;
			}

			return dvar_register_float_hook.invoke<game::dvar_t*>(hash, dvarName, value, min, max, flags);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Fix startup crashes
			utils::hook::set(0x633080_b, 0xC301B0);
			utils::hook::set(0x272F70_b, 0xC301B0);
			utils::hook::jump(0x46148_b, sub_46148, true);

			utils::hook::jump(0x64EF10_b, quit_stub, true);

			// Unlock fps in main menu
			utils::hook::set<BYTE>(0x3D8E1B_b, 0xEB);

			// Disable battle net popup
			utils::hook::nop(0x5F4496_b, 5);

			// Allow kbam input when gamepad is enabled
			utils::hook::nop(0x3D2F8E_b, 2);
			utils::hook::nop(0x3D0C9C_b, 6);

			// Prevent game from overriding cg_fov and cg_fovscale values
			gscr_set_save_dvar_hook.create(0x504C60_b, &gscr_set_save_dvar_stub);
			// Make cg_fov and cg_fovscale saved dvars
			dvar_register_float_hook.create(game::Dvar_RegisterFloat.get(), dvar_register_float_stub);
			// Don't make the game reset cg_fov and cg_fovscale along with other dvars
			utils::hook::nop(0x4C8A08_b, 5);
		}
	};
}

REGISTER_COMPONENT(patches::component)
