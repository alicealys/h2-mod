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
	}

	return normalfontlangs[lang] ~= true
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

		if (not usingspeciallanguage()) then
			if (id == 5 or (id >= 8 and id < 12) or id == 17) then
				LUI.MenuGenericButtons.ButtonLabelFont.Font = RegisterFont("fonts/noto_sans_sc.otf", 30)
				overrideyoffset = 0
				LUI.MenuGenericButtons.ButtonLabelFont.Height = 17
			elseif (id == 12) then
				LUI.MenuGenericButtons.ButtonLabelFont.Font = RegisterFont("fonts/noto_sans_arabic.ttf", 30)
				LUI.MenuGenericButtons.ButtonLabelFont.Height = 28
				overrideyoffset = 0
			elseif (id == 15) then
				LUI.MenuGenericButtons.ButtonLabelFont.Font = RegisterFont("fonts/noto_sans_kr.otf", 30)
				LUI.MenuGenericButtons.ButtonLabelFont.Height = 18
				overrideyoffset = 0
			end
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
