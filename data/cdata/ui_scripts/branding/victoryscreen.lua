local specopsmod = "mods/specops"

function CampaignCompleteMenu()
	Engine.ExecNow("profile_menuDvarsSetup")
	Engine.SetDvarBool("profileMenuOption_resumeIsGameCompleted", true)
	Engine.ExecNow("profile_menuDvarsFinish")
	Engine.Exec("updategamerprofile")

	local victory = LUI.UIImage.new({
		material = RegisterMaterial("victory"),
		top = -300,
		width = 1126,
		height = 563,
		alpha = 0
	})

	victory:registerAnimationState("show", {
		alpha = 1
	})

	victory:animateInSequence({
		{
			"default",
			0
		},
		{
			"show",
			1000
		}
	})

	local topoffset = 590
	local text = LUI.UIText.new({
		leftAnchor = true,
		rightAnchor = true,
		topAnchor = true,
		bottomAnchor = false,
		top = topoffset,
		bottom = topoffset + CoD.TextSettings.BodyFont18.Height,
		font = CoD.TextSettings.BodyFont18.Font,
		alignment = LUI.Alignment.Center,
		alpha = 0
	})

	text:registerAnimationState("show", {
		alpha = 1
	})

    text:setText(Engine.Localize("@MENU_SP_CONTINUE_TO_SPECIAL_OPS_CAPS"))
	victory:addElement(text)
	victory.text = text
	
	victory.skipLocked = true
	victory:addElement(LUI.UITimer.new(3000, "enable_quit", nil, true))
	victory:registerEventHandler("enable_quit", function (element, event)
		element.text:animateToState("show", 300)
		element.skipLocked = false
	end)

	local buttonhandler = function (element, event)
		if (element.skipLocked) then
			return 
		else
            if (mods.getinfo(specopsmod).isvalid) then
                mods.load(specopsmod)
            else
                game:openlink("specops")
                Engine.PlaySound(CoD.SFX.MenuAccept)
                Engine.PopupClosed(event.controller, "accept_normal")
                LUI.FlowManager.RequestCloseAllMenus(element)
            end
		end
	end
	
	local bindbutton = LUI.UIBindButton.new()
	bindbutton:registerEventHandler("button_start", buttonhandler)
	bindbutton:registerEventHandler("button_action", buttonhandler)
	bindbutton:registerEventHandler("button_secondary", buttonhandler)
	bindbutton.handlePrimary = true
	victory:addElement(bindbutton)

	return victory
end

LUI.MenuBuilder.m_types_build["MENU_CAMPAIGN_COMPLETED"] = CampaignCompleteMenu
