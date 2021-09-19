#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "chat.hpp"
#include "scheduler.hpp"
#include "command.hpp"

#include "game_console.hpp"
#include "fps.hpp"
#include "branding.hpp"

#include "ui_scripting.hpp"

#include "game/ui_scripting/lua/engine.hpp"

#include <utils/string.hpp>

namespace ui_scripting
{
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
				ui_scripting::lua::engine::run_frame();
				fps::draw();
				branding::draw();
				game_console::draw_console();
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
				const std::string name = params.get(1);
				scheduler::once([name]()
				{
					ui_scripting::lua::engine::open_menu(name);
				}, scheduler::pipeline::renderer);
			});

			command::add("closeluamenu", [](const command::params& params)
			{
				const std::string name = params.get(1);
				scheduler::once([name]()
				{
					ui_scripting::lua::engine::close_menu(name);
				}, scheduler::pipeline::renderer);
			});
		}
	};
}

REGISTER_COMPONENT(ui_scripting::component)
