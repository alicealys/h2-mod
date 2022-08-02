#include <std_include.hpp>
#include "value_conversion.hpp"
#include "../functions.hpp"
#include "../execution.hpp"
#include ".../../component/notifies.hpp"

namespace scripting::lua
{
	namespace
	{
		struct array_value
		{
			int index;
			script_value value;
		};

		sol::lua_value entity_to_array(lua_State* state, unsigned int id)
		{
			auto table = sol::table::create(state);
			auto metatable = sol::table::create(state);

			std::unordered_map<std::string, array_value> values;

			const auto offset = 0xA000 * (id & 3);

			auto current = game::scr_VarGlob->objectVariableChildren[id].firstChild;
			auto idx = 1;

			for (auto i = offset + current; current; i = offset + current)
			{
				const auto var = game::scr_VarGlob->childVariableValue[i];

				if (var.type == game::SCRIPT_NONE)
				{
					current = var.nextSibling;
					continue;
				}

				const auto string_value = (game::scr_string_t)((unsigned __int8)var.name_lo + (var.k.keys.name_hi << 8));
				const auto* str = game::SL_ConvertToString(string_value);

				std::string key = string_value < 0x40000 && str
					? str
					: std::to_string(idx++);

				game::VariableValue variable{};
				variable.type = var.type;
				variable.u = var.u.u;

				array_value value;
				value.index = i;
				value.value = variable;

				values[key] = value;

				current = var.nextSibling;
			}

			table["getkeys"] = [values]()
			{
				std::vector<std::string> _keys;

				for (const auto& entry : values)
				{
					_keys.push_back(entry.first);
				}

				return _keys;
			};

			metatable[sol::meta_function::new_index] = [values](const sol::table t, const sol::this_state s,
				const sol::lua_value& key_value, const sol::lua_value& value)
			{
				const auto key = key_value.is<int>()
					? std::to_string(key_value.as<int>())
					: key_value.as<std::string>();

				if (values.find(key) == values.end())
				{
					return;
				}

				const auto variable = convert({s, value}).get_raw();
				const auto i = values.at(key).index;

				game::scr_VarGlob->childVariableValue[i].type = (char)variable.type;
				game::scr_VarGlob->childVariableValue[i].u.u = variable.u;
			};

			metatable[sol::meta_function::index] = [values](const sol::table t, const sol::this_state s,
				const sol::lua_value& key_value)
			{
				const auto key = key_value.is<int>()
					? std::to_string(key_value.as<int>())
					: key_value.as<std::string>();

				if (values.find(key) == values.end())
				{
					return sol::lua_value{s, sol::lua_nil};
				}

				return convert(s, values.at(key).value);
			};

			metatable[sol::meta_function::length] = [values]()
			{
				return values.size();
			};

			table[sol::metatable_key] = metatable;

			return {state, table};
		}

		bool is_istring(const sol::lua_value& value)
		{
			if (!value.is<std::string>())
			{
				return false;
			}

			const auto str = value.as<std::string>();

			return str[0] == '&';
		}

		script_value string_to_istring(const sol::lua_value& value)
		{
			const auto str = value.as<std::string>().erase(0, 1);
			const auto string_value = game::SL_GetString(str.data(), 0);

			game::VariableValue variable{};
			variable.type = game::SCRIPT_ISTRING;
			variable.u.uintValue = string_value;

			const auto _ = gsl::finally([&variable]()
			{
				game::RemoveRefToValue(variable.type, variable.u);
			});

			return script_value(variable);
		}

		game::VariableValue convert_function(sol::lua_value value)
		{
			const auto function = value.as<sol::protected_function>();
			const auto index = (char*)notifies::vm_execute_hooks.size() + 1;

			notifies::vm_execute_hooks[index] = function;

			game::VariableValue func;
			func.type = game::SCRIPT_FUNCTION;
			func.u.codePosValue = index;

			return func;
		}

		sol::lua_value convert_function(lua_State* state, const char* pos)
		{
			return [pos](const entity& entity, const sol::this_state s, sol::variadic_args va)
			{
				std::vector<script_value> arguments{};

				for (auto arg : va)
				{
					arguments.push_back(convert({s, arg}));
				}

				return convert(s, scripting::exec_ent_thread(entity, pos, arguments));
			};
		}
	}

	sol::lua_value entity_to_struct(lua_State* state, unsigned int parent_id)
	{
		auto table = sol::table::create(state);
		auto metatable = sol::table::create(state);

		table["getentity"] = [parent_id]()
		{
			return entity(parent_id);
		};

		metatable[sol::meta_function::new_index] = [parent_id](const sol::table t, const sol::this_state s,
			const sol::lua_value& field, const sol::lua_value& value)
		{
			const auto new_variable = convert({s, value});
			if (field.is<unsigned int>())
			{
				scripting::set_object_variable(parent_id, field.as<unsigned int>(), new_variable);
			}
			else if (field.is<std::string>())
			{
				scripting::set_object_variable(parent_id, field.as<std::string>(), new_variable);
			}
		};

		metatable[sol::meta_function::index] = [parent_id](const sol::table t, const sol::this_state s,
			const sol::lua_value& field)
		{
			if (field.is<unsigned int>())
			{
				return convert(s, scripting::get_object_variable(parent_id, field.as<unsigned int>()));
			}
			else if (field.is<std::string>())
			{
				return convert(s, scripting::get_object_variable(parent_id, field.as<std::string>()));
			}

			return sol::lua_value{s, sol::lua_nil};
		};

		table[sol::metatable_key] = metatable;

		return {state, table};
	}

	script_value convert(const sol::lua_value& value)
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

		if (is_istring(value))
		{
			return string_to_istring(value);
		}

		if (value.is<std::string>())
		{
			return {value.as<std::string>()};
		}

		if (value.is<array>())
		{
			return {value.as<array>()};
		}

		if (value.is<entity>())
		{
			return {value.as<entity>()};
		}

		if (value.is<vector>())
		{
			return {value.as<vector>()};
		}

		if (value.is<animation>())
		{
			return {value.as<animation>()};
		}

		if (value.is<function_ptr>())
		{
			return {value.as<function_ptr>()};
		}

		if (value.is<sol::protected_function>())
		{
			return convert_function(value);
		}

		return {};
	}

	sol::lua_value convert(lua_State* state, const script_value& value)
	{
		if (value.is<int>())
		{
			return {state, value.as<int>()};
		}

		if (value.is<float>())
		{
			return {state, value.as<float>()};
		}

		if (value.is<std::string>())
		{
			return {state, value.as<std::string>()};
		}
		
		if (value.is<array>())
		{
			return {state, value.as<array>()};
		}

		if (value.is<std::function<void()>>())
		{
			return convert_function(state, value.get_raw().u.codePosValue);
		}

		if (value.is<entity>())
		{
			return {state, value.as<entity>()};
		}

		if (value.is<vector>())
		{
			return {state, value.as<vector>()};
		}

		if (value.is<animation>())
		{
			return {state, value.as<animation>()};
		}

		return {state, sol::lua_nil};
	}
}
