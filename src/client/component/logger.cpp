#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "console.hpp"
#include "loadscreen.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>

namespace logger
{
	namespace
	{
		utils::hook::detour com_error_hook;
		utils::hook::detour nullsub_48_hook;
		utils::hook::detour sub_32AEF0;

		game::dvar_t* logger_dev = nullptr;

		void print_error(const char* msg, ...)
		{
			char buffer[2048];

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			console::error("%s", buffer);
		}

		void print_com_error(int, const char* msg, ...)
		{
			char buffer[2048];

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			console::error("%s", buffer);
		}

		void com_error_stub(const int error, const char* msg, ...)
		{
			char buffer[2048];

			{
				va_list ap;
				va_start(ap, msg);

				vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

				va_end(ap);

				console::error("Error: %s\n", buffer);
			}

			loadscreen::clear();

			com_error_hook.invoke<void>(error, "%s", buffer);
		}

		void print_warning(const char* msg, ...)
		{
			char buffer[2048];

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			console::warn("%s", buffer);
		}

		void print(const char* msg, ...)
		{
			char buffer[2048];

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			console::info("%s", buffer);
		}

		void print_dev(const char* msg, ...)
		{
			if (!logger_dev->current.enabled)
			{
				return;
			}

			char buffer[2048];

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			console::info("%s", buffer);
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
				console::warn("%s", buffer);
			}
			else
			{
				if (!logger_dev->current.enabled)
				{
					return;
				}

				console::info("%s", buffer);
			}
		}


		void r_warn_once_per_frame_vsnprintf_stub(char* buffer, size_t buffer_length, char* msg, va_list va)
		{
			vsnprintf(buffer, buffer_length, msg, va);
			console::warn(buffer);
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

			utils::hook::call(0x140791A01, r_warn_once_per_frame_vsnprintf_stub);

			logger_dev = dvars::register_bool("logger_dev", false, game::DVAR_FLAG_SAVED, "Print dev stuff");
		}
	};
}

REGISTER_COMPONENT(logger::component)