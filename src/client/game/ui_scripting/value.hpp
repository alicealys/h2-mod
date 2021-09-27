#pragma once

namespace ui_scripting
{
	using value = std::variant<std::monostate, bool, int, float, std::string>;
	using arguments = std::vector<value>;
}
