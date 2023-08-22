local function GetVolume()
	return (Engine.GetProfileData("snd_volume") - SliderBounds.Volume.Min) / (SliderBounds.Volume.Max - SliderBounds.Volume.Min)
end

local function VolumeLess(a1)
	LUI.Options.ProfileDataMenuOption(a1, SliderBounds.Volume.Min, SliderBounds.Volume.Max, -SliderBounds.Volume.Step, "snd_volume")
end

local function VolumeMore(a1)
	LUI.Options.ProfileDataMenuOption(a1, SliderBounds.Volume.Min, SliderBounds.Volume.Max, SliderBounds.Volume.Step, "snd_volume")
end

local function dvarmenuoption(name, min, max, step)
    Engine.SetDvarFloat(name, math.min(max, math.max(min, Engine.GetDvarFloat(name) + step)))
end

local function getvolumefunc(name)
    return function()
        return (Engine.GetDvarFloat(name) - SliderBounds.Volume.Min) / (SliderBounds.Volume.Max - SliderBounds.Volume.Min)
    end
end

local function getvolumelessfunc(name)
    return function()
        dvarmenuoption(name, SliderBounds.Volume.Min, SliderBounds.Volume.Max, -SliderBounds.Volume.Step)
    end
end

local function getvolumemorefunc(name)
    return function()
        dvarmenuoption(name, SliderBounds.Volume.Min, SliderBounds.Volume.Max, SliderBounds.Volume.Step)
    end
end

