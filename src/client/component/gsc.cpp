#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "console.hpp"
#include "filesystem.hpp"
#include "scripting.hpp"
#include "gsc.hpp"
#include "scheduler.hpp"
#include "fastfiles.hpp"
#include "command.hpp"

#include "game/scripting/execution.hpp"
#include "game/scripting/functions.hpp"
#include "game/scripting/lua/error.hpp"

#include "notifies.hpp"

#include <xsk/gsc/types.hpp>
#include <xsk/gsc/interfaces/compiler.hpp>
#include <xsk/gsc/interfaces/decompiler.hpp>
#include <xsk/gsc/interfaces/assembler.hpp>
#include <xsk/gsc/interfaces/disassembler.hpp>
#include <xsk/utils/compression.hpp>
#include <xsk/resolver.hpp>
#include <interface.hpp>

#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>

namespace gsc
{
	void* func_table[0x1000]{};

	namespace
	{
		game::dvar_t* developer_script = nullptr;

		auto compiler = ::gsc::compiler();
		auto decompiler = ::gsc::decompiler();
		auto assembler = ::gsc::assembler();
		auto disassembler = ::gsc::disassembler();

		std::unordered_map<std::string, unsigned int> main_handles;
		std::unordered_map<std::string, unsigned int> init_handles;
		std::unordered_map<std::string, game::ScriptFile*> loaded_scripts;
		std::unordered_map<scripting::script_function, unsigned int> functions;
		std::optional<std::string> gsc_error;

		char* allocate_buffer(size_t size)
		{
			return utils::hook::invoke<char*>(0x14061E680, size, 4, 1, 5);
		}

		bool read_scriptfile(const std::string& name, std::string* data)
		{
			if (filesystem::read_file(name, data))
			{
				return true;
			}
			
			const auto name_str = name.data();
			if (game::DB_XAssetExists(game::ASSET_TYPE_RAWFILE, name_str) && 
				!game::DB_IsXAssetDefault(game::ASSET_TYPE_RAWFILE, name_str))
			{
				const auto asset = game::DB_FindXAssetHeader(game::ASSET_TYPE_RAWFILE, name.data(), false);
				const auto len = game::DB_GetRawFileLen(asset.rawfile);
				data->resize(len);
				game::DB_GetRawBuffer(asset.rawfile, data->data(), len);
				if (len > 0)
				{
					data->pop_back();
				}

				return true;
			}

			return false;
		}

		game::ScriptFile* load_custom_script(const char* file_name, const std::string& real_name)
		{
			if (loaded_scripts.find(real_name) != loaded_scripts.end())
			{
				return loaded_scripts[real_name];
			}

			std::string source_buffer{};
			if (!read_scriptfile(real_name + ".gsc", &source_buffer))
			{
				return nullptr;
			}

			auto data = std::vector<uint8_t>{source_buffer.begin(), source_buffer.end()};

			try
			{
				compiler->compile(real_name, data);
			}
			catch (const std::exception& e)
			{

				console::error("*********** script compile error *************\n");
				console::error("failed to compile '%s':\n%s", real_name.data(), e.what());
				console::error("**********************************************\n");
				return nullptr;
			}

			auto assembly = compiler->output();

			try
			{
				assembler->assemble(real_name, assembly);
			}
			catch (const std::exception& e)
			{
				console::error("*********** script compile error *************\n");
				console::error("failed to assemble '%s':\n%s", real_name.data(), e.what());
				console::error("**********************************************\n");
				return nullptr;
			}

			const auto script_file_ptr = reinterpret_cast<game::ScriptFile*>(allocate_buffer(sizeof(game::ScriptFile)));
			script_file_ptr->name = file_name;

			const auto stack = assembler->output_stack();
			script_file_ptr->len = static_cast<int>(stack.size());

			const auto script = assembler->output_script();
			script_file_ptr->bytecodeLen = static_cast<int>(script.size());

			const auto script_size = script.size();
			const auto buffer_size = script_size + stack.size() + 2;

			const auto buffer = allocate_buffer(buffer_size);
			std::memcpy(buffer, script.data(), script_size);
			std::memcpy(&buffer[script_size], stack.data(), stack.size()); 

			script_file_ptr->bytecode = &buffer[0];
			script_file_ptr->buffer = &buffer[script.size()];
			script_file_ptr->compressedLen = 0;

			loaded_scripts[real_name] = script_file_ptr;

			return script_file_ptr;
		}

