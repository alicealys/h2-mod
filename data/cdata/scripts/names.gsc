main()
{
    level.__pronouns__ = spawnstruct();
    level.__pronouns__.pronouns_first = [];
    level.__pronouns__.pronouns_second = [];
    level.__pronouns__.rand_iter = 1;
    level.__pronouns__.enabled = getdvarint("g_soldierPronouns");

    rand_iter_dvar = getdvarint("g_soldierPronounsRandIter");
    if (rand_iter_dvar != 0)
    {
        level.__pronouns__.rand_iter = rand_iter_dvar;
    }

    add_first_pronouns("she");
    add_first_pronouns("they");
    add_first_pronouns("it");
    add_first_pronouns("he");

    add_second_pronouns("her");
    add_second_pronouns("them");
    add_second_pronouns("its");
    add_second_pronouns("him");

    replacefunc(maps\_names::get_name, ::get_name);
}

add_first_pronouns(text)
{
    level.__pronouns__.pronouns_first[level.__pronouns__.pronouns_first.size] = text;
}

add_second_pronouns(text)
{
    level.__pronouns__.pronouns_second[level.__pronouns__.pronouns_second.size] = text;
}

random_value(rand, min, max)
{
    return rand % ((max - 1) + 1 - min) + min;
}

absolute_value(val)
{
    if (val < 0)
    {
        return val * -1;
    }

    return val;
}

next_rand(rand)
{
    return absolute_value(214013 * rand + 2531011);
}

get_random_pronouns(soldier_name)
{
    hash = hashstring(soldier_name);
    rand = absolute_value(hash);

    for (i = 0; i < level.__pronouns__.rand_iter; i++)
    {
        rand = next_rand(rand);
    }
    
    rand_1 = next_rand(rand);
    rand_2 = next_rand(rand_1);
    rand_3 = next_rand(rand_2);

    combined = random_value(rand_1, 0, 2) == 1;
    index_first = random_value(rand_2, 0, level.__pronouns__.pronouns_first.size);

    pronouns_first = level.__pronouns__.pronouns_first[index_first];
    pronouns_second = level.__pronouns__.pronouns_second[index_first];

    if (combined)
    {
        index_second = index_first;
        tries = 0;
        while (index_second == index_first && tries < 25)
        {
            index_second = random_value(rand_3, 0, level.__pronouns__.pronouns_first.size);
            ++tries;
        }

        pronouns_second = level.__pronouns__.pronouns_first[index_second];
    }

    return pronouns_first + "/" + pronouns_second;
}

format_name(soldier_name)
{
    if (!level.__pronouns__.enabled)
    {
        return soldier_name;
    }

    return soldier_name + " (" + get_random_pronouns(soldier_name) + ")";
}

get_name(var_0)
{
    if ( isdefined( self.team ) && self.team == "neutral" )
        return;

    if ( !isdefined( var_0 ) && level.script == "credits" )
    {
        self.airank = "private";
        return;
    }

    if ( isdefined( self.script_friendname ) )
    {
        if ( self.script_friendname == "none" )
            return;

        self.name = format_name(self.script_friendname);
        maps\_names::getrankfromname();
        self notify( "set name and rank" );
        return;
    }

    maps\_names::get_name_for_nationality( self.voice );
    self.name = format_name(self.name);
    self notify( "set name and rank" );
}
