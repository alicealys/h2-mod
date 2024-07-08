local lookcontrols = require("LUI.LookControls")

lookcontrols.CreateOptions = function(menu)
	menu:AddButton(nil, nil, nil, nil, nil, {
		variant = GenericButtonSettings.Variants.Select,
		button_text = Engine.Localize("MENU_MOUSE_SENSITIVITY"),
		desc_text = Engine.Localize("MENU_MOUSE_SENSITIVITY_DESC"),
		H1OptionButton = true,
		button_display_func = lookcontrols.GetSensitivityText,
		button_left_func = function (f8_arg0, f8_arg1)
			return lookcontrols.MouseDvarChange(menu.exclusiveController, SliderBounds.MouseSensitivity.Min, SliderBounds.MouseSensitivity.Max, -SliderBounds.MouseSensitivity.Step, "sensitivity")
		end,
		button_right_func = function (f9_arg0, f9_arg1)
			return lookcontrols.MouseDvarChange(menu.exclusiveController, SliderBounds.MouseSensitivity.Min, SliderBounds.MouseSensitivity.Max, SliderBounds.MouseSensitivity.Step, "sensitivity")
		end,
		buttonActionIsRightAction = true
	})

	menu:AddButton(nil, nil, nil, nil, nil, {
		variant = GenericButtonSettings.Variants.Select,
		button_text = Engine.Localize("MENU_MOUSE_ACCEL"),
		desc_text = Engine.Localize("MENU_MOUSE_ACCEL_DESC"),
		H1OptionButton = true,
		button_display_func = lookcontrols.GetAccelerationText,
		button_left_func = function (f10_arg0, f10_arg1)
			return lookcontrols.MouseDvarChange(menu.exclusiveController, SliderBounds.MouseAcceleration.Min, SliderBounds.MouseAcceleration.Max, -SliderBounds.MouseAcceleration.Step, "cl_mouseAccel")
		end,
		button_right_func = function (f11_arg0, f11_arg1)
			return lookcontrols.MouseDvarChange(menu.exclusiveController, SliderBounds.MouseAcceleration.Min, SliderBounds.MouseAcceleration.Max, SliderBounds.MouseAcceleration.Step, "cl_mouseAccel")
		end,
		buttonActionIsRightAction = true
	})

	LUI.Options.CreateOptionButton(menu, "ui_mousePitch", "@MENU_INVERT_MOUSE", "@MENU_INVERT_MOUSE_DESC", lookcontrols.GetPitchValues(), lookcontrols.UpdateMousePitch)

	LUI.Options.CreateOptionButton(menu, "m_filter", "@MENU_SMOOTH_MOUSE", "@MENU_SMOOTH_MOUSE_DESC", {
		{
			text = "@LUA_MENU_YES",
			value = true
		},
		{
			text = "@LUA_MENU_NO",
			value = false
		}
	})

	LUI.Options.CreateOptionButton(menu, "win_useWmInput", "@MENU_RAW_MOUSE", "@MENU_RAW_MOUSE_DESC", {
		{
			text = "@LUA_MENU_YES",
			value = true
		},
		{
			text = "@LUA_MENU_NO",
			value = false
		}
	})

	LUI.Options.CreateOptionButton(menu, "cl_freelook", "@MENU_FREE_LOOK", "@MENU_FREE_LOOK_DESC", {
		{
			text = "@LUA_MENU_YES",
			value = true
		},
		{
			text = "@LUA_MENU_NO",
			value = false
		}
	})

	LUI.Options.CreateControlBindButton(menu, "@MENU_LEAN_LEFT", "@MENU_LEAN_LEFT_DESC", "+leanleft")
	LUI.Options.CreateControlBindButton(menu, "@MENU_LEAN_RIGHT", "@MENU_LEAN_RIGHT_DESC", "+leanright")

	LUI.Options.CreateControlBindButton(menu, "@MENU_LOOK_UP", "@MENU_LOOK_UP_DESC", "+lookup")
	LUI.Options.CreateControlBindButton(menu, "@MENU_LOOK_DOWN", "@MENU_LOOK_DOWN_DESC", "+lookdown")
	LUI.Options.CreateControlBindButton(menu, "@MENU_HOLD_MOUSE_LOOK", "@MENU_HOLD_MOUSE_LOOK_DESC", "+mlook")
	LUI.Options.CreateControlBindButton(menu, "@MENU_CENTER_VIEW", "@MENU_CENTER_VIEW_DESC", "centerview")

	LUI.Options.InitScrollingList(menu.list, nil)
end
