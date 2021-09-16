#pragma once

namespace notifies
{
	extern std::unordered_map<const char*, sol::protected_function> vm_execute_hooks;
	extern bool hook_enabled;

	void add_entity_damage_callback(const sol::protected_function&);
	void clear_callbacks();
}