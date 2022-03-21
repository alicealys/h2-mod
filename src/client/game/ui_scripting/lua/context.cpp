#include <std_include.hpp>
#include "context.hpp"
#include "error.hpp"
#include "value_conversion.hpp"
#include "../../scripting/execution.hpp"
#include "../script_value.hpp"
#include "../execution.hpp"

#include "../../../component/ui_scripting.hpp"
#include "../../../component/scripting.hpp"
#include "../../../component/command.hpp"
#include "../../../component/fastfiles.hpp"
#include "../../../component/updater.hpp"
#include "../../../component/localized_strings.hpp"
#include "../../../component/mods.hpp"

#include "component/game_console.hpp"
#include "component/scheduler.hpp"

#include <utils/string.hpp>
#include <utils/nt.hpp>
#include <utils/io.hpp>
#include <utils/http.hpp>
#include <utils/cryptography.hpp>
#include <version.h>

namespace ui_scripting::lua
{
	namespace
	{
		const auto json_script = utils::nt::load_resource(LUA_JSON);

		scripting::script_value script_convert(const sol::lua_value& value)
		{
			if (value.is<int>())
			{
				return {value.as<int>()};
			}

			if (value.is<unsigned int>())
			{
				return {value.as<unsigned int>()};
			}

			if (value.is<bool>())
			{
				return {value.as<bool>()};
			}

			if (value.is<double>())
			{
				return {value.as<double>()};
			}

			if (value.is<float>())
			{
				return {value.as<float>()};
			}
			if (value.is<std::string>())
			{
				return {value.as<std::string>()};
			}

			if (value.is<scripting::vector>())
			{
				return {value.as<scripting::vector>()};
			}

			return {};
		}

		bool valid_dvar_name(const std::string& name)
		{
			for (const auto c : name)
			{
				if (!isalnum(c) && c != '_')
				{
					return false;
				}
			}
			
			return true;
		}

		void setup_io(sol::state& state)
		{
			state["io"]["fileexists"] = utils::io::file_exists;
			state["io"]["writefile"] = utils::io::write_file;
			state["io"]["movefile"] = utils::io::move_file;
			state["io"]["filesize"] = utils::io::file_size;
			state["io"]["createdirectory"] = utils::io::create_directory;
			state["io"]["directoryexists"] = utils::io::directory_exists;
			state["io"]["directoryisempty"] = utils::io::directory_is_empty;
			state["io"]["listfiles"] = utils::io::list_files;
			state["io"]["copyfolder"] = utils::io::copy_folder;
			state["io"]["removefile"] = utils::io::remove_file;
			state["io"]["removedirectory"] = utils::io::remove_directory;
			state["io"]["readfile"] = static_cast<std::string(*)(const std::string&)>(utils::io::read_file);
		}

		void setup_json(sol::state& state)
		{
			const auto json = state.safe_script(json_script, &sol::script_pass_on_error);
			handle_error(json);
			state["json"] = json;
		}

