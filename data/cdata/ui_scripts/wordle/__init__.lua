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

LUI.MenuBuilder.registerPopupType("wordle_widget", function(menu, controller)
    local headersize = 50
    local footersize = 30
    local padding = 10
    local size = 400
    local bordersize = 1
    local spacing = 5
    local boxsize = size / maxcols - spacing

    local width = padding + (boxsize + spacing) * maxcols
    local height = headersize + padding * 2 + footersize + (boxsize + spacing) * maxrows

    if (motd.hassolvedwordle()) then
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

    local widget = LUI.UIElement.new({
        topAnchor = true,
        leftAnchor = true,
        bottomAnchor = true,
        rightAnchor = true,
        left = (1280 - width) / 2,
        right = -(1280 - width) / 2,
        top = (720 - height) / 2,
        bottom = -(720 - height) / 2,
    })

    root:addElement(mainbg)
    root:addElement(widget)

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

    for i = 1, 6 do
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
    if (not motd.hassolvedwordle()) then
        widget:addElement(gridcontainer)
    else
        widget:addElement(info)
    end
    widget:addElement(LUI.DecoFrame.new(nil, LUI.DecoFrame.Grey))

    local addguess = function(index, word)
        local solution = motd.getwordlesolution()
        for i = 1, maxcols do
            local fullmatch = solution:sub(i, i) == word:sub(i, i)
            local partialmatch = solution:find(word:sub(i, i)) ~= nil

            if (fullmatch) then
                grid.rows[index].boxes[i]:doflip(wordlecolors.green, (i - 1) * 500)
            elseif (partialmatch) then
                grid.rows[index].boxes[i]:doflip(wordlecolors.yellow, (i - 1) * 500)
            else
                grid.rows[index].boxes[i]:doflip(wordlecolors.grey, (i - 1) * 500)
            end
        end
    end

    for i = 1, #state.guesses do
        grid.rows[i]:setword(state.guesses[i])
        addguess(i, state.guesses[i])
    end

    setscore()

    widget:registerEventHandler("keydown", function(element, event)
        if (event.key == "ESCAPE") then
	        LUI.FlowManager.RequestLeaveMenu(element)
        end

        if (state.completed) then
            return
        end

        if (#event.key == 1 and #state.currentword < maxcols) then
            state.currentword = string.lower(state.currentword .. event.key)
            grid.rows[state.currentrow].boxes[#state.currentword]:dopulse()
        elseif (event.key == "BACKSPACE" and #state.currentword > 0) then
            state.currentword = state.currentword:sub(1, #state.currentword - 1)
        end

        grid.rows[state.currentrow]:setword(state.currentword)

        if (event.key == "ENTER" and #state.currentword == maxcols) then
            if (motd.checkwordleword(state.currentword)) then
                local solution = motd.getwordlesolution()
                if (solution == state.currentword) then
                    state.completed = true
                    state.result = true
                    motd.solvewordle(state.result)
                    setscore()
                end
                
                table.insert(state.guesses, state.currentword)
                addguess(state.currentrow, state.currentword)

                state.currentword = ""

                if (state.currentrow < maxrows and not state.completed) then
                    state.currentrow = state.currentrow + 1
                elseif (state.currentrow == maxrows and not state.completed) then
                    state.completed = true
                    state.result = false
                    motd.solvewordle(state.result)
                    setscore()
                end
            end
        end
    end)

    return root
end)
