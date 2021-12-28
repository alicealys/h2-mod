#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>

namespace patches
{
	namespace
	{
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
		}
	};
}

REGISTER_COMPONENT(patches::component)
