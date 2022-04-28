#include <std_include.hpp>
#include "execution.hpp"
#include "component/ui_scripting.hpp"

#include <utils/string.hpp>

namespace ui_scripting
{
	void push_value(const script_value& value)
	{
		const auto state = *game::hks::lua_state;
		const auto value_ = value.get_raw();
		*state->m_apistack.top = value_;
		state->m_apistack.top++;
	}

	void push_value(const game::hks::HksObject& value)
	{
		const auto state = *game::hks::lua_state;
		*state->m_apistack.top = value;
		state->m_apistack.top++;
	}

	script_value get_return_value(int offset)
	{
		const auto state = *game::hks::lua_state;
		return state->m_apistack.top[-1 - offset];
	}

	arguments get_return_values()
	{
		const auto state = *game::hks::lua_state;
		const auto count = static_cast<int>(state->m_apistack.top - state->m_apistack.base);
		arguments values;

		for (auto i = count - 1; i >= 0; i--)
		{
			const auto v = get_return_value(i);
			values.push_back(v);
		}

		if (values.size() == 0)
		{
			values.push_back({});
		}

		return values;
	}

	bool notify(const std::string& name, const event_arguments& arguments)
	{
		const auto state = *game::hks::lua_state;
		if (state == nullptr)
		{
			return false;
		}

		const auto _1 = gsl::finally(game::LUI_LeaveCriticalSection);
		game::LUI_EnterCriticalSection();

		const auto globals = table((*::game::hks::lua_state)->globals.v.table);
		const auto engine = globals.get("Engine").as<table>();
		const auto root = engine.get("GetLuiRoot").as<function>().call({})[0].as<userdata>();
		const auto process_event = root.get("processEvent").as<function>();

		table event{};
		event.set("name", name);
		event.set("dispatchChildren", true);

		for (const auto& arg : arguments)
		{
			event.set(arg.first, arg.second);
		}

		process_event.call({root, event});
		return true;
	}

	arguments call_script_function(const function& function, const arguments& arguments)
	{
		const auto state = *game::hks::lua_state;
		state->m_apistack.top = state->m_apistack.base;
		const auto top = state->m_apistack.top;

		push_value(function);
		for (auto i = arguments.begin(); i != arguments.end(); ++i)
		{
			push_value(*i);
		}

		const auto num_args = static_cast<int>(arguments.size());

		game::hks::vm_call_internal(state, num_args, -1, 0);
		const auto args = get_return_values();
		state->m_apistack.top = top;
		return args;
	}

	script_value get_field(const userdata& self, const script_value& key)
	{
		const auto state = *game::hks::lua_state;
		const auto top = state->m_apistack.top;

		push_value(key);

		game::hks::HksObject value{};
		game::hks::HksObject userdata{};
		userdata.t = game::hks::TUSERDATA;
		userdata.v.ptr = self.ptr;

		game::hks::hks_obj_gettable(&value, state, &userdata, &state->m_apistack.top[-1]);
		state->m_apistack.top = top;
		return value;
	}

	script_value get_field(const table& self, const script_value& key)
	{
		const auto state = *game::hks::lua_state;
		const auto top = state->m_apistack.top;

		push_value(key);

		game::hks::HksObject value{};
		game::hks::HksObject userdata{};
		userdata.t = game::hks::TTABLE;
		userdata.v.ptr = self.ptr;

		game::hks::hks_obj_gettable(&value, state, &userdata, &state->m_apistack.top[-1]);
		state->m_apistack.top = top;
		return value;
	}

	void set_field(const userdata& self, const script_value& key, const script_value& value)
	{
		const auto state = *game::hks::lua_state;

		game::hks::HksObject userdata{};
		userdata.t = game::hks::TUSERDATA;
		userdata.v.ptr = self.ptr;

		game::hks::hks_obj_settable(state, &userdata, &key.get_raw(), &value.get_raw());
	}

	void set_field(const table& self, const script_value& key, const script_value& value)
	{
		const auto state = *game::hks::lua_state;

		game::hks::HksObject userdata{};
		userdata.t = game::hks::TTABLE;
		userdata.v.ptr = self.ptr;

		game::hks::hks_obj_settable(state, &userdata, &key.get_raw(), &value.get_raw());
	}
}
