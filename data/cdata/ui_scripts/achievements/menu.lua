LUI.MenuBuilder.registerType("achievements_menu", function(root, controller)
    local menuwidth = 500
	local menu = LUI.MenuTemplate.new(root, {
		menu_title = "@LUA_MENU_ACHIEVEMENTS",
		exclusiveController = 0,
		menu_width = menuwidth,
		menu_top_indent = LUI.MenuTemplate.spMenuOffset,
		showTopRightSmallBar = true,
		uppercase_title = true
	})

	local black_state = CoD.CreateState(nil, nil, nil, nil, CoD.AnchorTypes.All)
	black_state.red = 0
	black_state.blue = 0
	black_state.green = 0
	black_state.alpha = 0
	black_state.left = -100
	black_state.right = 100
	black_state.top = -100
	black_state.bottom = 100

	local black = LUI.UIImage.new(black_state)
	black:setPriority(-1000)

	black:registerAnimationState("BlackScreen", {
		alpha = 1
	})

	black:registerAnimationState("Faded", {
		alpha = 0
	})

	menu:addElement(black)

    local currentbackground = nil
	local changebackground = function(background, isvideobg)
        if (currentbackground == background) then
            return
        end

        currentbackground = background

		if (isvideobg) then
			PersistentBackground.ChangeBackground(nil, background)
		else
			PersistentBackground.ChangeBackground(background, "")
			PersistentBackground.ChangeBackground(background, nil)
		end

		black:animateInSequence( {
			{
				"BlackScreen",
				0
			},
			{
				"Faded",
				500
			}
		})
	end

    local itemheight = 80
    local itemwidth = menuwidth
    local itemspacing = 10
    local maxrowelements = 1
    
    local rowelements = 0
    local newrow = function()
        local currentrow = LUI.UIElement.new({
            topAnchor = true,
            leftAnchor = true,
            width = itemheight * maxrowelements,
            height = itemheight,
        })
    
        menu.list.currentrow = currentrow
        menu.list:addElement(currentrow)
        menu.list.currentrow:makeFocusable()
    end

    newrow()

    local addelement = function(element)
        local container = LUI.UIElement.new({
            topAnchor = true,
            leftAnchor = true,
            width = itemheight,
            height = itemheight,
            left = rowelements * (itemheight + itemspacing)
        })
        container:addElement(element)
        container:makeFocusable()
        menu.list.currentrow:addElement(container)
        rowelements = rowelements + 1
        if (rowelements == maxrowelements) then
            newrow()
            rowelements = 0
        end
    end

    local achievementtable = achievements.table()
    local unlockcount = 0
    for i = 0, #achievementtable do
        if (achievementtable[i]) then
            unlockcount = unlockcount + 1
        end
    end

    for i = 0, achievements.count() - 1 do
        local btnbg = LUI.UIImage.new({
            topAnchor = true,
            leftAnchor = true,
            width = itemwidth,
            height = itemheight,
            material = RegisterMaterial("h2_btn_unfocused")  
        })

        btnbg:setup9SliceImage()

        btnbg:registerAnimationState("locked", {
            material = RegisterMaterial("h2_btn_unfocused_locked")
        })

        local raritynames = {
            "common",
            "rare",
            "legendary",
            "epic",
        }

        local rarityname = raritynames[achievements.getrarity(i) + 1]

        local raritystrip = LUI.UIImage.new({
            topAnchor = true,
            leftAnchor = true,
            width = 10,
            height = itemheight,
            material = RegisterMaterial("depot_button_rarity_strip_" .. rarityname)  
        })

        raritystrip:registerAnimationState("hide", {
            alpha = 0
        })

        raritystrip:registerAnimationState("show", {
            alpha = 1
        })

        raritystrip:setup9SliceImage()

        local glow = LUI.UIImage.new({
            topAnchor = true,
            leftAnchor = true,
            width = itemwidth,
            height = itemheight,
            material = RegisterMaterial("depot_button_rarity_glow_" .. rarityname)
        })

        glow:registerAnimationState("hide", {
            alpha = 0
        })

        glow:registerAnimationState("show", {
            alpha = 1
        })

        local glow2 = LUI.UIImage.new({
            topAnchor = true,
            leftAnchor = true,
            width = itemwidth,
            height = itemheight,
            material = RegisterMaterial("depot_button_rarity_glow_" .. rarityname)
        })

        glow2:registerAnimationState("hide", {
            alpha = 0
        })

        glow2:registerAnimationState("show", {
            alpha = 1
        })

        glow:animateToState("hide")
        glow2:animateToState("hide")

        local achievementcontainer = LUI.UIElement.new({
            topAnchor = true,
            leftAnchor = true,
            width = itemwidth,
            height = itemheight,
        })

        local image = LUI.UIImage.new({
            topAnchor = true,
            leftAnchor = true,
            top = itemspacing / 2,
            left = itemspacing / 2 + 10,
            width = itemheight - itemspacing,
            height = itemheight - itemspacing,
            material = RegisterMaterial("trophy_" .. i) 
        })

        image:registerAnimationState("locked", {
            topAnchor = true,
            leftAnchor = true,
            top = itemspacing / 2 + 15,
            left = itemspacing / 2 + 15 + 10,
            width = itemheight - itemspacing - 30,
            height = itemheight - itemspacing - 30,
            material = RegisterMaterial("icon_lock_mini") 
        })

        local textwidth = itemwidth - itemheight - itemspacing * 2
        local title = LUI.UIText.new({
            topAnchor = true,
            leftAnchor = true,
            top = itemspacing + 5,
            left = itemheight + 5 + 10,
            color = {
                r = 0.7,
                g = 0.7,
                b = 0.7,
            },
            width = textwidth,
            font = CoD.TextSettings.Font23.Font,
            height = CoD.TextSettings.Font23.Height
        })

        local desc = LUI.UIText.new({
            topAnchor = true,
            leftAnchor = true,
            top = itemspacing + CoD.TextSettings.Font23.Height + 10,
            left = itemheight + 5 + 10,
            width = textwidth,
            alignment = LUI.Alignment.Left,
            color = {
                r = 0.7,
                g = 0.7,
                b = 0.7,
            },
            font = CoD.TextSettings.Font21.Font,
            height = CoD.TextSettings.Font21.Height
        })

        local focusedtext = {
            color = {r = 1, g = 1, b = 1}
        }
        desc:registerAnimationState("focused", focusedtext)
        title:registerAnimationState("focused", focusedtext)

        title:setText(Engine.ToUpperCase(Engine.Localize(achievements.getname(i))))
        desc:setText(Engine.Localize(achievements.getdetails(i)))

        achievementcontainer:setHandleMouse(true)
        local locked = not achievementtable[i]

        if (locked) then
            raritystrip:animateToState("hide")
        end

        local background = achievements.getbackground(i)
        achievementcontainer:registerEventHandler("mouseenter", function()
            Engine.PlaySound(CoD.SFX.MouseOver)

            if (background ~= nil) then
                changebackground(background, false)
            else
                changebackground("sp_menus_bg_regular", true)
            end

            if (locked) then
                btnbg:setImage(RegisterMaterial("h2_btn_focused_locked"))
            else
                raritystrip:animateToState("show")
                title:animateToState("focused")
                desc:animateToState("focused")
                glow:animateToState("show")
                glow2:animateToState("show")
            end
        end)
        achievementcontainer:registerEventHandler("mouseleave", function()
            if (locked) then
                btnbg:setImage(RegisterMaterial("h2_btn_unfocused_locked"))
            else
                title:animateToState("default")
                desc:animateToState("default")
                glow:animateToState("hide")
                glow2:animateToState("hide")
            end
        end)
    
        achievementcontainer:addElement(btnbg)
        achievementcontainer:addElement(glow)
        achievementcontainer:addElement(glow2)
        achievementcontainer:addElement(raritystrip)
        achievementcontainer:addElement(image)
        achievementcontainer:addElement(title)
        achievementcontainer:addElement(desc)

        if (locked) then
            btnbg:animateToState("locked")
            image:animateToState("locked")
            desc:setText("")
        end

        addelement(achievementcontainer)
    end

    if (rowelements == 0) then
        menu.list.currentrow:close()
    end

	LUI.Options.InitScrollingList(menu.list, nil, {
        rows = 5
    })

    local createprogressbar = function()
        local barwidth = itemwidth + 17
        local progressbar = LUI.UIElement.new({
            bottomAnchor = true,
            leftAnchor = true,
            width = barwidth,
            bottom = -90,
            height = 15
        })

        local bg = LUI.UIImage.new({
            topAnchor = true,
            leftAnchor = true,
            rightAnchor = true,
            bottomAnchor = true,
            alpha = 0.55,
            color = Colors.grey_14,
        })

        local bar = LUI.UIImage.new({
            topAnchor = true,
            leftAnchor = true,
            bottomAnchor = true,
            width = 0,
            material = RegisterMaterial("h1_ui_progressbar_green")
        })

        progressbar.setprogress = function(fraction)
            bar:registerAnimationState("progress", {
                topAnchor = true,
                leftAnchor = true,
                bottomAnchor = true,
                width = fraction * (barwidth),
            })
            
            bar:animateToState("progress", 300)
        end

        progressbar.setprogress(unlockcount / achievements.count())

        progressbar:addElement(bg)
        progressbar:addElement(bar)
        progressbar:addElement(LUI.DecoFrame.new(nil, LUI.DecoFrame.Grey))

        return progressbar
    end

    menu:addElement(createprogressbar())
    menu:AddBackButton()

    return menu
end)
