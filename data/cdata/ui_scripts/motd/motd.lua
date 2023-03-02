require("LUI.common_menus.MarketingComms")
require("LUI.common_menus.MarketingPopup")

LUI.CustomMarketingPopups = {ShowDepotOnboardingPopupIfPossible = function() end}

LUI.MenuBuilder.registerPopupType("motd", function()
    local data = motd.getmotd()
    return LUI.MenuBuilder.BuildRegisteredType("motd_main", {
        popupDataQueue = {data}
    })
end)

LUI.onmenuopen("main_campaign", function(menu)
    if (not motd.hasseentoday()) then
        motd.sethasseentoday()
        LUI.FlowManager.RequestPopupMenu(nil, "motd")
    end

    menu:AddHelp({
        name = "add_button_helper_text",
        button_ref = "button_alt2",
        helper_text = Engine.Localize("@MENU_OPEN_MOTD"),
        side = "right",
        clickable = true
    }, function()
        LUI.FlowManager.RequestPopupMenu(nil, "motd")
    end)
end)

LUI.common_menus.MarketingPopup.OnPopupAction = function(a1, a2)
	local data = a1.popupData
    if (type(data.link) == "string") then
        game:openlink(data.link)
    end
end

local marketingbase = LUI.MarketingPopup.Base
LUI.MarketingPopup.Base = function(a1, data, a3)
    local haslink = data.popupAction ~= nil and game:islink(data.popupAction)
    if (haslink) then
        data.link = data.popupAction
        data.popupAction = "depot"
    end

    local element = marketingbase(a1, data, a3)
    local blur = element:getFirstDescendentById("generic_popup_screen_overlay_blur"):getNextSibling()
    local parent = blur:getFirstChild():getNextSibling():getNextSibling():getNextSibling()
    local image = parent:getFirstChild()
    image:close()

    local state = LUI.DeepCopy(image:getAnimationStateInC("default"))
    local imagecontainer = LUI.UIStencilText.new(state)
    local material = RegisterMaterial(data.image)
    
    local ratio = Engine.GetMaterialAspectRatio(material)
    local width = 525
    local height = width / ratio

    local image = LUI.UIImage.new({
        leftAnchor = true,
        topAnchor = true,
        width = width,
        height = height,
        material = material
    })

    imagecontainer:addElement(image)
    parent:addElement(imagecontainer)
    
    return element
end
