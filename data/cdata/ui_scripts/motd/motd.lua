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

local function makelink(element, link)
	element:setHandleMouseMove(true)
	element:setHandleMouseButton(true)
	element:registerAnimationState("focused", {
		color = {
			r = 1,
			g = 1,
			b = 1
		}
	})

	local entered = false
	element:registerEventHandler("mouseenter", function()
		if (not entered) then
			Engine.PlaySound(CoD.SFX.MouseOver)
			entered = true
		end

		element:animateToState("focused")
	end)

	element:registerEventHandler("mouseleave", function()
		entered = false
		element:animateToState("default")
	end)

	element:registerEventHandler("leftmousedown", function()
		Engine.PlaySound(CoD.SFX.MouseClick)
		game:openlink(link)
	end)
end

local marketingbase = LUI.MarketingPopup.Base
LUI.MarketingPopup.Base = function(a1, data, a3)
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
