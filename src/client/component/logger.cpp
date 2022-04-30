#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game_console.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>

namespace logger
{
	namespace
	{
		utils::hook::detour com_error_hook;
		utils::hook::detour nullsub_48_hook;
		utils::hook::detour sub_32AEF0;

		void print_error(const char* msg, ...)
		{
			char buffer[2048];

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			game_console::print(game_console::con_type_error, buffer);
		}

		void print_com_error(int, const char* msg, ...)
		{
			char buffer[2048];

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			game_console::print(game_console::con_type_error, buffer);
		}

		void com_error_stub(const int error, const char* msg, ...)
		{
			char buffer[2048];

			{
				va_list ap;
				va_start(ap, msg);

				vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

				va_end(ap);

				game_console::print(game_console::con_type_error, "Error: %s\n", buffer);
			}

			com_error_hook.invoke<void>(error, "%s", buffer);
		}

		void print_warning(const char* msg, ...)
		{
			char buffer[2048];

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			game_console::print(game_console::con_type_warning, buffer);
		}

		void print(const char* msg, ...)
		{
			char buffer[2048];

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			game_console::print(game_console::con_type_info, buffer);
		}

		void print_dev(const char* msg, ...)
		{
			static auto* enabled = dvars::register_bool("logger_dev", false, game::DVAR_FLAG_SAVED);
			if (!enabled->current.enabled)
			{
				return;
			}

			char buffer[2048];

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			game_console::print(game_console::con_type_info, buffer);
		}

		void lui_print(const char* msg, ...)
		{
			char buffer[2048];

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			if (strstr(msg, "LUI WARNING:"))
			{
				game_console::print(game_console::con_type_warning, buffer);
			}
			else
			{
				static auto* enabled = dvars::register_bool("logger_dev", false, game::DVAR_FLAG_SAVED);
				if (!enabled->current.enabled)
				{
					return;
				}

				game_console::print(game_console::con_type_info, buffer);
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::jump(0x14032C620, print_warning, true);
			utils::hook::jump(0x14032C630, print_warning, true);
			utils::hook::jump(0x14032AEF0, lui_print, true);
			com_error_hook.create(0x1405A2D80, com_error_stub);
			utils::hook::jump(0x14013A98C, print);
		}
	};
}

REGISTER_COMPONENT(logger::component)