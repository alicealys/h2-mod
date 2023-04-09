LUI.addmenubutton("main_campaign", {
	index = 6,
	text = "@LUA_MENU_ACHIEVEMENTS",
	description = Engine.Localize("@LUA_MENU_ACHIEVEMENTS_DESC"),
	callback = function()
		LUI.FlowManager.RequestAddMenu(nil, "achievements_menu")
	end
})

LUI.addmenubutton("main_campaign", {
	index = 6,
	text = "@MENU_MODS",
	description = Engine.Localize("@MENU_MODS_DESC"),
	callback = function()
		LUI.FlowManager.RequestAddMenu(nil, "mods_menu")
	end
})

local maincampaign = LUI.MenuBuilder.m_types_build["main_campaign"]
LUI.MenuBuilder.m_types_build["main_campaign"] = function(...)
    local initlist = LUI.Options.InitScrollingList
    LUI.Options.InitScrollingList = function(list) 
    	initlist(list, nil ,{
            rows = 10
        })
    end

    local menu = maincampaign(...)
    LUI.Options.InitScrollingList = initlist
    return menu
end
