main()
{
    replacefunc(maps\gulag_code::surprise, ::surprise);
}

surprise()
{
    level endon("cancel_surprise");
    tv = getent("surprise_tv", "script_noteworthy");
    tv thread maps\gulag_code::surprise_tv();
    common_scripts\utility::flag_wait("player_nears_cell_door3");

    maps\gulag_code::surprise_damage_trigger();
    maps\gulag_code::surprise_flag_touching();

    tv thread maps\_utility::play_sound_on_entity("h2_zombie_easter_egg");
    tv.screen = spawn("script_model", tv.origin);
    tv.screen.angles = tv.angles;
    tv.screen setmodel("com_tv1_pho_zombie");

    wait 3.3;
    tv.screen delete();

    maps\_utility::giveachievement_wrapper("BRAINS");
}