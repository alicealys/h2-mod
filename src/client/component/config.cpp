#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "command.hpp"
#include "game_console.hpp"

#include <utils/hook.hpp>

namespace config
{
	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			dvars::register_bool("cg_autoUpdate", true, game::DvarFlags::DVAR_FLAG_SAVED);
		}
	};
}

REGISTER_COMPONENT(config::component)
