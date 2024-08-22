main()
{
    level.__pronouns__ = spawnstruct();
    level.__pronouns__.pronouns_first = [];
    level.__pronouns__.pronouns_second = [];

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

get_random_pronouns()
{
    combined = randomintrange(0, 2) == 1;

    index_first = randomintrange(0, level.__pronouns__.pronouns_first.size);

    pronouns_first = level.__pronouns__.pronouns_first[index_first];
    pronouns_second = level.__pronouns__.pronouns_second[index_first];

    if (combined)
    {
        index_second = index_first;
        while (index_second == index_first)
        {
            index_second = randomintrange(0, level.__pronouns__.pronouns_first.size); 
        }

        pronouns_second = level.__pronouns__.pronouns_first[index_second];
    }
    
    return pronouns_first + "/" + pronouns_second;
}

get_name(var_0)
{
    pronouns = " (" + get_random_pronouns() + ")";

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

        self.name = self.script_friendname+ pronouns;
        maps\_names::getrankfromname( self.name );
        self notify( "set name and rank" );
        return;
    }

    maps\_names::get_name_for_nationality( self.voice );
    self.name = self.name + pronouns;
    self notify( "set name and rank" );
}
