motd.getfeaturedtabtitle = function(index)
    return motd.getfeaturedtab(index).tab_title or ""
end

local animmsfull = 150
local animms = animmsfull / 2

LUI.onmenuopen("main_campaign", function(menu)
    if (mods.getloaded() ~= nil or motd.getnumfeaturedtabs() <= 0) then
        return
    end

    local left = 866
    local featuredcontainer = LUI.UIElement.new({
        leftAnchor = true,
        topAnchor = true,
        left = left,
        top = 420,
    })

    featuredcontainer:registerAnimationState("hide", {
        leftAnchor = true,
        topAnchor = true,
        left = left,
        top = 570,
    })

    featuredcontainer:registerAnimationState("show", {
        leftAnchor = true,
        topAnchor = true,
        left = left,
        top = 420,
    })

    local featuredstencil = LUI.UIStencilText.new({
        leftAnchor = true,
        topAnchor = true,
        height = 200,
        width = GenericMenuDims.menu_width_standard + (10 + 12) * 2,
    })

    local featured = LUI.UIElement.new({
        left = 20,
        leftAnchor = true,
        topAnchor = true,
        bottomAnchor = true,
        rightAnchor = true
    })

    local arrowcontainer = LUI.UIElement.new({
        leftAnchor = true,
        topAnchor = true,
        bottomAnchor = true,
        rightAnchor = true
    })

    arrowcontainer:registerAnimationState("hide", {
        alpha = 0
    })

    arrowcontainer:registerAnimationState("show", {
        alpha = 1
    })

    featured:addElement(arrowcontainer)

    featuredstencil:registerAnimationState("hide", {
        leftAnchor = true,
        topAnchor = true,
        height = GenericButtonSettings.Styles.FlatButton.height + 2,
        width = GenericMenuDims.menu_width_standard + (10 + 12) * 2,
    })

    local istopmost = function()
        return LUI.FlowManager.IsMenuTopmost(Engine.GetLuiRoot(), "main_campaign")
    end

    featuredstencil:registerAnimationState("show", {
        leftAnchor = true,
        topAnchor = true,
        height = 200,
        width = GenericMenuDims.menu_width_standard + (10 + 12) * 2,
    })

    local hoverelem = LUI.UIElement.new({
        leftAnchor = true,
        topAnchor = true,
        top = -10,
        height = 150 + GenericButtonSettings.Styles.FlatButton.height + 2 + 10,
        width = GenericMenuDims.menu_width_standard + 50,
        left = -25
    })

    local header = LUI.UIElement.new({
        topAnchor = true,
        leftAnchor = true,
        width = GenericMenuDims.menu_width_standard,
        height = GenericButtonSettings.Styles.FlatButton.height,
    })

    local headerbg = LUI.UIImage.new({
        topAnchor = true,
        leftAnchor = true,
        bottomAnchor = true,
        rightAnchor = true,
        alpha = 0.55,
        color = Colors.grey_14,
        material = RegisterMaterial("white")
    })

    local headerbutton = LUI.UIImage.new({
        leftAnchor = true,
        topAnchor = true,
        height = 10,
        width = 10,
        top = 10.5,
        left = 10,
        zRot = -90,
        color = {
            r = 0.6,
            g = 0.6,
            b = 0.6,
        },
        material = RegisterMaterial("widg_lobby_arrow")
    })

    headerbutton:registerEventHandler("mouseenter", function()
        if (not istopmost()) then
            return
        end

        Engine.PlaySound(CoD.SFX.MouseOver)
        headerbutton:animateToState("focused")
    end)

    headerbutton:registerEventHandler("mouseleave", function()
        headerbutton:animateToState("unfocused")
    end)

    headerbutton:registerAnimationState("down", {
        zRot = -90
    })

    headerbutton:registerAnimationState("right", {
        zRot = 0
    })

    headerbutton:registerAnimationState("focused", {
        color = Colors.h2.yellow
    })

    headerbutton:registerAnimationState("unfocused", {
        color = {
            r = 0.6,
            g = 0.6,
            b = 0.6,
        }
    })

    local tabcount = motd.getnumfeaturedtabs()

    headerbutton:setHandleMouse(true)
    local minimized = false
    headerbutton:registerEventHandler("leftmousedown", function()
        Engine.PlaySound(CoD.SFX.MenuAccept)
        headerbutton:processEvent({
            name = "mouseleave"
        })

        minimized = not minimized

        if (minimized) then
            featuredstencil:animateToState("hide", animms)
            featuredcontainer:animateToState("hide", animms)
            headerbutton:animateToState("right", animms)
            arrowcontainer:animateToState("hide", animms)
        else
            featuredstencil:animateToState("show", animms)
            featuredcontainer:animateToState("show", animms)
            headerbutton:animateToState("down", animms)

            if (tabcount > 1) then
                arrowcontainer:animateToState("show", animms)
            end
        end
    end)

    header:addElement(headerbg)
    header:addElement(headerbutton)

    local headertext = LUI.UIText.new({
        leftAnchor = true,
        left = GenericButtonSettings.Styles.FlatButton.text_padding_with_content + 10,
        top = -CoD.TextSettings.Font19.Height / 2 + 1.5,
        font = CoD.TextSettings.Font19.Font,
        height = CoD.TextSettings.Font19.Height
    })

    local rightoffset = -15
    local pips = {}
    local focusindex = 0

    local shiftto = nil

    for i = 1, tabcount do
        local pipmat = RegisterMaterial("h1_ui_featured_pip_unfocused")
        local ratio = Engine.GetMaterialAspectRatio(pipmat)
        local height = 14
    
        local pip = LUI.UIImage.new({
            topAnchor = true,
            rightAnchor = true,
            top = GenericButtonSettings.Styles.FlatButton.height / 2 - height / 2,
            width = ratio * height,
            height = height,
            material = RegisterMaterial("h1_ui_featured_pip_unfocused"),
            right = rightoffset
        })

        pip:setHandleMouse(true)
        pip:registerEventHandler("mouseenter", function()
            if (not istopmost() or minimized) then
                return
            end

            Engine.PlaySound(CoD.SFX.MouseOver)
        end)

        pip:registerEventHandler("leftmousedown", function()
            if (not istopmost() or minimized) then
                return
            end

            Engine.PlaySound(CoD.SFX.MenuAccept)
            shiftto(#pips - i)
        end)
    
        pip:registerAnimationState("focused", {
            material = RegisterMaterial("h2_ui_featured_pip_focused"),
        })

        pip:registerAnimationState("unfocused", {
            material = RegisterMaterial("h1_ui_featured_pip_unfocused"),
        })

        table.insert(pips, pip)
        header:addElement(pip)
        rightoffset = rightoffset - height
    end

    pips[#pips]:animateToState("focused")

    headertext:setText(Engine.ToUpperCase(Engine.Localize("@LUA_MENU_FEATURED")))

    local content = LUI.UIElement.new({
        topAnchor = true,
        leftAnchor = true,
        top = GenericButtonSettings.Styles.FlatButton.height + 3,
        width = GenericMenuDims.menu_width_standard,
        height = 150,
    })

    local stencil = LUI.UIStencilText.new({
        topAnchor = true,
        leftAnchor = true,
        bottomAnchor = true,
        rightAnchor = true,
        left = 1,
    })

    local imagelist = LUI.UIHorizontalList.new( {
		left = 0,
		leftAnchor = true,
		width = (GenericMenuDims.menu_width_standard) * tabcount,
		height = 150,
		spacing = 0
	} )

    stencil:addElement(imagelist)

    local contentborder = LUI.UIImage.new({
        topAnchor = true,
        leftAnchor = true,
        bottomAnchor = true,
        rightAnchor = true,
        alpha = 0,
        material = RegisterMaterial("h2_ui_btn_focused_stroke_square") 
    })

    contentborder:registerAnimationState("unfocused", {
        alpha = 0
    })

    contentborder:registerAnimationState("focused", {
        alpha = 1
    })

    contentborder:setup9SliceImage()

    local shift = function()
        imagelist:registerAnimationState("move", {
            leftAnchor = true,
            left = (focusindex) * -GenericMenuDims.menu_width_standard
        })

        imagelist:animateToState("move", animmsfull)
    end

    shiftto = function(index)
        local prevfocus = focusindex
        focusindex = (index) % tabcount

        pips[#pips - (prevfocus)]:animateToState("unfocused")
        pips[#pips - (focusindex)]:animateToState("focused")

        shift()
    end

    local shiftright = function()
        local prevfocus = focusindex
        focusindex = (focusindex + 1) % tabcount

        pips[#pips - (prevfocus)]:animateToState("unfocused")
        pips[#pips - (focusindex)]:animateToState("focused")

        shift()
    end

    local shiftleft = function()
        local prevfocus = focusindex
        focusindex = (focusindex - 1) % tabcount

        pips[#pips - (prevfocus)]:animateToState("unfocused")
        pips[#pips - (focusindex)]:animateToState("focused")

        shift()
    end

    for i = 1, tabcount do
        local panel = LUI.UIElement.new({
            topAnchor = true,
            leftAnchor = true,
            bottomAnchor = true,
            width = GenericMenuDims.menu_width_standard,
        })

        local text = LUI.UIText.new({
            bottomAnchor = true,
            leftAnchor = true,
            rightAnchor = true,
            alignment = LUI.Alignment.Center,
            bottom = (-CoD.TextSettings.Font21.Height / 2) + 2,
            height = CoD.TextSettings.Font21.Height,
            font = CoD.TextSettings.Font21.Font,
        })

        local textbg = LUI.UIImage.new({
            bottomAnchor = true,
            leftAnchor = true,
            rightAnchor = true,
            height = CoD.TextSettings.Font21.Height * 2,
            alpha = 0.75,
            material = RegisterMaterial("black")
        })

        text:setText(motd.getfeaturedtabtitle(i - 1))

        local material = RegisterMaterial("featured_panel_thumbnail_" .. i)
        local ratio = Engine.GetMaterialAspectRatio(material)
        local width = GenericMenuDims.menu_width_standard
        local height = width / ratio

        local contentimage = LUI.UIImage.new({
            topAnchor = true,
            leftAnchor = true,
            width = width,
            height = height,
            material = material,
        })
    
        panel:addElement(contentimage)
        panel:addElement(textbg)
        panel:addElement(text)
        imagelist:addElement(panel)
    end

    local autoscrolltimer = LUI.UITimer.new(3000, "autoscroll")
    featured:addElement(autoscrolltimer)
    featured:registerEventHandler("autoscroll", function()
        if (not minimized) then
            shiftright()
        end
    end)

    local addarrows = function()
        local arrowmat = RegisterMaterial("h1_prestige_leftright_arrow")
        local arrowratio = Engine.GetMaterialAspectRatio(arrowmat)
        local height = 20
        local width = arrowratio * height

        local arrowleft = LUI.UIImage.new({
            leftAnchor = true,
            left = -width - 10,
            top = -10,
            height = height,
            width = width,
            alpha = 0,
            material = RegisterMaterial("h1_prestige_leftright_arrow") 
        })

        local arrowright = LUI.UIImage.new({
            rightAnchor = true,
            right = 0,
            top = -10,
            height = height,
            width = width,
            zRot = 180,
            alpha = 0,
            material = RegisterMaterial("h1_prestige_leftright_arrow") 
        })

        local registeranimationstate = function(name, state)
            arrowleft:registerAnimationState(name, state)
            arrowright:registerAnimationState(name, state)
        end

        registeranimationstate("pulse", {
            scale = -0.1
        })
        
        registeranimationstate("highlight", {
            scale = 0
        })

        registeranimationstate("focused", {
            alpha = 1
        })

        registeranimationstate("unfocused", {
            alpha = 0
        })

        arrowleft:setHandleMouse(true)
        arrowright:setHandleMouse(true)

        local pulsearrow = function(arrow)
            arrow:animateInSequence({
                {
                    "highlight",
                    0
                },
                {
                    "pulse",
                    100
                },
                {
                    "highlight",
                    100
                }
            })
        end

        arrowleft:registerEventHandler("leftmousedown", function()
            if (not istopmost()) then
                return
            end

            pulsearrow(arrowleft)
            Engine.PlaySound(CoD.SFX.MouseOver)
            shiftleft()
        end)
        arrowright:registerEventHandler("leftmousedown", function()
            if (not istopmost()) then
                return
            end

            pulsearrow(arrowright)
            Engine.PlaySound(CoD.SFX.MouseOver)
            shiftright()
        end)

        arrowcontainer:addElement(arrowleft)
        arrowcontainer:addElement(arrowright)

        featured.arrowleft = arrowleft
        featured.arrowright = arrowright
    end

    addarrows()

    if (tabcount <= 1) then
        arrowcontainer:animateToState("hide")
    end

    featured:registerEventHandler("focused", function()
        contentborder:animateToState("focused", animms)
        featured.arrowleft:animateToState("focused", animms)
        featured.arrowright:animateToState("focused", animms)
        LUI.UITimer.Stop(autoscrolltimer)
    end)

    
    featured:registerEventHandler("unfocused", function()
        contentborder:animateToState("unfocused", animms)
        featured.arrowleft:animateToState("unfocused", animms)
        featured.arrowright:animateToState("unfocused", animms)
        LUI.UITimer.Reset(autoscrolltimer)
    end)

    hoverelem:setHandleMouseMove(true)

    hoverelem:registerEventHandler("mouseenter", function()
        if (not istopmost()) then
            return
        end

        featured:processEvent({
            name = "focused",
            dispatchChildren = true
        })
    end)

    hoverelem:registerEventHandler("mouseleave", function()
        featured:processEvent({
            name = "unfocused",
            dispatchChildren = true
        })
    end)

    header:addElement(headertext)
    header:addElement(LUI.DecoFrame.new(nil, LUI.DecoFrame.Grey))

    featured:addElement(header)

    content:addElement(stencil)
    content:addElement(LUI.DecoFrame.new(nil, LUI.DecoFrame.Grey))
    content:addElement(contentborder)

    content:setHandleMouse(true)

    content:registerEventHandler("leftmousedown", function()
        if (not istopmost() or minimized) then
            return
        end

        hoverelem:processEvent({
            name = "mouseleave",
        })

        Engine.PlaySound(CoD.SFX.MenuAccept)
        local data = motd.getfeaturedtab(focusindex)
        data.popup_image = "featured_panel_" .. (focusindex + 1)
        LUI.FlowManager.RequestPopupMenu( nil, "motd_main", true, nil, false, {
            popupDataQueue = {data}
        })
    end)

    content:registerEventHandler("mouseenter", function()
        if (not istopmost() or minimized) then
            return
        end

        Engine.PlaySound(CoD.SFX.MouseOver)
    end)

    featured:addElement(content)
    featured:addElement(hoverelem)

    featuredstencil:addElement(featured)
    featuredcontainer:addElement(featuredstencil)

    menu:addElement(featuredcontainer)
end)

