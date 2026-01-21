local wordlecolors = {
    grey = {
        r = 120 / 255,
        g = 124 / 255,
        b = 127 / 255,
    },
    yellow = {
        r = 200 / 255,
        g = 182 / 255,
        b = 83 / 255,
    },
    green = {
        r = 108 / 255,
        g = 169 / 255,
        b = 101 / 255,
    },
    black = {
        r = 0,
        g = 0,
        b = 0,
    },
    white = {
        r = 1,
        g = 1,
        b = 1,
    },
    white_alt = {
        r = 0.1,
        g = 0.1,
        b = 0.1,
    }
}

local maxrows = 6
local maxcols = 5

local state = {}
state.currentword = ""
state.currentrow = 1
state.completed = false
state.result = false
state.guesses = {}

local keyboardwidget = function()
    local spacing = 1
    local keysize = 25
    local widget = LUI.UIElement.new({
        leftAnchor = true,
        topAnchor = true,
        rightAnchor = true,
        height = keysize * 3 + spacing * 3 + 4
    })

    local bg = LUI.UIImage.new({
        leftAnchor = true,
        topAnchor = true,
        rightAnchor = true,
        bottomAnchor = true,
        material = RegisterMaterial("black"),
        alpha = 0.5,
    })

    widget:addElement(bg)
    widget:addElement(LUI.DecoFrame.new(nil, LUI.DecoFrame.Grey))

    local lines = {}
    table.insert(lines, "qwertyuiop")
    table.insert(lines, "asdfghjkl")
    table.insert(lines, "zxcvbnm")

    local grid = LUI.UIVerticalList.new({
        top = 2,
        leftAnchor = true,
        topAnchor = true,
        rightAnchor = true,
        leftAnchor = true,
        spacing = spacing,
    })

    widget.keys = {}

    local createkey = function(key)
        local box = LUI.UIElement.new({
            leftAnchor = true,
            topAnchor = true,
            height = keysize,
            width = keysize,
        })

        local bg = LUI.UIImage.new({
            leftAnchor = true,
            topAnchor = true,
            bottomAnchor = true,
            rightAnchor = true,
            material = RegisterMaterial("white"),
            color = wordlecolors.white_alt,
            alpha = 0.2
        })

        local text = LUI.UIText.new({
            leftAnchor = true,
            topAnchor = true,
            bottomAnchor = true,
            rightAnchor = true,
            font = RegisterFont("fonts/defaultBold.otf", 20),
            color = Colors.white,
        })

        box.setcolor = function(box, color)
            bg:registerAnimationState("color", {
                color = color
            })

            bg:setAlpha(0.8)

            bg:animateToState("color", 0)
        end

        text:setText(key:upper())
        box:addElement(bg)
        box:addElement(text)

        box.key = key
        table.insert(widget.keys, box)
        
        return box
    end

    for i = 1, #lines do
        local row = LUI.UIHorizontalList.new({
            leftAnchor = true,
            topAnchor = true,
            rightAnchor = true,
            height = keysize,
            spacing = spacing,
            alignment = LUI.Alignment.Center,
        })

        row.id = "row" .. i

        for o = 1, #lines[i] do
            row:addElement(createkey(lines[i]:sub(o, o)))
        end

        grid:addElement(row)
    end

    widget:addElement(grid)

    widget.setkeycolor = function(widget, key, color)
        for i = 1, #widget.keys do
            if (widget.keys[i].key:upper() == key:upper()) then
                widget.keys[i]:setcolor(color)
            end
        end
    end

    return widget
end