		void load_script(const std::string& name)
		{
			if (!game::Scr_LoadScript(name.data()))
			{
				return;
			}

			const auto main_handle = game::Scr_GetFunctionHandle(name.data(),
				xsk::gsc::h2::resolver::token_id("main"));
			const auto init_handle = game::Scr_GetFunctionHandle(name.data(),
				xsk::gsc::h2::resolver::token_id("init"));

			if (main_handle)
			{
				console::info("Loaded '%s::main'\n", name.data());
				main_handles[name] = main_handle;
			}

			if (init_handle)
			{
				console::info("Loaded '%s::init'\n", name.data());
				init_handles[name] = init_handle;
			}
		}

		void load_scripts(const std::filesystem::path& root_dir)
		{
			std::filesystem::path script_dir = root_dir / "scripts";
			if (!utils::io::directory_exists(script_dir.generic_string()))
			{
				return;
			}

			const auto scripts = utils::io::list_files(script_dir.generic_string());
			for (const auto& script : scripts)
			{
				if (!script.ends_with(".gsc"))
				{
					continue;
				}

				std::filesystem::path path(script);
				const auto relative = path.lexically_relative(root_dir).generic_string();
				const auto base_name = relative.substr(0, relative.size() - 4);

				load_script(base_name);
			}
		}

		void clear()
		{
			main_handles.clear();
			init_handles.clear();
			loaded_scripts.clear();
		}

		void load_gametype_script_stub(void* a1, void* a2)
		{
			utils::hook::invoke<void>(0x1404E1400, a1, a2);

			clear();

			fastfiles::enum_assets(game::ASSET_TYPE_RAWFILE, [](game::XAssetHeader header)
			{
				std::string name = header.rawfile->name;

				if (name.ends_with(".gsc") && name.starts_with("scripts/"))
				{
					const auto base_name = name.substr(0, name.size() - 4);
					load_script(base_name);
				}
			}, true);

			for (const auto& path : filesystem::get_search_paths())
			{
				load_scripts(path);
			}
		}

		void g_load_structs_stub()
		{
			for (auto& function_handle : main_handles)
			{
				console::info("Executing '%s::main'\n", function_handle.first.data());
				const auto thread = game::Scr_ExecThread(function_handle.second, 0);
				game::RemoveRefToObject(thread);
			}

			utils::hook::invoke<void>(0x140510B40);
		}

		void scr_load_level_stub()
		{
			utils::hook::invoke<void>(0x1404FD130);

			for (auto& function_handle : init_handles)
			{
				console::info("Executing '%s::init'\n", function_handle.first.data());
				const auto thread = game::Scr_ExecThread(function_handle.second, 0);
				game::RemoveRefToObject(thread);
			}
		}

		int db_is_xasset_default(int type, const char* name)
		{
			if (loaded_scripts.find(name) != loaded_scripts.end())
			{
				return 0;
			}

			return utils::hook::invoke<int>(0x1404143C0, type, name);
		}

