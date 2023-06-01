#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "scheduler.hpp"

#include "game/scripting/entity.hpp"
#include "game/scripting/execution.hpp"
#include "game/scripting/lua/value_conversion.hpp"
#include "game/scripting/lua/error.hpp"
#include "notifies.hpp"
#include "scripting.hpp"

#include <utils/hook.hpp>

namespace notifies
{
	bool hook_enabled = true;

	namespace
	{
		struct gsc_hook_t
		{
			bool is_lua_hook{};
			bool is_variable{};
			const char* target_pos{};
			size_t rel_source_pos{};
			sol::protected_function lua_function;
		};

		std::unordered_map<const char*, gsc_hook_t> vm_execute_hooks;
		utils::hook::detour scr_entity_damage_hook;
		std::vector<sol::protected_function> entity_damage_callbacks;

		char empty_function[2] = {0x32, 0x34}; // CHECK_CLEAR_PARAMS, END
		const char* target_function = nullptr;
		bool gsc_hooks_buffer[0x1400000]{};

		scripting::entity local_id_to_entity(unsigned int local_id)
		{
			return game::scr_VarGlob->objectVariableValue[local_id].u.f.next;
		}

		char* get_program_buffer()
		{
			return *reinterpret_cast<char**>(0x14B5E0B78);
		}

		size_t get_program_buffer_offset(const char* pos)
		{
			const auto rel_offset = pos - get_program_buffer();
			return rel_offset;
		}

		bool execute_vm_hook(const char* pos)
		{
			const auto program_buffer = get_program_buffer();
			if (pos >= program_buffer && (pos - program_buffer < 0x1400000))
			{
				const auto rel_offset = pos - program_buffer;
				if (!gsc_hooks_buffer[rel_offset])
				{
					hook_enabled = true;
					return false;
				}
			}

			const auto iter = vm_execute_hooks.find(pos);
			if (iter == vm_execute_hooks.end() || (!hook_enabled && !iter->second.is_variable))
			{
				hook_enabled = true;
				return false;
			}

			const auto& hook = iter->second;
			if (hook.is_lua_hook)
			{
				const auto& function = hook.lua_function;
				const auto state = function.lua_state();

				const auto self = local_id_to_entity(game::scr_VmPub->function_frame->fs.localId);
				std::vector<sol::lua_value> args;

				const auto top = game::scr_function_stack->top;
				for (auto* value = top; value->type != game::SCRIPT_END; --value)
				{
					args.push_back(scripting::lua::convert(state, *value));
				}

				const auto result = function(self, sol::as_args(args));
				scripting::lua::handle_error(result);
				target_function = empty_function;
			}
			else
			{
				target_function = hook.target_pos;
			}

			return true;
		}

