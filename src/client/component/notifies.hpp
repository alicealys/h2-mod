#pragma once

namespace notifies
{
	extern bool hook_enabled;

	void set_lua_hook(const char* pos, const sol::protected_function&);
	void set_gsc_hook(const char* source, const char* target);
	void clear_hook(const char* pos);
	size_t get_hook_count();

	void add_entity_damage_callback(const sol::protected_function&);
	void clear_callbacks();
}