#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "chat.hpp"
#include "scheduler.hpp"
#include "command.hpp"
#include "ui_scripting.hpp"

#include "game/ui_scripting/lua/engine.hpp"
#include "game/ui_scripting/lua/context.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

namespace ui_scripting
{
	namespace
	{

	}

	class component final : public component_interface
	{
	public:

		void post_unpack() override
		{
			scheduler::once([]()
			{
				ui_scripting::lua::engine::start();
			}, scheduler::pipeline::renderer);

			scheduler::loop([]()
			{
				ShowCursor(true);
				ui_scripting::lua::engine::run_frame();
			}, scheduler::pipeline::renderer);

			command::add("reloadmenus", []()
			{
				scheduler::once([]()
				{
					ui_scripting::lua::engine::start();
				}, scheduler::pipeline::renderer);
			});

			command::add("openluamenu", [](const command::params& params)
			{
				printf("command openmenu\n");
				const std::string name = params.get(1);
				scheduler::once([name]()
				{
					printf("command openmenu scheduler\n");
					ui_scripting::lua::open_menu(name);
				}, scheduler::pipeline::renderer);
			});

			command::add("closeluamenu", [](const command::params& params)
			{
				printf("command closemenu\n");
				const std::string name = params.get(1);
				scheduler::once([name]()
				{
					printf("command closemenu scheduler\n");
					ui_scripting::lua::close_menu(name);
				}, scheduler::pipeline::renderer);
			});

			utils::hook::nop(game::base_address + 0x5F22B1, 5);
		}
	};
}

REGISTER_COMPONENT(ui_scripting::component)
