#pragma once
#include "game/game.hpp"
#include "script_value.hpp"

namespace scripting
{
	class function_ptr final
	{
	public:
		function_ptr(const std::string& file, const std::string& name);

		const char* get_pos() const;
	private:
		const char* pos_;
	};
}