		void db_get_raw_buffer_stub(const game::RawFile* rawfile, char* buf, const int size)
		{
			if (rawfile->len > 0 && rawfile->compressedLen == 0)
			{
				std::memset(buf, 0, size);
				std::memcpy(buf, rawfile->buffer, std::min(rawfile->len, size));
				return;
			}

			utils::hook::invoke<void>(0x140413C40, rawfile, buf, size);
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

		void print_callstack()
		{
			for (auto frame = game::scr_VmPub->function_frame; frame != game::scr_VmPub->function_frame_start; --frame)
			{
				const auto pos = frame == game::scr_VmPub->function_frame 
					? game::scr_function_stack->pos 
					: frame->fs.pos;
				const auto function = find_function(frame->fs.pos);

				if (function.has_value())
				{
					console::warn("\tat function \"%s\" in file \"%s.gsc\"", 
						function.value().first.data(), function.value().second.data());
				}
				else
				{
					console::warn("\tat unknown location %p", pos);
				}
			}
		}

		std::optional<std::string> get_opcode_name(const std::uint8_t opcode)
		{
			try
			{
				return {xsk::gsc::h2::resolver::opcode_name(opcode)};
			}
			catch (...)
			{
				return {};
			}
		}

		void builtin_call_error(const std::string& error)
		{
			const auto pos = game::scr_function_stack->pos;
			const auto function_id = *reinterpret_cast<std::uint16_t*>(
				reinterpret_cast<size_t>(pos - 2));

			if (function_id > 0x1000)
			{
				console::warn("in call to builtin method \"%s\"%s",
					xsk::gsc::h2::resolver::method_name(function_id).data(), error.data());
			}
			else
			{
				console::warn("in call to builtin function \"%s\"%s",
					xsk::gsc::h2::resolver::function_name(function_id).data(), error.data());
			}
		}

		bool force_error_print = false;
		void* vm_error_stub(void* a1)
		{
			if (!developer_script->current.enabled && !force_error_print)
			{
				return utils::hook::invoke<void*>(0x140614670, a1);
			}

			console::warn("*********** script runtime error *************\n");

			const auto opcode_id = *reinterpret_cast<std::uint8_t*>(0x14BAA93E8);
			const std::string error = gsc_error.has_value()
				? utils::string::va(": %s", gsc_error.value().data())
				: "";

			if ((opcode_id >= 0x1A && opcode_id <= 0x20) || (opcode_id >= 0xA9 && opcode_id <= 0xAF))
			{
				builtin_call_error(error);
			}
			else
			{
				const auto opcode = get_opcode_name(opcode_id);
				if (opcode.has_value())
				{
					console::warn("while processing instruction %s%s\n",
						opcode.value().data(), error.data());
				}
				else
				{
					console::warn("while processing instruction 0x%X%s\n",
						opcode_id, error.data());
				}
			}

			force_error_print = false;
			gsc_error = {};

			print_callstack();
			console::warn("**********************************************\n");
			return utils::hook::invoke<void*>(0x140614670, a1);
		}

		std::string unknown_function_error{};
		void get_unknown_function_error(const char* code_pos)
		{
			const auto function = find_function(code_pos);
			if (function.has_value())
			{
				const auto& pos = function.value();
				unknown_function_error = utils::string::va(
					"while processing function '%s' in script '%s':\nunknown script '%s'",
					pos.first.data(), pos.second.data(), scripting::current_file.data()
				);
			}
			else
			{
				unknown_function_error = utils::string::va(
					"unknown script '%s'",
					scripting::current_file.data()
				);
			}
		}

		unsigned int current_filename{};
		std::string get_filename_name()
		{
			const auto filename_str = game::SL_ConvertToString(
				static_cast<game::scr_string_t>(current_filename));
			const auto id = std::atoi(filename_str);
			if (id == 0)
			{
				return filename_str;
			}

			return scripting::get_token_single(id);
		}


		void get_unknown_function_error(unsigned int thread_name)
		{
			const auto filename = get_filename_name();
			const auto name = scripting::get_token_single(thread_name);

			unknown_function_error = utils::string::va(
				"while processing script '%s':\nunknown function '%s::%s'",
				scripting::current_file.data(), filename.data(), name.data()
			);
		}

		void unknown_function_stub(const char* code_pos)
		{
			get_unknown_function_error(code_pos);
			game::Com_Error(game::ERR_DROP, "script link error\n%s", 
				unknown_function_error.data());
		}

		unsigned int find_variable_stub(unsigned int parent_id, unsigned int thread_name)
		{
			const auto res = game::FindVariable(parent_id, thread_name);
			if (!res)
			{
				get_unknown_function_error(thread_name);
				game::Com_Error(game::ERR_DROP, "script link error\n%s",
					unknown_function_error.data());
			}
			return res;
		}

		void register_gsc_functions_stub(void* a1, void* a2)
		{
			utils::hook::invoke<void>(0x140509F20, a1, a2);
			for (const auto& func : functions)
			{
				game::Scr_RegisterFunction(func.first, 0, func.second);
			}
		}

		scripting::script_value get_argument(int index)
		{
			if (index >= static_cast<int>(game::scr_VmPub->outparamcount))
			{
				return {};
			}

			return game::scr_VmPub->top[-index];
		}

		auto function_id_start = 0x320;
		void add_function(const std::string& name, scripting::script_function function)
		{
			if (xsk::gsc::h2::resolver::find_function(name))
			{
				const auto id = xsk::gsc::h2::resolver::function_id(name);
				functions[function] = id;
			}
			else
			{
				const auto id = ++function_id_start;
				xsk::gsc::h2::resolver::add_function(name, static_cast<std::uint16_t>(id));
				functions[function] = id;
			}
		}

		void execute_custom_function(scripting::script_function function)
		{
			auto error = false;

			try
			{
				function({});
			}
			catch (const std::exception& e)
			{
				error = true;
				force_error_print = true;
				gsc_error = e.what();
			}

			if (error)
			{
				game::Scr_ErrorInternal();
			}
		}
		
		void vm_call_builtin_stub(scripting::script_function function)
		{
			auto custom = false;
			{
				custom = functions.find(function) != functions.end();
			}

			if (!custom)
			{
				function({});
			}
			else
			{
				execute_custom_function(function);
			}
		}

		utils::hook::detour scr_emit_function_hook;
		void scr_emit_function_stub(unsigned int filename, unsigned int thread_name, char* code_pos)
		{
			current_filename = filename;
			scr_emit_function_hook.invoke<void>(filename, thread_name, code_pos);
		}

		std::string get_script_file_name(const std::string& name)
		{
			const auto id = xsk::gsc::h2::resolver::token_id(name);
			if (id == 0)
			{
				return name;
			}

			return std::to_string(id);
		}

		std::vector<std::uint8_t> decompile_scriptfile(const std::string& name, const std::string& real_name)
		{
			const auto* scriptfile = game::DB_FindXAssetHeader(game::ASSET_TYPE_SCRIPTFILE, name.data(), false).scriptfile;
			if (scriptfile == nullptr)
			{
				throw std::runtime_error(std::format("couldn't load scriptfile '{}'", real_name));
			}

			console::info("Decompiling scriptfile '%s'\n", real_name.data());

			std::vector<std::uint8_t> stack{scriptfile->buffer, scriptfile->buffer + scriptfile->len};
			std::vector<std::uint8_t> bytecode{scriptfile->bytecode, scriptfile->bytecode + scriptfile->bytecodeLen};
			
			auto decompressed_stack = xsk::utils::zlib::decompress(stack, static_cast<std::uint32_t>(stack.size()));

			disassembler->disassemble(name, bytecode, decompressed_stack);
			auto output = disassembler->output();

			decompiler->decompile(name, output);

			return decompiler->output();
		}
	}

