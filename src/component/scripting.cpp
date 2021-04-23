#include <stdinc.hpp>

#include "game/game.hpp"
#include <utils/hook.hpp>

#include "game/scripting/event.hpp"
#include "game/scripting/execution.hpp"
#include "game/scripting/lua/engine.hpp"

#include "scheduler.hpp"

namespace scripting
{
	std::unordered_map<int, std::unordered_map<std::string, int>> fields_table;

	namespace
	{
		utils::hook::detour vm_notify_hook;

		utils::hook::detour scr_load_level_hook;
		utils::hook::detour g_shutdown_game_hook;

		utils::hook::detour scr_add_class_field_hook;

		std::string last_event;

		void vm_notify_stub(const unsigned int notify_list_owner_id, const game::scr_string_t string_value,
		                    game::VariableValue* top)
		{
			const auto* string = game::SL_ConvertToString(string_value);
			if (string)
			{
				event e;
				e.name = string;
				e.entity = notify_list_owner_id;

				for (auto* value = top; value->type != game::SCRIPT_END; --value)
				{
					e.arguments.emplace_back(*value);
				}

				if (last_event != e.name)
				{
					last_event = e.name;

					//printf("notify: %s\n", e.name.data());
				}

				lua::engine::notify(e);
			}

			vm_notify_hook.invoke<void>(notify_list_owner_id, string_value, top);
		}

		void scr_load_level_stub()
		{
			scr_load_level_hook.invoke<void>();
			lua::engine::start();
		}

		void g_shutdown_game_stub(const int free_scripts)
		{
			lua::engine::stop();
			return g_shutdown_game_hook.invoke<void>(free_scripts);
		}
	
		void scr_add_class_field_stub(unsigned int classnum, game::scr_string_t _name, unsigned int canonicalString, unsigned int offset)
		{
			const auto name = game::SL_ConvertToString(_name);

			if (fields_table[classnum].find(name) == fields_table[classnum].end())
			{
				fields_table[classnum][name] = offset;
			}

			scr_add_class_field_hook.invoke<void>(classnum, _name, canonicalString, offset);
		}
	}

	void init()
	{
		vm_notify_hook.create(game::base_address + 0x5CC450, vm_notify_stub);

		scr_load_level_hook.create(game::base_address + 0x520AB0, scr_load_level_stub);
		g_shutdown_game_hook.create(game::base_address + 0x4CBAD0, g_shutdown_game_stub);

		scr_add_class_field_hook.create(game::base_address + 0x5C2C30, scr_add_class_field_stub);

		scheduler::loop([]()
		{
			lua::engine::run_frame();
		}, scheduler::pipeline::server);
	}
}