		void setup_vector_type(sol::state& state)
		{
			auto vector_type = state.new_usertype<scripting::vector>("vector", sol::constructors<scripting::vector(float, float, float)>());
			vector_type["x"] = sol::property(&scripting::vector::get_x, &scripting::vector::set_x);
			vector_type["y"] = sol::property(&scripting::vector::get_y, &scripting::vector::set_y);
			vector_type["z"] = sol::property(&scripting::vector::get_z, &scripting::vector::set_z);

			vector_type["r"] = sol::property(&scripting::vector::get_x, &scripting::vector::set_x);
			vector_type["g"] = sol::property(&scripting::vector::get_y, &scripting::vector::set_y);
			vector_type["b"] = sol::property(&scripting::vector::get_z, &scripting::vector::set_z);

			vector_type[sol::meta_function::addition] = sol::overload(
				[](const scripting::vector& a, const scripting::vector& b)
				{
					return scripting::vector(
						a.get_x() + b.get_x(),
						a.get_y() + b.get_y(),
						a.get_z() + b.get_z()
					);
				},
				[](const scripting::vector& a, const int value)
				{
					return scripting::vector(
						a.get_x() + value,
						a.get_y() + value,
						a.get_z() + value
					);
				}
			);

			vector_type[sol::meta_function::subtraction] = sol::overload(
				[](const scripting::vector& a, const scripting::vector& b)
				{
					return scripting::vector(
						a.get_x() - b.get_x(),
						a.get_y() - b.get_y(),
						a.get_z() - b.get_z()
					);
				},
				[](const scripting::vector& a, const int value)
				{
					return scripting::vector(
						a.get_x() - value,
						a.get_y() - value,
						a.get_z() - value
					);
				}
			);

			vector_type[sol::meta_function::multiplication] = sol::overload(
				[](const scripting::vector& a, const scripting::vector& b)
				{
					return scripting::vector(
						a.get_x() * b.get_x(),
						a.get_y() * b.get_y(),
						a.get_z() * b.get_z()
					);
				},
				[](const scripting::vector& a, const int value)
				{
					return scripting::vector(
						a.get_x() * value,
						a.get_y() * value,
						a.get_z() * value
					);
				}
			);

			vector_type[sol::meta_function::division] = sol::overload(
				[](const scripting::vector& a, const scripting::vector& b)
				{
					return scripting::vector(
						a.get_x() / b.get_x(),
						a.get_y() / b.get_y(),
						a.get_z() / b.get_z()
					);
				},
				[](const scripting::vector& a, const int value)
				{
					return scripting::vector(
						a.get_x() / value,
						a.get_y() / value,
						a.get_z() / value
					);
				}
			);

			vector_type[sol::meta_function::equal_to] = [](const scripting::vector& a, const scripting::vector& b)
			{
				return a.get_x() == b.get_x() &&
					   a.get_y() == b.get_y() &&
					   a.get_z() == b.get_z();
			};

			vector_type[sol::meta_function::length] = [](const scripting::vector& a)
			{
				return sqrt((a.get_x() * a.get_x()) + (a.get_y() * a.get_y()) + (a.get_z() * a.get_z()));
			};

			vector_type[sol::meta_function::to_string] = [](const scripting::vector& a)
			{
				return utils::string::va("{x: %f, y: %f, z: %f}", a.get_x(), a.get_y(), a.get_z());
			};
		}

