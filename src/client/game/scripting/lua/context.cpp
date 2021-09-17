#include <std_include.hpp>
#include "context.hpp"
#include "error.hpp"
#include "value_conversion.hpp"

#include "../execution.hpp"
#include "../functions.hpp"

#include "../../../component/notifies.hpp"
#include "../../../component/scripting.hpp"
#include "../../../component/command.hpp"
#include "../../../component/chat.hpp"

#include <utils/string.hpp>
#include <utils/io.hpp>

namespace scripting::lua
{
	namespace
	{
		vector normalize_vector(const vector& vec)
		{
			const auto length = sqrt((vec.get_x() * vec.get_x()) + (vec.get_y() * vec.get_y()) + (vec.get_z() * vec.get_z()));

			return vector(
				vec.get_x() / length,
				vec.get_y() / length,
				vec.get_z() / length
			);
		}

		void setup_entity_type(sol::state& state, event_handler& handler, scheduler& scheduler)
		{
			state["level"] = entity{*game::levelEntityId};

			state["io"]["fileexists"] = utils::io::file_exists;
			state["io"]["writefile"] = utils::io::write_file;
			state["io"]["filesize"] = utils::io::file_size;
			state["io"]["createdirectory"] = utils::io::create_directory;
			state["io"]["directoryexists"] = utils::io::directory_exists;
			state["io"]["directoryisempty"] = utils::io::directory_is_empty;
			state["io"]["listfiles"] = utils::io::list_files;
			state["io"]["copyfolder"] = utils::io::copy_folder;
			state["io"]["readfile"] = static_cast<std::string(*)(const std::string&)>(utils::io::read_file);

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

			auto entity_type = state.new_usertype<entity>("entity");

			for (const auto& func : method_map)
			{
				const auto name = utils::string::to_lower(func.first);
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
				const auto id = entity.get_entity_id();
				return scripting::lua::entity_to_struct(s, id);
			});

			entity_type["getstruct"] = [](const entity& entity, const sol::this_state s)
			{
				const auto id = entity.get_entity_id();
				return scripting::lua::entity_to_struct(s, id);
			};

			struct game
			{
			};
			auto game_type = state.new_usertype<game>("game_");
			state["game"] = game();

			for (const auto& func : function_map)
			{
				const auto name = utils::string::to_lower(func.first);
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

			game_type["say"] = [](const game&, const std::string& msg)
			{
				chat::print(msg);
			};

			game_type["detour"] = [](const game&, const sol::this_state s, const std::string& filename,
				const std::string function_name, const sol::protected_function& function)
			{
				const auto pos = get_function_pos(filename, function_name);
				notifies::vm_execute_hooks[pos] = function;

				auto detour = sol::table::create(function.lua_state());

				detour["disable"] = [pos]()
				{
					notifies::vm_execute_hooks.erase(pos);
				};

				detour["enable"] = [pos, function]()
				{
					notifies::vm_execute_hooks[pos] = function;
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

			game_type["getfunctions"] = [entity_type](const game&, const sol::this_state s, const std::string& filename)
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

		setup_entity_type(this->state_, this->event_handler_, this->scheduler_);

		printf("Loading script '%s'\n", this->folder_.data());
		this->load_script("__init__");
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
