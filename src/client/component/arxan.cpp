#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "game_module.hpp"
#include "scheduler.hpp"

#include <utils/hook.hpp>
#include <utils/nt.hpp>

namespace arxan
{
	namespace
	{
		enum dbg_funcs_e
		{
			DbgBreakPoint,
			DbgUserBreakPoint,
			DbgUiConnectToDbg,
			DbgUiContinue,
			DbgUiConvertStateChangeStructure,
			DbgUiDebugActiveProcess,
			DbgUiGetThreadDebugObject,
			DbgUiIssueRemoteBreakin,
			DbgUiRemoteBreakin,
			DbgUiSetThreadDebugObject,
			DbgUiStopDebugging,
			DbgUiWaitStateChange,
			DbgPrintReturnControlC,
			DbgPrompt,
			DBG_FUNCS_COUNT,
		};

		const char* dbg_funcs_names[] =
		{
			"DbgBreakPoint",
			"DbgUserBreakPoint",
			"DbgUiConnectToDbg",
			"DbgUiContinue",
			"DbgUiConvertStateChangeStructure",
			"DbgUiDebugActiveProcess",
			"DbgUiGetThreadDebugObject",
			"DbgUiIssueRemoteBreakin",
			"DbgUiRemoteBreakin",
			"DbgUiSetThreadDebugObject",
			"DbgUiStopDebugging",
			"DbgUiWaitStateChange",
			"DbgPrintReturnControlC",
			"DbgPrompt",
		};

		struct dbg_func_bytes_s
		{
			std::uint8_t buffer[15];
		};

		dbg_func_bytes_s dbg_func_bytes[DBG_FUNCS_COUNT];
		void* dbg_func_procs[DBG_FUNCS_COUNT]{};

		void store_debug_functions()
		{
			const utils::nt::library ntdll("ntdll.dll");

			for (auto i = 0; i < DBG_FUNCS_COUNT; i++)
			{
				dbg_func_procs[i] = ntdll.get_proc<void*>(dbg_funcs_names[i]);
				std::memcpy(dbg_func_bytes[i].buffer, dbg_func_procs[i], sizeof(dbg_func_bytes[i].buffer));
			}
		}

		void restore_debug_functions()
		{
			for (auto i = 0; i < DBG_FUNCS_COUNT; i++)
			{
				utils::hook::copy(dbg_func_procs[i], dbg_func_bytes[i].buffer, sizeof(dbg_func_bytes[i].buffer));
			}
		}
	}

	class component final : public component_interface
	{
	public:

		void post_load() override
		{
			store_debug_functions();
		}

		void post_unpack() override
		{
			scheduler::loop(restore_debug_functions, scheduler::async);
		}
	};
}

REGISTER_COMPONENT(arxan::component)
