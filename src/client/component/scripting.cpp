#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "scripting.hpp"
#include "gsc.hpp"

#include "game/scripting/event.hpp"
#include "game/scripting/functions.hpp"
#include "game/scripting/execution.hpp"
#include "game/scripting/lua/engine.hpp"

#include <utils/hook.hpp>
#include <utils/concurrency.hpp>

namespace scripting
{
	std::unordered_map<int, std::unordered_map<std::string, int>> fields_table;
	std::unordered_map<std::string, std::unordered_map<std::string, const char*>> script_function_table;
	std::unordered_map<std::string, std::vector<std::pair<std::string, const char*>>> script_function_table_sort;
	utils::concurrency::container<shared_table_t> shared_table;

	std::unordered_map<std::string, int> get_dvar_int_overrides;

	std::string current_file;

	namespace
	{
		utils::hook::detour vm_notify_hook;

		utils::hook::detour g_shutdown_game_hook;
		utils::hook::detour client_spawn_hook;
		utils::hook::detour sv_check_load_level_hook;

		utils::hook::detour scr_add_class_field_hook;

		utils::hook::detour scr_set_thread_position_hook;
		utils::hook::detour process_script_hook;

		utils::hook::detour sl_get_canonical_string_hook;

		utils::hook::detour respawn_hook;

		utils::hook::detour scr_run_current_threads_hook;

		game::dvar_t* scr_auto_respawn = nullptr;

		std::string current_scriptfile;
		unsigned int current_file_id{};

		std::vector<std::function<void(bool)>> shutdown_callbacks;

		std::unordered_map<unsigned int, std::string> canonical_string_table;

		using notify_list = std::vector<event>;
		utils::concurrency::container<notify_list> scheduled_notifies;

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

				lua::engine::handle_endon_conditions(e);

				scheduled_notifies.access([&](notify_list& list)
				{
					list.push_back(e);
				});
			}

