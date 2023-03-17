#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "script_error.hpp"
#include "script_extension.hpp"
#include "script_loading.hpp"

#include "component/console.hpp"
#include "component/command.hpp"
#include "component/notifies.hpp"

#include "game/scripting/script_value.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

namespace gsc
{
	std::uint16_t function_id_start = 0x320;
	void* func_table[FUNC_TABLE_SIZE];

	namespace
	{
		struct gsc_error : public std::runtime_error
		{
			using std::runtime_error::runtime_error;
		};

		std::unordered_map<std::uint16_t, game::BuiltinFunction> functions;

		bool force_error_print = false;
		std::optional<std::string> gsc_error_msg;

		utils::hook::detour scr_register_function_hook;

		unsigned int scr_get_function_stub(const char** p_name, int* type)
		{
			const auto result = utils::hook::invoke<unsigned int>(0x140509F20, p_name, type);

			for (const auto& [id, func] : functions)
			{
				game::Scr_RegisterFunction(func, 0, id);
			}

			return result;
		}

		void execute_custom_function(game::BuiltinFunction function)
		{
			auto error = false;

			try
			{
				function();
			}
			catch (const std::exception& e)
			{
				error = true;
				force_error_print = true;
				gsc_error_msg = e.what();
			}

			if (error)
			{
				game::Scr_ErrorInternal();
			}
		}

		bool is_in_game_memory_range(uintptr_t ptr)
		{
			return ptr <= BASE_ADDRESS + BINARY_PAYLOAD_SIZE;
		}

		bool is_in_game_memory_range(void* ptr)
		{
			return is_in_game_memory_range(reinterpret_cast<uintptr_t>(ptr));
		}

		void vm_call_builtin_function(const game::BuiltinFunction function)
		{
			const auto custom = !is_in_game_memory_range(function);
			if (!custom)
			{
				function();
			}
			else
			{
				execute_custom_function(function);
			}
		}

		void builtin_call_error(const std::string& error)
		{
			const auto pos = game::scr_function_stack->pos;
			const auto function_id = *reinterpret_cast<std::uint16_t*>(reinterpret_cast<std::size_t>(pos - 2));

			if (function_id > FUNC_TABLE_SIZE)
			{
				console::warn("in call to builtin method \"%s\"%s", gsc_ctx->meth_name(function_id).data(), error.data());
			}
			else
			{
				console::warn("in call to builtin function \"%s\"%s", gsc_ctx->func_name(function_id).data(), error.data());
			}
		}

		std::optional<std::string> get_opcode_name(const std::uint8_t opcode)
		{
			try
			{
				const auto index = gsc_ctx->opcode_enum(opcode);
				return {gsc_ctx->opcode_name(index)};
			}
			catch (...)
			{
				return {};
			}
		}

		void print_callstack()
		{
			for (auto frame = game::scr_VmPub->function_frame; frame != game::scr_VmPub->function_frame_start; --frame)
			{
				const auto pos = frame == game::scr_VmPub->function_frame ? game::scr_function_stack->pos : frame->fs.pos;
				const auto function = find_function(frame->fs.pos);

				if (function.has_value())
				{
					console::warn("\tat function \"%s\" in file \"%s.gsc\"\n", function.value().first.data(), function.value().second.data());
				}
				else
				{
					console::warn("\tat unknown location %p\n", pos);
				}
			}
		}

		bool is_call_opcode(const std::uint8_t opcode_id)
		{
			return (opcode_id >= 0x1A && opcode_id <= 0x20) || (opcode_id >= 0xA9 && opcode_id <= 0xAF);
		}

		void vm_error_stub(int mark_pos)
		{
			if (!developer_script->current.enabled && !force_error_print)
			{
				utils::hook::invoke<void>(0x140614670, mark_pos);
				return;
			}

			console::warn("******* script runtime error ********\n");
			const auto opcode_id = *reinterpret_cast<std::uint8_t*>(0x14BAA93E8);

			const std::string error = gsc_error_msg.has_value() ? std::format(": {}", gsc_error_msg.value()) : std::string();

			if (is_call_opcode(opcode_id))
			{
				builtin_call_error(error);
			}
			else
			{
				const auto opcode = get_opcode_name(opcode_id);
				if (opcode.has_value())
				{
					console::warn("while processing instruction %s%s\n", opcode.value().data(), error.data());
				}
				else
				{
					console::warn("while processing instruction 0x%X%s\n", opcode_id, error.data());
				}
			}

			force_error_print = false;
			gsc_error_msg = {};

			print_callstack();
			console::warn("************************************\n");
			utils::hook::invoke<void>(0x140614670, mark_pos);
		}

