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
				return false;
			}

			if (!hook_enabled && pos > (char*)vm_execute_hooks.size())
			{
				hook_enabled = true;
				return false;
			}

			const auto hook = vm_execute_hooks[pos];
			const auto state = hook.lua_state();

			const auto self_id = local_id_to_entity(game::scr_VmPub->function_frame->fs.localId);
			const auto self = scripting::entity(self_id);

			std::vector<sol::lua_value> args;

			const auto top = game::scr_function_stack->top;

			for (auto* value = top; value->type != game::SCRIPT_END; --value)
			{
				args.push_back(scripting::lua::convert(state, *value));
			}

			const auto result = hook(self, sol::as_args(args));
			scripting::lua::handle_error(result);

			const auto value = scripting::lua::convert({ state, result });
			const auto type = value.get_raw().type;

			game::Scr_ClearOutParams();

			if (result.valid() && type && type < game::SCRIPT_END)
			{
				scripting::push_value(value);
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

			a.jmp(game::base_address + 0x5C90B3);

			a.bind(replace);

			a.popad64();
			a.mov(r14, (char*)empty_function);
			a.jmp(end);
		}
	}

	void clear_callbacks()
	{
		vm_execute_hooks.clear();
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::jump(game::base_address + 0x5C90A5, utils::hook::assemble(vm_execute_stub), true);
		}
	};
}

REGISTER_COMPONENT(notifies::component)