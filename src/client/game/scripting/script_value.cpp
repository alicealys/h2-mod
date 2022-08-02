#include <std_include.hpp>
#include "script_value.hpp"
#include "entity.hpp"
#include "array.hpp"

#include <utils/string.hpp>

namespace scripting
{
	/***************************************************************
	 * Constructors
	 **************************************************************/

	script_value::script_value(const game::VariableValue& value)
		: value_(value)
	{
	}

	script_value::script_value(const int value)
	{
		game::VariableValue variable{};
		variable.type = game::SCRIPT_INTEGER;
		variable.u.intValue = value;

		this->value_ = variable;
	}

	script_value::script_value(const unsigned int value)
	{
		game::VariableValue variable{};
		variable.type = game::SCRIPT_INTEGER;
		variable.u.uintValue = value;

		this->value_ = variable;
	}

	script_value::script_value(const bool value)
		: script_value(static_cast<unsigned>(value))
	{
	}

	script_value::script_value(const float value)
	{
		game::VariableValue variable{};
		variable.type = game::SCRIPT_FLOAT;
		variable.u.floatValue = value;

		this->value_ = variable;
	}

	script_value::script_value(const double value)
		: script_value(static_cast<float>(value))
	{
	}

	script_value::script_value(const char* value)
	{
		game::VariableValue variable{};
		variable.type = game::SCRIPT_STRING;
		variable.u.stringValue = game::SL_GetString(value, 0);

		const auto _ = gsl::finally([&variable]()
		{
			game::RemoveRefToValue(variable.type, variable.u);
		});

		this->value_ = variable;
	}

	script_value::script_value(const std::string& value)
		: script_value(value.data())
	{
	}

	script_value::script_value(const entity& value)
	{
		game::VariableValue variable{};
		variable.type = game::SCRIPT_OBJECT;
		variable.u.pointerValue = value.get_entity_id();

		this->value_ = variable;
	}

	script_value::script_value(const array& value)
	{
		game::VariableValue variable{};
		variable.type = game::SCRIPT_OBJECT;
		variable.u.pointerValue = value.get_entity_id();

		this->value_ = variable;
	}

	script_value::script_value(const vector& value)
	{
		game::VariableValue variable{};
		variable.type = game::SCRIPT_VECTOR;
		variable.u.vectorValue = game::Scr_AllocVector(value);

		const auto _ = gsl::finally([&variable]()
		{
			game::RemoveRefToValue(variable.type, variable.u);
		});

		this->value_ = variable;
	}

	script_value::script_value(const animation& value)
	{
		game::VariableValue variable{};
		variable.type = 13;
		variable.u.value = value.get_value();

		this->value_ = variable;
	}

	script_value::script_value(const function_ptr& value)
	{
		game::VariableValue variable{};
		variable.type = game::SCRIPT_FUNCTION;
		variable.u.codePosValue = value.get_pos();

		this->value_ = variable;
	}

	/***************************************************************
	 * Integer
	 **************************************************************/

	template <>
	bool script_value::is<int>() const
	{
		return this->get_raw().type == game::SCRIPT_INTEGER;
	}

	template <>
	bool script_value::is<unsigned int>() const
	{
		return this->is<int>();
	}

	template <>
	bool script_value::is<bool>() const
	{
		return this->is<int>();
	}

	template <>
	int script_value::get() const
	{
		return this->get_raw().u.intValue;
	}

	template <>
	unsigned int script_value::get() const
	{
		return this->get_raw().u.uintValue;
	}

	template <>
	bool script_value::get() const
	{
		return this->get_raw().u.uintValue != 0;
	}

	/***************************************************************
	 * Float
	 **************************************************************/

	template <>
	bool script_value::is<float>() const
	{
		return this->get_raw().type == game::SCRIPT_FLOAT;
	}

	template <>
	bool script_value::is<double>() const
	{
		return this->is<float>();
	}

