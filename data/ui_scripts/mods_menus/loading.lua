function createdivider(menu, text)
	local element = LUI.UIElement.new( {
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

function string:truncate(length)
    if (#self <= length) then
        return self
    end

    return self:sub(1, length - 3) .. "..."
end

local maincampaign = LUI.MenuBuilder.m_types_build["main_campaign"]
LUI.MenuBuilder.m_types_build["main_campaign"] = function(a1, a2)
    local menu = maincampaign(a1, a2)
    local buttonlist = menu:getChildById("main_campaign_list")
    
    local button = menu:AddButton("$_MODS", function()
        LUI.FlowManager.RequestAddMenu(nil, "mods_menu")
    end, nil, true, nil, {
        desc_text = "Open mods menu"
    })

    buttonlist:removeElement(button)
    buttonlist:insertElement(button, 6)
    button.id = "mods_menu-button"

    local hintbox = menu.optionTextInfo
    local firstbutton = buttonlist:getFirstChild()
    hintbox:dispatchEventToRoot({
        name = "set_button_info_text",
        text = firstbutton.properties.desc_text,
        immediate = true
    })

    menu:CreateBottomDivider()
    menu:AddBottomDividerToList(buttonlist:getLastChild())
    menu:removeElement(menu.optionTextInfo)

    LUI.Options.InitScrollingList(menu.list, nil)
    menu:CreateBottomDivider()
    menu.optionTextInfo = LUI.Options.AddOptionTextInfo(menu)

    return menu
end

LUI.MenuBuilder.m_types_build["mods_menu"] = function(a1)
    local menu = LUI.MenuTemplate.new(a1, {
        menu_title = "$_MODS",
        exclusiveController = 0,
        menu_width = 400,
        menu_top_indent = LUI.MenuTemplate.spMenuOffset,
        showTopRightSmallBar = true
    })

    menu:AddButton("$_OPEN STORE", function()
        LUI.FlowManager.RequestAddMenu(nil, "mod_store_menu")
    end, nil, true, nil, {
        desc_text = "Open the menu store"
    })

    local modfolder = game:getloadedmod()
    if (modfolder ~= "") then
        createdivider(menu, "$_Loaded mod: " .. modfolder:truncate(20))

        menu:AddButton("$_UNLOAD", function()
            game:executecommand("unloadmod")
        end, nil, true, nil, {
            desc_text = "Unload the currently loaded mod"
        })
    end

    createdivider(menu, "$_Available mods")

    if (io.directoryexists("mods")) then
        local mods = io.listfiles("mods/")
        for i = 1, #mods do
            local desc = "Load " .. mods[i]
            local name = mods[i]

            local infofile = mods[i] .. "/info.json"
    
            if (io.fileexists(infofile)) then
                pcall(function()
                    local data = json.decode(io.readfile(infofile))
                    desc = string.format("%s\nAuthor: %s\nVersion: %s", 
                        data.description, data.author, data.version)
                    name = data.name
                end)
            end
    
            if (mods[i] ~= modfolder) then
                menu:AddButton("$_" .. name, function()
                    game:executecommand("loadmod " .. mods[i])
                end, nil, true, nil, {
                    desc_text = desc
                })
            end
        end
    end
    
    menu:AddBackButton(function(a1)
        Engine.PlaySound(CoD.SFX.MenuBack)
        LUI.FlowManager.RequestLeaveMenu(a1)
    end)

    LUI.Options.InitScrollingList(menu.list, nil)
    menu:CreateBottomDivider()
    menu.optionTextInfo = LUI.Options.AddOptionTextInfo(menu)

    return menu
end

local localize = Engine.Localize
Engine.Localize = function(...)
    local args = {...}

    if (type(args[1]) == "string" and args[1]:sub(1, 2) == "$_") then
        return args[1]:sub(3, -1)
    end

    return localize(table.unpack(args))
end