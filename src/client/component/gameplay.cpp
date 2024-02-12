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

		game::dvar_t* pm_snap_vector = nullptr;

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

		void pm_bouncing_stub_mp(utils::hook::assembler& a)
		{
			const auto no_bounce = a.newLabel();
			const auto loc_140691518 = a.newLabel();

			a.push(rax);

			a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&dvars::pm_bouncing)));
			a.mov(al, byte_ptr(rax, 0x10));
			a.cmp(byte_ptr(rbp, -0x5D), al);

			a.pop(rax);
			a.jz(no_bounce);
			a.jmp(0x140691481);

			a.bind(no_bounce);
			a.cmp(dword_ptr(rsp, 0x44), 0);
			a.jnz(loc_140691518);
			a.jmp(0x14069146F);

			a.bind(loc_140691518);
			a.jmp(0x140691518);
		}

		void sys_snap_vector(float* velocity)
		{
			if (!pm_snap_vector->current.enabled)
			{
				return;
			}

			velocity[0] = std::floorf(velocity[0] + 0.5f);
			velocity[1] = std::floorf(velocity[1] + 0.5f);
			velocity[2] = std::floorf(velocity[2] + 0.5f);
		}

		void add_snap_vector_call(utils::hook::assembler& a)
		{
			a.pushad64();
			a.mov(rcx, rsi);
			a.call_aligned(sys_snap_vector);
			a.popad64();
		}

		void pmove_single_stub1(utils::hook::assembler& a)
		{
			const auto loc_14068FEBD = a.newLabel();

			a.comiss(xmm5, xmm4);
			a.jbe(loc_14068FEBD);
			a.mov(rax, 0x3F800000);
			a.movq(xmm1, rax);
			a.jmp(0x14068FE99);

			a.bind(loc_14068FEBD);
			add_snap_vector_call(a);
			a.jmp(0x14068FEBD);
		}

		void pmove_single_stub2(utils::hook::assembler& a)
		{
			a.movss(dword_ptr(rsi, 8), xmm1);
			a.movss(dword_ptr(rsi, 4), xmm0);
			a.movss(dword_ptr(rsi), xmm6);

			add_snap_vector_call(a);
			a.jmp(0x14068FEBD);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			dvars::g_enableElevators = dvars::register_bool("g_enableElevators", false, 
				game::DVAR_FLAG_REPLICATED, "Enable elevators");
			dvars::jump_enableFallDamage = dvars::register_bool("jump_enableFallDamage", true, 
				game::DVAR_FLAG_REPLICATED, "Enable fall damage");
			dvars::jump_ladderPushVel = dvars::register_float("jump_ladderPushVel", 128.0f, 0.0f, 1024.0f, 
				game::DVAR_FLAG_REPLICATED, "The velocity of a jump off of a ladder");
			dvars::jump_spreadAdd = dvars::register_float("jump_spreadAdd", 64.0f, 0.0f, 512.0f, 
				game::DVAR_FLAG_REPLICATED, "The amount of spread scale to add as a side effect of jumping");

			dvars::pm_bouncing = dvars::register_bool("pm_bouncing", false,
				game::DVAR_FLAG_REPLICATED, "Enable bouncing");
			utils::hook::jump(0x14069145E, utils::hook::assemble(pm_bouncing_stub_mp), true);

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
			dvars::register_float("jump_height", 39.0f, 0.0f, 1000.0f,
				game::DVAR_FLAG_REPLICATED, "The maximum height of a player\'s jump");
			dvars::register_float("jump_stepSize", 18.0f, 0.0f, 64.0f, 
				game::DVAR_FLAG_REPLICATED, "The maximum step up to the top of a jump arc");
			dvars::register_float("g_gravity", 800.0f, 1.0f, 1000.0f, 
				game::DVAR_FLAG_REPLICATED, "Game gravity in inches per second squared");
			dvars::register_int("g_speed", 190, 0, 1000, 
				game::DVAR_FLAG_REPLICATED, "Player speed");

			pm_snap_vector = dvars::register_bool("pm_snapVector", false, game::DVAR_FLAG_REPLICATED, "Snap velocity vector (mp movement)");

			utils::hook::jump(0x14068FE8C, utils::hook::assemble(pmove_single_stub1), true);
			utils::hook::jump(0x14068FEAF, utils::hook::assemble(pmove_single_stub2), true);
		}
	};
}

REGISTER_COMPONENT(gameplay::component)
