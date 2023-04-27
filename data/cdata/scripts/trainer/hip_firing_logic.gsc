#include maps\trainer;
#include maps\trainer_anim;

main()
{
    replacefunc(maps\trainer_anim::anims, ::anims);
    replacefunc(maps\trainer::_id_B2CB, ::firing_range_hip_and_ads);
    replacefunc(maps\trainer::_id_D20A, ::foley_talk_animations);
}


player_firing_count()
{
	self endon ("death");
	level endon ("firing_count_ended");

	self.firing_count = 0;
	
	while (1)
	{
		self waittill( "weapon_fired" );
		self.firing_count++;

		if(self.firing_count > 2 && !common_scripts\utility::flag( "firing_count_ended" ) )
		{
			common_scripts\utility::flag_set("firing_count_ended");
			level notify ("firing_count_ended");
		}
	}
}

firing_range_hip_and_ads()
{
    common_scripts\utility::flag_wait( "firing_range_initialized" );
    var_0 = 1;
    thread _id_D20A();
    common_scripts\utility::flag_wait( "ps_train_fly_welcome" );
    soundscripts\_snd::snd_message( "aud_start_mix_practice_range" );
    common_scripts\utility::flag_wait( "foley_anim_pickup_weapon" );

    //Added 04.26.23 (12:50 PM)
    common_scripts\utility::flag_init( "firing_count_ended" );
    level.player thread player_firing_count();

    thread firing_range_player_pick_up_gun();

    if ( !_id_D39F() )
    {
        _id_BD26( "obj_rifle", &"TRAINER_PICK_UP_A_RIFLE_FROM", getent( "range_rifle", "script_noteworthy" ) );
        _id_C7C7( "obj_rifle", "current" );
        var_1 = getent( "pickup_rifle", "targetname" );

        if ( isdefined( var_1 ) )
            var_1 maps\_utility::glow( "h2_weapon_m4_grunt_obj" );

        level.foley thread _id_D25D( "nag_rifle_pickup_0", 2, "player_picked_up_rifle" );
    }
    else
        var_0 = 0;

    while ( !_id_D39F() )
        wait 0.05;

    common_scripts\utility::flag_set( "player_picked_up_rifle" );
    maps\_utility::ui_set_waypoint_center_fade_settings( 2000, 0, 0 );

    if ( var_0 == 0 )
    {
        _id_BD26( "obj_rifle", &"TRAINER_SHOOT_EACH_TARGET_WHILE1", getent( "firing_range", "targetname" ) );
        _id_C7C7( "obj_rifle", "current" );
    }
    else
    {
        _id_B2F2( "obj_rifle", &"TRAINER_SHOOT_EACH_TARGET_WHILE1" );
        _id_D393( "obj_rifle", getent( "firing_range", "targetname" ) );
    }

    maps\_utility::add_wait( maps\_utility::_wait, 2.0 );
    maps\_utility::add_abort( common_scripts\utility::flag_wait, "player_facing_targets_for_hip_fire" );
    level.foley maps\_utility::add_func( maps\_utility::dialogue_queue, "train_fly_turnaround" );
    thread maps\_utility::do_wait();
    thread maps\_utility::delaythread( 8, ::_id_C1DE, "player_facing_targets_for_hip_fire" );

    while ( !common_scripts\utility::within_fov( level.player.origin, level.player getplayerangles(), level._id_D0AB.origin, level.cosine["45"] ) )
        wait 0.05;

    common_scripts\utility::flag_set( "player_facing_targets_for_hip_fire" );
    clear_hints();
    level.foley thread _id_C90F( "nag_hip_fire_0", 2, "player_needs_to_fire_from_the_hip", "hip_fire_done" );

    if ( level.xenon )
        thread _id_CED3( "hip_attack", undefined );
    else
        thread _id_CED3( "pc_hip_attack", undefined );

    var_2 = 2;
    var_3 = 0;
    _id_C5CB( level._id_B03E, var_2, var_3, undefined, undefined, 1 );
    level.player notify( "did_action_stop_ads" );
    clear_hints();
    var_4 = undefined;

    if ( level.player common_scripts\utility::is_player_gamepad_enabled() )
        thread _id_CED3( "look_inversion", 5 );
    else
        thread _id_CED3( "look_inversion_pc", 5 );

    soundscripts\_snd::snd_message( "aud_start_mix_info_popup" );
    _id_C7C7( "obj_rifle", "done" );
    common_scripts\utility::flag_set( "hip_fire_done" );
    level.foley waittill( "train_fly_pickyourtargets" );
    thread maps\_utility::autosave_by_name( "ads_training" );
    level.foley waittill( "train_fly_crouchfirst" );
    _id_C7C7( "obj_rifle", "current" );
    thread _id_B2F2( "obj_rifle", &"TRAINER_SHOOT_EACH_TARGET_WHILE" );
    var_2 = 3;
    var_3 = 1;
    var_5 = 1;

    if ( level.player getstance() != "crouch" )
        thread _id_CED3( "crouch" );

    level.foley thread _id_C90F( "nag_ads_fire_0", 2, "player_needs_to_ADS", "firing_range_hip_and_ads_done" );
    level.foley thread _id_C90F( "nag_crouch_fire_0", 2, "player_needs_to_crouch", "firing_range_hip_and_ads_done" );
    _id_C5CB( level._id_C5CA, var_2, var_3, var_5 );
    level.player notify( "did_action_crouch" );
    level.player notify( "did_action_ads_360" );
    level.player notify( "did_action_ads" );
    clear_hints();
    _id_C7C7( "obj_rifle", "done" );
    common_scripts\utility::flag_set( "firing_range_hip_and_ads_done" );

    if ( level.player getstance() != "stand" )
        thread _id_CED3( "stand" );
}

