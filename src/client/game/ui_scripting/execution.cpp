#include <std_include.hpp>
#include "execution.hpp"
#include "stack_isolation.hpp"
#include "component/ui_scripting.hpp"

#include <utils/string.hpp>

namespace ui_scripting
{
	void push_value(const value& value)
	{
		const auto state = *game::hks::lua_state;
		const auto value_ = value.get_raw();
		*state->m_apistack.top = value_;
		state->m_apistack.top++;
	}

	value get_return_value(int offset)
	{
		const auto state = *game::hks::lua_state;
		const auto top = &state->m_apistack.top[-1 - offset];
		return *top;
	}

	void call_script_function(const function& function, const arguments& arguments)
	{
		const auto state = *game::hks::lua_state;

		stack_isolation _;
		for (auto i = arguments.rbegin(); i != arguments.rend(); ++i)
		{
			push_value(*i);
		}
		push_value(function);

		enable_error_hook();
		const auto __ = gsl::finally([]()
		{
			disable_error_hook();
		});

		// Not sure about this

		try
		{
			game::hks::vm_call_internal(state, static_cast<int>(arguments.size()), 0, 0);
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(std::string("Error executing script function: ") + e.what());
		}
	}

	value get_field(const userdata& self, const value& key)
	{
		const auto state = *game::hks::lua_state;

		stack_isolation _;
		push_value(key);

		enable_error_hook();
		const auto __ = gsl::finally([]()
		{
			disable_error_hook();
		});

		game::hks::HksObject value{};
		game::hks::HksObject userdata{};
		userdata.t = game::hks::TUSERDATA;
		userdata.v.ptr = self.ptr;

		try
		{
			game::hks::hks_obj_getfield(&value, state, &userdata, &state->m_apistack.top[-1]);
			return value;
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(std::string("Error getting userdata field: ") + e.what());
		}
	}

	value get_field(const table& self, const value& key)
	{
		const auto state = *game::hks::lua_state;

		stack_isolation _;
		push_value(key);

		enable_error_hook();
		const auto __ = gsl::finally([]()
		{
			disable_error_hook();
		});

		game::hks::HksObject value{};
		game::hks::HksObject userdata{};
		userdata.t = game::hks::TTABLE;
		userdata.v.ptr = self.ptr;

		try
		{
			game::hks::hks_obj_getfield(&value, state, &userdata, &state->m_apistack.top[-1]);
			return value;
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(std::string("Error getting table field: ") + e.what());
		}
	}

	void set_field(const userdata& self, const value& key, const value& value)
	{
		const auto state = *game::hks::lua_state;

		stack_isolation _;

		enable_error_hook();
		const auto __ = gsl::finally([]()
		{
			disable_error_hook();
		});

		game::hks::HksObject userdata{};
		userdata.t = game::hks::TUSERDATA;
		userdata.v.ptr = self.ptr;

		try
		{
			game::hks::hks_obj_settable(state, &userdata, &key.get_raw(), &value.get_raw());
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(std::string("Error setting userdata field: ") + e.what());
		}
	}

	void set_field(const table& self, const value& key, const value& value)
	{
		const auto state = *game::hks::lua_state;

		stack_isolation _;

		enable_error_hook();
		const auto __ = gsl::finally([]()
		{
			disable_error_hook();
		});

		game::hks::HksObject userdata{};
		userdata.t = game::hks::TTABLE;
		userdata.v.ptr = self.ptr;

		try
		{
			game::hks::hks_obj_settable(state, &userdata, &key.get_raw(), &value.get_raw());
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(std::string("Error setting table field: ") + e.what());
		}
	}

	arguments call_method(const userdata& self, const std::string& name, const arguments& arguments)
	{
		const auto function = ui_scripting::find_method(name);
		if (!function)
		{
			throw std::runtime_error("Function " + name + " not found");
		}

		stack_isolation _;
		push_value(self);
		for (auto i = arguments.rbegin(); i != arguments.rend(); ++i)
		{
			push_value(*i);
		}

		enable_error_hook();
		const auto __ = gsl::finally([]()
		{
			disable_error_hook();
		});

		try
		{
			const auto count = function(*game::hks::lua_state);
			std::vector<value> values;

			for (auto i = 0; i < count; i++)
			{
				values.push_back(get_return_value(i));
			}

			if (values.size() == 0)
			{
				values.push_back({});
			}

			return values;
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("Error executing method " + name + ": " + e.what());
		}
	}

	arguments call(const std::string& name, const arguments& arguments)
	{
		const auto function = ui_scripting::find_function(name);
		if (!function)
		{
			throw std::runtime_error("Function " + name + " not found");
		}

		stack_isolation _;
		for (auto i = arguments.rbegin(); i != arguments.rend(); ++i)
		{
			push_value(*i);
		}

		enable_error_hook();
		const auto __ = gsl::finally([]()
		{
			disable_error_hook();
		});

		try
		{
			const auto count = function(*game::hks::lua_state);
			std::vector<value> values;

			for (auto i = 0; i < count; i++)
			{
				values.push_back(get_return_value(i));
			}

			if (values.size() == 0)
			{
				values.push_back({});
			}

			return values;
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("Error executing function " + name + ": " + e.what());
		}
	}
}
