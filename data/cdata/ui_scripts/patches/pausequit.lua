if (Engine.InFrontend()) then
    return
end

LUI.onmenuopen("sp_pause_menu", function(element)
    local menu = element:getFirstChild()
    menu:AddButton("@MENU_QUIT_TO_DESKTOP", function()
        LUI.FlowManager.RequestAddMenu(nil, "quit_popmenu")
    end)
end)