		void setup_game_type(sol::state& state, event_handler& handler, scheduler& scheduler)
		{
			struct game
			{
			};
			auto game_type = state.new_usertype<game>("game_");
			state["game"] = game();

			game_type["time"] = []()
			{
				const auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
				return now.count();
			};

			game_type["executecommand"] = [](const game&, const std::string& command)
			{
				command::execute(command, false);
			};

			game_type["onframe"] = [&scheduler](const game&, const sol::protected_function& callback)
			{
				return scheduler.add(callback, 0, false);
			};

			game_type["ontimeout"] = [&scheduler](const game&, const sol::protected_function& callback,
			                                      const long long milliseconds)
			{
				return scheduler.add(callback, milliseconds, true);
			};

			game_type["oninterval"] = [&scheduler](const game&, const sol::protected_function& callback,
			                                       const long long milliseconds)
			{
				return scheduler.add(callback, milliseconds, false);
			};

			game_type["onnotify"] = [&handler](const game&, const std::string& event,
												  const event_callback& callback)
			{
				event_listener listener{};
				listener.callback = callback;
				listener.event = event;
				listener.is_volatile = false;

				return handler.add_event_listener(std::move(listener));
			};

			game_type["onnotifyonce"] = [&handler](const game&, const std::string& event,
													  const event_callback& callback)
			{
				event_listener listener{};
				listener.callback = callback;
				listener.event = event;
				listener.is_volatile = true;

				return handler.add_event_listener(std::move(listener));
			};

			game_type["isingame"] = []()
			{
				return ::game::CL_IsCgameInitialized() && ::game::g_entities[0].client;
			};

			game_type["getdvar"] = [](const game&, const sol::this_state s, const std::string& name)
			{
				const auto dvar = ::game::Dvar_FindVar(name.data());
				if (!dvar)
				{
					return sol::lua_value{s, sol::lua_nil};
				}

				const std::string value = ::game::Dvar_ValueToString(dvar, nullptr, &dvar->current);
				return sol::lua_value{s, value};
			};

			game_type["getdvarint"] = [](const game&, const sol::this_state s, const std::string& name)
			{
				const auto dvar = ::game::Dvar_FindVar(name.data());
				if (!dvar)
				{
					return sol::lua_value{s, sol::lua_nil};
				}

				const auto value = atoi(::game::Dvar_ValueToString(dvar, nullptr, &dvar->current));
				return sol::lua_value{s, value};
			};

			game_type["getdvarfloat"] = [](const game&, const sol::this_state s, const std::string& name)
			{
				const auto dvar = ::game::Dvar_FindVar(name.data());
				if (!dvar)
				{
					return sol::lua_value{s, sol::lua_nil};
				}

				const auto value = atof(::game::Dvar_ValueToString(dvar, nullptr, &dvar->current));
				return sol::lua_value{s, value};
			};

			game_type["setdvar"] = [](const game&, const std::string& name, const sol::lua_value& value)
			{
				if (!valid_dvar_name(name))
				{
					throw std::runtime_error("Invalid DVAR name, must be alphanumeric");
				}

				const auto hash = ::game::generateHashValue(name.data());
				std::string string_value;

				if (value.is<bool>())
				{
					string_value = utils::string::va("%i", value.as<bool>());
				} 
				else if (value.is<int>())
				{
					string_value = utils::string::va("%i", value.as<int>());
				} 
				else if (value.is<float>())
				{
					string_value = utils::string::va("%f", value.as<float>());
				}
				else if (value.is<scripting::vector>())
				{
					const auto v = value.as<scripting::vector>();
					string_value = utils::string::va("%f %f %f", 
						v.get_x(),
						v.get_y(),
						v.get_z()
					);
				}

				if (value.is<std::string>())
				{
					string_value = value.as<std::string>();
				}

				::game::Dvar_SetCommand(hash, "", string_value.data());
			};

			game_type["getwindowsize"] = [](const game&, const sol::this_state s)
			{
				const auto size = ::game::ScrPlace_GetViewPlacement()->realViewportSize;

				auto screen = sol::table::create(s.lua_state());
				screen["x"] = size[0];
				screen["y"] = size[1];

				return screen;
			};

			game_type["playmenuvideo"] = [](const game&, const std::string& video)
			{
				reinterpret_cast<void (*)(const char* a1, int a2, int a3)>
					(0x14071B970)(video.data(), 64, 0);
			};

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

			game_type["assetlist"] = [](const game&, const sol::this_state s, const std::string& type_string)
			{
				auto table = sol::table::create(s.lua_state());
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
				fastfiles::enum_assets(type, [type, &table, &index](const ::game::XAssetHeader header)
				{
					const auto asset = ::game::XAsset{type, header};
					const std::string asset_name = ::game::DB_GetXAssetName(&asset);
					table[index++] = asset_name;
				}, true);

				return table;
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
				localized_strings::override(string, value);
			};

			struct player
			{
			};
			auto player_type = state.new_usertype<player>("player_");
			state["player"] = player();

			player_type["notify"] = [](const player&, const sol::this_state s, const std::string& name, sol::variadic_args va)
			{
				if (!::game::CL_IsCgameInitialized() || !::game::g_entities[0].client)
				{
					throw std::runtime_error("Not in game");
				}

				::scheduler::once([s, name, args = std::vector<sol::object>(va.begin(), va.end())]()
				{
					try
					{
						std::vector<scripting::script_value> arguments{};

						for (auto arg : args)
						{
							arguments.push_back(script_convert({s, arg}));
						}

						const auto player = scripting::call("getentbynum", {0}).as<scripting::entity>();
						scripting::notify(player, name, arguments);
					}
					catch (...)
					{
					}
				}, ::scheduler::pipeline::server);
			};
		}