LUI.MenuBuilder.registerPopupType("wordle_widget", function(menu, controller)
    local headersize = 50
    local footersize = 30
    local padding = 10
    local size = 300
    local bordersize = 1
    local spacing = 5
    local boxsize = size / maxcols - spacing

    local width = padding + (boxsize + spacing) * maxcols
    local height = headersize + padding * 2 + footersize + (boxsize + spacing) * maxrows

    local offsety = 30
    if (motd.hassolvedwordle()) then
        offsety = 0
        height = headersize + padding * 2 + footersize + 50
    end

    local root = LUI.UIElement.new({
        topAnchor = true,
        leftAnchor = true,
        bottomAnchor = true,
        rightAnchor = true,
    })

    local mainbg = LUI.UIImage.new({
        topAnchor = true,
        leftAnchor = true,
        bottomAnchor = true,
        rightAnchor = true,
        material = RegisterMaterial("black"),
        alpha = 0.5,
    })

    local list = LUI.UIVerticalList.new({
        topAnchor = true,
        leftAnchor = true,
        bottomAnchor = true,
        rightAnchor = true,
        left = (1280 - width) / 2,
        right = -(1280 - width) / 2,
        top = (720 - height) / 2 - offsety,
        bottom = -(720 - height) / 2 - offsety,
        aligment = LUI.Alignment.Center,
        spacing = 5,
    })

    local widget = LUI.UIElement.new({
        topAnchor = true,
        leftAnchor = true,
        bottomAnchor = true,
        rightAnchor = true,
    })

    list:addElement(widget)
    root:addElement(mainbg)
    root:addElement(list)

    if (not motd.hassolvedwordle()) then
        widget.keyboard = keyboardwidget()
        list:addElement(widget.keyboard)
    end

    local bg = LUI.UIImage.new({
        topAnchor = true,
        leftAnchor = true,
        bottomAnchor = true,
        rightAnchor = true,
        material = RegisterMaterial("white"),
        color = {
            r = 0,
            g = 0,
            b = 0,
        },
        alpha = 0.5
    })

    local title = LUI.UIText.new({
        topAnchor = true,
        leftAnchor = true,
        rightAnchor = true,
        top = 5 + padding,
        height = 30,
        font = RegisterFont("fonts/default.otf", 30),
        alignment = LUI.Alignment.Center,
        color = wordlecolors.white
    })

    local desc = LUI.UIText.new({
        topAnchor = true,
        leftAnchor = true,
        rightAnchor = true,
        top = 5 + padding + 30,
        height = 15,
        font = RegisterFont("fonts/default.otf", 30),
        alignment = LUI.Alignment.Center,
        color = wordlecolors.white
    })

    local splashtext = LUI.UIText.new({
        leftAnchor = true,
        bottomAnchor = true,
        rightAnchor = true,
        bottom = -padding,
        height = 15,
        font = RegisterFont("fonts/default.otf", 30),
        alignment = LUI.Alignment.Center,
        color = wordlecolors.white,
        alpha = 0
    })

    splashtext:registerAnimationState("show", {
        alpha = 1
    })

    splashtext:registerAnimationState("hide", {
        alpha = 0
    })

    local info = LUI.UIText.new({
        topAnchor = true,
        leftAnchor = true,
        rightAnchor = true,
        top = 5 + padding + 60,
        height = 15,
        font = RegisterFont("fonts/default.otf", 30),
        alignment = LUI.Alignment.Center,
        color = wordlecolors.white
    })

    info:setText(Engine.Localize("@MOTD_WORDLE_COMPLETED"))

    local scoretext = LUI.UIText.new({
        leftAnchor = true,
        bottomAnchor = true,
        left = padding,
        bottom = -padding,
        height = 15,
        font = RegisterFont("fonts/default.otf", 30),
        alignment = LUI.Alignment.Left,
        color = wordlecolors.white
    })

    local setscore = function()
        scoretext:setText(Engine.Localize("@MOTD_WORDLE_SCORE", motd.getwordlescore()))
    end

    local createbox = function()
        local box = LUI.UIElement.new({
            topAnchor = true,
            leftAnchor = true,
            width = boxsize,
            height = boxsize,
        })

        local bg = LUI.UIElement.new({
            topAnchor = true,
            leftAnchor = true,
            bottomAnchor = true,
            rightAnchor = true
        })

        local bgbottom = LUI.UIImage.new({
            topAnchor = true,
            leftAnchor = true,
            bottomAnchor = true,
            rightAnchor = true,
            top = bordersize,
            left = bordersize,
            right = -bordersize,
            bottom = -bordersize,
            material = RegisterMaterial("white"),
            color = wordlecolors.white_alt,
            alpha = 0.8
        })

        local bgtop = LUI.UIImage.new({
            topAnchor = true,
            leftAnchor = true,
            bottomAnchor = true,
            rightAnchor = true,
            material = RegisterMaterial("white"),
            color = wordlecolors.grey,
            alpha = 0.2
        })

        bg:addElement(bgtop)
        bg:addElement(LUI.DecoFrame.new(nil, LUI.DecoFrame.Grey))

        local letterheight = 45

        local letter = LUI.UIText.new({
            topAnchor = true,
            leftAnchor = true,
            rightAnchor = true,
            bottomAnchor = true,
            font = RegisterFont("fonts/defaultBold.otf", 30),
            color = wordlecolors.white,
        })

        box:addElement(bg)
        box:addElement(letter)

        box:registerAnimationState("pulse_start", {
            topAnchor = true,
            leftAnchor = true,
            width = boxsize + 5,
            height = boxsize + 5,
            top = -2.5,
            left = -2.5,
        })

        box:registerAnimationState("pulse_end", {
            topAnchor = true,
            leftAnchor = true,
            top = 0,
            left = 0,
            width = boxsize,
            height = boxsize
        })

        box.doflip = function(box, color, delay)
            letter:registerAnimationState("flip_start", {
                topAnchor = true,
                leftAnchor = true,
                rightAnchor = true,
                bottomAnchor = true,
                top = boxsize / 2,
                bottom = -boxsize / 2
            })

            letter:registerAnimationState("flip_end", {
                topAnchor = true,
                leftAnchor = true,
                rightAnchor = true,
                bottomAnchor = true,
                color = wordlecolors.white,
            })

            bg:registerAnimationState("flip_start", {
                topAnchor = true,
                leftAnchor = true,
                bottomAnchor = true,
                rightAnchor = true,
                top = boxsize / 2,
                bottom = -boxsize / 2
            })

            bg:registerAnimationState("flip_end", {
                topAnchor = true,
                leftAnchor = true,
                bottomAnchor = true,
                rightAnchor = true,
                top = bordersize,
                left = bordersize,
                right = -bordersize,
                bottom = -bordersize,
            })

            local timerstart = LUI.UITimer.new(delay, "flip_start")
            bg:addElement(timerstart)

            bg:registerEventHandler("flip_start", function()
                local timerend = LUI.UITimer.new(250, "flip_end")
                bg:addElement(timerend)
                bg:registerEventHandler("flip_end", function()
                    timerend:close()
                    bgbottom:setAlpha(0)
                    bgtop:registerAnimationState("flip_end", {
                        color = color
                    })
                    bgtop:setAlpha(0.8)
                    bgtop:animateToState("flip_end", 0)
                end)

                timerstart:close()
                bg:animateInSequence({
                    {
                        "flip_start",
                        250,
                    },
                    {
                        "flip_end",
                        250
                    }
                })

                letter:animateInSequence({
                    {
                        "flip_start",
                        250,
                    },
                    {
                        "flip_end",
                        250
                    }
                })
            end)
        end

        box.dopulse = function()
            box:animateInSequence({
                {
                    "pulse_start",
                    100,
                },
                {
                    "pulse_end",
                    200
                }
            })
        end

        box.setcolor = function(box, color)
            bgtop:registerAnimationState("update", {
                color = color
            })
            bgtop:animateToState("update", 0)

            letter:registerAnimationState("update", {
                color = wordlecolors.white,
            })
            letter:animateToState("update", 0)
        end

        box.setletter = function(box, l)
            letter:setText(l:sub(1, 1):upper())
        end
            
        return box
    end

    local grid = LUI.UIVerticalList.new({
        topAnchor = true,
        leftAnchor = true,
        rightAnchor = true,
        bottomAnchor = true,
        spacing = spacing,
        alignment = LUI.Alignment.Center,
    })

    local createrow = function(id)
        local row = LUI.UIHorizontalList.new({
            topAnchor = true,
            leftAnchor = true,
            rightAnchor = true,
            height = boxsize,
            spacing = spacing,
            alignment = LUI.Alignment.Center,
        })

        row.id = "row" .. id
        row.boxes = {}

        for i = 1, maxcols do
            local box = createbox()
            row:addElement(box)
            table.insert(row.boxes, box)
        end

        row.setlettercolor = function(row, i, color)
            row.boxes[i]:setcolor(color)
        end

        row.setword = function(row, text)
            for i = 1, maxcols do
                if (i > #text) then
                    row.boxes[i]:setletter("")
                else
                    row.boxes[i]:setletter(text:sub(i, i))
                end
            end
        end

        return row
    end

    grid.rows = {}

    for i = 1, maxrows do
        local row = createrow(i)
        grid:addElement(row)
        table.insert(grid.rows, row)
    end

    title:setText(Engine.Localize("@MOTD_WORDLE_TITLE"))
    desc:setText(Engine.Localize("@MOTD_WORDLE_DESC", motd.getwordleid()))

    local gridcontainer = LUI.UIElement.new({
        topAnchor = true,
        leftAnchor = true,
        bottomAnchor = true,
        rightAnchor = true,
        top = headersize + spacing / 2 + padding * 1.5,
        left = padding,
        right = -padding,
    })

    gridcontainer:addElement(grid)
    
    widget:addElement(bg)
    widget:addElement(title)
    widget:addElement(desc)
    widget:addElement(scoretext)
    widget:addElement(splashtext)
    if (not motd.hassolvedwordle()) then
        widget:addElement(gridcontainer)
    else
        widget:addElement(info)
    end
    widget:addElement(LUI.DecoFrame.new(nil, LUI.DecoFrame.Grey))

    local calculateguess = function(word)
        local solution = motd.getwordlesolution()
        local colors = {}

        for i = 1, maxcols do
            table.insert(colors, 0)
        end
        
        local misplaced = {}

        for i = 1, maxcols do
            if (solution:sub(i, i) == word:sub(i, i)) then
                colors[i] = 1
            else
                table.insert(misplaced, solution:sub(i, i))
            end
        end

        local ismisplaced = function(l)
            for i = 1, #misplaced do
                if (misplaced[i] == l) then
                    return true
                end
            end
            return false
        end

        local popmisplaced = function(l)
            for i = 1, #misplaced do
                if (misplaced[i] == l) then
                    table.remove(misplaced, i)
                    break
                end
            end
        end

        for i = 1, maxcols do
            if (colors[i] ~= 1) then
                local letter = word:sub(i, i)
                if (ismisplaced(letter)) then
                    popmisplaced(letter)
                    colors[i] = 2
                else
                    colors[i] = 0
                end
            end
        end

        return colors
    end

    local isguessing = false
    local addguess = function(index, word, immediate)
        local colors = calculateguess(word)
        for i = 1, maxcols do
            local color = wordlecolors.grey
            if (colors[i] == 1) then
                color = wordlecolors.green
            elseif (colors[i] == 2) then
                color = wordlecolors.yellow
            end

            if (immediate) then
                widget.keyboard:setkeycolor(word:sub(i, i), color)
            end
            
            grid.rows[index].boxes[i]:doflip(color, (i - 1) * 500)
        end

        if (motd.getwordlesolution() == word) then
            state.completed = true
            state.result = true
            motd.solvewordle(state.result)
        end

        if (not immediate) then
            isguessing = true

            local timer = LUI.UITimer.new(2500, "finish_guess")
            widget:addElement(timer)
            widget:registerEventHandler("finish_guess", function()
                isguessing = false
                timer:close()
                setscore()

                for i = 1, maxcols do
                    local color = wordlecolors.grey
                    if (colors[i] == 1) then
                        color = wordlecolors.green
                    elseif (colors[i] == 2) then
                        color = wordlecolors.yellow
                    end

                    widget.keyboard:setkeycolor(word:sub(i, i), color)
                end
            end)
        end
    end

    if (not motd.hassolvedwordle()) then
        for i = 1, #state.guesses do
            grid.rows[i]:setword(state.guesses[i])
            addguess(i, state.guesses[i], true)
        end
    end

    setscore()

    local dosplash = function(text, delay, volatile)
        delay = delay or 0
        splashtext:setText(text)
        if (volatile) then
            splashtext:animateInSequence({
                {
                    "hide",
                    delay
                },
                {
                    "show",
                    0
                },
                {
                    "show",
                    1000
                },
                {
                    "hide",
                    500
                }
            })
        else
            splashtext:animateInSequence({
                {
                    "hide",
                    delay
                },
                {
                    "show",
                    0
                }
            })
        end
    end

    widget:registerEventHandler("keydown", function(element, event)
        if (event.key == "ESCAPE") then
	        LUI.FlowManager.RequestLeaveMenu(element)
        end

        if (state.completed or isguessing or motd.hassolvedwordle()) then
            return
        end

        if (#event.key == 1 and #state.currentword < maxcols) then
            state.currentword = string.lower(state.currentword .. event.key)
            grid.rows[state.currentrow].boxes[#state.currentword]:dopulse()
            Engine.PlaySound(CoD.SFX.MenuScroll)
        elseif (event.key == "BACKSPACE" and #state.currentword > 0) then
            state.currentword = state.currentword:sub(1, #state.currentword - 1)
            Engine.PlaySound(CoD.SFX.MenuScroll)
        end

        grid.rows[state.currentrow]:setword(state.currentword)

        if (event.key == "ENTER" and #state.currentword == maxcols) then
            if (motd.checkwordleword(state.currentword)) then
                local solution = motd.getwordlesolution()

                table.insert(state.guesses, state.currentword)
                addguess(state.currentrow, state.currentword)

                state.currentword = ""

                if (state.currentrow < maxrows and not state.completed) then
                    state.currentrow = state.currentrow + 1
                elseif (state.currentrow == maxrows and not state.completed) then
                    state.completed = true
                    state.result = false
                    motd.solvewordle(state.result)
                    dosplash(motd.getwordlesolution():upper(), 2500, false)
                    setscore()
                end
            else
                Engine.PlaySound(CoD.SFX.MenuAcceptDisabled)
            end
        end
    end)

    return root
end)
