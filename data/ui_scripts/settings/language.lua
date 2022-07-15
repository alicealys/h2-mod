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
game:addlocalizedstring("MENU_spanishna", "Español (Parcial)") -- text only (English dubbing)
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

local languages = {
	"english",
	"french",
	"german",
	"italian",
	"spanish",
	"russian",
	"polish",
	"portuguese",
	"japanese_full",
	"japanese_partial",
	"traditional_chinese",
	"simplified_chinese",
	"arabic",
	"czech",
	"spanishna",
	"korean",
	"english_safe",
	"russian_partial",
}

local function usingspeciallanguage()
	local id = Engine.GetCurrentLanguage() + 1
	local lang = languages[id] or "english"

	local normalfontlangs = {
		["english"] = true,
		["french"] = true,
		["german"] = true,
		["italian"] = true,
		["spanish"] = true,
		["portuguese"] = true,
		["spanishna"] = true,
		["english_safe"] = true,
		["russian"] = true,
		["polish"] = true,
		["russian_partial"] = true,
	}

	return normalfontlangs[lang] ~= true
end

LUI.UIButtonText.IsOffsetedLanguage = function()
	if Engine.IsRightToLeftLanguage() then
		return true
	elseif Engine.IsAsianLanguage() then
		return true
	else
		return false
	end
end

local lang = Engine.GetCurrentLanguage()
if (lang == 5 or lang == 6 or lang == 17) then
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

local arabicfont = RegisterFont("fonts/arabic.ttf", 30)
local koreanfont = RegisterFont("fonts/korean.ttf", 30)
local polrusfont = RegisterFont("polrus/fonts/default.otf", 30)
local japanesefont = RegisterFont("fonts/japanese.ttf", 30)
local chinesefont = RegisterFont("fonts/chinese.ttf", 30)

local function setchinesefont(lang)
	if (lang ~= CoD.Language.Traditional_chinese and lang ~= CoD.Language.Simplified_chinese) then
		return
	end

	LUI.MenuGenericButtons.ButtonLabelFont.Font = chinesefont
	LUI.MenuGenericButtons.ButtonLabelFont.Height = 22
	overrideyoffset = 1
end

local function setjapanesefont(lang)
	if (lang ~= CoD.Language.Japanese_full and lang ~= CoD.Language.Japanese_partial) then
		return
	end

	LUI.MenuGenericButtons.ButtonLabelFont.Font = japanesefont
	LUI.MenuGenericButtons.ButtonLabelFont.Height = 18
	overrideyoffset = 1
end

local function setkoreanfont(lang)
	if (lang ~= CoD.Language.Korean) then
		return
	end

	LUI.MenuGenericButtons.ButtonLabelFont.Font = koreanfont
	LUI.MenuGenericButtons.ButtonLabelFont.Height = 25
	overrideyoffset = 1
end

local function setarabicfont(lang)
	if (lang ~= CoD.Language.Arabic) then
		return
	end

	LUI.MenuGenericButtons.ButtonLabelFont.Font = arabicfont
	LUI.MenuGenericButtons.ButtonLabelFont.Height = 28
	overrideyoffset = 0
end

local function setpolrusfont(lang)
	if (lang ~= CoD.Language.Russian and lang ~= CoD.Language.Russian_partial and lang ~= CoD.Language.Polish) then
		return
	end

	LUI.MenuGenericButtons.ButtonLabelFont.Font = polrusfont
	overrideyoffset = nil
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
		elseif (lang ~= CoD.Language.Arabic and lang ~= CoD.Language.Korean) then
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
