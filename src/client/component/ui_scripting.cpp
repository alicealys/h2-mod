#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "chat.hpp"
#include "scheduler.hpp"
#include "command.hpp"

#include "game_console.hpp"
#include "fps.hpp"
#include "branding.hpp"

#include "ui_scripting.hpp"

#include "game/ui_scripting/lua/engine.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

namespace ui_scripting
{
	namespace
	{
		utils::hook::detour hksi_open_lib_hook;
		utils::hook::detour hksi_lual_error_hook;
		utils::hook::detour hksi_add_function_hook;
		utils::hook::detour hks_start_hook;
		utils::hook::detour hks_shutdown_hook;

		bool error_hook_enabled = false;

		std::unordered_map<std::string, game::hks::lua_function> functions;
		std::unordered_map<std::string, game::hks::lua_function> methods;

		void* hksi_open_lib_stub(game::hks::lua_State* s, const char* libname, game::hks::luaL_Reg* l)
		{
			for (auto i = l; i->name; ++i)
			{
				if (i->name == "__gc"s)
				{
					continue;
				}

				const auto lower = utils::string::to_lower(i->name);

				libname
					? functions[lower] = i->function
					: methods[lower] = i->function;
			}

			return hksi_open_lib_hook.invoke<void*>(s, libname, l);
		}

		void hksi_lual_error_stub(game::hks::lua_State* s, const char* fmt, ...)
		{
			char va_buffer[0x200] = { 0 };

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

			functions = {};
			methods = {};
			return hks_start_hook.invoke<void*>(a1);
		}

		void hks_shutdown_stub()
		{
			ui_scripting::lua::engine::stop();
			hks_shutdown_hook.invoke<void*>();
		}

		void hksi_add_function_stub(game::hks::lua_State* s, game::hks::lua_function f, int a3, const char* name, int a5)
		{
			if (name != "( lua_CFunction )LUI_CoD_LuaCall_UIExpression"s)
			{
				std::string name_ = name;
				const auto sub = utils::string::to_lower(name_.substr(13, name_.size() - 14));
				functions[sub] = f;
			}

			hksi_add_function_hook.invoke<void>(s, f, a3, name, a5);
		}
	}

	void enable_error_hook()
	{
		error_hook_enabled = true;
	}

	void disable_error_hook()
	{
		error_hook_enabled = false;
	}

	game::hks::lua_function find_function(const std::string& name)
	{
		const auto lower = utils::string::to_lower(name);
		if (functions.find(lower) == functions.end())
		{
			return 0;
		}
		return functions[lower];
	}

	game::hks::lua_function find_method(const std::string& name)
	{
		const auto lower = utils::string::to_lower(name);
		if (methods.find(lower) == methods.end())
		{
			return 0;
		}
		return methods[lower];
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
			scheduler::loop([]()
			{
				ui_scripting::lua::engine::run_frame();
				fps::draw();
				branding::draw();
				game_console::draw_console();
			}, scheduler::pipeline::renderer);

			command::add("reloadmenus", []()
			{
				scheduler::once([]()
				{
					ui_scripting::lua::engine::start();
				}, scheduler::pipeline::renderer);
			});

			command::add("openluamenu", [](const command::params& params)
			{
				const std::string name = params.get(1);
				scheduler::once([name]()
				{
					ui_scripting::lua::engine::open_menu(name);
				}, scheduler::pipeline::renderer);
			});

			command::add("closeluamenu", [](const command::params& params)
			{
				const std::string name = params.get(1);
				scheduler::once([name]()
				{
					ui_scripting::lua::engine::close_menu(name);
				}, scheduler::pipeline::renderer);
			});

			hks_start_hook.create(game::base_address + 0x328BE0, hks_start_stub);
			hks_shutdown_hook.create(game::base_address + 0x3203B0, hks_shutdown_stub);
			hksi_open_lib_hook.create(game::base_address + 0x2E4530, hksi_open_lib_stub);
			hksi_lual_error_hook.create(game::base_address + 0x2E3E40, hksi_lual_error_stub);
			hksi_add_function_hook.create(game::base_address + 0x2DB570, hksi_add_function_stub);
		}
	};
}

REGISTER_COMPONENT(ui_scripting::component)