	game::ScriptFile* find_script(game::XAssetType /*type*/, const char* name, int /*allow_create_default*/)
	{
		std::string real_name = name;
		const auto id = static_cast<std::uint16_t>(std::atoi(name));
		if (id)
		{
			real_name = xsk::gsc::h2::resolver::token_name(id);
		}

		const auto script = load_custom_script(name, real_name);
		if (script)
		{
			return script;
		}

		return game::DB_FindXAssetHeader(game::ASSET_TYPE_SCRIPTFILE, name, 1).scriptfile;
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			developer_script = dvars::register_bool("developer_script", false, 0, "Print GSC errors");

			// Allow custom scripts to include other custom scripts
			xsk::gsc::h2::resolver::init([](const auto& include_name)
			{
				const auto real_name = include_name + ".gsc";

				std::string file_buffer;
				if (!read_scriptfile(real_name, &file_buffer) || file_buffer.empty())
				{
					const auto name = get_script_file_name(include_name);
					if (game::DB_XAssetExists(game::ASSET_TYPE_SCRIPTFILE, name.data()))
					{
						return decompile_scriptfile(name, real_name);
					}
					else
					{
						throw std::runtime_error(std::format("couldn't load gsc file '{}'", real_name));
					}
				}

				std::vector<std::uint8_t> result;
				result.assign(file_buffer.begin(), file_buffer.end());

				return result;
			});

			utils::hook::call(0x1405C6177, find_script);
			utils::hook::call(0x1405C6187, db_is_xasset_default);

			// Loads scripts with an uncompressed stack
			utils::hook::call(0x1405C61E0, db_get_raw_buffer_stub);

			// load handles
			utils::hook::call(0x1404E17B2, load_gametype_script_stub);

			// execute handles
			utils::hook::call(0x1404C8F71, g_load_structs_stub);
			utils::hook::call(0x1404C8F80, scr_load_level_stub);

			utils::hook::call(0x1405CB94F, vm_error_stub);

			utils::hook::call(0x1405BC583, unknown_function_stub);
			utils::hook::call(0x1405BC5CF, unknown_function_stub);
			utils::hook::call(0x1405BC6BA, find_variable_stub);
			scr_emit_function_hook.create(0x1405BC5E0, scr_emit_function_stub);

			utils::hook::call(0x1405BCBAB, register_gsc_functions_stub);
			utils::hook::set<uint32_t>(0x1405BC7BC, 0x1000); // change builtin func count

#define RVA(ptr) static_cast<uint32_t>(reinterpret_cast<size_t>(ptr) - 0x140000000)
			utils::hook::set<uint32_t>(0x1405BC7C2 + 4, RVA(&func_table));
			utils::hook::inject(0x1405BCB78 + 3, &func_table);
			utils::hook::set<uint32_t>(0x1405CA678 + 4, RVA(&func_table));

			utils::hook::nop(0x1405CA683, 8);
			utils::hook::call(0x1405CA683, vm_call_builtin_stub);

			add_function("print", [](const game::scr_entref_t ref)
			{
				const auto num = game::Scr_GetNumParam();
				std::string buffer{};

				for (auto i = 0; i < num; i++)
				{
					const auto str = game::Scr_GetString(i);
					buffer.append(str);
					buffer.append("\t");
				}

				printf("%s\n", buffer.data());
			});

			add_function("assert", [](const game::scr_entref_t ref)
			{
				const auto expr = get_argument(0).as<int>();
				if (!expr)
				{
					throw std::runtime_error("assert fail");
				}
			});

			add_function("assertex", [](const game::scr_entref_t ref)
			{
				const auto expr = get_argument(0).as<int>();
				if (!expr)
				{
					const auto error = get_argument(1).as<std::string>();
					throw std::runtime_error(error);
				}
			});

			add_function("replacefunc", [](const game::scr_entref_t ref)
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

			add_function("getsoundlength", [](const game::scr_entref_t ref)
			{
				const auto name = get_argument(0);
				if (!name.is<std::string>())
				{
					throw std::runtime_error("parameter 1 must be a string");
				}

				const auto name_str = name.as<std::string>();
				const auto sound = game::DB_FindXAssetHeader(game::ASSET_TYPE_SOUND, name_str.data(), false).sound;
				if (!sound || !sound->count || !sound->head->soundFile || sound->head->soundFile->type != game::SAT_STREAMED)
				{
					return game::Scr_AddInt(-1);
				}

				return game::Scr_AddInt(sound->head->soundFile->u.streamSnd.totalMsec);
			});

			add_function("executecommand", [](const game::scr_entref_t ref)
			{
				const auto cmd = get_argument(0).as<std::string>();
				command::execute(cmd);
			});

			scripting::on_shutdown([](int free_scripts)
			{
				if (free_scripts)
				{
					xsk::gsc::h2::resolver::cleanup();
					clear();
				}
			});
		}
	};
}

REGISTER_COMPONENT(gsc::component)
