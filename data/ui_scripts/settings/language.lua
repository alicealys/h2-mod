game:addlocalizedstring("MENU_english", "English")
game:addlocalizedstring("MENU_french", "Français")
game:addlocalizedstring("MENU_german", "Deutsch")
game:addlocalizedstring("MENU_italian", "Italiano")
game:addlocalizedstring("MENU_spanish", "Español")
game:addlocalizedstring("MENU_russian", "Русский")
game:addlocalizedstring("MENU_polish", "Polski")
game:addlocalizedstring("MENU_portuguese", "Português")
game:addlocalizedstring("MENU_japanese_full", "日本語")
game:addlocalizedstring("MENU_japanese_partial", "日本語(一部)")
game:addlocalizedstring("MENU_traditional_chinese", "繁體中文")
game:addlocalizedstring("MENU_simplified_chinese", "简体中文")
game:addlocalizedstring("MENU_arabic", "العربية")
game:addlocalizedstring("MENU_czech", "České") -- ??
game:addlocalizedstring("MENU_spanishna", "Español (México)") -- text only (English dubbing)
game:addlocalizedstring("MENU_korean", "한국어")
game:addlocalizedstring("MENU_english_safe", "English (Safe)")
game:addlocalizedstring("MENU_russian_partial", "Русский (Только текст)") -- text only (English dubbing)

LUI.addmenubutton("pc_controls", {
	index = 4,
	text = "@LUA_MENU_CHOOSE_LANGUAGE",
	description = Engine.Localize("@LUA_MENU_CHOOSE_LANGUAGE_DESC"),
	callback = function()
		LUI.FlowManager.RequestAddMenu(nil, "choose_language_menu")
	end
})

local factory = LUI.UIGenericButton.ButtonLabelFactory
local overrideyoffset = nil
LUI.UIGenericButton.ButtonLabelFactory = function(data, ...)
	if (overrideyoffset) then
		data.yOffset = overrideyoffset
		overrideyoffset = nil
	end

	return factory(data, ...)
end

local arabicfont = RegisterFont("ara/fonts/default.otf", 30)
local koreanfont = RegisterFont("kor/fonts/default.otf", 30)
local polrusfont = RegisterFont("polrus/fonts/default.otf", 30)
local japanesefont = RegisterFont("jpf/fonts/default.otf", 30)
local chinesefont = RegisterFont("tch/fonts/default.otf", 30)

local function setchinesefont(lang)
	if (lang ~= CoD.Language.Traditional_chinese and lang ~= CoD.Language.Simplified_chinese) then
		return
	end

	LUI.MenuGenericButtons.ButtonLabelFont.Font = chinesefont
	LUI.MenuGenericButtons.ButtonLabelFont.Height = 17
	overrideyoffset = 2
end

local function setjapanesefont(lang)
	if (lang ~= CoD.Language.Japanese_full and lang ~= CoD.Language.Japanese_partial) then
		return
	end

	LUI.MenuGenericButtons.ButtonLabelFont.Font = japanesefont
	LUI.MenuGenericButtons.ButtonLabelFont.Height = 18.5
	overrideyoffset = 1.5
end

local function setkoreanfont(lang)
	if (lang ~= CoD.Language.Korean) then
		return
	end

	LUI.MenuGenericButtons.ButtonLabelFont.Font = koreanfont
	LUI.MenuGenericButtons.ButtonLabelFont.Height = 20
	overrideyoffset = 1.5
end

local function setarabicfont(lang)
	if (lang ~= CoD.Language.Arabic) then
		return
	end

	LUI.MenuGenericButtons.ButtonLabelFont.Font = arabicfont
	LUI.MenuGenericButtons.ButtonLabelFont.Height = 17
	overrideyoffset = 0
end

local function setpolrusfont(lang)
	if (lang ~= CoD.Language.Russian and lang ~= CoD.Language.Russian_partial and lang ~= CoD.Language.Polish) then
		return
	end

	LUI.MenuGenericButtons.ButtonLabelFont.Font = polrusfont
	overrideyoffset = 2.5
end

LUI.MenuBuilder.registerType("choose_language_menu", function(a1)
	local menu = LUI.MenuTemplate.new(a1, {
		menu_title = "@LUA_MENU_CHOOSE_LANGUAGE",
		menu_list_divider_top_offset = -(LUI.H1MenuTab.tabChangeHoldingElementHeight + H1MenuDims.spacing),
		menu_width = 300,
		uppercase_title = true
	})

	local languages = Engine.GetSupportedLanguages()

	for i = 1, #languages do
		local prevfont = LUI.MenuGenericButtons.ButtonLabelFont.Font
		local prevheight = LUI.MenuGenericButtons.ButtonLabelFont.Height
		local id = languages[i].id

		local lang = Engine.GetCurrentLanguage()

		if (lang == CoD.Language.Traditional_chinese or lang == CoD.Language.Simplified_chinese) then
			setjapanesefont(id)
			setkoreanfont(id)
			setarabicfont(id)
		elseif (lang == CoD.Language.Japanese_full or lang == CoD.Language.Japanese_partial) then
			setchinesefont(id)
			setkoreanfont(id)
			setarabicfont(id)
		elseif (lang ~= CoD.Language.Arabic or lang ~= CoD.Language.Korean) then
			setpolrusfont(id)
			setchinesefont(id)
			setjapanesefont(id)
			setkoreanfont(id)
			setarabicfont(id)
		end

		local button = menu:AddButton("", function()
			if (languages[i].id == Engine.GetCurrentLanguage()) then
				LUI.FlowManager.RequestLeaveMenu(nil, "choose_language_menu")
				return
			end

			LUI.FlowManager.RequestAddMenu(nil, "choose_language_confirm_popup", false, nil, true, {
				language = languages[i].id
			})
		end)

		overrideyoffset = nil

		LUI.MenuGenericButtons.ButtonLabelFont.Font = prevfont
		LUI.MenuGenericButtons.ButtonLabelFont.Height = prevheight

		local label = button:getFirstDescendentById("text_label")
		label:setText(Engine.ToUpperCase(languages[i].name))
	end

	LUI.Options.InitScrollingList(menu.list, nil, {
		rows = 11
	})
	menu:AddBackButton()

	return menu
end)

-- global patch
LUI.UIButtonText.IsOffsetedLanguage = function()
	return false
end

-- rus/pol patch

if (not Engine.InFrontend()) then
	local weaponinfodef = LUI.MenuBuilder.m_definitions["WeaponInfoHudDef"]
	LUI.MenuBuilder.m_definitions["WeaponInfoHudDef"] = function(...)
		local rus = CoD.Language.Russian
		CoD.Language.Russian = 1337
		local res = weaponinfodef(...)
		CoD.Language.Russian = rus
		return res
	end
else
	local levelselectmenu = LUI.sp_menus.LevelSelectMenu
	local setupinfobox = levelselectmenu.SetupInfoBoxLeftForArcadeMode
	levelselectmenu.SetupInfoBoxLeftForArcadeMode = function(...)
		local rus = CoD.Language.Russian
		CoD.Language.Russian = 1337
		local res = setupinfobox(...)
		CoD.Language.Russian = rus
		return res
	end
end

-- rus/pol/ara patch

local lang = Engine.GetCurrentLanguage()
if (lang == 5 or lang == 6 or lang == 12 or lang == 17) then
	local scale = function (size)
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
	
	CoD.TextSettings.H2TitleFont = {
		Font = RegisterFont("fonts/bank.ttf", 56),
		Height = scale(56)
	}
end