foley_talk_animations()
{
    level._id_D3E4 thread firing_range_anim_then_idle( level._id_CB75, "training_intro_begining", "training_intro_idle2" );
    level.foley.animnode firing_range_anim_then_idle( level.foley, "training_intro_begining", "training_intro_idle3" );
    common_scripts\utility::flag_wait( "hip_fire_done" );

    level notify ("firing_count_ended");

    if(common_scripts\utility::flag("firing_count_ended"))
    {
        maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk2", "train_fly_sprayedbullets" );
        maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk2", "train_fly_pickyourtargets", 1 );
        maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk2", "train_fly_howtherangers" );
        maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk2", "train_fly_crouchfirst", 1 );
        maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk2", "train_aft_sprayedbullets" );
        maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk2", "train_aft_pickyourtargets" );
        maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk2", "train_aft_howtherangers" );
    }
    else
    {
        maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk2", "train_fly_good");
        maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk2", "train_fly_pickyourtargets", 1);
        maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk2", "train_fly_howtherangers" );
        maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk2", "train_fly_crouchfirst", 1 );
        maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk2", "null" );
        maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk2", "train_aft_pickyourtargets" );
        maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk2", "train_aft_howtherangers" );
    }

    wait 0.5;

    level._id_D3E4 thread firing_range_anim_then_idle( level._id_CB75, "training_intro_talk2", "training_intro_idle" );
    level.foley.animnode firing_range_anim_then_idle( level.foley, "training_intro_talk2", "training_intro_idle2" );
    common_scripts\utility::flag_wait( "firing_range_hip_and_ads_done" );

    level._id_D3E4 thread firing_range_anim_then_idle( level._id_CB75, "training_intro_talk3", "training_intro_idle" );
    level.foley.animnode maps\_anim::anim_single_solo( level.foley, "training_intro_talk3" );
    common_scripts\utility::flag_set( "foley_done_talking_from_hip_ads_training" );
}

