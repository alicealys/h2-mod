#include <std_include.hpp>
#include "execution.hpp"
#include "types.hpp"
#include "stack_isolation.hpp"
#include "value.hpp"

namespace ui_scripting
{
	/***************************************************************
	 * Constructors
	 **************************************************************/

	value::value(const game::hks::HksObject& value)
		: value_(value)
	{
	}

	value::value(const int value)
	{
		game::hks::HksObject obj{};
		obj.t = game::hks::TNUMBER;
		obj.v.number = static_cast<float>(value);

		this->value_ = obj;
	}

	value::value(const unsigned int value)
	{
		game::hks::HksObject obj{};
		obj.t = game::hks::TNUMBER;
		obj.v.number = static_cast<float>(value);

		this->value_ = obj;
	}

	value::value(const bool value)
		: value(static_cast<unsigned>(value))
	{
	}

	value::value(const float value)
	{
		game::hks::HksObject obj{};
		obj.t = game::hks::TNUMBER;
		obj.v.number = static_cast<float>(value);

		this->value_ = obj;
	}

	value::value(const double value)
		: value(static_cast<float>(value))
	{
	}

	value::value(const char* value)
	{
		game::hks::HksObject obj{};
		stack_isolation _;

		const auto state = *game::hks::lua_state;
		game::hks::hksi_lua_pushlstring(state, value, (unsigned int)strlen(value));
		obj = state->m_apistack.top[-1];

		this->value_ = obj;
	}

	value::value(const std::string& value)
		: value(value.data())
	{
	}

	value::value(const lightuserdata& value)
	{
		this->value_.t = game::hks::TLIGHTUSERDATA;
		this->value_.v.ptr = value.ptr;
	}

	value::value(const userdata& value)
	{
		this->value_.t = game::hks::TUSERDATA;
		this->value_.v.ptr = value.ptr;
	}

	value::value(const table& value)
	{
		this->value_.t = game::hks::TTABLE;
		this->value_.v.ptr = value.ptr;
	}

	value::value(const function& value)
	{
		this->value_.t = game::hks::TIFUNCTION;
		this->value_.v.ptr = value.ptr;
	}

	/***************************************************************
	 * Integer
	 **************************************************************/

	template <>
	bool value::is<int>() const
	{
		return this->get_raw().t == game::hks::TNUMBER;
	}

	template <>
	bool value::is<unsigned int>() const
	{
		return this->is<int>();
	}

	template <>
	bool value::is<bool>() const
	{
		return this->is<int>();
	}

	template <>
	int value::get() const
	{
		return static_cast<int>(this->get_raw().v.number);
	}

	template <>
	unsigned int value::get() const
	{
		return static_cast<unsigned int>(this->get_raw().v.number);
	}

	template <>
	bool value::get() const
	{
		return this->get_raw().v.native != 0;
	}

	/***************************************************************
	 * Float
	 **************************************************************/

	template <>
	bool value::is<float>() const
	{
		return this->get_raw().t == game::hks::TNUMBER;
	}

	template <>
	bool value::is<double>() const
	{
		return this->is<float>();
	}

	template <>
	float value::get() const
	{
		return this->get_raw().v.number;
	}

	template <>
	double value::get() const
	{
		return static_cast<double>(this->get_raw().v.number);
	}

	/***************************************************************
	 * String
	 **************************************************************/

	template <>
	bool value::is<const char*>() const
	{
		return this->get_raw().t == game::hks::TSTRING;
	}

	template <>
	bool value::is<std::string>() const
	{
		return this->is<const char*>();
	}

	template <>
	const char* value::get() const
	{
		return this->get_raw().v.str->m_data;
	}

	template <>
	std::string value::get() const
	{
		return this->get<const char*>();
	}

	bool value::operator==(const value& other)
	{
		if (this->get_raw().t != other.get_raw().t)
		{
			return false;
		}

		if (this->get_raw().t == game::hks::TSTRING)
		{
			return this->get<std::string>() == other.get<std::string>();
		}

		return this->get_raw().v.native == other.get_raw().v.native;
	}

	/***************************************************************
	 * Lightuserdata
	 **************************************************************/

	template <>
	bool value::is<lightuserdata>() const
	{
		return this->get_raw().t == game::hks::TLIGHTUSERDATA;
	}

	template <>
	lightuserdata value::get() const
	{
		return this->get_raw().v.ptr;
	}

	/***************************************************************
	 * Userdata
	 **************************************************************/

	template <>
	bool value::is<userdata>() const
	{
		return this->get_raw().t == game::hks::TUSERDATA;
	}

	template <>
	userdata value::get() const
	{
		return this->get_raw().v.ptr;
	}

	/***************************************************************
	 * Table
	 **************************************************************/

	template <>
	bool value::is<table>() const
	{
		return this->get_raw().t == game::hks::TTABLE;
	}

	template <>
	table value::get() const
	{
		return this->get_raw().v.table;
	}

	/***************************************************************
	 * Function
	 **************************************************************/

	template <>
	bool value::is<function>() const
	{
		return this->get_raw().t == game::hks::TIFUNCTION;
	}

	template <>
	function value::get() const
	{
		return this->get_raw().v.ptr;
	}

	/***************************************************************
	 *
	 **************************************************************/

	const game::hks::HksObject& value::get_raw() const
	{
		return this->value_;
	}
}
