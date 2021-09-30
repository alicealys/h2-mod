#pragma once
#include "game/game.hpp"

namespace ui_scripting
{
	class lightuserdata;
	class userdata;
	class table;
	class function;

	class value
	{
	public:
		value() = default;
		value(const game::hks::HksObject& value);

		value(int value);
		value(unsigned int value);
		value(bool value);

		value(float value);
		value(double value);

		value(const char* value);
		value(const std::string& value);

		value(const lightuserdata& value);
		value(const userdata& value);
		value(const table& value);
		value(const function& value);

		bool value::operator==(const value& other);

		template <typename T>
		bool is() const;

		template <typename T>
		T as() const
		{
			if (!this->is<T>())
			{
				throw std::runtime_error("Invalid type");
			}

			return get<T>();
		}

		const game::hks::HksObject& get_raw() const;

	private:
		template <typename T>
		T get() const;

		game::hks::HksObject value_{};
	};

	using arguments = std::vector<value>;
}