		void setup_lui_types(sol::state& state, event_handler& handler, scheduler& scheduler)
		{
			auto userdata_type = state.new_usertype<userdata>("userdata_");

			userdata_type["new"] = sol::property(
				[](const userdata& userdata, const sol::this_state s)
				{
					return convert(s, userdata.get("new"));
				},
				[](const userdata& userdata, const sol::this_state s, const sol::lua_value& value)
				{
					userdata.set("new", convert({s, value}));
				}
			);

			userdata_type[sol::meta_function::index] = [](const userdata& userdata, const sol::this_state s, 
				const std::string& name)
			{
				return convert(s, userdata.get(name));
			};

			userdata_type[sol::meta_function::new_index] = [](const userdata& userdata, const sol::this_state s, 
				const std::string& name, const sol::lua_value& value)
			{
				userdata.set(name, convert({s, value}));
			};

			auto table_type = state.new_usertype<table>("table_");

			table_type["new"] = sol::property(
				[](const table& table, const sol::this_state s)
				{
					return convert(s, table.get("new"));
				},
				[](const table& table, const sol::this_state s, const sol::lua_value& value)
				{
					table.set("new", convert({s, value}));
				}
			);

			table_type["get"] = [](const table& table, const sol::this_state s,
				const std::string& name)
			{
				return convert(s, table.get(name));
			};

			table_type["set"] = [](const table& table, const sol::this_state s,
				const std::string& name, const sol::lua_value& value)
			{
				table.set(name, convert({s, value}));
			};

			table_type[sol::meta_function::index] = [](const table& table, const sol::this_state s,
				const std::string& name)
			{
				return convert(s, table.get(name));
			};

			table_type[sol::meta_function::new_index] = [](const table& table, const sol::this_state s,
				const std::string& name, const sol::lua_value& value)
			{
				table.set(name, convert({s, value}));
			};

			auto function_type = state.new_usertype<function>("function_");

			function_type[sol::meta_function::call] = [](const function& function, const sol::this_state s, sol::variadic_args va)
			{
				arguments arguments{};

				for (auto arg : va)
				{
					arguments.push_back(convert({s, arg}));
				}

				const auto values = function.call(arguments);
				std::vector<sol::lua_value> returns;

				for (const auto& value : values)
				{
					returns.push_back(convert(s, value));
				}

				return sol::as_returns(returns);
			};

			state["luiglobals"] = table((*::game::hks::lua_state)->globals.v.table);
			state["CoD"] = state["luiglobals"]["CoD"];
			state["LUI"] = state["luiglobals"]["LUI"];
			state["Engine"] = state["luiglobals"]["Engine"];
			state["Game"] = state["luiglobals"]["Game"];

			auto updater_table = sol::table::create(state.lua_state());

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

			state["updater"] = updater_table;
		}
	}

	context::context(std::string data, script_type type)
		: scheduler_(state_)
		  , event_handler_(state_)

	{
		this->state_.open_libraries(sol::lib::base,
		                            sol::lib::package,
		                            sol::lib::io,
		                            sol::lib::string,
		                            sol::lib::os,
		                            sol::lib::math,
		                            sol::lib::table);

		setup_io(this->state_);
		setup_json(this->state_);
		setup_vector_type(this->state_);
		setup_game_type(this->state_, this->event_handler_, this->scheduler_);
		setup_lui_types(this->state_, this->event_handler_, this->scheduler_);

		if (type == script_type::file)
		{
			this->folder_ = data;

			this->state_["include"] = [this](const std::string& file)
			{
				this->load_script(file);
			};

			sol::function old_require = this->state_["require"];
			auto base_path = utils::string::replace(this->folder_, "/", ".") + ".";
			this->state_["require"] = [base_path, old_require](const std::string& path)
			{
				return old_require(base_path + path);
			};

			this->state_["scriptdir"] = [this]()
			{
				return this->folder_;
			};

			printf("Loading ui script '%s'\n", this->folder_.data());
			this->load_script("__init__");
		}
		
		if (type == script_type::code)
		{
			handle_error(this->state_.safe_script(data, &sol::script_pass_on_error));
		}
	}

	context::~context()
	{
		this->state_.collect_garbage();
		this->scheduler_.clear();
		this->event_handler_.clear();
		this->state_ = {};
	}

	void context::run_frame()
	{
		this->scheduler_.run_frame();
		this->state_.collect_garbage();
	}

	void context::notify(const event& e)
	{
		this->scheduler_.dispatch(e);
		this->event_handler_.dispatch(e);
	}

	void context::load_script(const std::string& script)
	{
		if (!this->loaded_scripts_.emplace(script).second)
		{
			return;
		}

		const auto file = (std::filesystem::path{this->folder_} / (script + ".lua")).generic_string();
		handle_error(this->state_.safe_script_file(file, &sol::script_pass_on_error));
	}
}
