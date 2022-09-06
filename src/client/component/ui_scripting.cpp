#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "command.hpp"

#include "filesystem.hpp"
#include "localized_strings.hpp"
#include "scripting.hpp"
#include "fastfiles.hpp"
#include "mods.hpp"
#include "updater.hpp"
#include "console.hpp"
#include "language.hpp"

#include "game/ui_scripting/execution.hpp"
#include "game/scripting/execution.hpp"

#include "ui_scripting.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>
#include <utils/io.hpp>

namespace ui_scripting
{
	namespace
	{
		const auto lui_common = utils::nt::load_resource(LUI_COMMON);
		const auto lui_updater = utils::nt::load_resource(LUI_UPDATER);
		const auto lua_json = utils::nt::load_resource(LUA_JSON);

		std::unordered_map<game::hks::cclosure*, std::function<arguments(const function_arguments& args)>> converted_functions;

		utils::hook::detour hks_start_hook;
		utils::hook::detour hks_shutdown_hook;
		utils::hook::detour hks_package_require_hook;

		struct script
		{
			std::string name;
			std::string root;
		};

		struct globals_t
		{
			std::string in_require_script;
			std::vector<script> loaded_scripts;
			bool load_raw_script{};
			std::string raw_script_name{};
		};

		globals_t globals{};

		bool is_loaded_script(const std::string& name)
		{
			for (auto i = globals.loaded_scripts.begin(); i != globals.loaded_scripts.end(); ++i)
			{
				if (i->name == name)
				{
					return true;
				}
			}

			return false;
		}

		std::string get_root_script(const std::string& name)
		{
			for (auto i = globals.loaded_scripts.begin(); i != globals.loaded_scripts.end(); ++i)
			{
				if (i->name == name)
				{
					return i->root;
				}
			}

			return {};
		}

		table get_globals()
		{
			const auto state = *game::hks::lua_state;
			return state->globals.v.table;
		}

		void print_error(const std::string& error)
		{
			console::error("************** LUI script execution error **************\n");
			console::error("%s\n", error.data());
			console::error("********************************************************\n");
		}

		void print_loading_script(const std::string& name)
		{
			console::info("Loading LUI script '%s'\n", name.data());
		}

		std::string get_current_script()
		{
			const auto state = *game::hks::lua_state;
			game::hks::lua_Debug info{};
			game::hks::hksi_lua_getstack(state, 1, &info);
			game::hks::hksi_lua_getinfo(state, "nSl", &info);
			return info.short_src;
		}

		int load_buffer(const std::string& name, const std::string& data)
		{
			const auto state = *game::hks::lua_state;
			const auto sharing_mode = state->m_global->m_bytecodeSharingMode;
			state->m_global->m_bytecodeSharingMode = game::hks::HKS_BYTECODE_SHARING_ON;
			const auto _0 = gsl::finally([&]()
			{
				state->m_global->m_bytecodeSharingMode = sharing_mode;
			});

			game::hks::HksCompilerSettings compiler_settings{};
			return game::hks::hksi_hksL_loadbuffer(state, &compiler_settings, data.data(), data.size(), name.data());
		}

		void load_script(const std::string& name, const std::string& data)
		{
			globals.loaded_scripts.push_back({name, name});

			const auto lua = get_globals();
			const auto load_results = lua["loadstring"](data, name);

			if (load_results[0].is<function>())
			{
				const auto results = lua["pcall"](load_results);
				if (!results[0].as<bool>())
				{
					print_error(results[1].as<std::string>());
				}
			}
			else if (load_results[1].is<std::string>())
			{
				print_error(load_results[1].as<std::string>());
			}
		}

		void load_scripts(const std::string& script_dir)
		{
			if (!utils::io::directory_exists(script_dir))
			{
				return;
			}

			const auto scripts = utils::io::list_files(script_dir);

			for (const auto& script : scripts)
			{
				std::string data{};
				if (std::filesystem::is_directory(script) && utils::io::read_file(script + "/__init__.lua", &data))
				{
					print_loading_script(script);
					load_script(script + "/__init__.lua", data);
				}
			}
		}

