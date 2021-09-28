#include <std_include.hpp>
#include "value.hpp"
#include "execution.hpp"

#include "component/ui_scripting.hpp"

#include <utils/string.hpp>

namespace ui_scripting
{
	namespace
	{
		bool valid_state()
		{
			return *game::hks::lua_state != 0;
		}

		void push_value(const value& value_)
		{
			if (!valid_state())
			{
				throw std::runtime_error("Invalid lua state");
			}

			const auto state = *game::hks::lua_state;

			switch (value_.index())
			{
			case 1:
			{
				const auto value = std::get<bool>(value_);
				state->top->t = game::hks::HksObjectType::TBOOLEAN;
				state->top->v.boolean = value;
				state->top++;
				break;
			}
			case 2:
			{
				const auto value = std::get<int>(value_);
				state->top->t = game::hks::HksObjectType::TNUMBER;
				state->top->v.number = static_cast<float>(value);
				state->top++;
				break;
			}
			case 3:
			{
				const auto value = std::get<float>(value_);
				state->top->t = game::hks::HksObjectType::TNUMBER;
				state->top->v.number = value;
				state->top++;
				break;
			}
			case 4:
			{
				const auto str = std::get<std::string>(value_);
				game::hks::hksi_lua_pushlstring(state, str.data(), (unsigned int)str.size());
				break;
			}
			case 5:
			{
				const auto data = std::get<lightuserdata>(value_);

				state->top->t = game::hks::HksObjectType::TLIGHTUSERDATA;
				state->top->v.ptr = data.ptr;
				state->top++;
				break;
			}
			}
		}
	}

	bool is_integer(float number)
	{
		return static_cast<int>(number) == number;
	}

	value get_return_value(int offset)
	{
		if (!valid_state())
		{
			throw std::runtime_error("Invalid lua state");
		}

		const auto state = *game::hks::lua_state;
		const auto top = &state->top[-1 - offset];

		switch (top->t)
		{
		case game::hks::HksObjectType::TBOOLEAN:
		{
			return {top->v.boolean};
		}
		case game::hks::HksObjectType::TNUMBER:
		{
			const auto number = top->v.number;
			if (is_integer(number))
			{
				return {static_cast<int>(top->v.number)};
			}
			else
			{
				return {top->v.number};
			}
		}
		case game::hks::HksObjectType::TSTRING:
		{
			const auto value = top->v.str->m_data;
			return {std::string(value)};
		}
		case game::hks::HksObjectType::TLIGHTUSERDATA:
		{
			return lightuserdata{top->v.ptr};
		}
		default:
		{
			return {};
		}
		}
	}

	std::vector<value> call(const std::string& name, const arguments& arguments)
	{
		if (!valid_state())
		{
			throw std::runtime_error("Invalid lua state");
		}

		const auto state = *game::hks::lua_state;
		state->top = state->base;

		const auto function = ui_scripting::find_function(name);
		if (!function)
		{
			throw std::runtime_error("Function " + name + " not found");
		}

		const auto _ = gsl::finally([]()
		{
			disable_error_hook();
		});

		enable_error_hook();

		for (const auto& value_ : arguments)
		{
			push_value(value_);
		}

		try
		{
			const auto count = function(*game::hks::lua_state);
			std::vector<value> values;

			for (auto i = 0; i < count; i++)
			{
				values.push_back(get_return_value(i));
			}

			return values;
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("Error executing function " + name + ": " + e.what());
		}
	}
}
