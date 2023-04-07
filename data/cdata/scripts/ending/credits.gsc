#include maps\_endmission;

main()
{
    replacefunc(maps\_credit_h2_autogen::_id_B775, ::initiwcredits);

    // fix credits victory screen
    replacefunc(maps\_endmission::_nextmission, ::nextmission);
}

initcredits_h2mod()
{
    maps\_credits::addcenterimage("LOGO_H2MOD", 256, 64, 2);
    maps\_credits::addspace();
    maps\_credits::addcenterheading(&"CREDIT_H2MOD_DEVELOPERS");
    maps\_credits::addgap();
    maps\_credits::addcenternamedouble(&"CREDIT_H2MOD_FED", &"CREDIT_H2MOD_VLAD");
    maps\_credits::addcenternamedouble(&"CREDIT_H2MOD_FUTURE", &"CREDIT_H2MOD_NETADR");
    maps\_credits::addgap();
    maps\_credits::addcenterheading(&"CREDIT_H2MOD_SPECIAL_THANKS");
    maps\_credits::addgap();
    maps\_credits::addcenternamedouble(&"CREDIT_H2MOD_SNAKE", &"CREDIT_H2MOD_QUAK");
    maps\_credits::addcenternamedouble(&"CREDIT_H2MOD_XENSIK", &"CREDIT_H2MOD_JARIK");
}

initiwcredits()
{
    initcredits_h2mod();
    maps\_credit_h2_autogen::initiwcredits_iw();
    maps\_credit_h2_autogen::initiwcredits_beenox();
    maps\_credit_h2_autogen::initiwcredits_ca();
    maps\_credit_h2_autogen::initiwcredits_nerve();
    maps\_credit_h2_autogen::initiwcredits_irongalaxy();
}

nextmission(var_0)
{
    level notify("achievements_level_complete");

    if (level.script == "ending" && level._id_AC38 != "credits_1")
    {
        changelevel("");
    }
    else
    {
        if (!isdefined(var_0))
            var_0 = 0;

        level notify("nextmission");
        level.player notify("levelComplete");
        level.nextmission = 1;
        level.player enableinvulnerability();

        if (maps\_utility::arcademode())
        {
            level._id_AC81 = 1;
            thread _id_D36F::_id_AF95();
            common_scripts\utility::flag_wait("arcademode_ending_complete");
        }

        var_1 = undefined;
        setsaveddvar("ui_nextMission", "1");
        setdvar("ui_showPopup", "0");
        setdvar("ui_popupString", "");

        if (level.script == "ending")
        {
            level.script = "af_chase";
            nextmission(1);
            level.script = "ending";
        }

        game["previous_map"] = level.script;
        var_1 = level.missionsettings getlevelindex(level.script);

        if (!(level.script == "af_chase" && var_0) && !maps\_utility::achievement_conditions("IMMORTAL"))
        {
            _func_1FC(level.script, var_1);
            updategamerprofileall();
            maps\_endmission::check_immortal_achievement();
        }

        maps\_gameskill::auto_adust_zone_complete("aa_main_" + level.script);

        if (!isdefined(var_1))
        {
            missionsuccess(level.script);
            return;
        }

        if (level.script != "ending" && !(level.script == "af_chase" && var_0))
        {
            level.player thread maps\_hud_util::fade_out(1, undefined);
            soundscripts\_snd::snd_message("end_mission_fade_to_black");
            wait 2;
            maps\_utility::level_end_save();
        }

        if (level.script != "af_chase" || var_0)
        {
            level.missionsettings setlevelcompleted(var_1);
            var_3 = maps\_endmission::updatesppercent();
            updategamerprofile();

            if (level.missionsettings hasachievement(var_1))
                maps\_utility::giveachievement_wrapper(level.missionsettings getachievement(var_1));

            if (level.missionsettings haslevelveteranaward(var_1) && getlevelcompleted(var_1) == 4 && level.missionsettings check_other_haslevelveteranachievement(var_1))
                maps\_utility::giveachievement_wrapper(level.missionsettings getlevelveteranaward(var_1));

            if (level.missionsettings hasmissionhardenedaward() && level.missionsettings getlowestskill() > 2)
                maps\_utility::giveachievement_wrapper(level.missionsettings gethardenedaward());

            level.missionsettings maps\_endmission::_id_D1B9();
            var_4 = level.missionsettings.levels.size;
        }

        if (level.script == "af_chase" && var_0)
            return;

        if (level.script == "ending" && level._id_AC38 == "credits_1")
            common_scripts\utility::flag_wait("af_chase_nextmission");

        if (level.script == "airplane" || level.script == "ending")
        {
            if (level.credits_active)
            {
                level notify("credits_ended");
                credits_end();
            }

            setsaveddvar("ui_nextMission", "0");
            return;
        }
        else
        {
            var_4 = var_1 + 1;
        }

        if (getdvarint("limited_mode"))
        {
            setsaveddvar("ui_nextMission", "0");
            changelevel("");
            return;
        }

        if (maps\_utility::arcademode())
        {
            if (!getdvarint("arcademode_full"))
            {
                setsaveddvar("ui_nextMission", "0");
                changelevel("");
                return;
            }

            if (level.script == "cargoship")
            {
                changelevel("blackout", level.missionsettings getkeepweapons(var_1));
                return;
            }
            else if (level.script == "airlift")
            {
                changelevel("village_assault", level.missionsettings getkeepweapons(var_1));
                return;
            }
            else if (level.script == "jeepride")
            {
                changelevel("airplane", level.missionsettings getkeepweapons(var_1));
                return;
            }
        }

        if (level.missionsettings skipssuccess(var_1))
        {
            changelevel(level.missionsettings getlevelname(var_4), level.missionsettings getkeepweapons(var_1));
            return;
        }

        missionsuccess(level.missionsettings getlevelname(var_4), level.missionsettings getkeepweapons(var_1));
    }
}
