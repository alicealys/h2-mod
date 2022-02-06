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

LUI.addmenubutton("main_campaign", {
    index = 6,
    text = "$_MODS",
    description = "Load installed mods.",
    callback = function()
        LUI.FlowManager.RequestAddMenu(nil, "mods_menu")
    end
})

function getmodname(path)
    local name = path
    local desc = "Load " .. name
    local infofile = path .. "/info.json"

    if (io.fileexists(infofile)) then
        pcall(function()
            local data = json.decode(io.readfile(infofile))
            desc = string.format("%s\nAuthor: %s\nVersion: %s", 
                data.description, data.author, data.version)
            name = data.name
        end)
    end

    return name, desc
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
        if (LUI.MenuBuilder.m_types_build["mod_store_menu"]) then
            LUI.FlowManager.RequestAddMenu(nil, "mod_store_menu")
        end
    end, nil, true, nil, {
        desc_text = "Download and install mods."
    })

    local modfolder = game:getloadedmod()
    if (modfolder ~= "") then
        createdivider(menu, "$_Loaded mod: ^3" .. getmodname(modfolder):truncate(20))

        menu:AddButton("$_UNLOAD", function()
            game:executecommand("unloadmod")
        end, nil, true, nil, {
            desc_text = "Unload the currently loaded mod."
        })
    end

    createdivider(menu, "$_Available mods")

    if (io.directoryexists("mods")) then
        local mods = io.listfiles("mods/")
        for i = 1, #mods do
            if (io.directoryexists(mods[i]) and not io.directoryisempty(mods[i])) then
                local name, desc = getmodname(mods[i])

                if (mods[i] ~= modfolder) then
                    menu:AddButton("$_" .. name, function()
                        game:executecommand("loadmod " .. mods[i])
                    end, nil, true, nil, {
                        desc_text = desc
                    })
                end
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