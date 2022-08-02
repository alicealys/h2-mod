#include <std_include.hpp>
#include "array.hpp"
#include "script_value.hpp"
#include "execution.hpp"

namespace scripting
{
	function_ptr::function_ptr(const std::string& file, const std::string& name)
	{
		this->pos_ = get_function_pos(file, name);
	}

	const char* function_ptr::get_pos() const
	{
		return this->pos_;
	}
}
