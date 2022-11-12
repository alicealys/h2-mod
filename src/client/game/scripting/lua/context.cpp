#include <std_include.hpp>
#include "context.hpp"
#include "error.hpp"
#include "value_conversion.hpp"

#include "game/scripting/execution.hpp"

#include "component/notifies.hpp"
#include "component/scripting.hpp"
#include "component/command.hpp"
#include "component/fastfiles.hpp"
#include "component/mods.hpp"
#include "component/localized_strings.hpp"
#include "component/scheduler.hpp"

#include "game/ui_scripting/execution.hpp"

#include <xsk/gsc/types.hpp>
#include <xsk/resolver.hpp>

#include <utils/string.hpp>
#include <utils/io.hpp>
#include <utils/nt.hpp>

namespace scripting::lua
{
	namespace
	{
		const auto json_script = utils::nt::load_resource(LUA_JSON);

		vector normalize_vector(const vector& vec)
		{
			const auto length = sqrt((vec.get_x() * vec.get_x()) + (vec.get_y() * vec.get_y()) + (vec.get_z() * vec.get_z()));

			return vector(
				vec.get_x() / length,
				vec.get_y() / length,
				vec.get_z() / length
			);
		}

		void setup_json(sol::state& state)
		{
			const auto json = state.safe_script(json_script, &sol::script_pass_on_error);
			handle_error(json);
			state["json"] = json;
		}

		void setup_io(sol::state& state)
		{
			state["io"]["fileexists"] = utils::io::file_exists;
			state["io"]["writefile"] = utils::io::write_file;
			state["io"]["filesize"] = utils::io::file_size;
			state["io"]["createdirectory"] = utils::io::create_directory;
			state["io"]["directoryexists"] = utils::io::directory_exists;
			state["io"]["directoryisempty"] = utils::io::directory_is_empty;
			state["io"]["listfiles"] = utils::io::list_files;
			state["io"]["copyfolder"] = utils::io::copy_folder;
			state["io"]["readfile"] = static_cast<std::string(*)(const std::string&)>(utils::io::read_file);
		}

		void setup_vector_type(sol::state& state)
		{
			state["level"] = entity{*::game::levelEntityId};
			state["player"] = call("getentbynum", {0}).as<entity>();

			auto vector_type = state.new_usertype<vector>("vector", sol::constructors<vector(float, float, float)>());
			vector_type["x"] = sol::property(&vector::get_x, &vector::set_x);
			vector_type["y"] = sol::property(&vector::get_y, &vector::set_y);
			vector_type["z"] = sol::property(&vector::get_z, &vector::set_z);

			vector_type["r"] = sol::property(&vector::get_x, &vector::set_x);
			vector_type["g"] = sol::property(&vector::get_y, &vector::set_y);
			vector_type["b"] = sol::property(&vector::get_z, &vector::set_z);

			vector_type[sol::meta_function::addition] = sol::overload(
				[](const vector& a, const vector& b)
				{
					return vector(
						a.get_x() + b.get_x(),
						a.get_y() + b.get_y(),
						a.get_z() + b.get_z()
					);
				},
				[](const vector& a, const int value)
				{
					return vector(
						a.get_x() + value,
						a.get_y() + value,
						a.get_z() + value
					);
				}
			);

			vector_type[sol::meta_function::subtraction] = sol::overload(
				[](const vector& a, const vector& b)
				{
					return vector(
						a.get_x() - b.get_x(),
						a.get_y() - b.get_y(),
						a.get_z() - b.get_z()
					);
				},
				[](const vector& a, const int value)
				{
					return vector(
						a.get_x() - value,
						a.get_y() - value,
						a.get_z() - value
					);
				}
			);

			vector_type[sol::meta_function::multiplication] = sol::overload(
				[](const vector& a, const vector& b)
				{
					return vector(
						a.get_x() * b.get_x(),
						a.get_y() * b.get_y(),
						a.get_z() * b.get_z()
					);
				},
				[](const vector& a, const int value)
				{
					return vector(
						a.get_x() * value,
						a.get_y() * value,
						a.get_z() * value
					);
				}
			);

			vector_type[sol::meta_function::division] = sol::overload(
				[](const vector& a, const vector& b)
				{
					return vector(
						a.get_x() / b.get_x(),
						a.get_y() / b.get_y(),
						a.get_z() / b.get_z()
					);
				},
				[](const vector& a, const int value)
				{
					return vector(
						a.get_x() / value,
						a.get_y() / value,
						a.get_z() / value
					);
				}
			);

			vector_type[sol::meta_function::equal_to] = [](const vector& a, const vector& b)
			{
				return a.get_x() == b.get_x() &&
					   a.get_y() == b.get_y() &&
					   a.get_z() == b.get_z();
			};

			vector_type[sol::meta_function::length] = [](const vector& a)
			{
				return sqrt((a.get_x() * a.get_x()) + (a.get_y() * a.get_y()) + (a.get_z() * a.get_z()));
			};

			vector_type[sol::meta_function::to_string] = [](const vector& a)
			{
				return utils::string::va("{x: %f, y: %f, z: %f}", a.get_x(), a.get_y(), a.get_z());
			};

			vector_type["normalize"] = [](const vector& a)
			{
				return normalize_vector(a);
			};

			vector_type["toangles"] = [](const vector& a)
			{
				return call("vectortoangles", {a}).as<vector>();
			};

			vector_type["toyaw"] = [](const vector& a)
			{
				return call("vectortoyaw", {a}).as<vector>();
			};

			vector_type["tolerp"] = [](const vector& a)
			{
				return call("vectortolerp", {a}).as<vector>();
			};

			vector_type["toup"] = [](const vector& a)
			{
				return call("anglestoup", {a}).as<vector>();
			};

			vector_type["toright"] = [](const vector& a)
			{
				return call("anglestoright", {a}).as<vector>();
			};

			vector_type["toforward"] = [](const vector& a)
			{
				return call("anglestoforward", {a}).as<vector>();
			};
		}

