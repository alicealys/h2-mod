function createdivider(menu, text)
	local element = LUI.UIElement.new({
		leftAnchor = true,
		rightAnchor = true,
		left = 0,
		right = 0,
		topAnchor = true,
		bottomAnchor = false,
		top = 0,
		bottom = 33.33
	})

	element.scrollingToNext = true
	element:addElement(LUI.MenuBuilder.BuildRegisteredType("h1_option_menu_titlebar", {
		title_bar_text = Engine.ToUpperCase(Engine.Localize(text))
	}))

	menu.list:addElement(element)
end

LUI.addmenubutton("pc_controls", {
    index = 4,
    text = "$_GENERAL",
    description = "Set the client's settings.",
    callback = function()
        LUI.FlowManager.RequestAddMenu(nil, "settings_menu")
    end
})

LUI.MenuBuilder.m_types_build["settings_menu"] = function(a1)
    local menu = LUI.MenuTemplate.new(a1, {
		menu_title = "$_GENERAL",
		menu_list_divider_top_offset = -(LUI.H1MenuTab.tabChangeHoldingElementHeight + luiglobals.H1MenuDims.spacing),
		menu_width = luiglobals.GenericMenuDims.OptionMenuWidth
	})

    Engine.SetDvarFromString("ui_cg_autoUpdate", Engine.GetDvarBool("cg_autoUpdate") and "1" or "0")
    Engine.SetDvarFromString("ui_cg_drawFps", Engine.GetDvarInt("cg_drawFps") .. "")
    Engine.SetDvarFromString("ui_cg_speedGraph", Engine.GetDvarBool("cg_speedGraph") and "1" or "0")
    Engine.SetDvarFromString("ui_cg_drawSpeed", Engine.GetDvarBool("cg_drawSpeed") and "1" or "0")
    Engine.SetDvarFromString("ui_r_fullbright", Engine.GetDvarInt("r_fullbright") .. "")

    createdivider(menu, "$_UPDATES")

    LUI.Options.CreateOptionButton(
        menu, 
        "ui_cg_autoUpdate", 
        "$_AUTOMATIC UPDATES", 
        "Enable or disable automatic updates on startup.", 
        {
            {
                text = "$_ENABLED",
                value = "1"
            },
            {
                text = "$_DISABLED",
                value = "0"
            }
        }, nil, nil, function(value)
            Engine.SetDvarBool("cg_autoUpdate", Engine.GetDvarString("ui_cg_autoUpdate") == "1")
        end
    )

    menu:AddButton("$_CHECK FOR UPDATES", function()
        LUI.tryupdating(false)
    end, nil, true, nil, {
        desc_text = "Check for updates."
    })

    createdivider(menu, "$_DRAWING")

    LUI.Options.CreateOptionButton(
        menu, 
        "ui_cg_drawFps", 
        "$_DRAW FPS", 
        "Enable or disable drawing fps or viewpos on screen.", 
        {
            {
                text = "$_DISABLED",
                value = "0"
            },
            {
                text = "$_FPS ONLY",
                value = "1"
            },
            {
                text = "$_FPS AND VIEWPOS",
                value = "2"
            }
        }, nil, nil, function(value)
            Engine.SetDvarInt("cg_drawFps", tonumber(Engine.GetDvarString("ui_cg_drawFps")))
        end
    )

    LUI.Options.CreateOptionButton(
        menu, 
        "ui_cg_drawSpeed", 
        "$_DRAW SPEED", 
        "Enable or disable drawing the player speed on screen.", 
        {
            {
                text = "$_DISABLED",
                value = "0"
            },
            {
                text = "$_ENABLED",
                value = "1"
            }
        }, nil, nil, function(value)
            Engine.SetDvarBool("cg_drawSpeed", Engine.GetDvarString("ui_cg_drawSpeed") == "1")
        end
    )

    LUI.Options.CreateOptionButton(
        menu, 
        "ui_cg_speedGraph", 
        "$_DRAW SPEED GRAPH", 
        "Enable or disable the speed graph.",
        {
            {
                text = "$_DISABLED",
                value = "0"
            },
            {
                text = "$_ENABLED",
                value = "1"
            }
        }, nil, nil, function(value)
            Engine.SetDvarBool("cg_speedGraph", Engine.GetDvarString("ui_cg_speedGraph") == "1")
        end
    )

    createdivider(menu, "$_RENDERING")

    LUI.Options.CreateOptionButton(
        menu, 
        "ui_r_fullbright", 
        "$_FULLBRIGHT", 
        "Change the fullbright mode.", 
        {
            {
                text = "$_DISABLED",
                value = "0"
            },
            {
                text = "$_ENABLED",
                value = "1"
            },
            {
                text = "$_MODE 2",
                value = "2"
            },
            {
                text = "$_MODE 3",
                value = "3"
            }
        }, nil, nil, function(value)
            Engine.SetDvarInt("r_fullbright", tonumber(Engine.GetDvarString("ui_r_fullbright")))
        end
    )

	LUI.Options.InitScrollingList(menu.list, nil)
	LUI.Options.AddOptionTextInfo(menu)

    menu:AddBackButton()

    return menu
end