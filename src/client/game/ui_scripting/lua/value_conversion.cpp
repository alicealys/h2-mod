#include <std_include.hpp>
#include "value_conversion.hpp"
#include "../execution.hpp"

namespace ui_scripting::lua
{
	namespace
	{
		table convert_table(const sol::table& t)
		{
			table res{};

			t.for_each([res](const sol::object& key, const sol::object& value)
			{
				res.set(convert(key), convert(value));
			});

			return res;
		}
	}

	value convert(const sol::lua_value& value)
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

		if (value.is<lightuserdata>())
		{
			return {value.as<lightuserdata>()};
		}

		if (value.is<userdata>())
		{
			return {value.as<userdata>()};
		}

		if (value.is<table>())
		{
			return {value.as<table>()};
		}

		if (value.is<function>())
		{
			return {value.as<function>()};
		}

		if (value.is<sol::table>())
		{
			return {convert_table(value.as<sol::table>())};
		}

		return {};
	}

	sol::lua_value convert(lua_State* state, const value& value)
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

		if (value.is<lightuserdata>())
		{
			return {state, value.as<lightuserdata>()};
		}

		if (value.is<userdata>())
		{
			return {state, value.as<userdata>()};
		}

		if (value.is<table>())
		{
			return {state, value.as<table>()};
		}

		if (value.is<function>())
		{
			return {state, value.as<function>()};
		}

		return {state, sol::lua_nil};
	}
}
