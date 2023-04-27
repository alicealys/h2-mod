if (Engine.InFrontend()) then
    return
end

LUI.onmenuopen("sp_pause_menu", function(element)
    local menu = element:getFirstChild()
    
	local button = menu:AddButton("@LUA_MENU_ACHIEVEMENTS", function()
        LUI.FlowManager.RequestAddMenu(nil, "achievements_menu")
    end)

    menu.list:removeElement(button)
    menu.list:insertElement(button, 4)

    menu:AddButton("@MENU_QUIT_TO_DESKTOP", function()
        LUI.FlowManager.RequestAddMenu(nil, "quit_popmenu")
    end)
end)
