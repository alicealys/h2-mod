function element:animate(name, state, animationtime)
    local start = {
        x = self.x,
        y = self.y,
        w = self.w,
        h = self.h,
        color = self.color,
        backcolor = self.backcolor,
        bordercolor = self.bordercolor,
        borderwidth = self.borderwidth,
        fontsize = self.fontsize
    }

    local _end = {}
    for k, v in pairs(start) do
        _end[k] = state[k] or v
    end

    local diffs = {}
    for k, v in pairs(_end) do
        if (type(v) == "table") then
            local value = {}
            local different = false

            for _k, _v in pairs(v) do
                value[_k] = _v - start[k][_k]
                if (value[_k] ~= 0) then
                    different = true
                end
            end

            if (different) then
                diffs[k] = value
            end
        else
            local value = v - start[k]
            if (value ~= 0) then
                diffs[k] = v - start[k]
            end
        end
    end

    local timeout = nil
    local interval = nil
    local starttime = game:time()

    interval = game:onframe(function()
        local time = game:time()
        local percentage = (time - starttime) / animationtime

        if (percentage >= 1) then
            for k, v in pairs(diffs) do
                self[k] = _end[k]
            end
        else
            for k, v in pairs(diffs) do
                if (type(v) == "table") then
                    local value = {}
    
                    for _k, _v in pairs(v) do
                        value[_k] = start[k][_k] + _v * percentage
                    end
    
                    self[k] = value
                else
                    self[k] = start[k] + v * percentage
                end
            end
        end
    end)

    timeout = game:ontimeout(function()
        interval:clear()
        for k, v in pairs(diffs) do
            self[k] = _end[k]
        end
    end, animationtime)

    self:onnotifyonce("cancel_animation", function(_name)
        if (name == _name) then
            timeout:clear()
            interval:clear()
        end
    end)
end

function element:cancelanimations(name, callback)
    self:notify("cancel_animation", name)
    if (type(callback) == "function") then
        game:ontimeout(callback, 0)
    end
end