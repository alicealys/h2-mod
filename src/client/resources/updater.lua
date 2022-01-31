LUI.menucallbacks = {}
LUI.originalmenus = {}

LUI.onmenuopen = function(name, callback)
    if (not LUI.MenuBuilder.m_types_build[name]) then
        return
    end

    if (not LUI.menucallbacks[name]) then
        LUI.menucallbacks[name] = {}
    end

    luiglobals.table.insert(LUI.menucallbacks[name], callback)

    if (not LUI.originalmenus[name]) then
        LUI.originalmenus[name] = LUI.MenuBuilder.m_types_build[name]
        LUI.MenuBuilder.m_types_build[name] = function(...)
            local args = {...}
            local menu = LUI.originalmenus[name](table.unpack(args))

            for k, v in luiglobals.next, LUI.menucallbacks[name] do
                v(menu, table.unpack(args))
            end

            return menu
        end
    end
end

local addoptionstextinfo = LUI.Options.AddOptionTextInfo
LUI.Options.AddOptionTextInfo = function(menu)
    local result = addoptionstextinfo(menu)
    menu.optionTextInfo = result
    return result
end

LUI.addmenubutton = function(name, data)
    LUI.onmenuopen(name, function(menu)
        if (not menu.list) then
            return
        end

        local button = menu:AddButton(data.text, data.callback, nil, true, nil, {
            desc_text = data.description
        })

        local buttonlist = menu:getChildById(menu.type .. "_list")

        if (data.id) then
            button.id = data.id
        end

        if (data.index) then
            buttonlist:removeElement(button)
            buttonlist:insertElement(button, data.index)
        end

        local hintbox = menu.optionTextInfo
        menu:removeElement(hintbox)

        LUI.Options.InitScrollingList(menu.list, nil)
        menu.optionTextInfo = LUI.Options.AddOptionTextInfo(menu)
    end)
end

stack = {}
LUI.openmenu = function(menu, args)
    stack = args
    LUI.FlowManager.RequestAddMenu(nil, menu)
    return stack.ret
end

LUI.openpopupmenu = function(menu, args)
    stack = args
    LUI.FlowManager.RequestPopupMenu(nil, menu)
    return stack.ret
end

LUI.onmenuopen("main_lockout", function()
    if (game:sharedget("has_tried_updating") == "") then
        game:ontimeout(function()
            game:sharedset("has_tried_updating", "1")
            tryupdate(true)
        end, 0)
    end
end)

LUI.addmenubutton("pc_controls", {
    text = "$_CHECK FOR UPDATES",
    description = "Check for updates.",
    callback = function()
        tryupdate(false)
    end
})

stack = {}
LUI.MenuBuilder.m_types_build["generic_waiting_popup_"] = function (menu, event)
    local oncancel = stack.oncancel
	local popup = LUI.MenuBuilder.BuildRegisteredType("waiting_popup", {
		message_text = stack.text,
        isLiveWithCancel = true,
        cancel_func = function(...)
            local args = {...}
            oncancel()
            LUI.common_menus.CommonPopups.CancelCSSDownload(table.unpack(args))
        end
	})

    stack = {
        ret = popup
    }

    return popup
end

LUI.MenuBuilder.m_types_build["generic_yes_no_popup_"] = function()
    local callback = stack.callback
    local popup = LUI.MenuBuilder.BuildRegisteredType("generic_yesno_popup", {
		popup_title = stack.title,
		message_text = stack.text,
		yes_action = function()
            callback(true)
        end,
        no_action = function()
            callback(false)
        end
	})

    stack = {
        ret = popup
    }

    return popup
end

LUI.MenuBuilder.m_types_build["generic_confirmation_popup_"] = function()
    local popup = LUI.MenuBuilder.BuildRegisteredType( "generic_confirmation_popup", {
        cancel_will_close = false,
        popup_title = stack.title,
        message_text = stack.text,
        button_text = stack.buttontext,
        confirmation_action = stack.callback
    })

    stack = {
        ret = popup
    }

    return stack.ret
end

LUI.yesnopopup = function(data)
    for k, v in luiglobals.next, data do
        stack[k] = v
    end
    LUI.FlowManager.RequestPopupMenu(nil, "generic_yes_no_popup_")
    return stack.ret
end

LUI.confirmationpopup = function(data)
    for k, v in luiglobals.next, data do
        stack[k] = v
    end
    LUI.FlowManager.RequestPopupMenu(nil, "generic_confirmation_popup_")
    return stack.ret
end

function updaterpopup(oncancel)
    return LUI.openpopupmenu("generic_waiting_popup_", {
        oncancel = oncancel,
        withcancel = true,
        text = "Checking for updates..."
    })
