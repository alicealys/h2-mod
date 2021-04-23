#include <stdinc.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include <utils/hook.hpp>

namespace patches
{
	namespace
	{

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
		}
	};
}

REGISTER_COMPONENT(patches::component)