#using_animtree("generic_human");
anims()
{
    maps\_props::add_smoking_notetracks( "generic" );
    level.scr_anim["foley"]["training_intro_begining"] = %h2_training_intro_foley_begining;
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_begining", "train_fly_welcome" );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_begining", "train_fly_demonstration" );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_begining", "train_fly_nooffense" );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_begining", "train_fly_makesyoulook" );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_begining", "train_fly_showem" );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_begining", "train_fly_fireattargets" );
    maps\_anim::addnotetrack_flag( "foley", "activate_audiomix", "ps_train_fly_welcome", "training_intro_begining" );
    maps\_anim::addnotetrack_flag( "foley", "pickup", "foley_anim_pickup_weapon", "training_intro_begining" );
    level.scr_anim["translator"]["training_intro_begining"] = %h2_training_intro_translator_begining;
    maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_begining", "train_aft_demonstration" );
    maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_begining", "train_aft_makesyoulook" );
    level.scr_anim["foley"]["training_intro_idle"][0] = %h2_training_intro_foley_idle;
    level.scr_anim["foley"]["training_intro_idle2"][0] = %h2_training_intro_foley_idle2;
    level.scr_anim["foley"]["training_intro_idle3"][0] = %h2_training_intro_foley_idle3;
    level.scr_anim["translator"]["training_intro_idle"][0] = %h2_training_intro_translator_idle;
    level.scr_anim["translator"]["training_intro_idle2"][0] = %h2_training_intro_translator_idle2;
    level.scr_anim["trainee_01"]["training_intro_idle"][0] = %h2_training_intro_trainee_1_idle;
    level.scr_anim["foley"]["training_intro_talk2"] = %h2_training_intro_foley_talk2;
    /* maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk2", "train_fly_sprayedbullets" );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk2", "train_fly_pickyourtargets", 1 );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk2", "train_fly_howtherangers" );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk2", "train_fly_crouchfirst", 1 ); */
    level.scr_anim["foley"]["training_intro_talk3"] = %h2_training_intro_foley_talk3;
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk3", "train_fly_gottaaim" );
    level.scr_anim["foley"]["training_intro_talk4"] = %h2_training_intro_foley_talk4;
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk4", "train_fly_switching" );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk4", "train_fly_popinandout" );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk4", "train_fly_showemprivate", 1 );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk4", "train_fly_iftargetclose", 1 );
    level.scr_anim["foley"]["training_intro_talk5"] = %h2_training_intro_foley_talk5;
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk5", "train_fly_howyoudoit" );
    level.scr_anim["foley"]["training_intro_talk6"] = %h2_training_intro_foley_talk6;
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk6", "train_fly_lightcover" );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk6", "train_fly_theprivatehere", 1 );
    level.scr_anim["foley"]["training_intro_talk7"] = %h2_training_intro_foley_talk7;
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk7", "train_fly_tossafrag" );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk7", "train_fly_pickupfrag" );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk7", "train_fly_grenadedownrange", 1 );
    level.scr_anim["foley"]["training_intro_talk8"] = %h2_training_intro_foley_talk8;
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk8", "train_fly_good" );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_talk8", "train_fly_fragstendtoroll", 1 );
    level.scr_anim["foley"]["training_intro_transition_talk3to6"] = %h2_training_intro_foley_transition;
    level.scr_anim["translator"]["training_intro_talk2"] = %h2_training_intro_translator_talk2;
    /* maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk2", "train_aft_sprayedbullets" );
    maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk2", "train_aft_pickyourtargets" );
    maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk2", "train_aft_howtherangers" ); */
    level.scr_anim["translator"]["training_intro_talk3"] = %h2_training_intro_translator_talk3;
    maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk3", "train_aft_crouchfirst" );
    level.scr_anim["translator"]["training_intro_talk4"] = %h2_training_intro_translator_talk4;
    maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk4", "train_aft_switching" );
    maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk4", "train_aft_popinandout" );
    level.scr_anim["translator"]["training_intro_talk5"] = %h2_training_intro_translator_talk5;
    level.scr_anim["translator"]["training_intro_talk6"] = %h2_training_intro_translator_talk6;
    maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk6", "train_aft_lightcover" );
    level.scr_anim["translator"]["training_intro_talk7"] = %h2_training_intro_translator_talk7;
    maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk7", "train_aft_tossafrag" );
    maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk7", "train_aft_grenadedownrange" );
    level.scr_anim["translator"]["training_intro_talk8"] = %h2_training_intro_translator_talk8;
    maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_talk8", "train_aft_fragstendtoroll" );
    level.scr_anim["translator"]["training_intro_transition_talk3to6"] = %h2_training_intro_translator_transition;
    level.scr_anim["foley"]["training_intro_end"] = %h2_training_intro_foley_end;
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_end", "train_fly_thanksforhelp", 1 );
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "training_intro_end", "train_fly_gofirst" );
    level.scr_anim["translator"]["training_intro_end"] = %h2_training_intro_translator_end;
    level.scr_goaltime["translator"]["training_intro_end"] = 1;
    maps\_anim::addnotetrack_dialogue( "translator", "dialog", "training_intro_end", "train_aft_gofirst" );
    level.scr_anim["trainee_01"]["training_intro_pre_end_idle"][0] = %h2_training_intro_trainee_1_pre_end_idle;
    level.scr_anim["trainee_01"]["training_intro_end"] = %h2_training_intro_trainee_1_end;
    level.scr_goaltime["trainee_01"]["training_intro_end"] = 1;
    level.scr_anim["foley"]["training_intro_end_idle"][0] = %h2_training_intro_foley_end_idle;
    level.scr_anim["translator"]["training_intro_end_idle"][0] = %h2_training_intro_translator_end_idle;
    level.scr_anim["trainee_01"]["training_intro_end_idle"][0] = %h2_training_intro_trainee_1_end_idle;
    maps\_anim::addnotetrack_customfunction( "trainee_01", "fire_spray", ::_id_BA58 );
    level.scr_anim["soldier_wounded"]["laatpv_sequence"] = %training_latvee_wounded;
    level.scr_anim["soldier_door"]["laatpv_sequence"] = %training_latvee_soldier;
    level.scr_anim["generic"]["training_intro_soldier1_idle"][0] = %h2_training_intro_soldier1_idle;
    level.scr_anim["generic"]["training_intro_soldier1_pre_end_idle"][0] = %h2_training_intro_soldier1_pre_end_idle;
    level.scr_anim["generic"]["training_intro_soldier1_end"] = %h2_training_intro_soldier1_end;
    level.scr_goaltime["generic"]["training_intro_soldier1_end"] = 1;
    level.scr_anim["generic"]["training_intro_soldier1_end_idle"][0] = %h2_training_intro_soldier1_end_idle;
    level.scr_anim["generic"]["training_intro_soldier2_idle"][0] = %h2_training_intro_soldier2_idle;
    level.scr_anim["generic"]["training_intro_soldier2_pre_end_idle"][0] = %h2_training_intro_soldier2_pre_end_idle;
    level.scr_anim["generic"]["training_intro_soldier2_end"] = %h2_training_intro_soldier2_end;
    level.scr_goaltime["generic"]["training_intro_soldier2_end"] = 1;
    level.scr_anim["generic"]["training_intro_soldier2_end_idle"][0] = %h2_training_intro_soldier2_end_idle;
    level.scr_anim["generic"]["training_intro_soldier3_idle"][0] = %h2_training_intro_soldier3_idle;
    level.scr_anim["generic"]["training_intro_soldier3_pre_end_idle"][0] = %h2_training_intro_soldier3_pre_end_idle;
    level.scr_anim["generic"]["training_intro_soldier3_end"] = %h2_training_intro_soldier3_end;
    level.scr_goaltime["generic"]["training_intro_soldier3_end"] = 1;
    level.scr_anim["generic"]["training_intro_soldier3_end_idle"][0] = %h2_training_intro_soldier3_end_idle;
    level.scr_anim["generic"]["training_intro_soldier4_idle"][0] = %h2_training_intro_soldier4_idle;
    level.scr_anim["generic"]["training_intro_soldier4_pre_end_idle"][0] = %h2_training_intro_soldier4_pre_end_idle;
    level.scr_anim["generic"]["training_intro_soldier4_end"] = %h2_training_intro_soldier4_end;
    level.scr_goaltime["generic"]["training_intro_soldier4_end"] = 1;
    level.scr_anim["generic"]["training_intro_soldier4_end_idle"][0] = %h2_training_intro_soldier4_end_idle;
    level.scr_anim["generic"]["training_intro_soldier6_idle"][0] = %h2_training_intro_soldier6_idle;
    level.scr_anim["generic"]["training_intro_soldier6_pre_end_idle"][0] = %h2_training_intro_soldier6_pre_end_idle;
    level.scr_anim["generic"]["training_intro_soldier6_end"] = %h2_training_intro_soldier6_end;
    level.scr_goaltime["generic"]["training_intro_soldier6_end"] = 1;
    level.scr_anim["generic"]["training_intro_soldier6_end_idle"][0] = %h2_training_intro_soldier6_end_idle;
    level.scr_anim["generic"]["training_intro_soldier7_idle"][0] = %h2_training_intro_soldier7_idle;
    level.scr_anim["generic"]["training_intro_soldier7_pre_end_idle"][0] = %h2_training_intro_soldier7_pre_end_idle;
    level.scr_anim["generic"]["training_intro_soldier7_end"] = %h2_training_intro_soldier7_end;
    level.scr_goaltime["generic"]["training_intro_soldier7_end"] = 1;
    level.scr_anim["generic"]["training_intro_soldier7_end_idle"][0] = %h2_training_intro_soldier7_end_idle;
    level.scr_anim["generic"]["smoke_idle"][0] = %patrol_bored_idle_smoke;
    level.scr_anim["generic"]["smoke_reach"] = %patrol_bored_idle_smoke;
    level.scr_anim["generic"]["smoke"] = %patrol_bored_idle_smoke;
    level.scr_anim["generic"]["smoke_react"] = %patrol_bored_react_look_advance;
    level.scr_anim["generic"]["lean_smoke_idle"][0] = %parabolic_leaning_guy_smoking_idle;
    level.scr_anim["generic"]["lean_smoke_idle"][1] = %parabolic_leaning_guy_smoking_twitch;
    level.scr_anim["generic"]["lean_smoke_react"] = %parabolic_leaning_guy_react;
    level.scr_anim["lean_smoker"]["lean_smoke_idle"][0] = %parabolic_leaning_guy_smoking_idle;
    level.scr_anim["lean_smoker"]["lean_smoke_idle"][1] = %parabolic_leaning_guy_smoking_twitch;
    level.scr_anim["lean_smoker"]["lean_smoke_react"] = %parabolic_leaning_guy_react;
    level.scr_anim["generic"]["patrol_walk"] = %patrol_bored_patrolwalk;
    level.scr_anim["generic"]["patrol_walk_twitch"] = %patrol_bored_patrolwalk_twitch;
    level.scr_anim["generic"]["patrol_stop"] = %patrol_bored_walk_2_bored;
    level.scr_anim["generic"]["patrol_start"] = %patrol_bored_2_walk;
    level.scr_anim["generic"]["patrol_turn180"] = %patrol_bored_2_walk_180turn;
    level.scr_anim["generic"]["patrol_idle_1"] = %patrol_bored_idle;
    level.scr_anim["generic"]["patrol_idle_2"] = %patrol_bored_idle_smoke;
    level.scr_anim["generic"]["patrol_idle_3"] = %patrol_bored_idle_cellphone;
    level.scr_anim["generic"]["patrol_idle_4"] = %patrol_bored_twitch_bug;
    level.scr_anim["generic"]["patrol_idle_5"] = %patrol_bored_twitch_checkphone;
    level.scr_anim["generic"]["patrol_idle_6"] = %patrol_bored_twitch_stretch;
    level.scr_anim["generic"]["patrol_idle_smoke"] = %patrol_bored_idle_smoke;
    level.scr_anim["generic"]["patrol_idle_checkphone"] = %patrol_bored_twitch_checkphone;
    level.scr_anim["generic"]["patrol_idle_stretch"] = %patrol_bored_twitch_stretch;
    level.scr_anim["generic"]["patrol_idle_phone"] = %patrol_bored_idle_cellphone;
    level.scr_anim["generic"]["combat_jog"] = %combat_jog;
    level.scr_anim["generic"]["smoking_reach"] = %parabolic_leaning_guy_smoking_idle;
    level.scr_anim["generic"]["smoking"][0] = %parabolic_leaning_guy_smoking_idle;
    level.scr_anim["generic"]["smoking"][1] = %parabolic_leaning_guy_smoking_twitch;
    level.scr_anim["generic"]["smoking_react"] = %parabolic_leaning_guy_react;
    level.scr_anim["generic"]["patrol_turn_l45_rfoot"] = %h1_bored_patrol_walk_turn_l45_right_foot;
    level.scr_anim["generic"]["patrol_turn_l45_lfoot"] = %h1_bored_patrol_walk_turn_l45_left_foot;
    level.scr_anim["generic"]["patrol_turn_l90_rfoot"] = %h1_bored_patrol_walk_turn_l90_right_foot;
    level.scr_anim["generic"]["patrol_turn_l90_lfoot"] = %h1_bored_patrol_walk_turn_l90_left_foot;
    level.scr_anim["generic"]["patrol_turn_l135_rfoot"] = %h1_bored_patrol_walk_turn_l135_right_foot;
    level.scr_anim["generic"]["patrol_turn_l135_lfoot"] = %h1_bored_patrol_walk_turn_l135_left_foot;
    level.scr_anim["generic"]["patrol_turn_r45_rfoot"] = %h1_bored_patrol_walk_turn_r45_right_foot;
    level.scr_anim["generic"]["patrol_turn_r45_lfoot"] = %h1_bored_patrol_walk_turn_r45_left_foot;
    level.scr_anim["generic"]["patrol_turn_r90_rfoot"] = %h1_bored_patrol_walk_turn_r90_right_foot;
    level.scr_anim["generic"]["patrol_turn_r90_lfoot"] = %h1_bored_patrol_walk_turn_r90_left_foot;
    level.scr_anim["generic"]["patrol_turn_r135_rfoot"] = %h1_bored_patrol_walk_turn_r135_right_foot;
    level.scr_anim["generic"]["patrol_turn_r135_lfoot"] = %h1_bored_patrol_walk_turn_r135_left_foot;
    level.scr_anim["generic"]["patrol_walk_in_lfoot"] = %h1_bored_patrol_walk_turn_walk_in_left;
    level.scr_anim["generic"]["patrol_walk_out_lfoot"] = %h1_bored_patrol_walk_turn_walk_out_left;
    level.scr_anim["generic"]["patrol_walk_in_rfoot"] = %h1_bored_patrol_walk_turn_walk_in_right;
    level.scr_anim["generic"]["patrol_walk_out_rfoot"] = %h1_bored_patrol_walk_turn_walk_out_right;
    level.scr_anim["generic"]["training_jog_guy1"] = %training_jog_guy1;
    level.scr_anim["generic"]["training_jog_guy2"] = %training_jog_guy2;
    level.scr_anim["generic"]["casual_killer_jog_A"] = %casual_killer_jog_a;
    level.scr_anim["generic"]["casual_killer_jog_B"] = %casual_killer_jog_b;
    level.scr_anim["generic"]["freerunnerA_loop"] = %freerunnera_loop;
    level.scr_anim["generic"]["freerunnerB_loop"] = %freerunnerb_loop;
    level.scr_anim["generic"]["huntedrun_1_idle"] = %huntedrun_1_idle;
    level.scr_anim["generic"]["training_sleeping_in_chair"][0] = %training_sleeping_in_chair;
    level.scr_anim["generic"]["training_basketball_rest"][0] = %training_basketball_rest;
    level.scr_anim["generic"]["training_basketball_guy1"][0] = %training_basketball_guy1;
    level.scr_anim["generic"]["training_basketball_guy2"][0] = %training_basketball_guy2;
    level.scr_anim["generic"]["training_latvee_repair"][0] = %training_latvee_repair;
    level.scr_anim["generic"]["h2_training_pushups_guy1"][0] = %h2_training_pushups_guy1;
    level.scr_anim["generic"]["h2_training_pushups_guy2"][0] = %h2_training_pushups_guy2;
    level.scr_anim["generic"]["h2_training_jumpingjack_guy3"][0] = %h2_training_jumpingjack_guy3;
    level.scr_anim["generic"]["h2_training_latveerepair_underguy"][0] = %h2_training_latveerepair_underguy;
    maps\_anim::addnotetrack_customfunction( "generic", "detach_wrench", ::detachitem, "h2_training_latveerepair_underguy", "wrench" );
    maps\_anim::addnotetrack_customfunction( "generic", "attach_pliers", ::attachitem, "h2_training_latveerepair_underguy", "plier" );
    maps\_anim::addnotetrack_customfunction( "generic", "detach_pliers", ::detachitem, "h2_training_latveerepair_underguy", "plier" );
    maps\_anim::addnotetrack_customfunction( "generic", "attach_wrench", ::attachitem, "h2_training_latveerepair_underguy", "wrench" );
    level.scr_anim["generic"]["h2_training_latveerepair_behindguy"][0] = %h2_training_latveerepair_behindguy;
    level.scr_anim["generic"]["h2_training_latveerepair_topguy"][0] = %h2_training_latveerepair_topguy;
    level.scr_anim["generic"]["h2_training_soldierreading"][0] = %h2_training_soldierreading;
    level.scr_anim["generic"]["h2_training_rangerschilling_cell"][0] = %h2_training_rangerschilling_cell;
    level.scr_anim["generic"]["h2_training_rangerschilling_rifle"][0] = %h2_training_rangerschilling_rifle;
    level.scr_anim["generic"]["h2_training_jogging_runner1"][0] = %h2_training_jogging_runner1;
    level.scr_anim["generic"]["h2_training_jogging_runner2"][0] = %h2_training_jogging_runner2;
    level.scr_anim["generic"]["h2_training_jogging_runner3"][0] = %h2_training_jogging_runner3;
    level.scr_anim["generic"]["h2_training_jogging_runner4"][0] = %h2_training_jogging_runner4;
    level.scr_anim["generic"]["h2_training_shootingrange_ranger01"][0] = %h2_training_shootingrange_ranger01;
    level.scr_anim["generic"]["h2_training_shootingrange_ranger02"][0] = %h2_training_shootingrange_ranger02;
    level.scr_anim["generic"]["h2_training_shootingrange_ranger03"][0] = %h2_training_shootingrange_ranger03;
    level.scr_anim["generic"]["h2_training_shootingrange_ranger04"][0] = %h2_training_shootingrange_ranger04;
    level.scr_anim["generic"]["h2_training_stand_0"] = %h2_training_shootingrange_stand_shootloop_single;
    level.scr_anim["generic"]["h2_training_stand_1"] = %h2_training_shootingrange_stand_shootloop_burst;
    level.scr_anim["generic"]["h2_training_stand_2"] = %h2_training_shootingrange_stand_reload;
    level.scr_anim["generic"]["h2_training_stand_3"] = %h2_training_shootingrange_stand_gunjam;
    level.scr_anim["generic"]["h2_training_stand_4"] = %h2_training_shootingrange_stand_gunproblem;
    level.scr_anim["generic"]["h2_training_stand_5"] = %h2_training_shootingrange_stand_surprised;
    level.scr_anim["generic"]["h2_training_stand_6"] = %h2_training_shootingrange_stand_lookaround;
    level.scr_anim["generic"]["h2_training_stand_7"] = %h2_training_shootingrange_stand_2_crouch;
    level.scr_anim["generic"]["h2_training_crouch_0"] = %h2_training_shootingrange_crouch_shootloop_single;
    level.scr_anim["generic"]["h2_training_crouch_1"] = %h2_training_shootingrange_crouch_shootloop_mixed;
    level.scr_anim["generic"]["h2_training_crouch_2"] = %h2_training_shootingrange_crouch_reload;
    level.scr_anim["generic"]["h2_training_crouch_3"] = %h2_training_shootingrange_crouch_2_stand_reload;
    level.scr_anim["generic"]["h2_training_shootingrange_formation_student"][0] = %h2_training_shootingrange_formation_student;
    level.scr_anim["generic"]["h2_training_shootingrange_formation_teacher"][0] = %h2_training_shootingrange_formation_teacher;
    maps\_anim::addonstart_animsound( "generic", "h2_training_stand_0", "scn_h2_training_stand_0" );
    maps\_anim::addonstart_animsound( "generic", "h2_training_stand_1", "scn_h2_training_stand_1" );
    maps\_anim::addonstart_animsound( "generic", "h2_training_stand_2", "scn_h2_training_stand_2" );
    maps\_anim::addonstart_animsound( "generic", "h2_training_stand_3", "scn_h2_training_stand_3" );
    maps\_anim::addonstart_animsound( "generic", "h2_training_stand_4", "scn_h2_training_stand_4" );
    maps\_anim::addonstart_animsound( "generic", "h2_training_stand_5", "scn_h2_training_stand_5" );
    maps\_anim::addonstart_animsound( "generic", "h2_training_stand_6", "scn_h2_training_stand_6" );
    maps\_anim::addonstart_animsound( "generic", "h2_training_stand_7", "scn_h2_training_stand_7" );
    maps\_anim::addonstart_animsound( "generic", "h2_training_crouch_0", "scn_h2_training_crouch_0" );
    maps\_anim::addonstart_animsound( "generic", "h2_training_crouch_1", "scn_h2_training_crouch_1" );
    maps\_anim::addonstart_animsound( "generic", "h2_training_crouch_2", "scn_h2_training_crouch_2" );
    maps\_anim::addonstart_animsound( "generic", "h2_training_crouch_3", "scn_h2_training_crouch_3" );
    level.scr_animtree["generic"] = #animtree;
    level.scr_anim["generic"]["h2_training_triotalking_soldier01_intro"] = %h2_training_triotalking_soldier01_intro;
    level.scr_anim["generic"]["h2_training_triotalking_soldier01"][0] = %h2_training_triotalking_soldier01;
    level.scr_anim["generic"]["h2_training_triotalking_soldier02_intro"] = %h2_training_triotalking_soldier02_intro;
    level.scr_anim["generic"]["h2_training_triotalking_soldier02"][0] = %h2_training_triotalking_soldier02;
    level.scr_anim["generic"]["h2_training_triotalking_soldier03_intro"] = %h2_training_triotalking_soldier03_intro;
    level.scr_anim["generic"]["h2_training_triotalking_soldier03"][0] = %h2_training_triotalking_soldier03;
    level.scr_anim["generic"]["h2_training_handshake_walking_01_intro"] = %h2_training_handshake_walking_01_intro;
    level.scr_anim["generic"]["h2_training_handshake_walking_01"][0] = %h2_training_handshake_walking_01;
    level.scr_anim["generic"]["h2_training_handshake_walking_02_intro"] = %h2_training_handshake_walking_02_intro;
    level.scr_anim["generic"]["h2_training_handshake_walking_02"][0] = %h2_training_handshake_walking_02;
    level.scr_anim["generic"]["h2_training_handshake_join_01_intro"] = %h2_training_handshake_join_01_intro;
    level.scr_anim["generic"]["h2_training_handshake_join_01"][0] = %h2_training_handshake_join_01;
    level.scr_anim["generic"]["h2_training_handshake_join_02_intro"] = %h2_training_handshake_join_02_intro;
    level.scr_anim["generic"]["h2_training_handshake_join_02"][0] = %h2_training_handshake_join_02;
    level.scr_anim["generic"]["h2_training_bbq_guy"][0] = %h2_training_bbq_guy;
    level.scr_anim["generic"]["h2_training_daydream_sit_01"][0] = %h2_training_daydream_sit_01;
    level.scr_anim["generic"]["h2_training_daydream_lean_02"][0] = %h2_training_daydream_lean_02;
    level.scr_anim["generic"]["h2_training_daydream_sitground_01"][0] = %h2_training_daydream_sitground_01;
    level.scr_anim["generic"]["h2_training_daydream_leg_crate"][0] = %h2_training_daydream_leg_crate;
    level.scr_anim["generic"]["h2_training_daydream_sitchair"][0] = %h2_training_daydream_sitchair;
    level.scr_anim["generic"]["h2_training_chilling_ground_ranger01"][0] = %h2_training_chilling_ground_ranger01;
    level.scr_anim["generic"]["h2_training_chilling_ground_ranger02"][0] = %h2_training_chilling_ground_ranger02;
    level.scr_anim["generic"]["h2_training_crowdlife_duotalking_01"][0] = %h2_training_crowdlife_duotalking_01;
    level.scr_anim["generic"]["h2_training_crowdlife_duotalking_02"][0] = %h2_training_crowdlife_duotalking_02;
    level.scr_anim["generic"]["h2_training_controlroom_soldier"][0] = %h2_training_controlroom_soldier;
    level.scr_anim["generic"]["h2_training_burgertown_truck_soldier"][0] = %h2_training_burgertown_truck_soldier;
    level.scr_anim["generic"]["h2_parabolic_leaning_guy_smoking_idle"][0] = %h2_parabolic_leaning_guy_smoking_idle;
    level.scr_anim["generic"]["h1_killhouse_laptopguy_generic_typing"][0] = %h1_killhouse_laptopguy_generic_typing;
    level.scr_anim["generic"]["h1_killhouse_laptopguy_typing_variation"][0] = %h1_killhouse_laptopguy_typing_variation;
    level.scr_anim["generic"]["h2_training_basketball_guy1"][0] = %h2_training_basketball_guy1;
    level.scr_anim["generic"]["h2_training_basketball_guy2"][0] = %h2_training_basketball_guy2;
    level.scr_anim["generic"]["h2_roadkill_latvee_map_sequence_quiet_idle"][0] = %h2_roadkill_latvee_map_sequence_quiet_idle;
    level.scr_anim["generic"]["h2_training_shepherdwatching_shepherd_walk"] = %h2_training_shepherdwatching_shepherd_walk;
    level.scr_anim["generic"]["h2_training_shepherdwatching_shepherd_loop"][0] = %h2_training_shepherdwatching_shepherd_loop;
    level.scr_anim["generic"]["h2_training_shepherdwatching_shepherd_forward"] = %h2_training_shepherdwatching_shepherd_forward;
    level.scr_anim["generic"]["h2_training_shepherdwatching_shepherd_Endloop"][0] = %h2_training_shepherdwatching_shepherd_endloop;
    level.scr_anim["generic"]["h2_training_shepherdwatching_soldier01_walk"] = %h2_training_shepherdwatching_soldier01_walk;
    level.scr_anim["generic"]["h2_training_shepherdwatching_soldier01_loop"][0] = %h2_training_shepherdwatching_soldier01_loop;
    level.scr_anim["generic"]["h2_training_shepherdwatching_soldier01_forward"] = %h2_training_shepherdwatching_soldier01_forward;
    level.scr_anim["generic"]["h2_training_shepherdwatching_soldier01_Endloop"][0] = %h2_training_shepherdwatching_soldier01_endloop;
    level.scr_anim["generic"]["h2_training_shepherdwatching_soldier02_walk"] = %h2_training_shepherdwatching_soldier02_walk;
    level.scr_anim["generic"]["h2_training_shepherdwatching_soldier02_loop"][0] = %h2_training_shepherdwatching_soldier02_loop;
    level.scr_anim["generic"]["h2_training_shepherdwatching_soldier02_forward"] = %h2_training_shepherdwatching_soldier02_forward;
    level.scr_anim["generic"]["h2_training_shepherdwatching_soldier02_Endloop"][0] = %h2_training_shepherdwatching_soldier02_endloop;
    level.scr_anim["dunn"]["sitting_welcome_soldier"] = %h2_training_pit_sitting_welcome_soldier;
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "sitting_welcome_soldier", "train_cpd_welcomeback" );
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "sitting_welcome_soldier", "train_cpd_specialop" );
    level.scr_anim["dunn"]["sitting_welcome_soldier_grab"] = %h2_training_pit_sitting_welcome_pistol_soldier;
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "sitting_welcome_soldier_grab", "train_cpd_grabapistol" );
    level.scr_anim["dunn"]["sitting_welcome_soldier_sidearm"] = %h2_training_pit_sitting_welcome_sidearm_soldier;
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "sitting_welcome_soldier_sidearm", "train_cpd_alreadyhave" );
    level.scr_anim["dunn"]["sitting_idle_soldier"][0] = %h2_training_pit_sitting_idle_soldier;
    maps\_anim::addnotetrack_customfunction( "dunn", "stopidle", ::dunnreadytocontinue, "sitting_idle_soldier" );
    level.scr_anim["dunn"]["sitting_switchgun_VO1"] = %h2_training_pit_sitting_switchgun_vo1_soldier;
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "sitting_switchgun_VO1", "train_cpd_switchtorifle" );
    level.scr_anim["dunn"]["sitting_switchgun_VO1_switch"] = %h2_training_pit_sitting_switchgun_vo1_sidearm_soldier;
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "sitting_switchgun_VO1_switch", "train_cpd_tryswitching" );
    level.scr_anim["dunn"]["sitting_switchgun_idle"][0] = %h2_training_pit_sitting_switchgun_idle_soldier;
    maps\_anim::addnotetrack_customfunction( "dunn", "stopidle", ::dunnreadytocontinue, "sitting_switchgun_idle" );
    level.scr_anim["dunn"]["sitting_switchgun_VO2"] = %h2_training_pit_sitting_switchgun_vo2_soldier;
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "sitting_switchgun_VO2", "train_cpd_switchtosidearm" );
    level.scr_anim["dunn"]["open_case_soldier"] = %h2_training_pit_open_case_soldier;
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "open_case_soldier", "train_cpd_alwaysfaster" );
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "open_case_soldier", "train_cpd_smileforcameras" );
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "open_case_soldier", "train_cpd_timerstarts" );
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "open_case_soldier", "train_cpd_putusin" );
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "open_case_soldier", "train_cpd_socombrass" );
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "open_case_soldier", "train_cpd_realaction" );
    maps\_anim::addnotetrack_flag( "dunn", "case_flip_01", "case_flip_01", "open_case_soldier" );
    maps\_anim::addnotetrack_flag( "dunn", "case_flip_02", "case_flip_02", "open_case_soldier" );
    maps\_anim::addnotetrack_flag( "dunn", "button_press", "button_press", "open_case_soldier" );
    level.scr_anim["dunn"]["stand_wait_idle"] = %h2_training_pit_stand_wait_idle_soldier;
    level.scr_anim["dunn"]["stand_End_VO3"] = %h2_training_pit_stand_end_vo3_soldier;
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "stand_End_VO3", "train_cpd_donthaveallday" );
    level.scr_anim["dunn"]["stand_End_idle"][0] = %h2_training_pit_stand_end_idle_soldier;
    level.scr_anim["dunn"]["stand_End_VO4"] = %h2_training_pit_stand_end_vo4_soldier;
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "stand_End_VO4", "train_cpd_bothintrouble" );
    level.scr_anim["dunn"]["dunn_pit_outro"] = %h2_training_pit_outro;
    maps\_anim::addnotetrack_customfunction( "dunn", "Result_dialog", ::playdunnvo, "dunn_pit_outro" );
    maps\_anim::addnotetrack_customfunction( "dunn", "dialog", ::h2_dunn_headupstairs, "dunn_pit_outro" );
    level.scr_anim["dunn"]["dunn_pit_outro_idle"][0] = %h2_training_pit_outro_idle;
    level.scr_anim["dunn"]["dunn_pit_outro_retry"] = %h2_training_pit_outro_stay;
    maps\_anim::addnotetrack_customfunction( "dunn", "dialog", ::playdunnvo, "dunn_pit_outro_retry" );
    level.scr_anim["dunn"]["dunn_pit_outro_retry_idle"][0] = %h2_training_pit_outro_stay_idle;
    level.scr_anim["dunn"]["dunn_pit_tryagain"] = %h2_training_pit_outro_tryagain;
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "dunn_pit_tryagain", "train_cpd_anothergo" );
    level.scr_anim["dunn"]["h2_headupstairs_outro_out"] = %h2_training_pit_outro_out;
    maps\_anim::addnotetrack_dialogue( "dunn", "dialog", "h2_headupstairs_outro_out", "train_cpd_headupstairs" );
    level.scr_anim["generic"]["training_latvee_repair"][0] = %training_latvee_repair;
    level.scr_anim["generic"]["killhouse_laptop_idle"][0] = %killhouse_laptop_idle;
    level.scr_anim["generic"]["killhouse_laptop_idle"][1] = %killhouse_laptop_lookup;
    level.scr_anim["generic"]["killhouse_laptop_idle"][2] = %killhouse_laptop_twitch;
    level.scr_anim["generic"]["cliffhanger_welder_wing"][0] = %cliffhanger_welder_wing;
    level.scr_anim["generic"]["cliffhanger_welder_engine"][0] = %cliffhanger_welder_engine;
    level.scr_anim["generic"]["patrolstand_idle"][0] = %patrolstand_idle;
    level.scr_anim["generic"]["parabolic_guard_sleeper_idle"][0] = %parabolic_guard_sleeper_idle;
    level.scr_anim["generic"]["roadkill_cover_spotter_idle"][0] = %roadkill_cover_spotter_idle;
    level.scr_anim["generic"]["oilrig_balcony_smoke_idle"][0] = %oilrig_balcony_smoke_idle;
    level.scr_anim["generic"]["killhouse_gaz_idleB"][0] = %killhouse_gaz_idleb;
    level.scr_anim["generic"]["civilian_sitting_talking_A_2"][0] = %civilian_sitting_talking_a_2;
    level.scr_anim["generic"]["parabolic_leaning_guy_smoking_idle"][0] = %parabolic_leaning_guy_smoking_idle;
    level.scr_anim["generic"]["parabolic_leaning_guy_idle"][0] = %parabolic_leaning_guy_idle;
    level.scr_anim["generic"]["parabolic_leaning_guy_idle_training"][0] = %parabolic_leaning_guy_idle_training;
    level.scr_anim["generic"]["civilian_texting_sitting"][0] = %civilian_texting_sitting;
    level.scr_anim["generic"]["sitting_guard_loadAK_idle"][0] = %sitting_guard_loadak_idle;
    level.scr_anim["generic"]["afgan_caves_sleeping_guard_idle"][0] = %afgan_caves_sleeping_guard_idle;
    level.scr_anim["generic"]["cargoship_sleeping_guy_idle_2"][0] = %cargoship_sleeping_guy_idle_2;
    level.scr_anim["generic"]["civilian_sitting_talking_A_1"][0] = %civilian_sitting_talking_a_1;
    level.scr_anim["generic"]["bunker_toss_idle_guy1"][0] = %bunker_toss_idle_guy1;
    level.scr_anim["generic"]["roadkill_cover_radio_soldier3"][0] = %roadkill_cover_radio_soldier3;
    level.scr_anim["generic"]["civilian_sitting_talking_B_1"][0] = %civilian_sitting_talking_b_1;
    level.scr_anim["generic"]["civilian_smoking_A"][0] = %civilian_smoking_a;
    level.scr_anim["generic"]["civilian_reader_1"][0] = %civilian_reader_1;
    level.scr_anim["generic"]["civilian_reader_2"][0] = %civilian_reader_2;
    level.scr_anim["generic"]["guardA_sit_sleeper_sleep_idle"] = %guarda_sit_sleeper_sleep_idle;
    level.scr_anim["generic"]["roadkill_latvee_map_sequence_quiet_idle"][0] = %roadkill_latvee_map_sequence_quiet_idle;
    level.scr_anim["generic"]["guardB_sit_drinker_idle"][0] = %guardb_sit_drinker_idle;
    level.scr_anim["generic"]["guardB_standing_cold_idle"][0] = %guardb_standing_cold_idle;
    level.scr_anim["generic"]["civilian_texting_standing"][0] = %civilian_texting_standing;
    level.scr_anim["generic"]["killhouse_sas_2_idle"][0] = %killhouse_sas_2_idle;
    level.scr_anim["generic"]["killhouse_sas_3_idle"][0] = %killhouse_sas_3_idle;
    level.scr_anim["generic"]["killhouse_sas_price_idle"][0] = %killhouse_sas_price_idle;
    level.scr_anim["generic"]["killhouse_sas_1_idle"][0] = %killhouse_sas_1_idle;
    level.scr_anim["generic"]["little_bird_casual_idle_guy1"][0] = %little_bird_casual_idle_guy1;
    level.scr_anim["generic"]["sniper_escape_spotter_idle"][0] = %sniper_escape_spotter_idle;
    level.scr_anim["generic"]["patrol_bored_idle"][0] = %patrol_bored_idle;
    level.scr_anim["generic"]["training_woundedwalk_soldier_1"] = %training_woundedwalk_soldier_1;
    level.scr_anim["generic"]["training_woundedwalk_soldier_2"] = %training_woundedwalk_soldier_2;
    level.scr_anim["generic"]["hostage_pickup_runout_guy1"] = %hostage_pickup_runout_guy1;
    level.scr_anim["generic"]["hostage_pickup_runout_guy2"] = %hostage_pickup_runout_guy2;
    level.scr_anim["generic"]["DC_Burning_stop_bleeding_medic"] = %dc_burning_stop_bleeding_medic;
    maps\_anim::addnotetrack_attach( "generic", "attach prop", "clotting_powder_animated", "TAG_INHAND", "DC_Burning_stop_bleeding_medic" );
    maps\_anim::addnotetrack_detach( "generic", "dettach prop", "clotting_powder_animated", "TAG_INHAND", "DC_Burning_stop_bleeding_medic" );
    level.scr_anim["generic"]["DC_Burning_stop_bleeding_wounded"] = %dc_burning_stop_bleeding_wounded;
    level.scr_anim["generic"]["DC_Burning_stop_bleeding_medic_idle"][0] = %dc_burning_stop_bleeding_medic_endidle;
    level.scr_anim["generic"]["DC_Burning_stop_bleeding_wounded_idle"][0] = %dc_burning_stop_bleeding_wounded_endidle;
    level.scr_anim["carrier"]["wounded_pickup"] = %wounded_pickup_carrierguy;
    level.scr_anim["carried"]["wounded_pickup"] = %wounded_pickup_carriedguy;
    level.scr_anim["carrier"]["wounded_carry"] = %wounded_carry_fastwalk_carrier;
    level.scr_anim["carried"]["wounded_carry"] = %wounded_carry_fastwalk_wounded;
    level.scr_anim["generic"]["cliff_guardA_flick"] = %cliff_guarda_flick;
    level.scr_anim["generic"]["unarmed_climb_wall"] = %unarmed_climb_wall;
    level.scr_anim["generic"]["civilian_walk_coffee"][0] = %civilian_walk_coffee;
    level.scr_anim["generic"]["civilian_walk_cool"][0] = %civilian_walk_cool;
    level.scr_anim["generic"]["patrol_bored_patrolwalk"][0] = %patrol_bored_patrolwalk;
    level.scr_anim["generic"]["patrolwalk_swagger"][0] = %patrolwalk_swagger;
    level.scr_anim["generic"]["civilian_walk_hurried_1"][0] = %civilian_walk_hurried_1;
    level.scr_anim["generic"]["civilian_crowd_behavior_A"][0] = %civilian_crowd_behavior_a;
    level.scr_anim["generic"]["civilian_crazywalker_loop"][0] = %civilian_crazywalker_loop;
    level.scr_anim["generic"]["civilian_cellphonewalk"][0] = %civilian_cellphonewalk;
    level.scr_anim["generic"]["civilian_briefcase_walk_shoelace"][0] = %civilian_briefcase_walk_shoelace;
    level.scr_anim["generic"]["civilian_sodawalk"][0] = %civilian_sodawalk;
    maps\_anim::addnotetrack_customfunction( "generic", "footstep_right_large", ::bounce_fx_guy );
    maps\_anim::addnotetrack_customfunction( "generic", "footstep_left_large", ::bounce_fx_guy );
    maps\_anim::addnotetrack_customfunction( "generic", "footstep_right_small", ::bounce_fx_guy );
    maps\_anim::addnotetrack_customfunction( "generic", "footstep_left_small", ::bounce_fx_guy );
    level.scr_anim["generic"]["h2_training_ending_soldier02"] = %h2_training_ending_soldier02;
    level.scr_anim["generic"]["h2_training_ending_soldier03"] = %h2_training_ending_soldier03;
    maps\_anim::addnotetrack_dialogue( "generic", "dialog", "h2_training_ending_soldier03", "train_ar1_trapped" );
    maps\_anim::addnotetrack_customfunction( "generic", "dialog", ::guytalk, "h2_training_ending_soldier03", 3 );
    level.scr_anim["generic"]["h2_training_ending_soldier04"] = %h2_training_ending_soldier04;
    level.scr_anim["generic"]["h2_training_ending_injured"] = %h2_training_ending_injured;
    level.scr_anim["generic"]["h2_training_ending_soldier01"] = %h2_training_ending_soldier01;
    maps\_anim::addnotetrack_dialogue( "generic", "dialog", "h2_training_ending_soldier01", "train_ar2_blewthebridge" );
    maps\_anim::addnotetrack_customfunction( "generic", "dialog", ::guytalk, "h2_training_ending_soldier01", 1.5 );
    level.scr_anim["foley"]["h2_training_ending_foley"] = %h2_training_ending_foley;
    maps\_anim::addnotetrack_dialogue( "foley", "dialog", "h2_training_ending_foley", "train_fly_movingout" );
    level.scr_anim["generic"]["h2_training_ending_foleylatdriver"] = %h2_training_ending_foleylatdriver;
    level.scr_anim["generic"]["h2_training_ending_lat01_guy"] = %h2_training_ending_lat01_guy;
    level.scr_anim["generic"]["h2_training_ending_lat01_guy2"] = %h2_training_ending_lat01_guy2;
    level.scr_anim["generic"]["h2_training_ending_lat02_frontguy"] = %h2_training_ending_lat02_frontguy;
    level.scr_anim["generic"]["h2_training_ending_lat02_backguy"] = %h2_training_ending_lat02_backguy;
    level.scr_anim["generic"]["h2_training_ending_lat02_driver"] = %h2_training_ending_lat02_driver;
    level.scr_anim["generic"]["h2_training_ending_lat01_medic01"] = %h2_training_ending_lat01_medic01;
    level.scr_anim["generic"]["h2_training_ending_lat01_medic02"] = %h2_training_ending_lat01_medic02;
    level.scr_anim["generic"]["h2_training_ending_carrier"] = %h2_training_ending_carrier;
}