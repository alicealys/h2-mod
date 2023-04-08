if (Engine.InFrontend()) then
    return
end

local hud = LUI.UIElement.new({
    topAnchor = true,
    leftAnchor = true,
    width = 500,
    height = 500,
    left = 0
})

local hidetimer = LUI.UITimer.new(5000, "hide_toast")
local finishtimer = LUI.UITimer.new(6000, "notification_finished")
local animtime = 250

hud:setPriority(1000)

local isshowingachievement = false
local notificationqueue = {}
local shownotificationinternal = nil

local shownextnotification = function()
    isshowingachievement = false
    local data = notificationqueue[1]
    if (data == nil) then
        return
    end

    table.remove(notificationqueue, 1)
    shownotificationinternal(data)
end

local createtoast = function()
    local height = 80
    local width = 400
    local padding = 10
    local leftoffset = 10

    local titlefont = CoD.TextSettings.Font27
    local descfont = CoD.TextSettings.Font21

    local imagesize = height - padding * 2
    local textoffset = padding * 2 + imagesize
    local textyoffset = height / 2 - (titlefont.Height + descfont.Height) / 2
    local text2yoffset = 5

    local container = LUI.UIElement.new({
        topAnchor = true,
        leftAnchor = true,
        top = 100,
        left = 0,
        width = width,
        height = height
    })

    container:registerAnimationState("hide", {
        topAnchor = true,
        leftAnchor = true,
        top = 100,
        left = -1000,
        width = width,
        height = height
    })

    container:registerAnimationState("show", {
        topAnchor = true,
        leftAnchor = true,
        top = 100,
        left = 0,
        width = width,
        height = height
    })

    container:registerAnimationState("show_right", {
        topAnchor = true,
        leftAnchor = true,
        top = 100,
        left = 40,
        width = width,
        height = height
    })

    local bg = LUI.UIImage.new({
        topAnchor = true,
        leftAnchor = true,
        rightAnchor = true,
        bottomAnchor = true,
        color = {
            r = 0.1,
            g = 0.1,
            b = 0.1,
        },
        alpha = 0.55
    })

    local image = LUI.UIImage.new({
        topAnchor = true,
        leftAnchor = true,
        top = padding,
        left = padding + leftoffset,
        width = imagesize,
        height = imagesize,
    })

    local title = LUI.UIText.new({
        topAnchor = true,
        leftAnchor = true,
        top = textyoffset,
        left = textoffset + leftoffset,
        font = titlefont.Font,
        height = titlefont.Height
    })

    local desc = LUI.UIText.new({
        topAnchor = true,
        leftAnchor = true,
        width = width - textoffset - leftoffset - padding,
        alignment = LUI.Alignment.Left,
        top = textyoffset + titlefont.Height + text2yoffset,
        left = textoffset + leftoffset,
        font = descfont.Font,
        height = descfont.Height
    })

    container:addElement(bg)
    container:addElement(image)
    container:addElement(title)
    container:addElement(desc)
    container:addElement(LUI.DecoFrame.new(nil, LUI.DecoFrame.Grey))

    container:animateToState("hide")

    container:addElement(hidetimer)
    container:addElement(finishtimer)

    container:registerEventHandler("hide_toast", function()
        LUI.UITimer.Stop(hidetimer)
        container:animateToState("hide", animtime)
    end)

    container:registerEventHandler("notification_finished", function()
        LUI.UITimer.Stop(finishtimer)
        shownextnotification()
    end)

    hud:addElement(container)

    container.image = image
    container.title = title
    container.desc = desc

    return container
end

local toast = createtoast()

shownotificationinternal = function(data)
    isshowingachievement = true
    
    LUI.UITimer.Reset(hidetimer)
    LUI.UITimer.Reset(finishtimer)

    toast:animateToState("hide")
    toast:animateToState("show", animtime)

    toast.title:setText(data.title)
    toast.desc:setText(data.description)
    toast.image:setImage(data.image)

    if (data.sound) then
        Engine.PlaySound(data.sound)
    end
end

function addnotification(data)
    table.insert(notificationqueue, data)
    if (not isshowingachievement) then
        shownextnotification()
    end
end

local addachievementnotification = function(id)
    addnotification({
        title = Engine.Localize("ACHIEVEMENT_EARNED"),
        description = Engine.ToUpperCase(Engine.Localize("ACHIEVEMENT_NAME_" .. id)),
        image = RegisterMaterial("trophy_" .. id),
        sound = "ui_achievement_unlocked"
    })
end

hud:registerEventHandler("earned_achievement", function(element, event)
    addachievementnotification(event.id)
end)

LUI.roots.UIRoot0:addElement(hud)