		void setup_functions()
		{
			const auto lua = get_globals();

			lua["io"]["fileexists"] = utils::io::file_exists;
			lua["io"]["writefile"] = utils::io::write_file;
			lua["io"]["movefile"] = utils::io::move_file;
			lua["io"]["filesize"] = utils::io::file_size;
			lua["io"]["createdirectory"] = utils::io::create_directory;
			lua["io"]["directoryexists"] = utils::io::directory_exists;
			lua["io"]["directoryisempty"] = utils::io::directory_is_empty;
			lua["io"]["listfiles"] = utils::io::list_files;
			lua["io"]["removefile"] = utils::io::remove_file;
			lua["io"]["removedirectory"] = utils::io::remove_directory;
			lua["io"]["readfile"] = static_cast<std::string(*)(const std::string&)>(utils::io::read_file);

			using game = table;
			auto game_type = game();
			lua["game"] = game_type;

			game_type["sharedset"] = [](const game&, const std::string& key, const std::string& value)
			{
				scripting::shared_table.access([key, value](scripting::shared_table_t& table)
				{
					table[key] = value;
				});
			};

			game_type["sharedget"] = [](const game&, const std::string& key)
			{
				std::string result;
				scripting::shared_table.access([key, &result](scripting::shared_table_t& table)
				{
					result = table[key];
				});
				return result;
			};

			game_type["sharedclear"] = [](const game&)
			{
				scripting::shared_table.access([](scripting::shared_table_t& table)
				{
					table.clear();
				});
			};

			game_type["assetlist"] = [](const game&, const std::string& type_string)
			{
				auto table_ = table();
				auto index = 1;
				auto type_index = -1;

				for (auto i = 0; i < ::game::XAssetType::ASSET_TYPE_COUNT; i++)
				{
					if (type_string == ::game::g_assetNames[i])
					{
						type_index = i;
					}
				}

				if (type_index == -1)
				{
					throw std::runtime_error("Asset type does not exist");
				}

				const auto type = static_cast<::game::XAssetType>(type_index);
				fastfiles::enum_assets(type, [type, &table_, &index](const ::game::XAssetHeader header)
				{
					const auto asset = ::game::XAsset{ type, header };
					const std::string asset_name = ::game::DB_GetXAssetName(&asset);
					table_[index++] = asset_name;
				}, true);

				return table_;
			};

			game_type["getweapondisplayname"] = [](const game&, const std::string& name)
			{
				const auto alternate = name.starts_with("alt_");
				const auto weapon = ::game::G_GetWeaponForName(name.data());

				char buffer[0x400] = {0};
				::game::CG_GetWeaponDisplayName(weapon, alternate, buffer, 0x400);

				return std::string(buffer);
			};

			game_type["getloadedmod"] = [](const game&)
			{
				return mods::mod_path;
			};

			game_type["addlocalizedstring"] = [](const game&, const std::string& string,
				const std::string& value)
			{
				localized_strings::override(string, value, true);
			};

			game_type["setlanguage"] = [](const game&, const std::string& language)
			{
				language::set(language);
			};

			lua["Engine"]["SetLanguage"] = [](const int index)
			{
				language::set_from_index(index);
				updater::relaunch();
			};

			using player = table;
			auto player_type = player();
			lua["player"] = player_type;

			player_type["notify"] = [](const player&, const std::string& name, const variadic_args& va)
			{
				if (!::game::CL_IsCgameInitialized() || !::game::g_entities[0].client)
				{
					throw std::runtime_error("Not in game");
				}

				const auto to_string = get_globals()["tostring"];
				const auto arguments = get_return_values();
				std::vector<std::string> args{};
				for (const auto& value : va)
				{
					const auto value_str = to_string(value);

					args.push_back(value_str[0].as<std::string>());
				}

				::scheduler::once([name, args]()
				{
					try
					{
						std::vector<scripting::script_value> arguments{};

						for (const auto& arg : args)
						{
							arguments.push_back(arg);
						}

						const auto player = scripting::call("getentbynum", {0}).as<scripting::entity>();
						scripting::notify(player, name, arguments);
					}
					catch (...)
					{
					}
				}, ::scheduler::pipeline::server);
			};

			auto updater_table = table();
			lua["updater"] = updater_table;

			updater_table["relaunch"] = updater::relaunch;

			updater_table["sethastriedupdate"] = updater::set_has_tried_update;
			updater_table["gethastriedupdate"] = updater::get_has_tried_update;
			updater_table["autoupdatesenabled"] = updater::auto_updates_enabled;

			updater_table["startupdatecheck"] = updater::start_update_check;
			updater_table["isupdatecheckdone"] = updater::is_update_check_done;
			updater_table["getupdatecheckstatus"] = updater::get_update_check_status;
			updater_table["isupdateavailable"] = updater::is_update_available;

			updater_table["startupdatedownload"] = updater::start_update_download;
			updater_table["isupdatedownloaddone"] = updater::is_update_download_done;
			updater_table["getupdatedownloadstatus"] = updater::get_update_download_status;
			updater_table["cancelupdate"] = updater::cancel_update;
			updater_table["isrestartrequired"] = updater::is_restart_required;

			updater_table["getlasterror"] = updater::get_last_error;
			updater_table["getcurrentfile"] = updater::get_current_file;
		}

