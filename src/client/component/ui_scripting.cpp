#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "command.hpp"

#include "filesystem.hpp"
#include "scripting.hpp"
#include "fastfiles.hpp"
#include "mods.hpp"
#include "mod_stats.hpp"
#include "updater.hpp"
#include "console.hpp"
#include "language.hpp"
#include "config.hpp"
#include "motd.hpp"
#include "achievements.hpp"
#include "camera.hpp"

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

		using lua_function_t = std::function<arguments(const function_arguments& args)>;
		std::vector<std::unique_ptr<lua_function_t>> lua_functions;

		utils::hook::detour hks_start_hook;
		utils::hook::detour hks_shutdown_hook;
		utils::hook::detour hks_package_require_hook;

		struct globals_t
		{
			std::string in_require_script;
			std::unordered_map<std::string, std::string> loaded_scripts;
			bool load_raw_script{};
			std::string raw_script_name{};
		};

		globals_t globals{};

		bool is_loaded_script(const std::string& name)
		{
			return globals.loaded_scripts.contains(name);
		}

		std::string get_root_script(const std::string& name)
		{
			const auto itr = globals.loaded_scripts.find(name);
			return itr == globals.loaded_scripts.end() ? std::string() : itr->second;
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
			globals.loaded_scripts[name] = name;

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

		script_value json_to_lua(const nlohmann::json& json)
		{
			if (json.is_object())
			{
				table object;
				for (const auto& [key, value] : json.items())
				{
					object[key] = json_to_lua(value);
				}
				return object;
			}

			if (json.is_array())
			{
				table array;
				auto index = 1;
				for (const auto& value : json.array())
				{
					array[index++] = json_to_lua(value);
				}
				return array;
			}

			if (json.is_boolean())
			{
				return json.get<bool>();
			}

			if (json.is_number_integer())
			{
				return json.get<int>();
			}

			if (json.is_number_float())
			{
				return json.get<float>();
			}

			if (json.is_string())
			{
				return json.get<std::string>();
			}

			return {};
		}

		nlohmann::json lua_to_json(const script_value& value)
		{
			if (value.is<bool>())
			{
				return value.as<bool>();
			}
			
			if (value.is<int>())
			{
				return value.as<int>();
			}

			if (value.is<float>())
			{
				return value.as<float>();
			}

			if (value.is<std::string>())
			{
				return value.as<std::string>();
			}
			
			if (value.get_raw().t == game::hks::TNIL)
			{
				return {};
			}

			throw std::runtime_error("lua value must be of primitive type (boolean, integer, float, string)");
		}

		void setup_functions()
		{
			const auto lua = get_globals();

			lua["io"] = table();
			lua["io"]["fileexists"] = filesystem::safe_io_func<bool>(utils::io::file_exists);
			lua["io"]["writefile"] = filesystem::safe_write_file;
			lua["io"]["filesize"] = filesystem::safe_io_func<size_t>(utils::io::file_size);
			lua["io"]["createdirectory"] = filesystem::safe_io_func<bool>(utils::io::create_directory);
			lua["io"]["directoryexists"] = filesystem::safe_io_func<bool>(utils::io::directory_exists);
			lua["io"]["directoryisempty"] = filesystem::safe_io_func<bool>(utils::io::directory_is_empty);
			lua["io"]["listfiles"] = filesystem::safe_io_func<std::vector<std::string>>(utils::io::list_files);
			lua["io"]["removefile"] = filesystem::safe_io_func<bool>(utils::io::remove_file);
			lua["io"]["removedirectory"] = filesystem::safe_io_func<bool>(utils::io::remove_directory);
			lua["io"]["readfile"] = filesystem::safe_io_func<std::string>(
				static_cast<std::string(*)(const std::string&)>(utils::io::read_file));

			auto language_table = table();
			language_table["isnonlatin"] = language::is_non_latin;
			language_table["isslavic"] = language::is_slavic;
			language_table["isarabic"] = language::is_arabic;
			language_table["isasian"] = language::is_asian;
			language_table["iscustom"] = language::is_custom;

			lua["language"] = language_table;

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
					const auto asset = ::game::XAsset{type, header};
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
				const auto& path = mods::get_mod();
				return path.value_or("");
			};

			game_type["setlanguage"] = [](const game&, const std::string& language)
			{
				language::set(language);
			};

			game_type["fastfileexists"] = [](const game&, const std::string& name)
			{
				return fastfiles::exists(name);
			};

			game_type["openlink"] = [](const game&, const std::string& name)
			{
				const auto links = motd::get_links();
				const auto link = links.find(name);
				if (link == links.end())
				{
					return;
				}

				ShellExecuteA(nullptr, "open", link->second.data(), nullptr, nullptr, SW_SHOWNORMAL);
			};

			game_type["getlinkurl"] = [](const game&, const std::string& name)
				-> script_value
			{
				const auto links = motd::get_links();
				const auto link = links.find(name);
				if (link == links.end())
				{
					return script_value();
				}

				return link->second;
			};

			game_type["islink"] = [](const game&, const std::string& name)
			{
				const auto links = motd::get_links();
				const auto link = links.find(name);
				return link != links.end();
			};

			lua["string"]["escapelocalization"] = [](const std::string& str)
			{
				return "\x1F"s.append(str);
			};

			lua["string"]["el"] = lua["string"]["escapelocalization"];

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
			updater_table["shouldforceupdate"] = updater::should_force_update;

			updater_table["getlasterror"] = updater::get_last_error;
			updater_table["getcurrentfile"] = updater::get_current_file;

			updater_table["getcurrentbranch"] = []()
			{
				const auto branch = updater::get_current_branch();
				return static_cast<int>(branch);
			};

			updater_table["setbranch"] = [](const std::uint32_t branch)
			{
				updater::set_branch(static_cast<updater::git_branch>(branch));
			};

			updater_table["develop"] = static_cast<int>(updater::branch_develop);
			updater_table["main"] = static_cast<int>(updater::branch_main);

			auto mods_table = table();
			lua["mods"] = mods_table;

			mods_table["getloaded"] = []() -> script_value
			{
				const auto& mod = mods::get_mod();
				if (mod.has_value())
				{
					return mod.value();
				}

				return {};
			};

			mods_table["getlist"] = mods::get_mod_list;
			mods_table["getinfo"] = [](const std::string& mod)
			{
				table info_table{};
				const auto info = mods::get_mod_info(mod);
				const auto has_value = info.has_value();
				info_table["isvalid"] = has_value;

				if (!has_value)
				{
					return info_table;
				}

				const auto& map = info.value();
				for (const auto& [key, value] : map.items())
				{
					info_table[key] = json_to_lua(value);
				}

				return info_table;
			};

			mods_table["load"] = [](const std::string& mod)
			{
				scheduler::once([=]()
				{
					mods::load(mod);
				}, scheduler::main);
			};

			mods_table["unload"] = []
			{
				scheduler::once([]()
				{
					mods::unload();
				}, scheduler::main);
			};
			
			auto mods_stats_table = table();
			mods_table["stats"] = mods_stats_table;
			
			const auto set_stats_func = [](const variadic_args& va)
			{
				mod_stats::get_stats().access([&](mod_stats::mod_stats_t& stats)
				{
					auto obj = &stats;
					for (auto i = 0u; i < va.size() - 1; i++)
					{
						const auto key = va[i].as<std::string>();

						if (!obj->contains(key))
						{
							obj->operator[](key) = nlohmann::json::object();
						}

						obj = &obj->at(key);
					}

					const auto json_value = lua_to_json(va.back());
					*obj = json_value;

					mod_stats::set_modified();
				});
			};

			mods_stats_table["set"] = set_stats_func;
			game_type["statsset"] = [&](const game&, const variadic_args& va)
			{
				return set_stats_func(va);
			};

			const auto get_stats_func = [](const variadic_args& va, int offset)
			{
				const auto value = mod_stats::get_stats().access<nlohmann::json>([&](mod_stats::mod_stats_t& stats)
					-> nlohmann::json
				{
					auto obj = &stats;
					for (auto i = 0u; i < va.size() - offset; i++)
					{
						const auto key = va[i].as<std::string>();

						if (!obj->contains(key))
						{
							return {};
						}

						obj = &obj->at(key);
					}

					return *obj;
				});

				return value;
			};

			mods_stats_table["get"] = [&](const variadic_args& va)
			{
				return json_to_lua(get_stats_func(va, 0));
			};

			game_type["statsget"] = [&](const game&, const variadic_args& va)
			{
				return json_to_lua(get_stats_func(va, 0));
			};

			const auto has_stats_func = [](const variadic_args& va)
			{
				const auto value = mod_stats::get_stats().access<bool>([&](mod_stats::mod_stats_t& stats)
					-> nlohmann::json
				{
					auto obj = &stats;
					for (auto i = 0u; i < va.size(); i++)
					{
						const auto key = va[i].as<std::string>();

						if (!obj->contains(key))
						{
							return false;
						}

						obj = &obj->at(key);
					}

					return !obj->is_null();
				});

				return value;
			};

			mods_stats_table["has"] = has_stats_func;
			game_type["statshas"] = [&](const game&, const variadic_args& va)
			{
				return has_stats_func(va);
			};

			mods_stats_table["getor"] = [&](const variadic_args& va)
			{
				const auto& default_value = va.back();
				const auto value = get_stats_func(va, 1);

				if (value.is_null())
				{
					return default_value;
				}
				else
				{
					return json_to_lua(value);
				}
			};

			mods_stats_table["setstruct"] = [](const std::string& mapname,
				const std::string& key, const script_value& value)
			{
				const auto json_value = lua_to_json(value);
				mod_stats::set_struct(mapname, key, json_value);
			};

			mods_stats_table["getstruct"] = [](const std::string& mapname,
				const std::string& key)
			{
				return json_to_lua(mod_stats::get_struct(mapname, key));
			};

			mods_stats_table["getstructor"] = [](const std::string& mapname,
				const std::string& key, const script_value& default_value)
			{
				const auto json_default_value = lua_to_json(default_value);
				return json_to_lua(mod_stats::get_struct(mapname, key, json_default_value));
			};

			mods_stats_table["save"] = mod_stats::write;
			mods_stats_table["getall"] = []()
			{
				return json_to_lua(mod_stats::get_all());
			};

			mods_stats_table["setfromjson"] = [](const std::string& data)
			{
				const auto json = nlohmann::json::parse(data);
				mod_stats::set_all(json);
			};

			auto config_table = table();
			lua["config"] = config_table;

			config_table["get"] = [](const std::string& key, const variadic_args& va)
				-> script_value
			{
				const auto value = config::get_raw(key);
				return json_to_lua(value);
			};

			config_table["getstring"] = [](const std::string& key, const variadic_args& va)
				-> script_value
			{
				const auto default_value = va.size() >= 1 ? va[0] : script_value("");
				const auto value = config::get<nlohmann::json>(key);

				if (!value.has_value())
				{
					return default_value;
				}

				const auto& json = value.value();
				if (!json.is_string())
				{
					return default_value;
				}

				return json.get<std::string>();
			};

			config_table["getint"] = [](const std::string& key, const variadic_args& va)
				-> script_value
			{
				const auto default_value = va.size() >= 1 ? va[0] : script_value(0);
				const auto value = config::get<nlohmann::json>(key);

				if (!value.has_value())
				{
					return default_value;
				}

				const auto& json = value.value();
				if (!json.is_number_integer())
				{
					return default_value;
				}

				return json.get<int>();
			};

			config_table["getfloat"] = [](const std::string& key, const variadic_args& va)
				-> script_value
			{
				const auto default_value = va.size() >= 1 ? va[0] : script_value(0.f);
				const auto value = config::get<nlohmann::json>(key);

				if (!value.has_value())
				{
					return default_value;
				}

				const auto& json = value.value();
				if (!json.is_number())
				{
					return default_value;
				}

				return json.get<float>();
			};

			config_table["getbool"] = [](const std::string& key, const variadic_args& va)
				-> script_value
			{
				const auto default_value = va.size() >= 1 ? va[0] : script_value(false);
				const auto value = config::get<nlohmann::json>(key);

				if (!value.has_value())
				{
					return default_value;
				}

				const auto& json = value.value();
				if (!json.is_boolean())
				{
					return default_value;
				}

				return json.get<bool>();
			};

			config_table["set"] = [](const std::string& key, const script_value& value)
			{
				config::set(key, lua_to_json(value));
			};

			auto motd_table = table();
			lua["motd"] = motd_table;

			motd_table["getnumfeaturedtabs"] = motd::get_num_featured_tabs;
			motd_table["getmotd"] = []()
			{
				return json_to_lua(motd::get_motd());
			};

			motd_table["getfeaturedtab"] = [](const int index)
			{
				return json_to_lua(motd::get_featured_tab(index));
			};

			motd_table["hasseentoday"] = []()
			{
				const auto last_seen = config::get<uint64_t>("motd_last_seen");
				if (!last_seen.has_value())
				{
					return false;
				}

				const auto value = static_cast<time_t>(last_seen.value());
				const auto before = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::from_time_t(value));
				const auto now = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
				const auto diff = std::chrono::sys_days{now} - std::chrono::sys_days{before};
				return diff.count() < 1;
			};

			motd_table["sethasseentoday"] = []()
			{
				const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				config::set<uint64_t>("motd_last_seen", static_cast<uint64_t>(now));
			};

			motd_table["hasmotd"] = motd::has_motd;

			auto achievements_table = table();
			lua["achievements"] = achievements_table;

			achievements_table["hasachievement"] = achievements::has_achievement;
			achievements_table["getrarity"] = achievements::get_rarity;
			achievements_table["getname"] = achievements::get_name;
			achievements_table["getdetails"] = achievements::get_details;
			achievements_table["getbackground"] = achievements::get_background;
			achievements_table["issecret"] = achievements::is_secret;
			achievements_table["count"] = achievements::get_count;

			achievements_table["table"] = []()
			{
				table table{};

				achievements::achievement_file_t file{};
				achievements::get_achievements(&file);

				for (auto i = 0; i < achievements::ACHIEVEMENT_TOTAL_COUNT; i++)
				{
					table[i] = file.achievements[i];
				}

				return table;
			};

			table camera_table;
			lua["camera"] = camera_table;

			camera::clear_lua();
			camera_table["enablefreemove"] = camera::enable_free_move;
			camera_table["disablefreemove"] = camera::disable_free_move;
			camera_table["isfreemoveenabled"] = camera::is_free_move_enabled;
			camera_table["setposition"] = camera::set_camera_position;
			camera_table["setangles"] = camera::set_camera_angles;
			camera_table["getposition"] = camera::get_camera_position;
			camera_table["setusingoriginoverride"] = camera::set_using_origin_override;
			camera_table["setusinganglesoverride"] = camera::set_using_angles_override;
			camera_table["setcallback"] = camera::set_callback;
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

			lua["print"] = [](const variadic_args& va)
			{
				std::string buffer{};
				const auto to_string = get_globals()["tostring"];

				for (auto i = 0; i < va.size(); i++)
				{
					const auto& arg = va[i];
					const auto str = to_string(arg)[0].as<std::string>();
					buffer.append(str);

					if (i < va.size() - 1)
					{
						buffer.append("\t");
					}
				}

				console::info("%s\n", buffer.data());
			};

			lua["table"]["unpack"] = lua["unpack"];
			lua["luiglobals"] = lua;

			lua["printmemoryusage"] = []()
			{
				utils::hook::invoke<void>(0x14031F470);
			};

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
			camera::clear_lua();
			lua_functions.clear();
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
				globals.loaded_scripts[globals.raw_script_name] = globals.in_require_script;
				return load_buffer(globals.raw_script_name, utils::io::read_file(globals.raw_script_name));
			}

			return utils::hook::invoke<int>(0x1402D9410, state, compiler_options, reader, reader_data, chunk_name);
		}
		
		int main_handler(game::hks::lua_State* state)
		{
			static std::string error_str;
			auto error = false;

			try
			{
				const auto value = state->m_apistack.base[-1];
				if (value.t != game::hks::TCFUNCTION)
				{
					return 0;
				}

				const auto closure = value.v.cClosure;
				if (closure->m_numUpvalues < 1)
				{
					return 0;
				}

				const auto function = reinterpret_cast<lua_function_t*>(closure->m_upvalues->v.i64);
				const auto args = get_return_values();
				const auto results = function->operator()(args);

				for (const auto& result : results)
				{
					push_value(result);
				}

				return static_cast<int>(results.size());
			}
			catch (const std::exception& e)
			{
				error_str = e.what();
				error = true;
			}

			if (error)
			{
				game::hks::hksi_luaL_error(state, error_str.data());
			}

			return 0;
		}

		int removed_function_stub(game::hks::lua_State* /*state*/)
		{
			return 0;
		}
	}

	template <typename F>
	game::hks::cclosure* convert_function(F f)
	{
		const auto state = *game::hks::lua_state;
		const auto top = state->m_apistack.top;

		game::hks::HksObject func_ptr{};
		func_ptr.t = game::hks::TUI64;
		func_ptr.v.i64 = 0;

		push_value(func_ptr);
		const auto closure = game::hks::cclosure_Create(state, main_handler, 1, 0, 0);
		state->m_apistack.top = top;

		const auto function = wrap_function(f);
		const auto& iterator = lua_functions.insert(lua_functions.end(), std::make_unique<lua_function_t>(function));
		const auto ptr = iterator->get();

		closure->m_upvalues[0].t = game::hks::TUI64;
		closure->m_upvalues[0].v.i64 = reinterpret_cast<size_t>(ptr);

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

			// remove unsafe functions
			utils::hook::nop(0x1402D885A, 1);
			utils::hook::jump(0x14031E700, 0x1402D86E0);

			utils::hook::jump(0x1402BFCC0, removed_function_stub); // io
			utils::hook::jump(0x1402C0150, removed_function_stub); // os
			utils::hook::jump(0x1402C1020, removed_function_stub); // serialize
			utils::hook::jump(0x1402C0FF0, removed_function_stub); // hks
			utils::hook::jump(0x1402C0470, removed_function_stub); // debug
			utils::hook::nop(0x1402BFC48, 5); // coroutine

			// profile
			utils::hook::jump(0x1402B6250, removed_function_stub);
			utils::hook::jump(0x1402B6260, removed_function_stub);
			utils::hook::jump(0x1402B6270, removed_function_stub);
			utils::hook::jump(0x1402B6330, removed_function_stub);

			utils::hook::jump(0x1402B7FD0, removed_function_stub);
			utils::hook::jump(0x1402B7C40, removed_function_stub);
			utils::hook::jump(0x1402BAC30, removed_function_stub);

			utils::hook::jump(0x1402B5480, removed_function_stub);
			utils::hook::jump(0x1402C2030, removed_function_stub);
			utils::hook::jump(0x1402C2180, removed_function_stub);

			utils::hook::jump(0x1402B6F70, removed_function_stub);
			utils::hook::jump(0x1402BD890, removed_function_stub);
		}
	};
}

REGISTER_COMPONENT(ui_scripting::component)
