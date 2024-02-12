main()
{
    replacefunc(maps\char_museum::ramirez_jukebox, ::ramirez_jukebox);
}

ramirez_jukebox()
{
    common_scripts\utility::flag_wait("museum_ready");
    wait 4;

    jukebox = getent("ramirez_jukebox", "targetname");
    jukebox makeusable();

    for (;;)
    {
        jukebox waittill("trigger");
        index = randomint(37);
        jukebox maps\_utility::play_sound_on_entity("museum_ramirez_ee_" + index);
        maps\_utility::giveachievement_wrapper("RAMIREZ");
    }
}