		void start()
		{
			globals = {};

			const auto lua = get_globals();
			lua["EnableGlobals"]();

			table keydown_event{};
			keydown_event["key"] = "";
			keydown_event["keynum"] = 0;

			table keyup_event{};
			keyup_event["key"] = "";
			keyup_event["keynum"] = 0;

			lua["LUI"]["CachedEvents"]["keydown"] = keydown_event;
			lua["LUI"]["CachedEvents"]["keyup"] = keyup_event;

			setup_functions();

			lua["print"] = function(reinterpret_cast<game::hks::lua_function>(0x1402B81C0));
			lua["table"]["unpack"] = lua["unpack"];
			lua["luiglobals"] = lua;

			load_script("lui_common", lui_common);
			load_script("lui_updater", lui_updater);
			load_script("lua_json", lua_json);

			for (const auto& path : filesystem::get_search_paths_rev())
			{
				load_scripts(path + "/ui_scripts/");
			}
		}

		void try_start()
		{
			try
			{
				start();
			}
			catch (const std::exception& e)
			{
				console::error("Failed to load LUI scripts: %s\n", e.what());
			}
		}

		void hks_start_stub(char a1)
		{
			const auto _0 = gsl::finally(&try_start);
			return hks_start_hook.invoke<void>(a1);
		}

		void hks_shutdown_stub()
		{
			converted_functions.clear();
			globals = {};
			hks_shutdown_hook.invoke<void>();
		}

		void* hks_package_require_stub(game::hks::lua_State* state)
		{
			const auto script = get_current_script();
			const auto root = get_root_script(script);
			globals.in_require_script = root;
			return hks_package_require_hook.invoke<void*>(state);
		}

		game::XAssetHeader db_find_xasset_header_stub(game::XAssetType type, const char* name, int allow_create_default)
		{
			if (!is_loaded_script(globals.in_require_script))
			{
				return game::DB_FindXAssetHeader(type, name, allow_create_default);
			}

			const auto folder = globals.in_require_script.substr(0, globals.in_require_script.find_last_of("/\\"));
			std::string name_ = utils::string::replace(name, "~", ".");
			const std::string target_script = folder + "/" + name_ + ".lua";

			if (utils::io::file_exists(target_script))
			{
				globals.load_raw_script = true;
				globals.raw_script_name = target_script;
				return static_cast<game::XAssetHeader>(reinterpret_cast<game::LuaFile*>(1));
			}
			else if (name_.starts_with("ui/LUI/"))
			{
				return game::DB_FindXAssetHeader(type, name, allow_create_default);
			}

			return static_cast<game::XAssetHeader>(nullptr);
		}

		int hks_load_stub(game::hks::lua_State* state, void* compiler_options, 
			void* reader, void* reader_data, const char* chunk_name)
		{
			if (globals.load_raw_script)
			{
				globals.load_raw_script = false;
				globals.loaded_scripts.push_back({globals.raw_script_name, globals.in_require_script});
				return load_buffer(globals.raw_script_name, utils::io::read_file(globals.raw_script_name));
			}
			else
			{
				return utils::hook::invoke<int>(0x1402D9410, state, compiler_options, reader, 
					reader_data, chunk_name);
			}
		}
		
		std::string current_error;
		int main_handler(game::hks::lua_State* state)
		{
			bool error = false;

			try
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

				const auto& function = converted_functions[closure];

				const auto args = get_return_values();
				const auto results = function(args);

				for (const auto& result : results)
				{
					push_value(result);
				}

				return static_cast<int>(results.size());
			}
			catch (const std::exception& e)
			{
				current_error = e.what();
				error = true;
			}

			if (error)
			{
				game::hks::hksi_luaL_error(state, current_error.data());
			}

			return 0;
		}
	}

	template <typename F>
	game::hks::cclosure* convert_function(F f)
	{
		const auto state = *game::hks::lua_state;
		const auto closure = game::hks::cclosure_Create(state, main_handler, 0, 0, 0);
		converted_functions[closure] = wrap_function(f);
		return closure;
	}

	class component final : public component_interface
	{
	public:

		void post_unpack() override
		{
			utils::hook::call(0x14030BF2B, db_find_xasset_header_stub);
			utils::hook::call(0x14030C079, db_find_xasset_header_stub);
			utils::hook::call(0x14030C104, hks_load_stub);

			hks_package_require_hook.create(0x1402B4DA0, hks_package_require_stub);
			hks_start_hook.create(0x140328BE0, hks_start_stub);
			hks_shutdown_hook.create(0x1403203B0, hks_shutdown_stub);
		}
	};
}

REGISTER_COMPONENT(ui_scripting::component)
