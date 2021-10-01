#pragma once
#include "game/game.hpp"
#include "types.hpp"
#include "value.hpp"

namespace ui_scripting
{
	void push_value(const value& value);
	value get_return_value(int offset);

	arguments call_script_function(const function& function, const arguments& arguments);

	value get_field(const userdata& self, const value& key);
	value get_field(const table& self, const value& key);
	void set_field(const userdata& self, const value& key, const value& value);
	void set_field(const table& self, const value& key, const value& value);

	arguments call_method(const userdata& self, const std::string& name, const arguments& arguments);
	arguments call(const std::string& name, const arguments& arguments);
}
