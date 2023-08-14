#pragma once

#include "game/ui_scripting/execution.hpp"

namespace camera
{
	void enable_free_move();
	void disable_free_move();
	bool is_free_move_enabled();

	void set_camera_position(const float x, const float y, const float z);
	void set_camera_angles(const float x, const float y, const float z);

	void set_using_origin_override(bool value);
	void set_using_angles_override(bool value);

	ui_scripting::table get_camera_position();

	void set_callback(const ui_scripting::script_value& callback);

	void clear_lua();
}
