#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "console.hpp"

#include <utils/io.hpp>
#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace battle_net
{
	namespace
	{

	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Disable battle net game service
			utils::hook::set<std::uint32_t>(0x140272F70, 0xC301B0);

			// Disable battle net popup
			utils::hook::nop(0x1405F4496, 5);
		}
	};
}

REGISTER_COMPONENT(battle_net::component)