		void setup_entity_type(sol::state& state, event_handler& handler, scheduler& scheduler)
		{
			state["level"] = entity{*::game::levelEntityId};
			state["player"] = call("getentbynum", {0}).as<entity>();

			auto animation_type = state.new_usertype<animation>("animation");

			auto array_type = state.new_usertype<array>("array", sol::constructors<array()>());

			array_type["erase"] = [](const array& array, const sol::this_state s,
				const sol::lua_value& key)
			{
				if (key.is<int>())
				{
					const auto index = key.as<int>() - 1;
					array.erase(index);
				}
				else if (key.is<std::string>())
				{
					const auto _key = key.as<std::string>();
					array.erase(_key);
				}
			};

			array_type["push"] = [](const array& array, const sol::this_state s,
				const sol::lua_value& value)
			{
				const auto _value = convert(value);
				array.push(_value);
			};

			array_type["pop"] = [](const array& array, const sol::this_state s)
			{
				return convert(s, array.pop());
			};

			array_type["get"] = [](const array& array, const sol::this_state s,
				const sol::lua_value& key)
			{
				if (key.is<int>())
				{
					const auto index = key.as<int>() - 1;
					return convert(s, array.get(index));
				}
				else if (key.is<std::string>())
				{
					const auto _key = key.as<std::string>();
					return convert(s, array.get(_key));
				}

				return sol::lua_value{s, sol::lua_nil};
			};

			array_type["set"] = [](const array& array, const sol::this_state s,
				const sol::lua_value& key, const sol::lua_value& value)
			{
				const auto _value = convert(value);
				const auto nil = _value.get_raw().type == 0;

				if (key.is<int>())
				{
					const auto index = key.as<int>() - 1;
					nil ? array.erase(index) : array.set(index, _value);
				}
				else if (key.is<std::string>())
				{
					const auto _key = key.as<std::string>();
					nil ? array.erase(_key) : array.set(_key, _value);
				}
			};

			array_type["size"] = [](const array& array, const sol::this_state s)
			{
				return array.size();
			};

			array_type[sol::meta_function::length] = [](const array& array, const sol::this_state s)
			{
				return array.size();
			};

			array_type[sol::meta_function::index] = [](const array& array, const sol::this_state s,
				const sol::lua_value& key)
			{
				if (key.is<int>())
				{
					const auto index = key.as<int>() - 1;
					return convert(s, array.get(index));
				}
				else if (key.is<std::string>())
				{
					const auto _key = key.as<std::string>();
					return convert(s, array.get(_key));
				}

				return sol::lua_value{s, sol::lua_nil};
			};

			array_type[sol::meta_function::new_index] = [](const array& array, const sol::this_state s,
				const sol::lua_value& key, const sol::lua_value& value)
			{
				const auto _value = convert(value);
				const auto nil = _value.get_raw().type == 0;

				if (key.is<int>())
				{
					const auto index = key.as<int>() - 1;
					nil ? array.erase(index) : array.set(index, _value);
				}
				else if (key.is<std::string>())
				{
					const auto _key = key.as<std::string>();
					nil ? array.erase(_key) : array.set(_key, _value);
				}
			};

			array_type["getkeys"] = [](const array& array, const sol::this_state s)
			{
				std::vector<sol::lua_value> keys;

				const auto keys_ = array.get_keys();
				for (const auto& key : keys_)
				{
					keys.push_back(convert(s, key));
				}
				
				return keys;
			};

			array_type["getentity"] = [](const array& array, const sol::this_state s)
			{
				return array.get_raw();
			};

			auto entity_type = state.new_usertype<entity>("entity");

			for (const auto& func : xsk::gsc::h2::resolver::get_methods())
			{
				const auto name = std::string(func.first);
				entity_type[name.data()] = [name](const entity& entity, const sol::this_state s, sol::variadic_args va)
				{
					std::vector<script_value> arguments{};

					for (auto arg : va)
					{
						arguments.push_back(convert({s, arg}));
					}

					return convert(s, entity.call(name, arguments));
				};
			}

			entity_type["set"] = [](const entity& entity, const std::string& field,
			                        const sol::lua_value& value)
			{
				entity.set(field, convert(value));
			};

			entity_type["get"] = [](const entity& entity, const sol::this_state s, const std::string& field)
			{
				return convert(s, entity.get(field));
			};

			entity_type["notify"] = [](const entity& entity, const sol::this_state s, const std::string& event, 
									   sol::variadic_args va)
			{
				std::vector<script_value> arguments{};

				for (auto arg : va)
				{
					arguments.push_back(convert({s, arg}));
				}

				notify(entity, event, arguments);
			};

			entity_type["onnotify"] = [&handler](const entity& entity, const std::string& event,
			                                     const event_callback& callback)
			{
				event_listener listener{};
				listener.callback = callback;
				listener.entity = entity;
				listener.event = event;
				listener.is_volatile = false;

				return handler.add_event_listener(std::move(listener));
			};

			entity_type["onnotifyonce"] = [&handler](const entity& entity, const std::string& event,
			                                         const event_callback& callback)
			{
				event_listener listener{};
				listener.callback = callback;
				listener.entity = entity;
				listener.event = event;
				listener.is_volatile = true;

				return handler.add_event_listener(std::move(listener));
			};

			entity_type["call"] = [](const entity& entity, const sol::this_state s, const std::string& function,
			                         sol::variadic_args va)
			{
				std::vector<script_value> arguments{};

				for (auto arg : va)
				{
					arguments.push_back(convert({s, arg}));
				}

				return convert(s, entity.call(function, arguments));
			};

			entity_type[sol::meta_function::new_index] = [](const entity& entity, const std::string& field,
															const sol::lua_value& value)
			{
				entity.set(field, convert(value));
			};

			entity_type[sol::meta_function::index] = [](const entity& entity, const sol::this_state s, const std::string& field)
			{
				return convert(s, entity.get(field));
			};

			entity_type["struct"] = sol::property([](const entity& entity, const sol::this_state s)
			{
				return entity;
			});

			entity_type["getstruct"] = [](const entity& entity, const sol::this_state s)
			{
				return entity;
			};

			entity_type["scriptcall"] = [](const entity& entity, const sol::this_state s, const std::string& filename,
				const std::string function, sol::variadic_args va)
			{
				std::vector<script_value> arguments{};

				for (auto arg : va)
				{
					arguments.push_back(convert({s, arg}));
				}

				notifies::hook_enabled = false;
				const auto result = convert(s, call_script_function(entity, filename, function, arguments));
				notifies::hook_enabled = true;

				return result;
			};

			entity_type["getentref"] = [](const entity& entity)
			{
				const auto entref = entity.get_entity_reference();
				std::vector<unsigned int> returns = {entref.entnum, entref.classnum};
				return sol::as_returns(returns);
			};
		}

