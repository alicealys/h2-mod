#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "scheduler.hpp"

#include "game/scripting/entity.hpp"
#include "game/scripting/execution.hpp"
#include "game/scripting/lua/value_conversion.hpp"
#include "game/scripting/lua/error.hpp"
#include "notifies.hpp"

#include <utils/hook.hpp>

namespace notifies
{
	std::unordered_map<const char*, sol::protected_function> vm_execute_hooks;
	bool hook_enabled = true;

	namespace
	{
		utils::hook::detour scr_entity_damage_hook;
		std::vector<sol::protected_function> entity_damage_callbacks;

		char empty_function[2] = {0x32, 0x34}; // CHECK_CLEAR_PARAMS, END

		unsigned int local_id_to_entity(unsigned int local_id)
		{
			const auto variable = game::scr_VarGlob->objectVariableValue[local_id];
			return variable.u.f.next;
		}

		bool execute_vm_hook(const char* pos)
		{
			if (vm_execute_hooks.find(pos) == vm_execute_hooks.end())
			{
				hook_enabled = true;
				return false;
			}

			if (!hook_enabled && pos > reinterpret_cast<char*>(vm_execute_hooks.size()))
			{
				hook_enabled = true;
				return false;
			}

			const auto& hook = vm_execute_hooks[pos];
			const auto state = hook.lua_state();

			const scripting::entity self = local_id_to_entity(game::scr_VmPub->function_frame->fs.localId);

			std::vector<sol::lua_value> args;

			const auto top = game::scr_function_stack->top;

			for (auto* value = top; value->type != game::SCRIPT_END; --value)
			{
				args.push_back(scripting::lua::convert(state, *value));
			}

			const auto result = hook(self, sol::as_args(args));
			scripting::lua::handle_error(result);

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
			a.mov(r14, reinterpret_cast<char*>(empty_function));
			a.jmp(end);
		}

		sol::lua_value convert_entity(lua_State* state, const game::gentity_s* ent)
		{
			if (!ent)
			{
				return {};
			}

			const auto player = scripting::call("getentbynum", {ent->s.entityNum});

			return scripting::lua::convert(state, player);
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

			const auto _vec = scripting::vector(vec);

			return scripting::lua::convert(state, _vec);
		}

		std::string convert_mod(const int meansOfDeath)
		{
			const auto value = reinterpret_cast<game::scr_string_t**>(0x140BF49B0)[meansOfDeath];
			const auto string = game::SL_ConvertToString(*value);

			return string;
		}

		void scr_entity_damage_stub(game::gentity_s* self, game::gentity_s* inflictor, game::gentity_s* attacker, const float* vDir, const float* vPoint, 
			int damage, int dflags, const unsigned int hitLoc, const unsigned int weapon, bool isAlternate, unsigned int a11, const int meansOfDeath, unsigned int a13, unsigned int a14)
		{
			{
				const std::string _hitLoc = reinterpret_cast<const char**>(0x140BF4AA0)[hitLoc];
				const auto _mod = convert_mod(meansOfDeath);
				const auto _weapon = get_weapon_name(weapon, isAlternate);

				for (const auto& callback : entity_damage_callbacks)
				{
					const auto state = callback.lua_state();

					const auto _self = convert_entity(state, self);
					const auto _inflictor = convert_entity(state, inflictor);
					const auto _attacker = convert_entity(state, attacker);
					const auto _vDir = convert_vector(state, vDir);

					const auto result = callback(_self, _inflictor, _attacker, damage, _mod, _weapon, _vDir, _hitLoc);
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

			scr_entity_damage_hook.invoke<void>(self, inflictor, attacker, vDir, vPoint, damage, dflags, hitLoc, weapon, isAlternate, a11, meansOfDeath, a13, a14);
		}
	}

	void add_entity_damage_callback(const sol::protected_function& callback)
	{
		entity_damage_callbacks.push_back(callback);
	}

	void clear_callbacks()
	{
		vm_execute_hooks.clear();
		entity_damage_callbacks.clear();
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::jump(0x1405C90A5, utils::hook::assemble(vm_execute_stub), true);

			scr_entity_damage_hook.create(0x1404BD2E0, scr_entity_damage_stub);
		}
	};
}

REGISTER_COMPONENT(notifies::component)