		void vm_execute_stub(utils::hook::assembler& a)
		{
			const auto replace = a.newLabel();
			const auto end = a.newLabel();

			a.pushad64();

			a.mov(rcx, r14);
			a.call_aligned(execute_vm_hook);

			a.cmp(al, 0);
			a.jne(replace);

			a.popad64();
			a.jmp(end);

			a.bind(end);

			a.movzx(r15d, byte_ptr(r14));
			a.inc(r14);
			a.mov(dword_ptr(rbp, 0xA4), r15d);

			a.jmp(0x1405C90B3);

			a.bind(replace);

			a.popad64();
			a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&target_function)));
			a.mov(r14, rax);
			a.jmp(end);
		}

		sol::lua_value convert_entity(lua_State* state, const game::gentity_s* ent)
		{
			if (!ent)
			{
				return {};
			}

			const scripting::entity entity{game::scr_entref_t(ent->s.entityNum, 0)};
			return scripting::lua::convert(state, entity);
		}

		std::string get_weapon_name(unsigned int weapon, bool isAlternate)
		{
			char output[1024] = {0};
			game::BG_GetWeaponNameComplete(weapon, isAlternate, output, 1024);
			return output;
		}

		sol::lua_value convert_vector(lua_State* state, const float* vec)
		{
			if (!vec)
			{
				return {};
			}

			return scripting::lua::convert(state, scripting::vector(vec));
		}

		std::string convert_mod(const int meansOfDeath)
		{
			const auto value = reinterpret_cast<game::scr_string_t**>(0x140BF49B0)[meansOfDeath];
			return game::SL_ConvertToString(*value);
		}

		void scr_entity_damage_stub(game::gentity_s* self, game::gentity_s* inflictor, game::gentity_s* attacker, const float* v_dir, const float* v_point,
			int damage, int dflags, const unsigned int means_of_death, const unsigned int weapon, bool is_alternate, 
			unsigned int a11, const int hit_loc, unsigned int a13, unsigned int a14)
		{
			{
				const std::string hit_loc_str = reinterpret_cast<const char**>(0x140BF4AA0)[hit_loc];
				const auto mod_str = convert_mod(means_of_death);
				const auto weapon_str = get_weapon_name(weapon, is_alternate);

				for (const auto& callback : entity_damage_callbacks)
				{
					const auto state = callback.lua_state();

					const auto self_ent = convert_entity(state, self);
					const auto inflictor_ent = convert_entity(state, inflictor);
					const auto attacker_ent = convert_entity(state, attacker);
					const auto v_dir_vec = convert_vector(state, v_dir);

					const auto result = callback(self_ent, inflictor_ent, attacker_ent, damage, mod_str, weapon_str, v_dir_vec, hit_loc_str);
					scripting::lua::handle_error(result);

					if (result.valid())
					{
						const auto value = result.get<sol::object>();
						if (value.is<int>())
						{
							damage = value.as<int>();
						}
					}
				}

				if (damage == 0)
				{
					return;
				}
			}

			scr_entity_damage_hook.invoke<void>(self, inflictor, attacker, v_dir, v_point, damage, dflags, 
				means_of_death, weapon, is_alternate, a11, hit_loc, a13, a14);
		}

		void set_hook(const char* pos, gsc_hook_t& hook)
		{
			if (!hook.is_variable)
			{
				const auto rel_pos = get_program_buffer_offset(pos);
				gsc_hooks_buffer[rel_pos] = true;
				hook.rel_source_pos = rel_pos;
			}

			vm_execute_hooks[pos] = hook;
		}
	}

	void add_entity_damage_callback(const sol::protected_function& callback)
	{
		entity_damage_callbacks.push_back(callback);
	}

	void clear_callbacks()
	{
		for (auto i = vm_execute_hooks.begin(); i != vm_execute_hooks.end();)
		{
			if (i->second.is_lua_hook)
			{
				if (!i->second.is_variable)
				{
					gsc_hooks_buffer[i->second.rel_source_pos] = false;
				}

				i = vm_execute_hooks.erase(i);
			}
			else
			{
				++i;
			}
		}

		entity_damage_callbacks.clear();
	}

	void set_lua_hook(const char* pos, const sol::protected_function& callback, bool is_variable)
	{
		gsc_hook_t hook{};
		hook.is_lua_hook = true;
		hook.is_variable = is_variable;
		hook.lua_function = callback;
		set_hook(pos, hook);
	}

	void set_gsc_hook(const char* source, const char* target)
	{
		gsc_hook_t hook{};
		hook.is_lua_hook = false;
		hook.target_pos = target;
		set_hook(source, hook);
	}

	void clear_hook(const char* pos)
	{
		const auto rel_offset = get_program_buffer_offset(pos);
		gsc_hooks_buffer[rel_offset] = false;
		vm_execute_hooks.erase(pos);
	}

	size_t get_hook_count()
	{
		return vm_execute_hooks.size();
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::jump(0x1405C90A5, utils::hook::assemble(vm_execute_stub), true);

			scr_entity_damage_hook.create(0x1404BD2E0, scr_entity_damage_stub);

			scripting::on_shutdown([](bool free_scripts, bool post_shutdown)
			{
				if (free_scripts && !post_shutdown)
				{
					std::memset(gsc_hooks_buffer, 0, sizeof(gsc_hooks_buffer));
					vm_execute_hooks.clear();
				}
			});
		}
	};
}

REGISTER_COMPONENT(notifies::component)