		void setup_game_type(sol::state& state, event_handler& handler, scheduler& scheduler)
		{
			struct game
			{
			};
			auto game_type = state.new_usertype<game>("game_");
			state["game"] = game();

			for (const auto& func : xsk::gsc::h2::resolver::get_functions())
			{
				const auto name = std::string(func.first);
				game_type[name] = [name](const game&, const sol::this_state s, sol::variadic_args va)
				{
					std::vector<script_value> arguments{};

					for (auto arg : va)
					{
						arguments.push_back(convert({s, arg}));
					}

					return convert(s, call(name, arguments));
				};
			}

			game_type["call"] = [](const game&, const sol::this_state s, const std::string& function,
			                       sol::variadic_args va)
			{
				std::vector<script_value> arguments{};

				for (auto arg : va)
				{
					arguments.push_back(convert({s, arg}));
				}

				return convert(s, call(function, arguments));
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

			game_type["executecommand"] = [](const game&, const std::string& command)
			{
				command::execute(command, false);
			};

			game_type["setdiscordstate"] = [](const game&, const std::string& state)
			{
				command::execute(utils::string::va("setdiscordstate %s", state.data()), false);
			};

			game_type["setdiscorddetails"] = [](const game&, const std::string& state)
			{
				command::execute(utils::string::va("setdiscorddetails %s", state.data()), false);
			};

			game_type["say"] = [](const game&)
			{
			};

			game_type["detour"] = [](const game&, const sol::this_state s, const std::string& filename,
				const std::string function_name, const sol::protected_function& function)
			{
				const auto pos = get_function_pos(filename, function_name);
				notifies::set_lua_hook(pos, function);

				auto detour = sol::table::create(function.lua_state());

				detour["disable"] = [pos]()
				{
					notifies::clear_hook(pos);
				};

				detour["enable"] = [&]()
				{
					notifies::set_lua_hook(pos, function);
				};

				detour["invoke"] = [filename, function_name](const entity& entity, const sol::this_state s, sol::variadic_args va)
				{
					std::vector<script_value> arguments{};

					for (auto arg : va)
					{
						arguments.push_back(convert({s, arg}));
					}

					notifies::hook_enabled = false;
					const auto result = convert(s, call_script_function(entity, filename, function_name, arguments));
					notifies::hook_enabled = true;

					return result;
				};

				return detour;
			};

			game_type["getfunctions"] = [](const game&, const sol::this_state s, const std::string& filename)
			{
				if (scripting::script_function_table.find(filename) == scripting::script_function_table.end())
				{
					throw std::runtime_error("File '" + filename + "' not found");
				}

				auto functions = sol::table::create(s.lua_state());

				for (const auto& function : scripting::script_function_table[filename])
				{
					functions[function.first] = [filename, function](const entity& entity, const sol::this_state s, sol::variadic_args va)
					{
						std::vector<script_value> arguments{};

						for (auto arg : va)
						{
							arguments.push_back(convert({s, arg}));
						}

						notifies::hook_enabled = false;
						const auto result = convert(s, call_script_function(entity, filename, function.first, arguments));
						notifies::hook_enabled = true;

						return result;
					};
				}

				return functions;
			};

			game_type["include"] = [](const game&, const sol::this_state s,
				const std::string& filename)
			{
				sol::state_view state = s;

				if (scripting::script_function_table.find(filename) == scripting::script_function_table.end())
				{
					throw std::runtime_error("File '" + filename + "' not found");
				}

				for (const auto& function : scripting::script_function_table[filename])
				{
					const auto name = utils::string::to_lower(function.first);

					state["game_"][name] = [filename, function](const game&, const sol::this_state s,
						sol::variadic_args va)
					{
						std::vector<script_value> arguments{};

						for (auto arg : va)
						{
							arguments.push_back(convert({s, arg}));
						}

						notifies::hook_enabled = false;
						const auto result = convert(s, call_script_function(*::game::levelEntityId, filename, function.first, arguments));
						notifies::hook_enabled = true;
					};

					state["entity"][name] = [filename, function](const entity& entity, const sol::this_state s,
						sol::variadic_args va)
					{
						std::vector<script_value> arguments{};

						for (auto arg : va)
						{
							arguments.push_back(convert({s, arg}));
						}

						notifies::hook_enabled = false;
						const auto result = convert(s, call_script_function(entity, filename, function.first, arguments));
						notifies::hook_enabled = true;
					};
				}
			};

			game_type["scriptcall"] = [](const game&, const sol::this_state s, const std::string& filename,
				const std::string function, sol::variadic_args va)
			{
				std::vector<script_value> arguments{};

				for (auto arg : va)
				{
					arguments.push_back(convert({s, arg}));
				}

				const auto level = entity{*::game::levelEntityId};

				notifies::hook_enabled = false;
				const auto result = convert(s, call_script_function(level, filename, function, arguments));
				notifies::hook_enabled = true;

				return result;
			};

			game_type["onentitydamage"] = [](const game&, const sol::protected_function& callback)
			{
				notifies::add_entity_damage_callback(callback);
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

			game_type["getentbyref"] = [](const game&, const sol::this_state s, 
				const unsigned int entnum, const unsigned int classnum)
			{
				const auto id = ::game::Scr_GetEntityId(entnum, classnum);
				if (id)
				{
					return convert(s, scripting::entity{id});
				}
				else
				{
					return sol::lua_value{s, sol::lua_nil};
				}
			};

			game_type["getvarusage"] = [](const game&)
			{
				auto count = 0;
				for (auto i = 0; i < 56320; i++)
				{
					const auto value = ::game::scr_VarGlob->objectVariableValue[i];
					count += value.w.type != 24;
				}
				return count;
			};

			game_type["getchildvarusage"] = [](const game&)
			{
				auto count = 0;
				for (auto i = 0; i < 384000; i++)
				{
					const auto value = ::game::scr_VarGlob->childVariableValue[i];
					count += value.type != 24;
				}
				return count;
			};

			game_type["getloadedmod"] = [](const game&)
			{
				const auto& mod = mods::get_mod();
				return mod.value_or("");
			};

			game_type["addlocalizedstring"] = [](const game&, const std::string& string,
				const std::string& value)
			{
				localized_strings::override(string, value, true);
			};

			game_type["overridedvarint"] = [](const game&, const std::string& dvar, const int value)
			{
				scripting::get_dvar_int_overrides[dvar] = value;
			};

			game_type["removedvarintoverride"] = [](const game&, const std::string& dvar)
			{
				scripting::get_dvar_int_overrides.erase(dvar);
			};

			game_type["luinotify"] = [](const game&, const std::string& name, const std::string& data)
			{
				::scheduler::once([=]()
				{
					ui_scripting::notify(name, {{"data", data}});
				}, ::scheduler::pipeline::lui);
			};

			auto function_ptr_type = state.new_usertype<function_ptr>("functionptr", 
				sol::constructors<function_ptr(const std::string&, const std::string&)>());

			function_ptr_type["getpos"] = [](const function_ptr& ptr)
			{
				return reinterpret_cast<uint64_t>(ptr.get_pos());
			};
		}
	}

	context::context(std::string folder)
		: folder_(std::move(folder))
		  , scheduler_(state_)
		  , event_handler_(state_)

	{
		this->state_.open_libraries(sol::lib::base,
		                            sol::lib::package,
		                            sol::lib::io,
		                            sol::lib::string,
		                            sol::lib::os,
		                            sol::lib::math,
		                            sol::lib::table);

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

		setup_io(this->state_);
		setup_json(this->state_);
		setup_vector_type(this->state_);
		setup_entity_type(this->state_, this->event_handler_, this->scheduler_);
		setup_game_type(this->state_, this->event_handler_, this->scheduler_);

		printf("Loading script '%s'\n", this->folder_.data());
		this->load_script("__init__");
	}

	context::context()
		: folder_({})
		  , scheduler_(state_)
		  , event_handler_(state_)

	{
		this->state_.open_libraries(sol::lib::base,
		                            sol::lib::package,
		                            sol::lib::io,
		                            sol::lib::string,
		                            sol::lib::os,
		                            sol::lib::math,
		                            sol::lib::table);

		this->state_["include"] = []()
		{
		};

		this->state_["require"] = []()
		{
		};

		setup_io(this->state_);
		setup_json(this->state_);
		setup_vector_type(this->state_);
		setup_entity_type(this->state_, this->event_handler_, this->scheduler_);
		setup_game_type(this->state_, this->event_handler_, this->scheduler_);
	}

	std::string context::load(const std::string& code)
	{
		try
		{
			const auto result = this->state_.safe_script(code, &sol::script_pass_on_error);
			if (result.valid())
			{
				const auto object = result.get<sol::object>();
				return this->state_["tostring"](object).get<std::string>();
			}
			else
			{
				const sol::error error = result;
				return error.what();
			}
		}
		catch (const std::exception& e)
		{
			return e.what();
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
		this->event_handler_.dispatch(e);
	}

	void context::handle_endon_conditions(const event& e)
	{
		this->scheduler_.dispatch(e);
		this->event_handler_.handle_endon_conditions(e);
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
