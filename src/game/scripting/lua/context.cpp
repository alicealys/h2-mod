#include <stdinc.hpp>
#include "context.hpp"
#include "error.hpp"
#include "value_conversion.hpp"

#include "../execution.hpp"
#include "../functions.hpp"

#include "../../../component/command.hpp"

#include <utils/string.hpp>

namespace scripting::lua
{
	namespace
	{
		void setup_entity_type(sol::state& state, event_handler& handler, scheduler& scheduler)
		{
			state["level"] = entity{*game::levelEntityId};

			auto vector_type = state.new_usertype<vector>("vector", sol::constructors<vector(float, float, float)>());
			vector_type["x"] = sol::property(&vector::get_x, &vector::set_x);
			vector_type["y"] = sol::property(&vector::get_y, &vector::set_y);
			vector_type["z"] = sol::property(&vector::get_z, &vector::set_z);

			vector_type["r"] = sol::property(&vector::get_x, &vector::set_x);
			vector_type["g"] = sol::property(&vector::get_y, &vector::set_y);
			vector_type["b"] = sol::property(&vector::get_z, &vector::set_z);

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
