local spacing = 10
local topoffset = 15

local extrawidth = 50
local extraheight1 = 0

local function makelink(element, link)
	element:setHandleMouseMove(true)
	element:setHandleMouseButton(true)
	element:registerAnimationState("focused", {
		color = {
			r = 1,
			g = 1,
			b = 1
		}
	})

	local entered = false
	element:registerEventHandler("mouseenter", function()
		if (not entered) then
			Engine.PlaySound(CoD.SFX.MouseOver)
			entered = true
		end

		element:animateToState("focused")
	end)

	element:registerEventHandler("mouseleave", function()
		entered = false
		element:animateToState("default")
	end)

	element:registerEventHandler("leftmousedown", function()
		Engine.PlaySound(CoD.SFX.MouseClick)
		game:openlink(link)
	end)
end

local addlinkinfo = function(menu, key, value, link)
	local text = Engine.Localize(value)
	local element = menu:AddInfo(Engine.Localize(key), function()
		return text
	end)

	local linkelement = element:getFirstChild():getNextSibling()
	makelink(linkelement, link)
end

local function getnumcredits()
	local csv = "credits.csv"
	local rowcount = Engine.TableGetRowCount(csv)
	return rowcount
end

local extraheight2 = getnumcredits() * 25 / 2

local function addcredits(menu)
	local cols = {
		name = 0,
		link = 1
	}

	local csv = "credits.csv"
	local rowcount = Engine.TableGetRowCount(csv)

	local num = 0
	local creditselement = LUI.UIElement.new({
		topAnchor = true,
		leftAnchor = true,
		left = 500,
		width = 30,
		top = 255
	})

	menu:addElement(creditselement)

	menu:AddInfo(Engine.Localize("MENU_CREDITS"), function()
		return ""
	end)

	local num = 0
	local addcredit = function(name, link)
		local row = num % 4
		local col = num >= 4 and 0 or 1
		local text = Engine.Localize(name)

		local element = LUI.UIText.new({
			rightAnchor = true,
			topAnchor = true,
			right = -10 + (col * -100),
			top = 25 * row,
			width = 90,
			height = LUI.MenuGenericButtons.ButtonLabelFont.Height * 0.8,
			font = LUI.MenuGenericButtons.ButtonLabelFont.Font,
			color = Colors.h1.light_grey,
			alignment = Engine.IsRightToLeftLanguage() and LUI.Alignment.None or LUI.Alignment.Right
		})

		element.id = "credits_" .. num
		element:setText(text)
		creditselement:addElement(element)
		makelink(element, link)

		num = num + 1
	end

	for i = 0, rowcount - 1 do
        local name = Engine.TableLookupByRow(csv, i, cols.name)
        local link = Engine.TableLookupByRow(csv, i, cols.link)
		addcredit(name, link)
	end
end

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

	f6_local7 = f6_local7 + f6_local6 - extraheight1

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

	addlinkinfo(menu, "MENU_SYSINFO_CUSTOMER_SUPPORT_LINK", "MENU_SYSINFO_CUSTOMER_SUPPORT_URL", "github")

	luiglobals.GenericMenuDims.OptionMenuWidth = optionmenuwidth

	menu:AddBackButton()
	menu:registerEventHandler("menu_close", LUI.SystemInfo.LeaveMenu)

	return menu
end

local startmap = Engine.StreamingInstallMap
Engine.StreamingInstallMap = function(map, args, ...)
	if (args == "set ui_play_credits 1; devmap ending;") then
		LUI.FlowManager.RequestAddMenu(nil, "credits_menu")
	else
		startmap(map, args, ...)
	end
end

local creditsmap = function()
	LUI.FlowManager.PushMenuStackToDVar()

	if LUI.LevelSelect.IsAllLevelCompleted() then
		Engine.SetDvarString("ui_char_museum_mode", "credits_2", true)
	else
		Engine.SetDvarString("ui_char_museum_mode", "credits_black", true)
	end

	startmap("ending", "set ui_play_credits 1; devmap ending;")
end

LUI.MenuBuilder.registerType("credits_menu", function(f6_arg0, f6_arg1)
	local f6_local0 = LUI.MenuTemplate.spMenuOffset
	local title = "MENU_SP_CREDITS_CAPS"

	local f6_local2 = false
	local f6_local3 = 0

	local menu = LUI.MenuTemplate.new(f6_arg0, {
		menu_title = title,
		menu_top_indent = f6_local0 + f6_local3,
		showSelectButton = false,
		skipAnim = f6_local2
	})

    PersistentBackground.ChangeBackground(nil, CoD.Background.CampaignRegular)

	menu:AddButton("MENU_SP_CREDITS_CAPS", creditsmap, nil, true, nil, {
		desc_text = Engine.Localize("@LUA_MENU_CREDITS_DESC")
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

	f6_local7 = f6_local7 + f6_local6 - extraheight2

	local f6_local9 = luiglobals.GenericMenuDims.OptionMenuWidth + 100
	local f6_local10 = menu.properties
	local topoffset2 = LUI.MenuTemplate.ListTop + LUI.MenuTemplate.spMenuOffset + 50

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

	addlinkinfo(menu, "MENU_SYSINFO_CUSTOMER_SUPPORT_LINK", "MENU_SYSINFO_CUSTOMER_SUPPORT_URL", "github")
	addlinkinfo(menu, "MENU_SYSINFO_DONATION_LINK", "MENU_SYSINFO_DONATION_URL", "donate")

	addcredits(menu)

	luiglobals.GenericMenuDims.OptionMenuWidth = optionmenuwidth

	menu:AddBackButton()
	menu:registerEventHandler("menu_close", LUI.SystemInfo.LeaveMenu)

	return menu
end)
