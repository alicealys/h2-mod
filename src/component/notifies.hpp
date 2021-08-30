#pragma once

namespace notifies
{
	extern std::unordered_map<const char*, sol::protected_function> vm_execute_hooks;
	extern bool hook_enabled;

	void clear_callbacks();
}