LUI.addmenubutton("pc_controls", {
    index = 4,
    text = "@LUA_MENU_CHOOSE_LANGUAGE",
    description = Engine.Localize("@LUA_MENU_CHOOSE_LANGUAGE_DESC"),
    callback = function()
        LUI.FlowManager.RequestAddMenu(nil, "choose_language_menu")
    end
})

local universalfont = RegisterFont("fonts/mix.ttf", 30)

local function setuniversalfont(lang)
    LUI.MenuGenericButtons.ButtonLabelFont.Font = universalfont
end

local function togglecustomfonts()
    LUI.FlowManager.RequestAddMenu(nil, "generic_yesno_popup", false, nil, true, {
        popup_title = Engine.Localize("@MENU_CCS_RESTART_CONFIRMATION_TITLE"),
        message_text = Engine.Localize("@MENU_HDR_REBOOT_DESC"),
        yes_action = function()
            config.set("disable_custom_fonts", not config.get("disable_custom_fonts"))
            updater.relaunch()
        end
    })
end

LUI.MenuBuilder.registerType("choose_language_menu", function(a1)
    local menu = LUI.MenuTemplate.new(a1, {
        menu_title = "@LUA_MENU_CHOOSE_LANGUAGE",
        menu_list_divider_top_offset = -(LUI.H1MenuTab.tabChangeHoldingElementHeight + H1MenuDims.spacing),
        menu_width = 240,
        uppercase_title = true
    })

    local languages = Engine.GetSupportedLanguages()
    local originalfont = LUI.MenuGenericButtons.ButtonLabelFont.Font

    for i = 1, #languages do
        local id = languages[i].id

        local lang = Engine.GetCurrentLanguage()

        if language.isasian() or not language.isnonlatin() or not config.get("disable_custom_fonts") then
            setuniversalfont(id)
        end

        local button = menu:AddButton("", function()
            LUI.FlowManager.RequestAddMenu(nil, "choose_language_confirm_popup", false, nil, true, {
                language = languages[i].id
            })
        end, languages[i].id == lang, true, nil, {
            desc_text = Engine.Localize("LOCALE_" .. (languages[i].id))
        })

        local label = button:getFirstDescendentById("text_label")
        label:setText(Engine.ToUpperCase(languages[i].name))
    end

    LUI.MenuGenericButtons.ButtonLabelFont.Font = originalfont

    LUI.Options.InitScrollingList(menu.list, nil, {
        rows = 10
    })

    LUI.Options.AddOptionTextInfo(menu)

    if (language.isnonlatin() and not language.iscustom()) then
        if (config.get("disable_custom_fonts")) then
            menu:AddHelp({
                name = "add_button_helper_text",
                button_ref = "button_alt2",
                helper_text = Engine.Localize("@LUA_MENU_FALLBACK_ENABLE"),
                side = "right",
                clickable = true
            }, togglecustomfonts)
        else
            menu:AddHelp({
                name = "add_button_helper_text",
                button_ref = "button_alt2",
                helper_text = Engine.Localize("@LUA_MENU_FALLBACK_DISABLE"),
                side = "right",
                clickable = true
            }, togglecustomfonts)
        end
    end

    menu:AddBackButton()

    return menu
end)

-- fix for Y-offset in button text (russian_partial, default font)
if config.get("language") == "russian_partial" and config.get("disable_custom_fonts") then
    LUI.UIButtonText.IsOffsetedLanguage = function()
        return true
    end
end

if not (config.get("disable_custom_fonts")) then
    -- fix for Y-offset in button text (global patch, custom font)
    LUI.UIButtonText.IsOffsetedLanguage = function()
        return false
    end

    -- fix for ammo counter (polish/russian)
    if not Engine.InFrontend() then
        local weaponinfodef = LUI.MenuBuilder.m_definitions["WeaponInfoHudDef"]
        LUI.MenuBuilder.m_definitions["WeaponInfoHudDef"] = function(...)
            local rus = CoD.Language.Russian
            CoD.Language.Russian = 1337
            local pol = CoD.Language.Polish
            CoD.Language.Polish = 1338
            local res = weaponinfodef(...)
            CoD.Language.Russian = rus
            CoD.Language.Polish = pol
            return res
        end
    else
        local levelselectmenu = LUI.sp_menus.LevelSelectMenu
        local setupinfobox = levelselectmenu.SetupInfoBoxLeftForArcadeMode
        levelselectmenu.SetupInfoBoxLeftForArcadeMode = function(...)
            local rus = CoD.Language.Russian
            CoD.Language.Russian = 1337
            local pol = CoD.Language.Polish
            CoD.Language.Polish = 1338
            local res = setupinfobox(...)
            CoD.Language.Russian = rus
            CoD.Language.Polish = pol
            return res
        end
    end

    -- fix for ammo counter (global patch)
    if language.isnonlatin() then
        local scale = function(size)
            return size * 720 / 1080
        end

        CoD.TextSettings.SP_HudAmmoStatusText = {
            Font = RegisterFont("fonts/bank.ttf", 16),
            Height = 16
        }

        CoD.TextSettings.SP_HudAmmoCounterFont = {
            Font = RegisterFont("fonts/bank.ttf", 34),
            Height = 34
        }

        CoD.TextSettings.HudAmmoCounterFont = {
            Font = RegisterFont("fonts/bank.ttf", 105),
            Height = 64
        }
        -- forced gothic font for headers (arabic/slavic)
        if language.isslavic() or language.isarabic() then
            CoD.TextSettings.H2TitleFont = {
                Font = RegisterFont("fonts/bank.ttf", 56),
                Height = scale(56)
            }
        end
    end
end
