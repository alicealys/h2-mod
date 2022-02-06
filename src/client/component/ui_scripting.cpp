#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "chat.hpp"
#include "scheduler.hpp"
#include "command.hpp"

#include "ui_scripting.hpp"

#include "game/ui_scripting/lua/engine.hpp"
#include "game/ui_scripting/execution.hpp"
#include "game/ui_scripting/lua/error.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

namespace ui_scripting
{
	namespace
	{
		std::unordered_map<game::hks::cclosure*, sol::protected_function> converted_functions;

		utils::hook::detour hksi_lual_error_hook;
		utils::hook::detour hksi_lual_error_hook2;
		utils::hook::detour hks_start_hook;
		utils::hook::detour hks_shutdown_hook;

		bool error_hook_enabled = false;

		void hksi_lual_error_stub(game::hks::lua_State* s, const char* fmt, ...)
		{
			char va_buffer[2048] = {0};

			va_list ap;
			va_start(ap, fmt);
			vsprintf_s(va_buffer, fmt, ap);
			va_end(ap);

			const auto formatted = std::string(va_buffer);

			if (!error_hook_enabled)
			{
				return hksi_lual_error_hook.invoke<void>(s, formatted.data());
			}
			else
			{
				throw std::runtime_error(formatted);
			}
		}

		void* hks_start_stub(char a1)
		{
			const auto _ = gsl::finally([]()
			{
				ui_scripting::lua::engine::start();
			});

			return hks_start_hook.invoke<void*>(a1);
		}

		void hks_shutdown_stub()
		{
			ui_scripting::lua::engine::stop();
			hks_shutdown_hook.invoke<void*>();
		}
	}

	int main_function_handler(game::hks::lua_State* state)
	{
		const auto value = state->m_apistack.base[-1];
		if (value.t != game::hks::TCFUNCTION)
		{
			return 0;
		}

		const auto closure = value.v.cClosure;
		if (converted_functions.find(closure) == converted_functions.end())
		{
			return 0;
		}

		const auto function = converted_functions[closure];
		const auto count = static_cast<int>(state->m_apistack.top - state->m_apistack.base);
		const auto arguments = get_return_values(count);
		const auto s = function.lua_state();

		std::vector<sol::lua_value> converted_args;

		for (const auto& argument : arguments)
		{
			converted_args.push_back(lua::convert(s, argument));
		}

		const auto results = function(sol::as_args(converted_args));
		lua::handle_error(results);

		for (const auto& result : results)
		{
			push_value(lua::convert({s, result}));
		}

		return results.return_count();
	}

	void add_converted_function(game::hks::cclosure* closure, const sol::protected_function& function)
	{
		converted_functions[closure] = function;
	}

	void clear_converted_functions()
	{
		converted_functions.clear();
	}

	void enable_error_hook()
	{
		error_hook_enabled = true;
	}

	void disable_error_hook()
	{
		error_hook_enabled = false;
	}

	void notify(const event& e)
	{
		lua::engine::notify(e);
	}

	class component final : public component_interface
	{
	public:

		void post_unpack() override
		{
			scheduler::loop(ui_scripting::lua::engine::run_frame, scheduler::pipeline::lui);

			hks_start_hook.create(0x328BE0_b, hks_start_stub);
			hks_shutdown_hook.create(0x3203B0_b, hks_shutdown_stub);
			hksi_lual_error_hook.create(0x2E3E40_b, hksi_lual_error_stub);
			hksi_lual_error_hook2.create(0x2DCB40_b, hksi_lual_error_stub);
		}
	};
}

REGISTER_COMPONENT(ui_scripting::component)