	template <>
	float script_value::get() const
	{
		return this->get_raw().u.floatValue;
	}

	template <>
	double script_value::get() const
	{
		return static_cast<double>(this->get_raw().u.floatValue);
	}

	/***************************************************************
	 * String
	 **************************************************************/

	template <>
	bool script_value::is<const char*>() const
	{
		return this->get_raw().type == game::SCRIPT_STRING;
	}

	template <>
	bool script_value::is<std::string>() const
	{
		return this->is<const char*>();
	}

	template <>
	const char* script_value::get() const
	{
		return game::SL_ConvertToString(static_cast<game::scr_string_t>(this->get_raw().u.stringValue));
	}

	template <>
	std::string script_value::get() const
	{
		return this->get<const char*>();
	}

	/***************************************************************
	 * Array
	 **************************************************************/

	template <>
	bool script_value::is<std::vector<script_value>>() const
	{
		if (this->get_raw().type != game::SCRIPT_OBJECT)
		{
			return false;
		}

		const auto id = this->get_raw().u.uintValue;
		const auto type = game::scr_VarGlob->objectVariableValue[id].w.type;

		return type == game::SCRIPT_ARRAY;
	}

	/***************************************************************
	 * Struct
	 **************************************************************/

	template <>
	bool script_value::is<std::map<std::string, script_value>>() const
	{
		if (this->get_raw().type != game::SCRIPT_OBJECT)
		{
			return false;
		}

		const auto id = this->get_raw().u.uintValue;
		const auto type = game::scr_VarGlob->objectVariableValue[id].w.type;

		return type == game::SCRIPT_STRUCT;
	}

	/***************************************************************
	 * Function
	 **************************************************************/

	template <>
	bool script_value::is<std::function<void()>>() const
	{
		return this->get_raw().type == game::SCRIPT_FUNCTION;
	}

	/***************************************************************
	 * Entity
	 **************************************************************/

	template <>
	bool script_value::is<entity>() const
	{
		return this->get_raw().type == game::SCRIPT_OBJECT;
	}

	template <>
	entity script_value::get() const
	{
		return entity(this->get_raw().u.pointerValue);
	}

	/***************************************************************
	 * Array
	 **************************************************************/

	template <>
	bool script_value::is<array>() const
	{
		if (this->get_raw().type != game::SCRIPT_OBJECT)
		{
			return false;
		}

		const auto id = this->get_raw().u.uintValue;
		const auto type = game::scr_VarGlob->objectVariableValue[id].w.type;

		return type == game::SCRIPT_ARRAY;
	}

	template <>
	array script_value::get() const
	{
		return array(this->get_raw().u.uintValue);
	}

	/***************************************************************
	 * Vector
	 **************************************************************/

	template <>
	bool script_value::is<vector>() const
	{
		return this->get_raw().type == game::SCRIPT_VECTOR;
	}

	template <>
	vector script_value::get() const
	{
		return this->get_raw().u.vectorValue;
	}

	/***************************************************************
	 * Animation
	 **************************************************************/

	template <>
	bool script_value::is<animation>() const
	{
		return this->get_raw().type == 13;
	}

	template <>
	animation script_value::get() const
	{
		return this->get_raw().u.uintValue;
	}

	/***************************************************************
	 *
	 **************************************************************/

	const game::VariableValue& script_value::get_raw() const
	{
		return this->value_.get();
	}

	std::string script_value::to_string() const
	{
		if (this->is<int>())
		{
			return utils::string::va("%i", this->as<int>());
		}

		if (this->is<float>())
		{
			return utils::string::va("%f", this->as<float>());
		}

		if (this->is<std::string>())
		{
			return this->as<std::string>();
		}

		if (this->is<vector>())
		{
			const auto vec = this->as<vector>();
			return utils::string::va("(%g, %g, %g)",
				vec.get_x(),
				vec.get_y(),
				vec.get_z()
			);
		}

		return {};
	}
}
