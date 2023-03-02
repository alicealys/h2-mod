require("LUI.common_menus.MarketingComms")
require("LUI.common_menus.MarketingPopup")
LUI.CustomMarketingPopups = {ShowDepotOnboardingPopupIfPossible = function() end}

LUI.onmenuopen("main_campaign", function(menu)
    if (not motd.hasseentoday()) then
        motd.sethasseentoday()
        local data = motd.getmotd()
        LUI.FlowManager.RequestPopupMenu( self, "motd_main", true, nil, false, {
            popupDataQueue = {data}
        })
    end
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

    local helpertext = element:getFirstDescendentById("helper_text_text")
    if (haslink and helpertext) then
        helpertext:setText(Engine.Localize("@MENU_OPEN_LINK"))
    end
    
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
