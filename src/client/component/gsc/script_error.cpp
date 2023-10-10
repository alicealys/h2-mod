#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "game/game.hpp"

#include "script_extension.hpp"
#include "script_error.hpp"

#include "component/scripting.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace gsc
{
	std::array<const char*, 27> var_typename =
	{
		"undefined",
		"object",
		"string",
		"localized string",
		"vector",
		"float",
		"int",
		"codepos",
		"precodepos",
		"function",
		"builtin function",
		"builtin method",
		"stack",
		"animation",
		"pre animation",
		"thread",
		"thread",
		"thread",
		"thread",
		"struct",
		"removed entity",
		"entity",
		"array",
		"removed thread",
		"<free>",
		"thread list",
		"endon list",
	};

	namespace
	{
		utils::hook::detour scr_emit_function_hook;

		unsigned int current_filename = 0;

		std::string unknown_function_error;

		void scr_emit_function_stub(unsigned int filename, unsigned int thread_name, char* code_pos)
		{
			current_filename = filename;
			scr_emit_function_hook.invoke<void>(filename, thread_name, code_pos);
		}

		std::string get_filename_name()
		{
			const auto filename_str = game::SL_ConvertToString(static_cast<game::scr_string_t>(current_filename));
			const auto id = std::atoi(filename_str);
			if (!id)
			{
				return filename_str;
			}

			return scripting::get_token(id);
		}

		void get_unknown_function_error(const char* code_pos)
		{
			const auto function = find_function(code_pos);
			if (function.has_value())
			{
				const auto& pos = function.value();
				unknown_function_error = std::format(
					"while processing function '{}' in script '{}':\nunknown script '{}'", pos.first, pos.second, scripting::current_file
				);
			}
			else
			{
				unknown_function_error = std::format("unknown script '{}'", scripting::current_file);
			}
		}

		void get_unknown_function_error(unsigned int thread_name)
		{
			const auto filename = get_filename_name();
			const auto name = scripting::get_token(thread_name);

			unknown_function_error = std::format(
				"while processing script '{}':\nunknown function '{}::{}'", scripting::current_file, filename, name
			);
		}

		void compile_error_stub(const char* code_pos, [[maybe_unused]] const char* msg)
		{
			get_unknown_function_error(code_pos);
			game::Com_Error(game::ERR_DROP, "script link error\n%s", unknown_function_error.data());
		}

		unsigned int find_variable_stub(unsigned int parent_id, unsigned int thread_name)
		{
			const auto res = game::FindVariable(parent_id, thread_name);
			if (!res)
			{
				get_unknown_function_error(thread_name);
				game::Com_Error(game::ERR_DROP, "script link error\n%s", unknown_function_error.data());
			}

			return res;
		}

		unsigned int scr_get_object(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				auto* value = game::scr_VmPub->top - index;
				if (value->type == game::VAR_POINTER)
				{
					return value->u.pointerValue;
				}

				scr_error(false, "Type %s is not an object", var_typename[value->type]);
			}

			scr_error(false, "parameter %u does not exist", index + 1);
			return 0;
		}

		unsigned int scr_get_const_string(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				auto* value = game::scr_VmPub->top - index;
				if (game::Scr_CastString(value))
				{
					assert(value->type == game::VAR_STRING);
					return value->u.stringValue;
				}

				game::Scr_ErrorInternal();
			}

			scr_error(false, "parameter %u does not exist", index + 1);
			return 0;
		}

		unsigned int scr_get_const_istring(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				auto* value = game::scr_VmPub->top - index;
				if (value->type == game::VAR_ISTRING)
				{
					return value->u.stringValue;
				}

				scr_error(false, "type %s is not a localized string", var_typename[value->type]);
			}

			scr_error(false, "parameter %u does not exist", index + 1);
			return 0;
		}

		void scr_validate_localized_string_ref(int parm_index, const char* token, int token_len)
		{
			assert(token);
			assert(token_len >= 0);

			if (token_len < 2)
			{
				return;
			}

			for (auto char_iter = 0; char_iter < token_len; ++char_iter)
			{
				if (!std::isalnum(static_cast<unsigned char>(token[char_iter])) && token[char_iter] != '_')
				{
					scr_error(false, "illegal localized string reference: %s must contain only alpha-numeric characters and underscores", token);
				}
			}
		}

		void scr_get_vector(unsigned int index, float* vector_value)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				auto* value = game::scr_VmPub->top - index;
				if (value->type == game::VAR_VECTOR)
				{
					std::memcpy(vector_value, value->u.vectorValue, sizeof(std::float_t[3]));
					return;
				}

				scr_error(false, "type %s is not a vector", var_typename[value->type]);
			}

			scr_error(false, "parameter %u does not exist", index + 1);
		}

		int scr_get_int(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				auto* value = game::scr_VmPub->top - index;
				if (value->type == game::VAR_INTEGER)
				{
					return value->u.intValue;
				}

				scr_error(false, "type %s is not an int", var_typename[value->type]);
			}

			scr_error(false, "parameter %u does not exist", index + 1);
			return 0;
		}

		float scr_get_float(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				auto* value = game::scr_VmPub->top - index;
				if (value->type == game::VAR_FLOAT)
				{
					return value->u.floatValue;
				}

				if (value->type == game::VAR_INTEGER)
				{
					return static_cast<float>(value->u.intValue);
				}

				scr_error(false, "type %s is not a float", var_typename[value->type]);
			}

			scr_error(false, "parameter %u does not exist", index + 1);
			return 0.0f;
		}

		int scr_get_pointer_type(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				if ((game::scr_VmPub->top - index)->type == game::VAR_POINTER)
				{
					return static_cast<int>(game::GetObjectType((game::scr_VmPub->top - index)->u.uintValue));
				}

				scr_error(false, "type %s is not an object", var_typename[(game::scr_VmPub->top - index)->type]);
			}

			scr_error(false, "parameter %u does not exist", index + 1);
			return 0;
		}

		int scr_get_type(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				return (game::scr_VmPub->top - index)->type;
			}

			scr_error(false, "parameter %u does not exist", index + 1);
			return 0;
		}

		const char* scr_get_type_name(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				return var_typename[(game::scr_VmPub->top - index)->type];
			}

			scr_error(false, "parameter %u does not exist", index + 1);
			return nullptr;
		}

		void g_model_index_no_cache_stub(utils::hook::assembler& a)
		{
			a.lea(ecx, qword_ptr(rbx, 0xA81));
			a.call(0x1406B3580);
			a.cmp(eax, 1);
			a.mov(rcx, rdi); // restore rcx
			a.jmp(0x140290351);
		}

		void g_model_index_no_cache_error_stub(const char* model)
		{
			scr_error(false, "model \"%s\" must be precached", model);
		}
	}

	std::optional<std::pair<std::string, std::string>> find_function(const char* pos)
	{
		for (const auto& file : scripting::script_function_table_sort)
		{
			for (auto i = file.second.begin(); i != file.second.end() && std::next(i) != file.second.end(); ++i)
			{
				const auto next = std::next(i);
				if (pos >= i->second && pos < next->second)
				{
					return {std::make_pair(i->first, file.first)};
				}
			}
		}

		return {};
	}

	class error final : public component_interface
	{
	public:
		void post_unpack() override
		{
			scr_emit_function_hook.create(0x1405BC5E0, scr_emit_function_stub);

			utils::hook::call(0x1405BC583, compile_error_stub); // LinkFile
			utils::hook::call(0x1405BC5CF, compile_error_stub); // LinkFile
			utils::hook::call(0x1405BC6BA, find_variable_stub); // Scr_EmitFunction

			// Restore basic error messages for commonly used scr functions
			utils::hook::jump(0x1405C7950, scr_get_object);
			utils::hook::jump(0x1405C7420, scr_get_const_string);
			utils::hook::jump(0x1405C71E0, scr_get_const_istring);
			utils::hook::jump(0x1404FEED0, scr_validate_localized_string_ref);
			utils::hook::jump(0x1405C7E90, scr_get_vector);
			utils::hook::jump(0x1405C7890, scr_get_int);
			utils::hook::jump(0x1405C7730, scr_get_float);

			utils::hook::jump(0x1405C7B70, scr_get_pointer_type);
			utils::hook::jump(0x1405C7D40, scr_get_type);
			utils::hook::jump(0x1405C7DB0, scr_get_type_name);

			// add error print to G_PrecacheModel
			utils::hook::jump(0x140290340, utils::hook::assemble(g_model_index_no_cache_stub), true);
			utils::hook::call(0x140290365, g_model_index_no_cache_error_stub);
		}
	};
}

REGISTER_COMPONENT(gsc::error)
