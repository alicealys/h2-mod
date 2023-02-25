#include <std_include.hpp>
#include "value_conversion.hpp"
#include "../functions.hpp"
#include "../execution.hpp"
#include ".../../component/notifies.hpp"

namespace scripting::lua
{
	namespace
	{
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
			const auto index = reinterpret_cast<char*>(notifies::get_hook_count() + 1);
			notifies::set_lua_hook(index, function);

			game::VariableValue func{};
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
