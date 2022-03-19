local spacing = 10
local topoffset = 15

local extrawidth = 50
local extraheight = 40

LUI.MenuBuilder.m_types_build["SystemInfo"] = function (f6_arg0, f6_arg1)
	local f6_local0 = LUI.MenuTemplate.spMenuOffset
	local title = "LUA_MENU_SYSTEM_INFO_CAPS"

	local f6_local2 = false
	local f6_local3 = 0

	local menu = LUI.MenuTemplate.new(f6_arg0, {
		menu_title = title,
		menu_top_indent = f6_local0 + f6_local3,
		showSelectButton = false,
		skipAnim = f6_local2
	})

	menu:setClass(LUI.SystemInfo)
	menu:PopulateMissingProps(f6_arg1)
	menu:ValidateProps(f6_arg1)
	menu.id = "systemInfo_id"

	local f6_local5 = 300
	local f6_local6 = LUI.MenuTemplate.ListTop + f6_local0
	local f6_local7 = f6_arg1.menu_height
	if not f6_local7 then
		f6_local7 = f6_local5
	end

	f6_local7 = f6_local7 + f6_local6 - extraheight

	local f6_local9 = luiglobals.GenericMenuDims.OptionMenuWidth + 100
	local f6_local10 = menu.properties
	local topoffset2 = LUI.MenuTemplate.ListTop + LUI.MenuTemplate.spMenuOffset

	local decobox = LUI.MenuBuilder.BuildRegisteredType("h1_box_deco", {
		decoTopOffset = topoffset2 - topoffset + 15,
		decoBottomOffset = -f6_local7,
		decoRightOffset = -665 + extrawidth
	})

	menu:addElement(decobox)

	local decoleft = CoD.CreateState(0, 0.5, 8, 0.5, CoD.AnchorTypes.TopLeft)
	decoleft.color = luiglobals.Colors.h1.light_grey
	decobox:addElement(LUI.UILine.new(decoleft))

	local decoright = CoD.CreateState(0, 0.5, -8, 0.5, CoD.AnchorTypes.TopRight)
	decoright.color = luiglobals.Colors.h1.light_grey
	decobox:addElement(LUI.UILine.new(decoright))

	local element = LUI.UIVerticalList.new({
		leftAnchor = true,
		rightAnchor = true,
		topAnchor = true,
		bottomAnchor = true,
		left = spacing,
		right = 100,
		top = topoffset2 + 15,
		bottom = 0,
		spacing = spacing * 0.8
	})

	element.id = "systemInfoList_id"
	menu.vlist = element
	menu:addElement(element)

	local optionmenuwidth = luiglobals.GenericMenuDims.OptionMenuWidth
	luiglobals.GenericMenuDims.OptionMenuWidth = optionmenuwidth + extrawidth

	menu:AddInfo(Engine.Localize("MENU_SYSINFO_VERSION"), function()
		return Engine.GetBuildNumber()
	end)

	menu:AddInfo(Engine.Localize("MENU_SYSINFO_CUSTOMER_SUPPORT_LINK"), function()
		return Engine.Localize("MENU_SYSINFO_CUSTOMER_SUPPORT_URL")
	end)

	menu:AddInfo(Engine.Localize("MENU_SYSINFO_DONATION_LINK"), function()
		return Engine.Localize("MENU_SYSINFO_DONATION_URL")
	end)

	luiglobals.GenericMenuDims.OptionMenuWidth = optionmenuwidth

	menu:AddBackButton()
	menu:registerEventHandler("menu_close", LUI.SystemInfo.LeaveMenu)

	return menu
end