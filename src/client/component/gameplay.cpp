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
		utils::hook::detour pm_crashland_hook;

		void pm_player_trace_stub(game::pmove_t* pm, game::trace_t* results, const float* start,
			const float* end, const game::Bounds* bounds, int passEntityNum, int contentMask)
		{
			pm_player_trace_hook.invoke<void>(pm, results, start, end, bounds, passEntityNum, contentMask);

			// By setting startsolid to false we allow the player to clip through solid objects above their head
			if (dvars::g_enableElevators->current.enabled)
			{
				results->startsolid = false;
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
			a.jmp(0x1406878CD);

			a.bind(allsolid);
			a.jmp(0x1406878D4);
		}

		void pm_crashland_stub(void* ps, void* pm)
		{
			if (dvars::jump_enableFallDamage->current.enabled)
			{
				pm_crashland_hook.invoke<void>(ps, pm);
			}
		}

		void jump_pushoffladder_stub(utils::hook::assembler& a)
		{
			a.push(rax);

			a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&dvars::jump_ladderPushVel)));
			a.mulss(xmm7, dword_ptr(rax, 0x10));
			a.mulss(xmm6, dword_ptr(rax, 0x10));

			a.pop(rax);

			a.jmp(0x14067AC7C);
		}

		void jump_start_stub(utils::hook::assembler& a)
		{
			// Game code hook skipped
			a.movss(xmm0, dword_ptr(rbx, 0x3D4));
			a.and_(dword_ptr(rbx, 0x54), ~(game::PMF_TIME_HARDLANDING | game::PMF_TIME_KNOCKBACK));

			a.push(rax);
			a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&dvars::jump_spreadAdd)));
			a.addss(xmm0, dword_ptr(rax, 0x10));
			a.pop(rax);

			a.jmp(0x14067AE1A);

		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			dvars::g_enableElevators = dvars::register_bool("g_enableElevators", false, game::DvarFlags::DVAR_FLAG_NONE);
			dvars::jump_enableFallDamage = dvars::register_bool("jump_enableFallDamage", true, game::DVAR_FLAG_REPLICATED);
			dvars::jump_ladderPushVel = dvars::register_float("jump_ladderPushVel", 128.0f, 0.0f, 1024.0f, game::DVAR_FLAG_REPLICATED);
			dvars::jump_spreadAdd = dvars::register_float("jump_spreadAdd", 64.0f, 0.0f, 512.0f, game::DVAR_FLAG_REPLICATED);

			// Influence PM_JitterPoint code flow so the trace->startsolid checks are 'ignored' 
			pm_player_trace_hook.create(0x14068F0A0, &pm_player_trace_stub);

			// If g_enableElevators is 1 the 'ducked' flag will always be removed from the player state
			utils::hook::jump(0x1406878C1, utils::hook::assemble(pm_trace_stub), true);

			pm_crashland_hook.create(0x140688A20, &pm_crashland_stub);

			utils::hook::jump(0x14067AC6C, utils::hook::assemble(jump_pushoffladder_stub), true);
			utils::hook::nop(0x14067AC78, 4);

			utils::hook::jump(0x14067AE03, utils::hook::assemble(jump_start_stub), true);
			utils::hook::nop(0x14067AE0F, 4);

			// Dvars already present in-game
			dvars::register_float("jump_height", 39.0f, 0.0f, 1000.0f, game::DVAR_FLAG_REPLICATED);
			dvars::register_float("jump_stepSize", 18.0f, 0.0f, 64.0f, game::DVAR_FLAG_REPLICATED);
			dvars::register_float("g_gravity", 800.0f, 1.0f, 1000.0f, game::DVAR_FLAG_REPLICATED);
			dvars::register_int("g_speed", 190, 0, 1000, game::DVAR_FLAG_REPLICATED);
		}
	};
}

REGISTER_COMPONENT(gameplay::component)