			vm_notify_hook.invoke<void>(notify_list_owner_id, string_value, top);
		}

		void clear_scheduled_notifies()
		{
			get_dvar_int_overrides.clear();
			scheduled_notifies.access([](notify_list& list)
			{
				list.clear();
			});
		}

		void client_spawn_stub(const game::gentity_s* client)
		{
			client_spawn_hook.invoke<void>(client);
			scr_auto_respawn->current.enabled = true;
			clear_scheduled_notifies();
			lua::engine::start();
		}

		void g_shutdown_game_stub(const int free_scripts)
		{
			if (free_scripts)
			{
				script_function_table_sort.clear();
				script_function_table.clear();
				canonical_string_table.clear();
			}

			for (const auto& callback : shutdown_callbacks)
			{
				callback(free_scripts);
			}

			clear_scheduled_notifies();
			lua::engine::stop();
			g_shutdown_game_hook.invoke<void>(free_scripts);
		}
	
		void scr_add_class_field_stub(unsigned int classnum, game::scr_string_t name, unsigned int canonicalString, unsigned int offset)
		{
			const auto name_ = game::SL_ConvertToString(name);

			if (fields_table[classnum].find(name_) == fields_table[classnum].end())
			{
				fields_table[classnum][name_] = offset;
			}

			scr_add_class_field_hook.invoke<void>(classnum, name, canonicalString, offset);
		}

		void process_script_stub(const char* filename)
		{
			current_scriptfile = filename;

			const auto file_id = atoi(filename);
			if (file_id)
			{
				current_file_id = file_id;
			}
			else
			{
				current_file_id = 0;
				current_file = filename;
			}

			process_script_hook.invoke<void>(filename);
		}

		std::vector<std::string> get_token_names(unsigned int id)
		{
			auto result = scripting::find_token(id);

			if (canonical_string_table.find(id) != canonical_string_table.end())
			{
				result.push_back(canonical_string_table[id]);
			}

			return result;
		}

		void add_function_sort(unsigned int id, const char* pos)
		{
			std::string filename = current_file;
			if (current_file_id)
			{
				filename = scripting::get_token_single(current_file_id);
			}

			if (script_function_table_sort.find(filename) == script_function_table_sort.end())
			{
				const auto script = gsc::find_script(game::ASSET_TYPE_SCRIPTFILE, current_scriptfile.data(), false);
				if (script)
				{
					const auto end = &script->bytecode[script->bytecodeLen];
					script_function_table_sort[filename].emplace_back("__end__", end);
				}
			}

			const auto name = get_token_single(id);
			auto& itr = script_function_table_sort[filename];
			itr.insert(itr.end() - 1, {name, pos});
		}

		void add_function(const std::string& file, unsigned int id, const char* pos)
		{
			const auto function_names = scripting::get_token_names(id);
			for (const auto& name : function_names)
			{
				script_function_table[file][name] = pos;
			}
		}

		void scr_set_thread_position_stub(unsigned int thread_name, const char* code_pos)
		{
			add_function_sort(thread_name, code_pos);

			if (current_file_id)
			{
				const auto names = scripting::get_token_names(current_file_id);
				for (const auto& name : names)
				{
					add_function(name, thread_name, code_pos);
				}
			}
			else
			{
				add_function(current_file, thread_name, code_pos);
			}

			scr_set_thread_position_hook.invoke<void>(thread_name, code_pos);
		}


		char sv_check_load_level_stub(void* save_game)
		{
			const auto result = sv_check_load_level_hook.invoke<char>(save_game);
			if (save_game != nullptr)
			{
				scr_auto_respawn->current.enabled = true;
				clear_scheduled_notifies();
				lua::engine::start();
			}
			return result;
		}

		unsigned int sl_get_canonical_string_stub(const char* str)
		{
			const auto result = sl_get_canonical_string_hook.invoke<unsigned int>(str);
			canonical_string_table[result] = str;
			return result;
		}

		void respawn_stub()
		{
			if (!scr_auto_respawn->current.enabled)
			{
				return;
			}

			respawn_hook.invoke<void>();
		}

		void scr_run_current_threads_stub()
		{
			notify_list list_copy{};
			scheduled_notifies.access([&](notify_list& list)
			{
				list_copy = list;
				list.clear();
			});

			for (const auto& e : list_copy)
			{
				lua::engine::notify(e);
			}

			scr_run_current_threads_hook.invoke<void>();
		}

		utils::hook::detour scr_get_dvar_int_hook;
		void scr_get_dvar_int_stub()
		{
			const auto dvar = game::Scr_GetString(0);

			if (get_dvar_int_overrides.find(dvar) != get_dvar_int_overrides.end())
			{
				game::Scr_AddInt(get_dvar_int_overrides[dvar]);
				return;
			}

			scr_get_dvar_int_hook.invoke<void>();
		}
	}

	std::string get_token_single(unsigned int id)
	{
		if (canonical_string_table.find(id) != canonical_string_table.end())
		{
			return canonical_string_table[id];
		}

		return scripting::find_token_single(id);
	}

	void on_shutdown(const std::function<void(bool)>& callback)
	{
		shutdown_callbacks.push_back(callback);
	}

	std::optional<std::string> get_canonical_string(const unsigned int id)
	{
		if (canonical_string_table.find(id) == canonical_string_table.end())
		{
			return {};
		}

		return {canonical_string_table[id]};
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			vm_notify_hook.create(0x1405CC450, vm_notify_stub);

			g_shutdown_game_hook.create(0x1404CBAD0, g_shutdown_game_stub);
			client_spawn_hook.create(0x1404B0710, client_spawn_stub);
			sv_check_load_level_hook.create(0x1406B2940, sv_check_load_level_stub);

			scr_add_class_field_hook.create(0x1405C2C30, scr_add_class_field_stub);
			scr_set_thread_position_hook.create(0x1405BC7E0, scr_set_thread_position_stub);
			process_script_hook.create(0x1405C6160, process_script_stub);
			sl_get_canonical_string_hook.create(game::SL_GetCanonicalString, sl_get_canonical_string_stub);

			scr_auto_respawn = dvars::register_bool("scr_autoRespawn", true, 0, "Automatically respawn player on death");
			respawn_hook.create(0x1404B1E00, respawn_stub);

			scr_run_current_threads_hook.create(0x1405C8370, scr_run_current_threads_stub);

			scr_get_dvar_int_hook.create(0x1404F0730, scr_get_dvar_int_stub);

			scheduler::loop([]()
			{
				lua::engine::run_frame();
			}, scheduler::pipeline::server);
		}
	};
}

REGISTER_COMPONENT(scripting::component)
