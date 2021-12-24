#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "command.hpp"
#include "scheduler.hpp"
#include "scripting.hpp"

#include "game/scripting/event.hpp"
#include "game/scripting/functions.hpp"
#include "game/scripting/execution.hpp"
#include "game/scripting/lua/engine.hpp"

#include <utils/hook.hpp>

namespace scripting
{
	std::unordered_map<int, std::unordered_map<std::string, int>> fields_table;
	std::unordered_map<std::string, std::unordered_map<std::string, const char*>> script_function_table;

	namespace
	{
		utils::hook::detour vm_notify_hook;

		utils::hook::detour g_shutdown_game_hook;
		utils::hook::detour player_spawn_hook;

		utils::hook::detour scr_add_class_field_hook;

		utils::hook::detour scr_set_thread_position_hook;
		utils::hook::detour process_script_hook;

		std::string current_file;

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

				lua::engine::notify(e);
			}

			vm_notify_hook.invoke<void>(notify_list_owner_id, string_value, top);
		}

		void player_spawn_stub(const game::gentity_s* player)
		{
			player_spawn_hook.invoke<void>(player);
			lua::engine::start();
		}

		void g_shutdown_game_stub(const int free_scripts)
		{
			lua::engine::stop();
			g_shutdown_game_hook.invoke<void>(free_scripts);
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

		void process_script_stub(const char* filename)
		{
			current_file = filename;

			const auto file_id = atoi(filename);
			if (file_id)
			{
				current_file = scripting::find_token(file_id);
			}

			process_script_hook.invoke<void>(filename);
		}

		void scr_set_thread_position_stub(unsigned int threadName, const char* codePos)
		{
			const auto function_name = scripting::find_token(threadName);
			script_function_table[current_file][function_name] = codePos;
			scr_set_thread_position_hook.invoke<void>(threadName, codePos);
		}

		utils::hook::detour sub_6B2940_hook;
		char sub_6B2940_stub(void* a1)
		{
			const auto result = sub_6B2940_hook.invoke<char>(a1);
			if (a1 != nullptr)
			{
				lua::engine::start();
			}
			return result;
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			vm_notify_hook.create(0x5CC450_b, vm_notify_stub);

			g_shutdown_game_hook.create(0x4CBAD0_b, g_shutdown_game_stub);
			player_spawn_hook.create(0x4B0710_b, player_spawn_stub);

			scr_add_class_field_hook.create(0x5C2C30_b, scr_add_class_field_stub);
			scr_set_thread_position_hook.create(0x5BC7E0_b, scr_set_thread_position_stub);
			process_script_hook.create(0x5C6160_b, process_script_stub);

			// Loading last checkpoint doesn't call spawn player again (player_spawn_hook)
			// Not sure what this function does but `a1` is != nullptr when loading
			// the last checkpoint so we need to start lua in this context
			sub_6B2940_hook.create(0x6B2940_b, sub_6B2940_stub);

			scheduler::loop([]()
			{
				lua::engine::run_frame();
			}, scheduler::pipeline::server);
		}
	};
}

REGISTER_COMPONENT(scripting::component)
