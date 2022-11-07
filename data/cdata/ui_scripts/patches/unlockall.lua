if (Engine.InFrontend()) then
    local levelselectmenu = LUI.sp_menus.LevelSelectMenu
    levelselectmenu.DisplayUnlockAllFooterPrompt = function(f79_arg0, f79_arg1)
        if not Engine.GetDvarBool("profileMenuOption_hasUnlockedAll_SP") then
            f79_arg0:AddHelp({
                name = "add_button_helper_text",
                button_ref = "button_alt2",
                helper_text = Engine.Localize("@LUA_MENU_CAMPAIGN_UNLOCKED_ALL_TITLE"),
                side = "right",
                clickable = true
            }, levelselectmenu.UnlockAllPopup)
        else
            f79_arg0:AddHelp({
                name = "add_button_helper_text",
                button_ref = "button_alt2",
                helper_text = Engine.Localize("@LUA_MENU_CANCEL_UNLOCK_CAPS"),
                side = "right",
                clickable = true
            }, levelselectmenu.SetUnlockAll)
        end
    end

    levelselectmenu.UnlockAllPopup = function(f70_arg0, f70_arg1)
        LUI.FlowManager.RequestAddMenu(nil, "request_yesno_popup_generic", true, f70_arg1.controller, nil, {
            popup_title = Engine.Localize("@LUA_MENU_CAMPAIGN_UNLOCKED_ALL_TITLE"),
            yes_action = levelselectmenu.SetUnlockAll,
            message_text = Engine.Localize("@MENU_COMPLETED_CHEAT") .. ". " ..
                Engine.Localize("@LUA_MENU_CAMPAIGN_UNLOCKED_ALL_DESC")
        })
    end
end

CoD.AllowUnlockAllInShip = true