		void scr_print()
		{
			const auto num = game::Scr_GetNumParam();
			std::string buffer{};

			for (auto i = 0u; i < num; i++)
			{
				const auto str = game::Scr_GetString(i);
				buffer.append(str);
				buffer.append("\t");
			}

			console::info("%s\n", buffer.data());
		}

		void assert_cmd()
		{
			if (!game::Scr_GetInt(0))
			{
				scr_error(true, "assert fail");
			}
		}

		void assert_ex_cmd()
		{
			if (!game::Scr_GetInt(0))
			{
				scr_error(true, "assert fail: %s", game::Scr_GetString(1));
			}
		}

		void assert_msg_cmd()
		{
			scr_error(true, "assert fail: %s", game::Scr_GetString(0));
		}

		scripting::script_value get_argument(int index)
		{
			if (index >= static_cast<int>(game::scr_VmPub->outparamcount))
			{
				return {};
			}

			return game::scr_VmPub->top[-index];
		}
	}

	void scr_error(bool force_print, const char* fmt, ...)
	{
		{
			char buffer[2048]{};

			va_list ap;
			va_start(ap, fmt);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, ap);

			va_end(ap);

			force_error_print = force_print;
			gsc_error_msg = buffer;
		}

		game::Scr_ErrorInternal();
	}

	void add_function(const std::string& name, game::BuiltinFunction function)
	{
		if (gsc_ctx->func_exists(name))
		{
			const auto id = gsc_ctx->func_id(name);
			functions[id] = function;
		}
		else
		{
			const auto id = ++function_id_start;
			functions[id] = function;
			gsc_ctx->func_add(name, id);
		}
	}

	class extension final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::set<uint32_t>(0x1405BC7BC, FUNC_TABLE_SIZE); // change builtin func count

			utils::hook::set<uint32_t>(0x1405BC7C2 + 4, RVA(&func_table));
			utils::hook::inject(0x1405BCB78 + 3, &func_table);
			utils::hook::set<uint32_t>(0x1405CA678 + 4, RVA(&func_table));

			utils::hook::nop(0x1405CA683, 8);
			utils::hook::call(0x1405CA683, vm_call_builtin_function);

			utils::hook::call(0x1405CB94F, vm_error_stub); // LargeLocalResetToMark

			utils::hook::call(0x1405BCBAB, scr_get_function_stub);

			add_function("print", scr_print);
			add_function("println", scr_print);

			add_function("assert", assert_cmd);
			add_function("assertex", assert_ex_cmd);
			add_function("assertmsg", assert_msg_cmd);

			add_function("replacefunc", []()
			{
				const auto what = get_argument(0).get_raw();
				const auto with = get_argument(1).get_raw();

				if (what.type != game::SCRIPT_FUNCTION)
				{
					throw std::runtime_error("parameter 1 must be a function");
				}

				if (with.type != game::SCRIPT_FUNCTION)
				{
					throw std::runtime_error("parameter 2 must be a function");
				}

				notifies::set_gsc_hook(what.u.codePosValue, with.u.codePosValue);
			});

			add_function("getsoundlength", []()
			{
				const auto name = game::Scr_GetString(0);
				const auto sound = game::DB_FindXAssetHeader(game::ASSET_TYPE_SOUND, name, false).sound;
				if (!sound || !sound->count || !sound->head->soundFile || sound->head->soundFile->type != game::SAT_STREAMED)
				{
					return game::Scr_AddInt(-1);
				}

				return game::Scr_AddInt(sound->head->soundFile->u.streamSnd.totalMsec);
			});

			add_function("executecommand", []()
			{
				const auto cmd = game::Scr_GetString(0);
				command::execute(cmd);
			});
		}
	};
}

REGISTER_COMPONENT(gsc::extension)
