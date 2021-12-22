#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>

namespace gameplay
{
	namespace
	{
		utils::hook::detour pm_player_trace_hook;

		void pm_player_trace_stub(game::pmove_t* pm, game::trace_t* trace, const float* f3,
			const float* f4, const game::Bounds* bounds, int a6, int a7)
		{
			pm_player_trace_hook.invoke<void>(pm, trace, f3, f4, bounds, a6, a7);

			// By setting startsolid to false we allow the player to clip through solid objects above their head
			if (dvars::g_enableElevators->current.enabled)
			{
				trace->startsolid = false;
			}
		}

		void pm_trace_stub(utils::hook::assembler& a)
		{
			const auto stand = a.newLabel();
			const auto allsolid = a.newLabel();

			a.call(rsi); // Game code 

			a.push(rax);

			a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&dvars::g_enableElevators)));
			a.mov(al, byte_ptr(rax, 0x10));
			a.cmp(al, 1);

			a.pop(rax);

			a.jz(stand); // Always stand up

			a.cmp(byte_ptr(rsp, 0x89), 0); // Game code trace[0].allsolid == false
			a.jnz(allsolid);

			a.bind(stand);
			a.jmp(0x6878CD_b);

			a.bind(allsolid);
			a.jmp(0x6878D4_b);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			dvars::g_enableElevators = dvars::register_bool("g_enableElevators", false,
				game::DvarFlags::DVAR_FLAG_NONE, true);

			// Influence PM_JitterPoint code flow so the trace->startsolid checks are 'ignored' 
			pm_player_trace_hook.create(0x068F0A0_b, &pm_player_trace_stub);

			// If g_enableElevators is 1 the 'ducked' flag will always be removed from the player state
			utils::hook::jump(0x6878C1_b, utils::hook::assemble(pm_trace_stub), true);
		}
	};
}

REGISTER_COMPONENT(gameplay::component)