end

function verifyfiles(files)
    local needed = {}
    local needtoupdatebinary = false
    local binaryname = game:binaryname()

    for i = 1, #files do
        local name = files[i][1]

        if (not io.fileexists(name) or game:sha(io.readfile(name)) ~= files[i][3]) then
            if (name == game:binaryname()) then
                needtoupdatebinary = true
            end

            table.insert(needed, files[i])
        end
    end

    return needed, needtoupdatebinary
end

local canceled = false

function downloadfiles(popup, files, callback)
    local text = popup:getchildren()[7]
    local folder = game:environment() == "develop" and "data-dev" or "data"

    if (#files == 0) then
        callback(true)
        return
    end

    local total = 0
    local filedownloaded = function()
        total = total + 1
        if (total == #files) then
            callback(true)
        end
    end

    local download = nil
    local stop = false
    download = function(index)
        if (canceled or stop) then
            return
        end

        local filename = files[index][1]

        local url = "https://master.fed0001.xyz/" .. folder .. "/" .. filename
        text:setText(string.format("Downloading file [%i/%i]\n%s", index, #files, filename))

        if (filename == game:binaryname()) then
            io.movefile(filename, filename .. ".old")
        end

        httprequesttofile(url, filename, function(valid, success)
            if (not valid) then
                callback(false, "Invalid server response")
                stop = true
                return
            end

            if (not success) then
                callback(false, "Failed to write file " .. filename)
                stop = true
                return
            end

            filedownloaded()

            if (files[index + 1]) then
                download(index + 1)
            else
                callback(true)
            end
        end)
    end

    download(1)
end

function userdata_:getchildren()
    local children = {}
    local first = self:getFirstChild()

    while (first) do
        table.insert(children, first)
        first = first:getNextSibling()
    end

    return children
end

function tryupdate(autoclose)
    canceled = false
    local popup = updaterpopup(function()
        canceled = true
    end)

    local text = popup:getchildren()[7]
    local file = game:environment() == "develop" and "files-dev.json" or "files.json"

    httprequest("https://master.fed0001.xyz/" .. file, function(valid, data)
        if (not valid) then
            text:setText("Update check failed: Invalid server response")
            return
        end

        local valid = pcall(function()
            local files = json.decode(data)
            local needed, updatebinary = verifyfiles(files)

            if (#needed == 0) then
                text:setText("No updates available")
                if (autoclose) then
                    LUI.common_menus.CommonPopups.CancelCSSDownload(popup, {})
                end
                return
            end

            local download = function()
                local gotresult = false
                downloadfiles(popup, needed, function(result, error)
                    if (gotresult or canceled) then
                        return
                    end
    
                    gotresult = true

                    if (not result) then
                        text:setText("Update failed: " .. error)
                        return
                    end

                    if (updatebinary) then
                        LUI.confirmationpopup({
                            title = "RESTART REQUIRED",
                            text = "Update requires restart, relaunch now?",
                            buttontext = "RESTART",
                            callback = function()
                                game:relaunch()
                            end
                        })
                        return
                    end
    
                    if (result and #needed > 0) then
                        game:executecommand("lui_restart")
                        return
                    end
    
                    text:setText("Update successful!")
    
                    if (autoclose) then
                        LUI.common_menus.CommonPopups.CancelCSSDownload(popup, {})
                    end
                end)
            end

            if (autoclose) then
                download()
            else
                LUI.yesnopopup({
                    title = "NOTICE",
                    text = "An update is available, proceed with installation?",
                    callback = function(result)
                        if (not result) then
                            LUI.common_menus.CommonPopups.CancelCSSDownload(popup, {})
                        else
                            download()
                        end
                    end
                })
            end
        end)

        if (not valid) then
            text:setText("Update failed: unknown error")
        end
    end)
end

LUI.tryupdating = function(autoclose)
    tryupdate(autoclose)
end

function httprequest(url, callback)
    local request = game:httpget(url)
    local listener = nil
    listener = game:onnotify("http_request_done", function(id, valid, data)
        if (id ~= request) then
            return
        end

        listener:clear()
        callback(valid, data)
    end)
end

function httprequesttofile(url, dest, callback)
    local request = game:httpgettofile(url, dest)
    local listener = nil
    listener = game:onnotify("http_request_done", function(id, valid, success)
        if (id ~= request) then
            return
        end

        listener:clear()
        callback(valid, success)
    end)
end

local localize = Engine.Localize
Engine.Localize = function(...)
    local args = {...}

    if (type(args[1]) == "string" and args[1]:sub(1, 2) == "$_") then
        return args[1]:sub(3, -1)
    end

    return localize(table.unpack(args))
end