LUI.PCAudio.new = function (a1)
	local menu = LUI.MenuTemplate.new(a1, {
		menu_title = Engine.ToUpperCase(Engine.Localize("@LUA_MENU_AUDIO_OPTIONS")),
		menu_top_indent = Engine.IsMultiplayer() and 0 or LUI.MenuTemplate.spMenuOffset,
		menu_width = GenericMenuDims.OptionMenuWidth
	})

	LUI.Options.AddButtonOptionVariant(
        menu, 
        GenericButtonSettings.Variants.Slider, 
        "@MENU_MASTER_VOLUME", 
        "@MENU_MASTER_VOLUME_DESC", 
        GetVolume, 
        VolumeLess, 
        VolumeMore
    )

    LUI.Options.AddButtonOptionVariant(
        menu, 
        GenericButtonSettings.Variants.Slider, 
        "@MENU_MUSIC_VOLUME", 
        "@MENU_MUSIC_VOLUME_DESC", 
        getvolumefunc("snd_musicVolume"), 
        getvolumelessfunc("snd_musicVolume"), 
        getvolumemorefunc("snd_musicVolume")
    )

	local speakeroptions = {
		{
			text = "@LUA_MENU_AUTODETECT_SPEAKERS",
			value = 0
		}
	}

	local speakerconfig = Engine.GetDvarInt("snd_detectedSpeakerConfig")
	if speakerconfig > 1 then
		speakeroptions[#speakeroptions + 1] = {
			text = "@LUA_MENU_STEREO",
			value = 2
		}
		speakeroptions[#speakeroptions + 1] = {
			text = "@LUA_MENU_HEADPHONE",
			value = 50
		}
	end

	if speakerconfig > 2 then
		speakeroptions[#speakeroptions + 1] = {
			text = "@LUA_MENU_FOUR_SPEAKERS",
			value = 4
		}
	end

	if speakerconfig > 4 then
		speakeroptions[#speakeroptions + 1] = {
			text = "@LUA_MENU_FIVE_ONE_SPEAKERS",
			value = 6
		}
	end

	if speakerconfig > 6 then
		speakeroptions[#speakeroptions + 1] = {
			text = "@LUA_MENU_SEVEN_ONE_SPEAKERS",
			value = 8
		}
	end

	LUI.Options.CreateOptionButton(
        menu, 
        "snd_speakerConfig", 
        "@LUA_MENU_SPEAKER_CONFIG", 
        "@LUA_MENU_SPEAKER_CONFIG_DESC", 
        speakeroptions
    )

	if Engine.IsMultiplayer() then
		LUI.Options.AddButtonOptionVariant(
            menu, 
            GenericButtonSettings.Variants.Select, 
            "@LUA_MENU_MUSIC", 
            "@LUA_MENU_MUSIC_DESC",
            LUI.Options.GetDvarEnableTextFunc("snd_musicDisabled", true), 
            LUI.Options.ToggleDvarFunc("snd_musicDisabled"), 
            LUI.Options.ToggleDvarFunc("snd_musicDisabled")
        )

		LUI.Options.AddButtonOptionVariant(
            menu, 
            GenericButtonSettings.Variants.Select, 
            "@LUA_MENU_ANNOUNCER", 
            "@LUA_MENU_ANNOUNCER_DESC", 
            LUI.Options.GetDvarEnableTextFunc("snd_announcerDisabled", true), 
            LUI.Options.ToggleDvarFunc("snd_announcerDisabled"), 
            LUI.Options.ToggleDvarFunc("snd_announcerDisabled")
        )

		LUI.Options.AddButtonOptionVariant(
            menu, 
            GenericButtonSettings.Variants.Select, 
            "@LUA_MENU_BATTLECHATTER", 
            "@LUA_MENU_BATTLECHATTER_DESC", 
            LUI.Options.GetDvarEnableTextFunc("snd_battleChatterDisabled", true),
            LUI.Options.ToggleDvarFunc("snd_battleChatterDisabled"), 
            LUI.Options.ToggleDvarFunc("snd_battleChatterDisabled")
        )

		LUI.Options.AddButtonOptionVariant(
            menu, 
            GenericButtonSettings.Variants.Select, 
            "@LUA_MENU_PUSH_TO_TALK", 
            "@LUA_MENU_PUSH_TO_TALK_DESC", 
            LUI.Options.GetDvarEnableTextFunc("cl_pushToTalk", false), 
            LUI.Options.ToggleDvarFunc("cl_pushToTalk"), 
            LUI.Options.ToggleDvarFunc("cl_pushToTalk")
        )
	end

	if not Engine.IsMultiplayer() and Engine.ShouldShowSubtitlesOption() then
		LUI.Options.CreateControlProfileDataButton(
            menu, 
            "subtitles", 
            "profile_toggleSubtitles", 
            nil, 
            "LUA_MENU_SUBTITLES", 
            "PLATFORM_UI_OPTIONS_SUBTITLES_DESC", 
            {
                {
                    text = "@LUA_MENU_DISABLED",
                    value = false
                },
                {
                    text = "@LUA_MENU_ENABLED",
                    value = true
                }
		    }
        )
	end

	LUI.Options.AddButtonOptionVariant(
        menu,
        GenericButtonSettings.Variants.Select, 
        "@LUA_MENU_HITSOUND", 
        "@LUA_MENU_HITSOUND_DESC", 
        LUI.Options.GetDvarEnableTextFunc("snd_hitsoundDisabled", true), 
        LUI.Options.ToggleDvarFunc("snd_hitsoundDisabled"), 
        LUI.Options.ToggleDvarFunc("snd_hitsoundDisabled")
    )
	
    if Engine.IsPC() and Engine.GetDvarBool("snd_wasapiSampleRateConverterNeeded") then
		LUI.Options.AddButtonOptionVariant(
            menu, 
            GenericButtonSettings.Variants.Select, 
            "@LUA_MENU_LOWQUALITYAUDIO", 
            "@LUA_MENU_LOWQUALITYAUDIO_DESC",
            LUI.Options.GetDvarEnableTextFunc("snd_lowQualityAudio", false), 
            LUI.Options.ToggleDvarFunc("snd_lowQualityAudio"), 
            LUI.Options.ToggleDvarFunc("snd_lowQualityAudio")
        )
	end

	LUI.Options.InitScrollingList(menu.list, nil)
	LUI.Options.AddOptionTextInfo(menu)

	menu:AddBackButton(function(a1, a2)
		Engine.ExecNow("profile_menuDvarsFinish", a2.controller)
		Engine.Exec("updategamerprofile")
		LUI.FlowManager.RequestLeaveMenu(a1)
	end)

	return menu
end

LUI.MenuBuilder.m_types_build["pc_audio"] = LUI.PCAudio.new
