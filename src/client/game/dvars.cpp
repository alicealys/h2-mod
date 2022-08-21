#include <std_include.hpp>
#include <utils/string.hpp>

#include "dvars.hpp"
#include "game.hpp"

namespace dvars
{
	game::dvar_t* con_inputBoxColor = nullptr;
	game::dvar_t* con_inputHintBoxColor = nullptr;
	game::dvar_t* con_outputBarColor = nullptr;
	game::dvar_t* con_outputSliderColor = nullptr;
	game::dvar_t* con_outputWindowColor = nullptr;
	game::dvar_t* con_inputDvarMatchColor = nullptr;
	game::dvar_t* con_inputDvarValueColor = nullptr;
	game::dvar_t* con_inputDvarInactiveValueColor = nullptr;
	game::dvar_t* con_inputCmdMatchColor = nullptr;

	game::dvar_t* jump_enableFallDamage = nullptr;
	game::dvar_t* jump_ladderPushVel = nullptr;
	game::dvar_t* jump_spreadAdd = nullptr;

	game::dvar_t* r_fullbright = nullptr;

	game::dvar_t* cg_legacyCrashHandling = nullptr;

	game::dvar_t* g_enableElevators = nullptr;

	std::string dvar_get_vector_domain(const int components, const game::dvar_limits& domain)
	{
		if (domain.vector.min == -FLT_MAX)
		{
			if (domain.vector.max == FLT_MAX)
			{
				return utils::string::va("Domain is any %iD vector", components);
			}
			else
			{
				return utils::string::va("Domain is any %iD vector with components %g or smaller", components,
				                         domain.vector.max);
			}
		}
		else if (domain.vector.max == FLT_MAX)
		{
			return utils::string::va("Domain is any %iD vector with components %g or bigger", components,
			                         domain.vector.min);
		}
		else
		{
			return utils::string::va("Domain is any %iD vector with components from %g to %g", components,
			                         domain.vector.min, domain.vector.max);
		}
	}

	std::string dvar_get_domain(const game::dvar_type type, const game::dvar_limits& domain)
	{
		std::string str;

		switch (type)
		{
		case game::dvar_type::boolean:
			return "Domain is 0 or 1"s;

		case game::dvar_type::value:
			if (domain.value.min == -FLT_MAX)
			{
				if (domain.value.max == FLT_MAX)
				{
					return "Domain is any number"s;
				}

				return utils::string::va("Domain is any number %g or smaller", domain.value.max);
			}

			if (domain.value.max == FLT_MAX)
			{
				return utils::string::va("Domain is any number %g or bigger", domain.value.min);
			}

			return utils::string::va("Domain is any number from %g to %g", domain.value.min, domain.value.max);

		case game::dvar_type::vec2:
			return dvar_get_vector_domain(2, domain);

		case game::dvar_type::rgb:
		case game::dvar_type::vec3:
			return dvar_get_vector_domain(3, domain);

		case game::dvar_type::vec4:
			return dvar_get_vector_domain(4, domain);

		case game::dvar_type::integer:
			if (domain.enumeration.stringCount == INT_MIN)
			{
				if (domain.integer.max == INT_MAX)
				{
					return "Domain is any integer"s;
				}

				return utils::string::va("Domain is any integer %i or smaller", domain.integer.max);
			}

			if (domain.integer.max == INT_MAX)
			{
				return utils::string::va("Domain is any integer %i or bigger", domain.integer.min);
			}

			return utils::string::va("Domain is any integer from %i to %i", domain.integer.min, domain.integer.max);

		case game::dvar_type::color:
			return "Domain is any 4-component color, in RGBA format"s;

		case game::dvar_type::enumeration:
			str = "Domain is one of the following:"s;

			for (auto string_index = 0; string_index < domain.enumeration.stringCount; ++string_index)
			{
				str += utils::string::va("\n  %2i: %s", string_index, domain.enumeration.strings[string_index]);
			}

			return str;

		case game::dvar_type::string:
			return "Domain is any text"s;

		default:
			return utils::string::va("unhandled dvar type '%i'", type);
		}
	}

	constexpr int generate_hash(const char* string)
	{
		const char* v1;
		char v2, v6;
		int v4, v5, v7;
		char* end_ptr;

		v1 = string;
		v2 = *string;

		if (v2 == 48 && v1[1] == 120)
		{
			return strtoul(v1 + 2, &end_ptr, 16);
		}

		v4 = v2;

		if ((v2 - 65) <= 0x19u)
		{
			v4 = v2 + 32;
		}

		v5 = 0xB3CB2E29 * static_cast<unsigned int>(v4 ^ 0x319712C3);

		if (v2)
		{
			do
			{
				v6 = *++v1;
				v7 = v6;
				if ((v6 - 65) <= 0x19u)
				{
					v7 = v6 + 32;
				}

				v5 = 0xB3CB2E29 * static_cast<unsigned int>(v5 ^ v7);
			} while (v6);
		}

		return v5;
	}

	dvar_info::dvar_info(const std::string& name, const std::string& description)
		: name(name)
		  , description(description)
	{
		this->hash = generate_hash(name.data());
	}

	std::vector<dvar_info> dvar_list =
	{
		{
			"ac_test_value",
			"Test functionality",
		},
		{
			"accessToSubscriberContent",
			"Whether to display the subscriber maps.",
		},
		{
			"actionSlotsHide",
			"Hide the actionslots.",
		},
		{
			"actor_spaceLightingOffset",
			"Offset from a Space Actor's origin used to determine what lights effect this actor.",
		},
		{
			"actor_trace_bound_offset",
			"Will offset the actor's hit box in the bullet trace logic.",
		},
		{
			"actorVisibilityCheckAngleDistMax",
			"Distance above which we set angle to actorVisibilityMinCheckAngle.",
		},
		{
			"actorVisibilityCheckAngleDistMin",
			"Distance below which we set angle to 90.",
		},
		{
			"actorVisibilityMinCheckAngle",
			"Angle outside which we don't check for actor visibility.",
		},
		{
			"ai_accuracy_attackerCountDecrease",
			"Accuracy multiplied by this number for each additional attacker upto ai_accuracy_attackerCountMax",
		},
		{
			"ai_accuracy_attackerCountMax",
			"Accuracy multiplied by ai_accuracy_attackerCountDecrease for each additional attacker upto this",
		},
		{
			"ai_accuracyDistScale",
			"Distance scale for AI accuracy calculations. Higher = less accurate",
		},
		{
			"ai_allowOverTrim",
			"Whether to allow trimming beyond the lookahead node, which should not be a safe action",
		},
		{
			"ai_backtrackCheckPathDir",
			"Whether to abort backtracking unless the offset to the current node aligns with that node's path dir",
		},
		{
			"ai_badPathSpam",
			"Write debugging information for a bad AI paths of suppressed actors.  Non-suppressed actors will still print, but see ai_disableAllPathSpam",
		},
		{
			"ai_badPlace_clearSeveredPath",
			"If set, AI will have their path cleared if the first link runs through a bad place",
		},
		{
			"ai_busyEventDistBullet",
			"",
		},
		{
			"ai_busyEventDistDeath",
			"",
		},
		{
			"ai_busyEventDistExplosion",
			"",
		},
		{
			"ai_busyEventDistFootstep",
			"",
		},
		{
			"ai_busyEventDistFootstepSprint",
			"",
		},
		{
			"ai_busyEventDistFootstepWalk",
			"",
		},
		{
			"ai_busyEventDistGrenadePing",
			"",
		},
		{
			"ai_busyEventDistGunShot",
			"",
		},
		{
			"ai_busyEventDistGunShotTeam",
			"",
		},
		{
			"ai_busyEventDistNewEnemy",
			"",
		},
		{
			"ai_busyEventDistPain",
			"",
		},
		{
			"ai_busyEventDistProjImpact",
			"",
		},
		{
			"ai_busyEventDistProjPing",
			"",
		},
		{
			"ai_busyEventDistSilencedShot",
			"",
		},
		{
			"ai_corpseCount",
			"Scriptable maximum number of AI corpses",
		},
		{
			"ai_corpseLimit",
			"Archived maximum number of AI corpses - for PC config files",
		},
		{
			"ai_count",
			"Maximum number of AI",
		},
		{
			"ai_coverScore_coverType",
			"Cover node score used when weighting based on distance to node",
		},
		{
			"ai_coverScore_dangerousNode",
			"Cover node score used for nodes that are dangerous to use (someone died there). This value is subtracted",
		},
		{
			"ai_coverScore_distance",
			"Cover node score used when weighting based on distance to node",
		},
		{
			"ai_coverScore_engagement",
			"How much AI prefer cover nodes that are within the engagement distance ranges",
		},
		{
			"ai_coverScore_nodeAngle",
			"Cover node score used when weighting based on distance to node",
		},
		{
			"ai_coverScore_playerLos",
			"Cover node score used when weighting based on distance to node",
		},
		{
			"ai_coverScore_priority",
			"Cover node score used when weighting based on distance to node",
		},
		{
			"ai_coverScore_targetDir",
			"How strongly an AI will want to move to correct engagement distance",
		},
		{
			"ai_coverScore_visibility",
			"Cover node score used when weighting based on distance to node",
		},
		{
			"ai_coverSearchInterval",
			"AI cover search interval when the don't have cover, in milliseconds",
		},
		{
			"ai_dangerReactGoalRadius",
			"Goal radius to use when reacting to danger",
		},
		{
			"ai_debugAccuracy",
			"Enable debugging information for accuracy",
		},
		{
			"ai_debugAnimDeltas",
			"Display animation delta debug information",
		},
		{
			"ai_debugCorpsePlant",
			"Display debug information for AI corpse planting",
		},
		{
			"ai_debugCoverEntityNum",
			"Display debug info for cover",
		},
		{
			"ai_debugCoverSelection",
			"Enable debugging information for cover selection",
		},
		{
			"ai_debugEngagementDist",
			"Show the engagement distances for currently selected AI",
		},
		{
			"ai_debugEntIndex",
			"Entity index of an entity to debug",
		},
		{
			"ai_debugFindPath",
			"Display AI 'find path' debugging information",
		},
		{
			"ai_debugFindPathDirect",
			"Display AI 'find direct path' debugging information",
		},
		{
			"ai_debugFindPathLock",
			"Find path lock",
		},
		{
			"ai_debugFindPathWidth",
			"Display paths with the given width",
		},
		{
			"ai_debugGrenadeFailSafe",
			"Display grenade fail safe debug information",
		},
		{
			"ai_debugGrenadeHintArc",
			"Grenade hint arc in degrees",
		},
		{
			"ai_debugGunBlocked",
			"Show gun blocked by wall traces",
		},
		{
			"ai_debugMayMove",
			"Display debug information for AI 'may move' calculations",
		},
		{
			"ai_debugMeleeAttackSpots",
			"Enable debugging information for melee attack spots",
		},
		{
			"ai_debugOverlay",
			"Display AI debug overlay",
		},
		{
			"ai_debugPlayerLOS",
			"Debug information for AI staying out of player LOS",
		},
		{
			"ai_debugTargetChange",
			"Enable debugging information for target changes",
		},
		{
			"ai_debugTargets",
			"Show primary and secondary targets",
		},
		{
			"ai_debugThreatSelection",
			"Enable debugging information for threat selection",
		},
		{
			"ai_debugVolume",
			"Show volume entity",
		},
		{
			"ai_disableAllPathSpam",
			"Disable any ai path debugging information, even for non-suppressed actors",
		},
		{
			"ai_disableSpawn",
			"Do not spawn AI",
		},
		{
			"ai_dodgeCheckPathDir",
			"Whether to abort a dodge attempt if the path segment created between the dodge end node and the next regular path node goes against the path direction",
		},
		{
			"ai_dog_sharpTurnAngleRun",
			"Dog will always try a sharp-turn when running a turn at this min angle.",
		},
		{
			"ai_dropAkimboChance",
			"Chance that an AI will drop akimbo weapons if available for the weapon",
		},
		{
			"ai_eventDistBadPlace",
			"",
		},
		{
			"ai_eventDistBullet",
			"",
		},
		{
			"ai_eventDistDeath",
			"",
		},
		{
			"ai_eventDistExplosion",
			"",
		},
		{
			"ai_eventDistFootstep",
			"",
		},
		{
			"ai_eventDistFootstepSprint",
			"",
		},
		{
			"ai_eventDistFootstepWalk",
			"",
		},
		{
			"ai_eventDistGrenadePing",
			"",
		},
		{
			"ai_eventDistGunShot",
			"",
		},
		{
			"ai_eventDistGunShotTeam",
			"",
		},
		{
			"ai_eventDistNewEnemy",
			"",
		},
		{
			"ai_eventDistPain",
			"",
		},
		{
			"ai_eventDistProjImpact",
			"",
		},
		{
			"ai_eventDistProjPing",
			"",
		},
		{
			"ai_eventDistSilencedShot",
			"",
		},
		{
			"ai_foliageSeeThroughDist",
			"Maximum distance AI ignore foliage for sight trace to targets",
		},
		{
			"ai_friendlyFireBlockDuration",
			"Friendly fire movement block duration",
		},
		{
			"ai_friendlySuppression",
			"Whether AI fire will suppression teammates or not.",
		},
		{
			"ai_friendlySuppressionDist",
			"Max distance at which AI suppress teammates",
		},
		{
			"ai_frontShieldDamageReduction",
			"Non bullet damage reduction for AI with front shield getting hit within shield angle. (bullets do 0 damage if blocked by shield)",
		},
		{
			"ai_grenadeReturn_approachMinDot",
			"Minimal dot product between the approach and throw vectors to perform a grenade return",
		},
		{
			"ai_grenadeReturn_debug",
			"Turns on debug info for AI grenade returns",
		},
		{
			"ai_grenadeReturn_extraFuseTime",
			"The amount of time (in ms) to add to a grenade fuse when trying to return grenade that's below minFuseTime",
		},
		{
			"ai_grenadeReturn_minDistSqr",
			"Minimal distance to a grenade to consider it for a return so that transition anims will play",
		},
		{
			"ai_grenadeReturn_minFuseTime",
			"If the fuse time drops below this value when an ally is attempting to return a grenade, add extra fuse time",
		},
		{
			"ai_grenadeReturn_stationary",
			"If set, AI will attempt to return grenades that they are within pickup distance - regardless of min dist",
		},
		{
			"ai_grenadeReturn_traceToGrenade",
			"If set, AI will only attempt to return grenades when they have a clear sight trace to the grenade",
		},
		{
			"ai_hardTurn_traceFromOrigin",
			"Whether to trace from the origin or from the lookaheadPos when triggering a hard turn based on a future collision",
		},
		{
			"ai_markDangerousNodes",
			"If set, AI will mark nodes as dangerous when they're pinned down in cover or have been killed",
		},
		{
			"ai_maxGrenadeThrowSpeed",
			"Maximum AI grenade throw speed",
		},
		{
			"ai_moveOrientMode",
			"Debug AI Orient Mode",
		},
		{
			"ai_noDodge",
			"AI won't dodge to the side",
		},
		{
			"ai_pathChokePointCost",
			"The distance AI would travel around to randomly avoid going through a choke point.",
		},
		{
			"ai_pathLookaheadMinDistance",
			"Minimum 2D valid distance to continue using the path lookahead position, otherwise it will uses a newly computed lookahead position",
		},
		{
			"ai_pathMomentum",
			"Momentum factor for continuing motion in previous direction. 0 for no momentum carry over",
		},
		{
			"ai_pathNegotiationOverlapCost",
			"The distance AI would travel around to randomly avoid going through a choke point.",
		},
		{
			"ai_pathRandomPercent",
			"Amount of random cost percent to add to each path node so AI always take slightly different routes",
		},
		{
			"ai_pathSmoothFromNoncodeMove",
			"Whether to perform path smoothing when freshly transitioned from animation-driven motion to code movement",
		},
		{
			"ai_pathSmoothFromStationary",
			"Whether to perform path smoothing based on the actor's facing direction when movement begins for a stationary actor",
		},
		{
			"ai_pathsmoothing",
			"Toggle pathsmoothing on all AI.",
		},
		{
			"ai_playerADSTargetTime",
			"Duration which the AI will not step in front of the player ADS'ing at a target (millisec)",
		},
		{
			"ai_playerFarAccuracy",
			"Accuracy for AI far away from the player",
		},
		{
			"ai_playerFarRange",
			"Minimum range for AI to use 'far' accuracy",
		},
		{
			"ai_playerLOSHalfWidth",
			"Friendly AI will try not to claim cover nodes in this width of player's line of sight",
		},
		{
			"ai_playerLOSMinTime",
			"Player line of sight effect will take effect after player is almost stationary for this time (millisec)",
		},
		{
			"ai_playerLOSRange",
			"Friendly AI will try not to claim cover nodes in this width of player's line of sight",
		},
		{
			"ai_playerNearAccuracy",
			"Accuracy for an AI near to a player",
		},
		{
			"ai_playerNearRange",
			"Maximum range for AI to use 'near' accuracy",
		},
		{
			"ai_showAmbushNodes",
			"Show ambush nodes AI is considering",
		},
		{
			"ai_showBadPlaces",
			"Display debug information for 'bad places'",
		},
		{
			"ai_ShowCanshootChecks",
			"Display debugging information for 'can shoot' checks",
		},
		{
			"ai_showDodge",
			"Display debug information for AI dodging",
		},
		{
			"ai_showLastKnownEnemyPos",
			"Shows the last known position of enemies",
		},
		{
			"ai_showLikelyEnemyPathNode",
			"Show likely enemy path node",
		},
		{
			"ai_showNearbyNodes",
			"Show nearby nodes",
		},
		{
			"ai_showNearestNodes",
			"Show nearest node of AI",
		},
		{
			"ai_showNodes",
			"Show AI navigation node debug information",
		},
		{
			"ai_showNodesDist",
			"Maximum distance from the camera at which AI nodes are shown",
		},
		{
			"ai_showPathFindAnomalies",
			"0 = off, 1 = show failsafe path anomalies(GREEN), 2 = show backtracking path anomalies(BLUE), 3 = show all anomalies",
		},
		{
			"ai_showPathFindNodes",
			"Show nodes consider in latest path find",
		},
		{
			"ai_showPaths",
			"Show AI navigation paths",
		},
		{
			"ai_showPotentialThreatDir",
			"Display AI potential threat direction",
		},
		{
			"ai_showRegion",
			"Draw all the cover in a volume for debugging",
		},
		{
			"ai_showSuppression",
			"Draw the suppression planes for this entity",
		},
		{
			"ai_showTeamMove",
			"Display debug information for AI team movement",
		},
		{
			"ai_showVisData",
			"Display debug information for visibility data",
		},
		{
			"ai_showVisDataDist",
			"Maximum distance for visibility data debugging information to be shown",
		},
		{
			"ai_threatUpdateInterval",
			"AI target threat update interval in milliseconds",
		},
		{
			"ai_turnAnim_checkIntervalMinLookahead",
			"Minimum lookaheadDist to check for turn anims on an interval - if the lookahead is shorter, turns will be checked for every frame",
		},
		{
			"ai_turnAnim_defaultCheckIntervalDist",
			"Base distance covered after a prediction trace failure to try another turn anim",
		},
		{
			"ai_turnAnim_defaultCheckIntervalSpeed",
			"Base speed used to scale the default check interval distance - faster speeds will check more frequently",
		},
		{
			"ai_turnAnim_maxLookaheadDist",
			"Maximum lookahead distance where an actor may consider performing a preemptive turn animation (because they're about to round a corner)",
		},
		{
			"ai_turnAnim_minAfterTurnLookaheadDist",
			"Minimum distance between the turn anim end point and the lookaheadPos",
		},
		{
			"ai_turnAnim_newPathTurnMaxLookahead",
			"Maximum lookahead where we will turn from the lookahead position to the next node; when the lookahead is above this threshold, just turn to face the lookahead (since the next node beyond it is so far away)",
		},
		{
			"aim_accel_turnrate_debug",
			"Turn on debugging info for the acceleration",
		},
		{
			"aim_accel_turnrate_enabled",
			"Enable/disable acceleration of the turnrates",
		},
		{
			"aim_accel_turnrate_lerp",
			"The acceleration of the turnrates",
		},
		{
			"aim_aimAssistRangeScale",
			"Scales the weapon's aim assist range",
		},
		{
			"aim_autoaim_debug",
			"Turn on auto aim debugging",
		},
		{
			"aim_autoaim_enabled",
			"Turn on auto aim",
		},
		{
			"aim_autoaim_lerp",
			"The rate in degrees per second that the auto aim will converge to its target",
		},
		{
			"aim_autoaim_region_height",
			"The height of the auto aim region in virtual screen coordinates(0-480)",
		},
		{
			"aim_autoaim_region_width",
			"The width of the auto aim region in virtual screen coordinates(0-640)",
		},
		{
			"aim_autoAimRangeScale",
			"Scales the weapon's auto aim range",
		},
		{
			"aim_automelee_debug",
			"Turn on auto melee debugging",
		},
		{
			"aim_automelee_enabled",
			"Turn on auto melee",
		},
		{
			"aim_automelee_lerp",
			"The rate in degrees per second that the auto aim will converge to its target",
		},
		{
			"aim_automelee_range",
			"The range of the auto melee",
		},
		{
			"aim_automelee_region_height",
			"The height of the auto melee region in virtual screen coordinates (0 - 480)",
		},
		{
			"aim_automelee_region_width",
			"The width of the auto melee region in virtual screen coordinates (0 - 640)",
		},
		{
			"aim_input_graph_debug",
			"Debug the view input graphs",
		},
		{
			"aim_input_graph_enabled",
			"Use graph for adjusting view input",
		},
		{
			"aim_input_graph_index",
			"Which input graph to use",
		},
		{
			"aim_lockon_debug",
			"Turn on debugging info for aim lock on",
		},
		{
			"aim_lockon_deflection",
			"The amount of stick deflection for the lockon to activate",
		},
		{
			"aim_lockon_enabled",
			"Aim lock on helps the player to stay on target",
		},
		{
			"aim_lockon_region_height",
			"The height of the auto aim region in virtual screen coordinates(0-480)",
		},
		{
			"aim_lockon_region_width",
			"The width of the auto aim region in virtual screen coordinates(0-640)",
		},
		{
			"aim_lockon_strength",
			"The amount of aim assistance given by the target lock on",
		},
		{
			"aim_resist_pitch_enabled",
			"Aim resist pitch applies resistance to the input at the edges of the view cone when the view cone is locked",
		},
		{
			"aim_resist_view_threshold",
			"The minimum input threshold to apply resistance to (0-1)",
		},
		{
			"aim_resist_yaw_enabled",
			"Aim resist yaw applies resistance to the input at the edges of the view cone when the view cone is locked",
		},
		{
			"aim_scale_view_axis",
			"Scale the influence of each input axis so that the major axis has more influence on the control",
		},
		{
			"aim_slowdown_debug",
			"Turn on debugging info for aim slowdown",
		},
		{
			"aim_slowdown_enabled",
			"Slowdown the turn rate when the cross hair passes over a target",
		},
		{
			"aim_slowdown_pitch_scale",
			"The vertical aim assist slowdown ratio from the hip",
		},
		{
			"aim_slowdown_pitch_scale_ads",
			"The vertical aim assist slowdown ratio from the hip",
		},
		{
			"aim_slowdown_prioritization_crosshair_dist_prop",
			"Auto aim prioritization - entities considered close if within this proportion of distance",
		},
		{
			"aim_slowdown_prioritization_dist_prop",
			"Auto aim prioritization - entities considered close if within this proportion of distance",
		},
		{
			"aim_slowdown_prioritization_non_character_neg",
			"Auto aim prioritization - amount of negation applied to non characters considered close to other entities",
		},
		{
			"aim_slowdown_region_height",
			"The screen height of the aim assist slowdown region",
		},
		{
			"aim_slowdown_region_width",
			"The screen width of the aim slowdown region",
		},
		{
			"aim_slowdown_yaw_scale",
			"The horizontal aim assist slowdown ratio from the hip",
		},
		{
			"aim_slowdown_yaw_scale_ads",
			"The horizontal aim assist slowdown ratio from the hip",
		},
		{
			"aim_target_sentient_radius",
			"The radius used to calculate target bounds for a sentient(actor or player)",
		},
		{
			"aim_turnrate_pitch",
			"The vertical turn rate for aim assist when firing from the hip",
		},
		{
			"aim_turnrate_pitch_ads",
			"The turn rate up and down for aim assist when aiming down the sight",
		},
		{
			"aim_turnrate_yaw",
			"The horizontal turn rate for aim assist when firing from the hip",
		},
		{
			"aim_turnrate_yaw_ads",
			"The horizontal turn rate for aim assist when firing from the hip",
		},
		{
			"airburstAdjustDistance",
			"Distance to append to airburst locked in distance.",
		},
		{
			"ammoCounterHide",
			"Hide the Ammo Counter",
		},
		{
			"arcademode",
			"arcade mode",
		},
		{
			"arcademode_full",
			"arcade mode full",
		},
		{
			"arcademode_lives",
			"arcade mode lives",
		},
		{
			"armory_contentpacks_enabled",
			"Allowed armory content packs. 0: none , 1: first armory content pack enabled, 2: first and second armory content pack enabled",
		},
		{
			"autobolt_explosions_to_vehicles",
			"Turns on system to automatically bolt explosions to vehicles. Used for chase scenes.",
		},
		{
			"autoexit",
			"Time to spend in match before shutting down the exe (in ms).",
		},
		{
			"automatedTester",
			"Whether we are currently running an automated tester.",
		},
		{
			"avi_DontUseMemcardStorage",
			"Prevent usage of local mem cards for avi rendering and uploading",
		},
		{
			"band_12players",
			"12 player bandwidth req'd",
		},
		{
			"band_18players",
			"18 player bandwidth req'd",
		},
		{
			"band_2players",
			"2 player bandwidth req'd",
		},
		{
			"band_4players",
			"4 player bandwidth req'd",
		},
		{
			"band_8players",
			"8 player bandwidth req'd",
		},
		{
			"bg_aimSpreadMoveSpeedThreshold",
			"When player is moving faster than this speed, the aim spread will increase",
		},
		{
			"bg_allowScuffFootsteps",
			"If true, scuff sounds will be played when the player rotates in place.",
		},
		{
			"bg_animUnspeedScaledDuration",
			"How long into the start of a speed_scaled_transition anim that speed scaling should be ignored.",
		},
		{
			"bg_audioHeatLevelHighThreshold",
			"Minimum heat level to start playing the high heat start/loop/stop fire sounds.",
		},
		{
			"bg_audioHeatLevelMedThreshold",
			"Minimum heat level to start playing the high heat start/loop/stop fire sounds.",
		},
		{
			"bg_bulletExplDmgFactor",
			"Weapon damage multiplier that will be applied at the center of the slash damage area.",
		},
		{
			"bg_bulletExplRadius",
			"The radius of the bullet splash damage, where the damage gradually falls off to 0.",
		},
		{
			"bg_checkPlayerOutsideSolid",
			"Turn on assert that player is outside collision",
		},
		{
			"bg_compassShowEnemies",
			"Whether enemies are visible on the compass at all times",
		},
		{
			"bg_debugProne",
			"Show prone debug information",
		},
		{
			"bg_fallDamageMaxHeight",
			"The height that a player will take maximum damage when falling",
		},
		{
			"bg_fallDamageMinHeight",
			"The height that a player will start to take minimum damage if they fall",
		},
		{
			"bg_foliagesnd_fastinterval",
			"The time between each foliage sound when moving slowly",
		},
		{
			"bg_foliagesnd_maxspeed",
			"The speed that a player must be going to make minimum noise while moving through foliage",
		},
		{
			"bg_foliagesnd_minspeed",
			"The speed that a player must be going to make minimum noise while moving through foliage",
		},
		{
			"bg_foliagesnd_resetinterval",
			"The time interval before foliage sounds are reset after the player has stopped moving",
		},
		{
			"bg_foliagesnd_slowinterval",
			"The time between each foliage sound when moving slowly",
		},
		{
			"bg_forceDualWield",
			"Force akimbo for all possible weapons",
		},
		{
			"bg_forceExplosiveBullets",
			"When set, all bullet weapons will fire explosive rounds (Simulates Perk)",
		},
		{
			"bg_ladder_yawcap",
			"The maximum angle that a player can look around while on a ladder",
		},
		{
			"bg_landFXLightHeight",
			"The max height that a player will play light land FX and camera shakes",
		},
		{
			"bg_landFXMediumHeight",
			"The max height that a player will play light land FX and camera shakes",
		},
		{
			"bg_lowGravity",
			"Low gravity for slow or floaty objects, in inches per second per second",
		},
		{
			"bg_maxGrenadeIndicatorSpeed",
			"Maximum speed of grenade that will show up in indicator and can be thrown back.",
		},
		{
			"bg_prone_yawcap",
			"The maximum angle that a player can look around while on a ladder",
		},
		{
			"bg_radiusDamageMax",
			"The maximum radius damage radius for non-client and non-actor entities",
		},
		{
			"bg_shieldHitEncodeHeightVM",
			"The decoding range, in height, of a client's viewmodel shield.",
		},
		{
			"bg_shieldHitEncodeHeightWorld",
			"The encoding range, in height, of a client's world shield.  A hit in this range is encoded into one of 8 rows.",
		},
		{
			"bg_shieldHitEncodeWidthVM",
			"The decoding range, in width, of a client's viewmodel shield.",
		},
		{
			"bg_shieldHitEncodeWidthWorld",
			"The encoding range, in width, of a client's world shield.  A hit in this range is encoded into one of 16 collumns.",
		},
		{
			"bg_shock_lookControl",
			"Alter player control during shellshock",
		},
		{
			"bg_shock_lookControl_fadeTime",
			"The time for the shellshock player control to fade in seconds",
		},
		{
			"bg_shock_lookControl_maxpitchspeed",
			"Maximum pitch movement rate while shellshocked in degrees per second",
		},
		{
			"bg_shock_lookControl_maxyawspeed",
			"Maximum yaw movement rate while shell shocked in degrees per second",
		},
		{
			"bg_shock_lookControl_mousesensitivityscale",
			"Sensitivity scale to apply to a shellshocked player",
		},
		{
			"bg_shock_movement",
			"Affect player's movement speed duringi shellshock",
		},
		{
			"bg_shock_screenBlurBlendFadeTime",
			"The amount of time in seconds for the shellshock effect to fade",
		},
		{
			"bg_shock_screenBlurBlendTime",
			"The amount of time in seconds for the shellshock effect to fade",
		},
		{
			"bg_shock_screenFlashShotFadeTime",
			"In seconds, how soon from the end of the effect to start blending out the whiteout layer.",
		},
		{
			"bg_shock_screenFlashWhiteFadeTime",
			"In seconds, how soon from the end of the effect to start blending out the whiteout layer.",
		},
		{
			"bg_shock_screenType",
			"Shell shock screen effect type",
		},
		{
			"bg_shock_sound",
			"Play shell shock sound",
		},
		{
			"bg_shock_soundDryLevel",
			"Shell shock sound dry level",
		},
		{
			"bg_shock_soundEnd",
			"Shellshock end sound alias",
		},
		{
			"bg_shock_soundEndAbort",
			"Shellshock aborted end sound alias",
		},
		{
			"bg_shock_soundFadeInTime",
			"Shell shock sound fade in time in seconds",
		},
		{
			"bg_shock_soundFadeOutTime",
			"Shell shock sound fade out time in seconds",
		},
		{
			"bg_shock_soundLoop",
			"Shellshock loop alias",
		},
		{
			"bg_shock_soundLoopEndDelay",
			"Sound loop end offset time from the end of the shellshock in seconds",
		},
		{
			"bg_shock_soundLoopFadeTime",
			"Shell shock sound loop fade time in seconds",
		},
		{
			"bg_shock_soundLoopSilent",
			"The sound that gets blended with the shellshock loop alias",
		},
		{
			"bg_shock_soundModEndDelay",
			"The delay from the end of the shell shock to the end of the sound modification",
		},
		{
			"bg_shock_soundRoomType",
			"Shell shock sound reverb room type",
		},
		{
			"bg_shock_soundSubmix",
			"Shell shock submix to apply",
		},
		{
			"bg_shock_soundWetLevel",
			"Shell shock sound wet level",
		},
		{
			"bg_shock_viewKickFadeTime",
			"The time for the shellshock kick effect to fade",
		},
		{
			"bg_shock_viewKickPeriod",
			"The period of the shellshock view kick effect",
		},
		{
			"bg_shock_viewKickRadius",
			"Shell shock kick radius",
		},
		{
			"bg_softLandingMaxDamage",
			"The max amount of damage (as % of max health) if soft landing is in effect",
		},
		{
			"bg_softLandingMaxHeight",
			"The height that a player will start to take minimum damage if they fall",
		},
		{
			"bg_softLandingMinHeight",
			"The height that a player will start to take minimum damage if they fall",
		},
		{
			"bg_viewBobAmplitudeBase",
			"The base speed-based view bob amplitude",
		},
		{
			"bg_viewBobAmplitudeRoll",
			"The amplitude applied to the roll for view bobbing",
		},
		{
			"bg_viewBobMax",
			"The maximum allowed bob amplitude",
		},
		{
			"bg_viewBobMocap",
			"Use new mocap motion for viewmodel bob",
		},
		{
			"bg_viewKickMax",
			"The maximum view kick",
		},
		{
			"bg_viewKickMin",
			"The minimum view kick",
		},
		{
			"bg_viewKickRandom",
			"The random direction scale view kick",
		},
		{
			"bg_viewKickScale",
			"The scale to apply to the damage done to calculate damage view kick",
		},
		{
			"bg_weaponBobAmplitudeBase",
			"The base speed-based weapon bob amplitude",
		},
		{
			"bg_weaponBobAmplitudeRoll",
			"The amplitude applied to the roll for weapon bobbing",
		},
		{
			"bg_weaponBobLag",
			"The lag that will be applied the weapon bob cycle",
		},
		{
			"bg_weaponBobMax",
			"The maximum allowed weapon/viewmodel bob amplitude",
		},
		{
			"bullet_penetration_damage",
			"When false, bullets that have penetrated an object will not do damage to AI.",
		},
		{
			"bullet_penetration_enabled",
			"When false, disables bullet penetration completely.",
		},
		{
			"bullet_penetrationActorHitsActors",
			"When false, bullets shot by actors that have penetrated an object will not do damage to other actors.",
		},
		{
			"bullet_penetrationHitsClients",
			"When false, bullets that have penetrated an object will not do damage to AI.",
		},
		{
			"bullet_penetrationMinFxDist",
			"Min distance a penetrated bullet must travel before it'll trigger the effects",
		},
		{
			"bullet_ricochetBaseChance",
			"The base chance a bullet has of ricocheting off of a riot shield.",
		},
		{
			"ca_auto_signin",
			"CoD Anywhere start sign-in task automatically on startup or first party sign-in",
		},
		{
			"ca_do_mlc",
			"CoD Anywhere Do Multi Login check",
		},
		{
			"ca_intra_only",
			"CoD Anywhere Intra Network Only",
		},
		{
			"ca_require_signin",
			"CoD Anywhere require sign in to enter MP",
		},
		{
			"ca_show_signup_request",
			"CoD Anywhere should you show new users a popup requesting they create a CoD Account?",
		},
		{
			"cameraShakeRemoteHelo_Angles",
			"Remote helicopter gunner cam, range to shake the view.",
		},
		{
			"cameraShakeRemoteHelo_Freqs",
			"Remote helicopter gunner cam, how fast to shake.",
		},
		{
			"cameraShakeRemoteHelo_SpeedRange",
			"Remote helicopter gunner cam, range of missile speed to scale the shaking.",
		},
		{
			"cameraShakeRemoteMissile_Angles",
			"Remote missile-cam, range to shake the view.",
		},
		{
			"cameraShakeRemoteMissile_Freqs",
			"Remote missile-cam, how fast to shake.",
		},
		{
			"cameraShakeRemoteMissile_SpeedRange",
			"Remote missile-cam, range of missile speed to scale the shaking.",
		},
		{
			"capturePlaybackDisableSRE",
			"Disable the SRE pop up.",
		},
		{
			"cg_autosimAllowVariableTime",
			"Allow the weapon to do speed up and slow down timing.",
		},
		{
			"cg_autosimFireSpeedScale",
			"1.0: normal speed, 2.0: 200%% speed, 0.5: 50%% speed",
		},
		{
			"cg_autosimMinWeaponSpeed",
			"Minimum distance between autosimmed weapon shots",
		},
		{
			"cg_autosimStopPreviousShot",
			"Always try to stop the previous autosim shot",
		},
		{
			"cg_blood",
			"Show blood effects",
		},
		{
			"cg_bloodLimit",
			"Limit blood effects (to 'prevent excess blood stacking')",
		},
		{
			"cg_bloodLimitMsec",
			"When limiting blood effects, number of milliseconds between effects.",
		},
		{
			"cg_bloodThickColor",
			"Color of the blood overlay's thick blood splatter",
		},
		{
			"cg_bloodThinColor",
			"Color of the blood overlay's thin blood splatter",
		},
		{
			"cg_brass",
			"Weapons eject brass",
		},
		{
			"cg_breathingSoundLvl1Cutoff",
			"",
		},
		{
			"cg_breathingSoundLvl2Cutoff",
			"",
		},
		{
			"cg_breathingSoundsEnabled",
			"Enable sprint breathing sounds.",
		},
		{
			"cg_bulletForce",
			"Force applied from bullet hit",
		},
		{
			"cg_camAngleOffset",
			"Pitch, Yaw, and Roll offsets in degrees",
		},
		{
			"cg_camAngleOverride",
			"If true angle offsets override the actual view angles instead of modifying them.",
		},
		{
			"cg_camOffset",
			"Position offset",
		},
		{
			"cg_centertime",
			"The time for a center printed message to fade",
		},
		{
			"cg_cinematicCanPause",
			"Ingame cinematics can be paused",
		},
		{
			"cg_cinematicFullscreen",
			"Draw ingame cinematics full screen",
		},
		{
			"cg_ColorBlind_EnemyTeam",
			"Enemy team color for color blind people",
		},
		{
			"cg_ColorBlind_MyTeam",
			"Player team color for color blind people",
		},
		{
			"cg_constantSizeHeadIcons",
			"Head icons are the same size regardless of distance from the player",
		},
		{
			"cg_crawlBlendOutTimeFiring",
			"Milleseconds to blend out of crawl anims to firing weapon",
		},
		{
			"cg_crawlBlendTime",
			"Milleseconds to blend in and out of crawl anims",
		},
		{
			"cg_crawlMaxSpeed",
			"At this speed, the crawl anim will play at an anim rate of 1",
		},
		{
			"cg_crawlMinSpeed",
			"Minimum speed before player will start crawl anim",
		},
		{
			"cg_crosshairAlpha",
			"The alpha value of the crosshair",
		},
		{
			"cg_crosshairAlphaMin",
			"The minimum alpha value of the crosshair when it fades in",
		},
		{
			"cg_crosshairDynamic",
			"Crosshair is Dynamic",
		},
		{
			"cg_crosshairEnemyColor",
			"The crosshair color when over an enemy",
		},
		{
			"cg_crosshairVerticalOffset",
			"Amount to vertically offset the crosshair from the center.",
		},
		{
			"cg_cullBulletAngle",
			"Cull bullet trajectories that don't fall within this fov",
		},
		{
			"cg_cullBullets",
			"Whether to cull bullet fire prediction if trajectory doesn't pass your view or anywhere near you",
		},
		{
			"cg_cursorHints",
			"Draw cursor hints where:\n 0: no hints",
		},
		{
			"cg_damageIndicatorMinAngleDiffMax",
			"",
		},
		{
			"cg_damageIndicatorMinAngleDiffMin",
			"",
		},
		{
			"cg_debug_overlay_viewport",
			"Remove the sniper overlay so you can check that the scissor window is correct.",
		},
		{
			"cg_debugDrawHandDragInfo",
			"Draw info about hand drag when swimming",
		},
		{
			"cg_debugDrawHitBox",
			"Display character hit boxes",
		},
		{
			"cg_debugevents",
			"Output event debug information.  EV_NONE turns it off.  EV_MAX_EVENTS shows all events.  Otherwise, show only specified value.",
		},
		{
			"cg_debugInfoCornerOffset",
			"Offset from top-right corner, for cg_drawFPS, etc",
		},
		{
			"cg_debugLookAt",
			"Debug look at information",
		},
		{
			"cg_disableScreenShake",
			"Turns off screen shakes when turned on. Dev-only",
		},
		{
			"cg_dobjdump",
			"Output dobj info for the given entity id",
		},
		{
			"cg_draw2D",
			"Draw 2D screen elements",
		},
		{
			"cg_drawBreathHint",
			"Draw a 'hold breath to steady' hint",
		},
		{
			"cg_drawBuildName",
			"Draw build name",
		},
		{
			"cg_drawCenterLines",
			"Draw a horizontal line and a vertical line through the center of the screen",
		},
		{
			"cg_drawCrosshair",
			"Turn on weapon crosshair",
		},
		{
			"cg_drawDamageDirection",
			"Draw hit direction arrow.",
		},
		{
			"cg_drawDamageFlash",
			"Draw flash when hit.",
		},
		{
			"cg_drawDebugAudioClientTriggers",
			"Display client side trigger audio debug information",
		},
		{
			"cg_drawDebugBones",
			"Draw bones for entity number.",
		},
		{
			"cg_drawDebugBonesBind",
			"Draw bind pose when drawing bones with cg_drawDebugBones.",
		},
		{
			"cg_drawDebugBonesClosest",
			"updates cg_drawDebugBones to closest entity to camera.",
		},
		{
			"cg_drawDebugClientTriggers",
			"Display client side trigger debug information",
		},
		{
			"cg_drawDebugTags",
			"Draw tags for entity number.",
		},
		{
			"cg_drawDebugVisionClientTriggers",
			"Display client side trigger vision debug information",
		},
		{
			"cg_drawEffectNum",
			"Draw counts of effects and elements",
		},
		{
			"cg_drawFPS",
			"Draw frames per second",
		},
		{
			"cg_drawFPSLabels",
			"Draw FPS Info Labels",
		},
		{
			"cg_drawFriendlyFireCrosshair",
			"draw the friendly fire crosshair (friendly move)",
		},
		{
			"cg_drawGun",
			"Draw the view model",
		},
		{
			"cg_drawHealth",
			"Draw health bar",
		},
		{
			"cg_drawHUD",
			"Draw HUD elements",
		},
		{
			"cg_drawImagecache",
			"Draw imagecache/streaming state",
		},
		{
			"cg_drawMantleHint",
			"Draw a 'press key to mantle' hint",
		},
		{
			"cg_drawMapBuildInfo",
			"Draw timestamps and command line options from the compile process",
		},
		{
			"cg_drawMapBuildInfoX",
			"X offset for the BSP debug info string",
		},
		{
			"cg_drawMapBuildInfoY",
			"Y offset for the BSP debug info string",
		},
		{
			"cg_drawMaterial",
			"Draw debugging information for materials",
		},
		{
			"cg_drawpaused",
			"Draw paused screen",
		},
		{
			"cg_drawPlayerBoundingBox",
			"Draw a red bounding box at player's server position",
		},
		{
			"cg_drawPlayerPosInFreeMove",
			"Draw an orange box at the player's pos in noclip/ufo.",
		},
		{
			"cg_drawrumbledebug",
			"Display rumble debug information",
		},
		{
			"cg_drawScriptUsage",
			"Draw debugging information for scripts",
		},
		{
			"cg_drawShellshock",
			"Draw shellshock & flashbang screen effects.",
		},
		{
			"cg_drawTransients",
			"Draw transient fastfile state",
		},
		{
			"cg_drawTurretCrosshair",
			"Draw a cross hair when using a turret",
		},
		{
			"cg_drawVersion",
			"Draw the game version",
		},
		{
			"cg_drawVersionX",
			"X offset for the version string",
		},
		{
			"cg_drawVersionY",
			"Y offset for the version string",
		},
		{
			"cg_drawViewpos",
			"Draw viewpos",
		},
		{
			"cg_dumpAnims",
			"Output animation info for the given entity id",
		},
		{
			"cg_dumpAnimsToScreen",
			"Output animation info for the given entity id",
		},
		{
			"cg_dumpAnimsToScreenAllNodes",
			"Show all animations for cg_dumpAnimsToScreen entity, even if the animation has no weight.",
		},
		{
			"cg_enableWaterSurfaceTransitionFx",
			"Turns on water surface transition fx system.",
		},
		{
			"cg_equipmentSounds",
			"Play equipment sounds",
		},
		{
			"cg_errordecay",
			"Decay for predicted error",
		},
		{
			"cg_explodeForce",
			"Force applied from explosion hit",
		},
		{
			"cg_explodeMinForce",
			"Force below which dynents won't even bother waking up",
		},
		{
			"cg_explodeSpinScale",
			"Scale of the random offset from the center of mass for explosion forces.",
		},
		{
			"cg_explodeUpbias",
			"Upward bias applied to force directions from explosion hits",
		},
		{
			"cg_explodingBulletForce",
			"Force applied from bullet explosion hit",
		},
		{
			"cg_explodingBulletMinForce",
			"Force below which dynents won't even bother waking up",
		},
		{
			"cg_explodingBulletSpinScale",
			"Scale of the random offset from the center of mass for explosion forces.",
		},
		{
			"cg_explodingBulletUpbias",
			"Upward bias applied to force directions from explosion hits",
		},
		{
			"cg_foliagesnd_alias",
			"The sound that plays when an actor or player enters a foliage clip brush.",
		},
		{
			"cg_followEnt",
			"Entity number to follow when in cg_ufo",
		},
		{
			"cg_footsteps",
			"Play footstep sounds that are NOT sprint",
		},
		{
			"cg_footstepsSprint",
			"Play sprint footstep sounds",
		},
		{
			"cg_fov",
			"The field of view angle in degrees for client 0",
		},
		{
			"cg_fov1",
			"The field of view angle in degrees for client 0",
		},
		{
			"cg_fovMin",
			"The minimum possible field of view",
		},
		{
			"cg_fovNonVehAdd",
			"The field of view angle added in degrees when not in a vehicle",
		},
		{
			"cg_fovScale",
			"Scale applied to the field of view",
		},
		{
			"cg_gameBoldMessageWidth",
			"The maximum character width of the bold game messages",
		},
		{
			"cg_gameMessageWidth",
			"The maximum character width of the game messages",
		},
		{
			"cg_gun_x",
			"Forward position of the viewmodel",
		},
		{
			"cg_gun_y",
			"Right position of the viewmodel",
		},
		{
			"cg_gun_z",
			"Up position of the viewmodel",
		},
		{
			"cg_headIconMinScreenRadius",
			"The minumum radius of a head icon on the screen",
		},
		{
			"cg_hintFadeTime",
			"Time in milliseconds for the cursor hint to fade",
		},
		{
			"cg_hudDamageIconHeight",
			"The height of the damage icon",
		},
		{
			"cg_hudDamageIconInScope",
			"Draw damage icons when aiming down the sight of a scoped weapon",
		},
		{
			"cg_hudDamageIconOffset",
			"The offset from the center of the damage icon",
		},
		{
			"cg_hudDamageIconOverlayTime",
			"The amount of time (in ms) for the overlay portion of the damage icon to stay on screen",
		},
		{
			"cg_hudDamageIconStartFadeTime",
			"The amount of time (in ms) before the damage icon begins to fade",
		},
		{
			"cg_hudDamageIconTime",
			"The total amount of time (in ms) for the damage icon to stay on screen after damage is taken",
		},
		{
			"cg_hudDamageIconWidth",
			"The width of the damage icon",
		},
		{
			"cg_hudGrenadeIconEnabledFlash",
			"Show the grenade indicator for flash grenades",
		},
		{
			"cg_hudGrenadeIconHeight",
			"The height of the grenade indicator icon",
		},
		{
			"cg_hudGrenadeIconInScope",
			"Show the grenade indicator when aiming down the sight of a scoped weapon",
		},
		{
			"cg_hudGrenadeIconMaxRangeFlash",
			"The maximum distance that a flashbang can be from a player in order to be shown on the grenade indicator",
		},
		{
			"cg_hudGrenadeIconMaxRangeFrag",
			"The maximum distance that a grenade can be from a player in order to be shown on the grenade indicator",
		},
		{
			"cg_hudGrenadeIconOffset",
			"The offset from the center of the screen for a grenade icon",
		},
		{
			"cg_hudGrenadeIconWidth",
			"The width of the grenade indicator icon",
		},
		{
			"cg_hudGrenadePointerHeight",
			"The height of the grenade indicator pointer",
		},
		{
			"cg_hudGrenadePointerPivot",
			"The pivot point of th grenade indicator pointer",
		},
		{
			"cg_hudGrenadePointerPulseFreq",
			"The number of times per second that the grenade indicator flashes in Hertz",
		},
		{
			"cg_hudGrenadePointerPulseMax",
			"The maximum alpha of the grenade indicator pulse. Values higher than 1 will cause the indicator to remain at full brightness for longer",
		},
		{
			"cg_hudGrenadePointerPulseMin",
			"The minimum alpha of the grenade indicator pulse. Values lower than 0 will cause the indicator to remain at full transparency for longer",
		},
		{
			"cg_hudGrenadePointerWidth",
			"The width of the grenade indicator pointer",
		},
		{
			"cg_hudLighting_basic_additiveLumOffset",
			"[basic] Offset applied to additive light color.",
		},
		{
			"cg_hudLighting_basic_additiveLumScale",
			"[basic] Scale applied to additive light color.",
		},
		{
			"cg_hudLighting_basic_ambientLumOffset",
			"[basic] Offset applied to ambient light color.",
		},
		{
			"cg_hudLighting_basic_ambientLumScale",
			"[basic] Scale applied to ambient light color.",
		},
		{
			"cg_hudLighting_basic_diffuseLumOffset",
			"[basic] Offset applied to diffuse light color.",
		},
		{
			"cg_hudLighting_basic_diffuseLumScale",
			"[basic] Scale applied to diffuse light color.",
		},
		{
			"cg_hudLighting_basic_specExponent",
			"[basic] Specular exponent.  Higher values result in sharper highlights.",
		},
		{
			"cg_hudLighting_basic_specLumOffset",
			"[basic] Offset applied to spec light luminance.",
		},
		{
			"cg_hudLighting_basic_specLumScale",
			"[basic] Scale applied to spec light luminance.",
		},
		{
			"cg_hudLighting_blood_additiveLumOffset",
			"[blood] Offset applied to additive light color.",
		},
		{
			"cg_hudLighting_blood_additiveLumScale",
			"[blood] Scale applied to additive light color.",
		},
		{
			"cg_hudLighting_blood_ambientLumOffset",
			"[blood] Offset applied to ambient light color.",
		},
		{
			"cg_hudLighting_blood_ambientLumScale",
			"[blood] Scale applied to ambient light color.",
		},
		{
			"cg_hudLighting_blood_diffuseLumOffset",
			"[blood] Offset applied to diffuse light color.",
		},
		{
			"cg_hudLighting_blood_diffuseLumScale",
			"[blood] Scale applied to diffuse light color.",
		},
		{
			"cg_hudLighting_blood_specExponent",
			"[blood] Specular exponent.  Higher values result in sharper highlights.",
		},
		{
			"cg_hudLighting_blood_specLumOffset",
			"[blood] Offset applied to spec light luminance.",
		},
		{
			"cg_hudLighting_blood_specLumScale",
			"[blood] Scale applied to spec light luminance.",
		},
		{
			"cg_hudLighting_fadeSharpness",
			"This controls how sharp the lines are when fading using the mask alpha.  Higher values are sharper.",
		},
		{
			"cg_hudMapBorderWidth",
			"The size of the full map's border, filled by the CG_PLAYER_FULLMAP_BORDER ownerdraw",
		},
		{
			"cg_hudMapFriendlyHeight",
			"",
		},
		{
			"cg_hudMapFriendlyWidth",
			"",
		},
		{
			"cg_hudMapPlayerHeight",
			"",
		},
		{
			"cg_hudMapPlayerWidth",
			"",
		},
		{
			"cg_hudMapRadarLineThickness",
			"Thickness, relative to the map width, of the radar texture that sweeps across the full screen map",
		},
		{
			"cg_hudStanceFlash",
			"The background color of the flash when the stance changes",
		},
		{
			"cg_hudStanceHintPrints",
			"Draw helpful text to say how to change stances",
		},
		{
			"cg_invalidCmdHintBlinkInterval",
			"Blink rate of an invalid command hint",
		},
		{
			"cg_invalidCmdHintDuration",
			"Duration of an invalid command hint",
		},
		{
			"cg_jumpADSShakeReductionMax",
			"Max percentage that ADS can reduce jump/land additive anims",
		},
		{
			"cg_landingSounds",
			"Play landing on surface sounds",
		},
		{
			"cg_mantleSounds",
			"Play mantle equipment sounds",
		},
		{
			"cg_mapLocationSelectionCursorSpeed",
			"Speed of the cursor when selecting a location on the map",
		},
		{
			"cg_marks_ents_player_only",
			"Marks on entities from player's bullets only.",
		},
		{
			"cg_minCullBulletDist",
			"Don't cull bullet trajectories that are within this distance to you.",
		},
		{
			"cg_modPrvDrawAxis",
			"Draw Axes in the model previewer",
		},
		{
			"cg_modPrvMruAnims",
			"",
		},
		{
			"cg_modPrvMruModels",
			"",
		},
		{
			"cg_objectiveListWrapCountStandard",
			"The amount of on-screen length to wrap an objective in wide-screen mode",
		},
		{
			"cg_objectiveListWrapCountWidescreen",
			"The amount of on-screen length to wrap an objective in wide-screen mode",
		},
		{
			"cg_player_shield_quickraise_override",
			"Overrides all weapon quickraise state timers after using exo-shield.  0 disables the override.",
		},
		{
			"cg_playerCollideWithPhysics",
			"Enable player collision with client-side physics objects.",
		},
		{
			"cg_playerFovScale0",
			"Scale applied to player 0 field of view",
		},
		{
			"cg_playerFovScale1",
			"Scale applied to player 1 field of view",
		},
		{
			"cg_rumble_devgui_duration",
			"Duration of rumble",
		},
		{
			"cg_rumble_devgui_loop",
			"Enable a looping rumble",
		},
		{
			"cg_scriptIconSize",
			"Size of Icons defined by script",
		},
		{
			"cg_shoulderFollowEnt",
			"When set to true follow the ent set in cg_followEnt in a fixed position, over the shoulder cam.",
		},
		{
			"cg_shoulderFollowOffsets",
			"Set the offsets for the shoulder cam. Usage cg_shoulderFollowOffsets [dist] [height]",
		},
		{
			"cg_showmiss",
			"Show prediction errors",
		},
		{
			"cg_silencedWeaponPingRangeSquared",
			"Square of the range at which the firing of a silenced weapon will ping on the compass.",
		},
		{
			"cg_skipDObjFilterIntoCells",
			"Skips CPU logic to push entities into visible set cells. Helps situations where there are large amounts of moving entities that are mostly visible.",
		},
		{
			"cg_small_dev_string_fontscale",
			"Font scale for a small development only display string",
		},
		{
			"cg_sprintMeterDisabledColor",
			"The color of the sprint meter when the sprint meter is full",
		},
		{
			"cg_sprintMeterEmptyColor",
			"The color of the sprint meter when the sprint meter is full",
		},
		{
			"cg_sprintMeterFullColor",
			"The color of the sprint meter when the sprint meter is full",
		},
		{
			"cg_subtitleMinTime",
			"The minimum time that the subtitles are displayed on screen in seconds",
		},
		{
			"cg_subtitleWidthStandard",
			"The width of the subtitles on a non wide-screen",
		},
		{
			"cg_subtitleWidthWidescreen",
			"The width of the subtitle on a wide-screen",
		},
		{
			"cg_testVar",
			"For random development usage.",
		},
		{
			"cg_traceProfilingDist",
			"If non-negative then length of trace from camera to measure cost of CG_LocationalTrace.",
		},
		{
			"cg_vectorFieldsDraw",
			"Enables debug drawing of vector field instances. (0=off, 1=no depth test, 2=depth test)",
		},
		{
			"cg_vectorFieldsForceUniform",
			"Forces all vector field assets to represent a single, uniform direction",
		},
		{
			"cg_viewmodelAnimatedCrawl",
			"Toggle anim play viewmodel crawl anims rather than procedural",
		},
		{
			"cg_viewmodelAnimatedJumps",
			"Toggle anim play viewmodel jump anims rather than procedural",
		},
		{
			"cg_viewmodelAnimBlending",
			"Toggle anim blending for viewmodel anims",
		},
		{
			"cg_viewVehicleInfluence",
			"The influence on the view angles from being in a vehicle",
		},
		{
			"cg_viewZSmoothingMax",
			"Threshold for the maximum smoothing distance we'll do",
		},
		{
			"cg_viewZSmoothingMin",
			"Threshold for the minimum smoothing distance it must move to smooth",
		},
		{
			"cg_viewZSmoothingTime",
			"Amount of time to spread the smoothing over",
		},
		{
			"cg_waterSheeting_distortionScaleFactor",
			"Distortion uv scales (Default to 1)",
		},
		{
			"cg_waterSheeting_magnitude",
			"Distortion magnitude",
		},
		{
			"cg_waterSheeting_radius",
			"Tweak dev var; Glow radius in pixels at 640x480",
		},
		{
			"cg_waterSurfaceTransition_BlockingFxMoveRate",
			"Rate at which the surface blocking efx moves across the screen.",
		},
		{
			"cg_waterSurfaceTransition_FastVelocityThreshold",
			"Water surface transitions at velocities above this threshold are considered fast.",
		},
		{
			"cg_waterSurfaceTransition_TransitionDepthTop",
			"Upper-bound of the depth over which the surface blocking efx is on screen.",
		},
		{
			"cg_waterSurfaceTransition_VisionBlendBottom",
			"Lower-bound of the depth over which underwater vision set blends in.",
		},
		{
			"cg_waterSurfaceTransition_VisionBlendTop",
			"Upper-bound of the depth over which underwater vision set blends in.",
		},
		{
			"cg_weapHitCullAngle",
			"Angle of cone within which to cull back facing weapon hit effects",
		},
		{
			"cg_weapHitCullEnable",
			"When true, cull back facing weapon hit fx.",
		},
		{
			"cg_weaponCycleDelay",
			"The delay after cycling to a new weapon to prevent holding down the cycle weapon button from cycling too fast",
		},
		{
			"cg_weaponHintsCoD1Style",
			"Draw weapon hints in CoD1 style: with the weapon name, and with the icon below",
		},
		{
			"chaplinCheat",
			"",
		},
		{
			"cl_accessibilityAkimboEnabled",
			"True if accessibility configurations adjust input for akimbo weapons",
		},
		{
			"cl_accessibilityTurboEnabled",
			"True if accessibility configurations do continuous fire for akimbo weapons",
		},
		{
			"cl_analog_attack_threshold",
			"The threshold before firing",
		},
		{
			"cl_anglespeedkey",
			"Multiplier for max angle speed for gamepad and keyboard",
		},
		{
			"cl_cinematicUnpaused",
			"Allows cinematics to play while the client is paused",
		},
		{
			"cl_dirSelConvergenceTime",
			"Time to converge to the new direction when selecting a direction on the map.",
		},
		{
			"cl_disable_pause",
			"Disable Pause",
		},
		{
			"cl_force_paused",
			"Force the client to be paused. Can't be overridden by LUA scripts, the start button, etc.",
		},
		{
			"cl_forceFoVEnabled",
			"Force a modified FoV calculation for capture",
		},
		{
			"cl_forceFoVx",
			"Force the X FoV - auto calculate it from aspect ratio and Y FoV if less than 1",
		},
		{
			"cl_forceFoVy",
			"Force the Y FoV - auto calculate it from aspect ratio and X FoV if less than 1",
		},
		{
			"cl_freelook",
			"Enable looking with mouse",
		},
		{
			"cl_freemove",
			"Fly about the level",
		},
		{
			"cl_freemoveScale",
			"Scale how fast you move in cl_freemove mode",
		},
		{
			"cl_lessprint",
			"Print less to the console by filtering out certain spammy channels",
		},
		{
			"cl_modifiedDebugPlacement",
			"Modify the location of debug output (outside of safe area)",
		},
		{
			"cl_mouseAccel",
			"Mouse acceleration",
		},
		{
			"cl_noprint",
			"Print nothing to the console",
		},
		{
			"cl_paused",
			"Pause the game",
		},
		{
			"cl_paused_simple",
			"Toggling pause won't do any additional special processing if true.",
		},
		{
			"cl_pitchspeed",
			"Max pitch speed in degrees for game pad",
		},
		{
			"cl_showmouserate",
			"Print mouse rate debugging information to the console",
		},
		{
			"cl_shownet",
			"Display network debugging information",
		},
		{
			"cl_showServerCommands",
			"Enable debug prints for server commands",
		},
		{
			"cl_spawnDebug",
			"Turn on debug lines for spawning traces",
		},
		{
			"cl_stanceHoldTime",
			"The time to hold the stance button before the player goes prone",
		},
		{
			"cl_testAnimWeight",
			"test animation weighting",
		},
		{
			"cl_waterMarkEnabled",
			"",
		},
		{
			"cl_waterMarkText",
			"",
		},
		{
			"cl_yawspeed",
			"Max yaw speed in degrees for game pad and keyboard",
		},
		{
			"client_trigger_draw",
			"Draw client trigger geometry",
		},
		{
			"client_trigger_drawDepthTest",
			"Display client trigger geo with depth information",
		},
		{
			"client_trigger_drawDistance",
			"Client trigger draw distance",
		},
		{
			"com_animCheck",
			"Check anim tree",
		},
		{
			"com_attractmode",
			"Run attract mode",
		},
		{
			"com_attractmodeduration",
			"Time when controller is unused before attract mode is enabled",
		},
		{
			"com_cinematicEndInWhite",
			"Set by script. True if cinematic ends with a white screen.",
		},
		{
			"com_completionResolveCommand",
			"Command to run when the message box successfully closes",
		},
		{
			"com_errorMessage",
			"Most recent error message",
		},
		{
			"com_errorRemoveKeyCatcher",
			"True if clearing the error message should also remove the ui focus",
		},
		{
			"com_errorResolveCommand",
			"Command to run when they close the error box",
		},
		{
			"com_errorTitle",
			"Title of the most recent error message",
		},
		{
			"com_filter_output",
			"Use console filters for filtering output.",
		},
		{
			"com_maxfps",
			"Cap frames per second",
		},
		{
			"com_maxFrameTime",
			"Time slows down if a frame takes longer than this many milliseconds",
		},
		{
			"com_playerProfile",
			"Set to the name of the profile",
		},
		{
			"com_reconDumpFrameStats",
			"Dump frame stats for Recon",
		},
		{
			"com_reconDumpGpuTimings",
			"Dump GPU timings for Recon",
		},
		{
			"com_statmon",
			"Draw stats monitor",
		},
		{
			"com_timescale",
			"",
		},
		{
			"commerce_dl_retry_step",
			"Step in m/s for the commerce download retry",
		},
		{
			"commerce_manifest_file_max_retry_time",
			"Max time that the commerce manifest can retry",
		},
		{
			"commerce_manifest_file_retry_step",
			"Step in m/s for the commerce manifest retry",
		},
		{
			"commerce_max_dl_retry_time",
			"Max time that the commerce download can retry",
		},
		{
			"commerce_max_retry_time",
			"Max time that the commerce upload can retry",
		},
		{
			"commerce_retry_step",
			"Step in m/s for the commerce upload retry",
		},
		{
			"compass",
			"Display Compass",
		},
		{
			"compassClampIcons",
			"If true, friendlies and enemy pings clamp to the edge of the radar.  If false, they disappear off the edge.",
		},
		{
			"compassCoords",
			"x = North-South coord base value, \ny = East-West coord base value, \nz = scale (game units per coord unit)",
		},
		{
			"compassDebug",
			"Compass Debugging Mode",
		},
		{
			"compassECoordCutoff",
			"Left cutoff for the scrolling east-west coords",
		},
		{
			"compassEnemyFootstepEnabled",
			"Enables enemies showing on the compass because of moving rapidly nearby.",
		},
		{
			"compassEnemyFootstepMaxRange",
			"The maximum distance at which an enemy may appear on the compass due to 'footsteps'",
		},
		{
			"compassEnemyFootstepMaxZ",
			"The maximum vertical distance enemy may be from the player and appear on the compass due to 'footsteps'",
		},
		{
			"compassEnemyFootstepMinSpeed",
			"The minimum speed an enemy must be moving to appear on the compass due to 'footsteps'",
		},
		{
			"compassFastRadarUpdateTime",
			"Time between radar updates for the fast radar mode",
		},
		{
			"compassFriendlyHeight",
			"",
		},
		{
			"compassFriendlyWidth",
			"",
		},
		{
			"compassHideSansObjectivePointer",
			"Hide the compass, but leave the obective pointer visible.",
		},
		{
			"compassHideVehicles",
			"When enabled, disables the CG_PLAYER_COMPASS_VEHICLES ownerdraw.",
		},
		{
			"compassMaxRange",
			"The maximum range from the player in world space that objects will be shown on the compass",
		},
		{
			"compassMinRadius",
			"The minimum radius from the center of the compass that objects will appear.",
		},
		{
			"compassMinRange",
			"The minimum range from the player in world space that objects will appear on the compass",
		},
		{
			"compassObjectiveArrowHeight",
			"",
		},
		{
			"compassObjectiveArrowOffset",
			"The offset of the objective arrow inward from the edge of the compass map",
		},
		{
			"compassObjectiveArrowRotateDist",
			"Distance from the corner of the compass map at which the objective arrow rotates to 45 degrees",
		},
		{
			"compassObjectiveArrowWidth",
			"",
		},
		{
			"compassObjectiveDetailDist",
			"When an objective is closer than this distance (in meters), the icon will not be drawn on the tickertape.",
		},
		{
			"compassObjectiveDrawLines",
			"Draw horizontal and vertical lines to the active target, if it is within the minimap boundries",
		},
		{
			"compassObjectiveHeight",
			"",
		},
		{
			"compassObjectiveIconHeight",
			"",
		},
		{
			"compassObjectiveIconWidth",
			"",
		},
		{
			"compassObjectiveMaxHeight",
			"The maximum height that an objective is considered to be on this level",
		},
		{
			"compassObjectiveMaxRange",
			"The maximum range at which an objective is visible on the compass",
		},
		{
			"compassObjectiveMinAlpha",
			"The minimum alpha for an objective at the edge of the compass",
		},
		{
			"compassObjectiveMinDistRange",
			"The distance that objective transition effects play over, centered on compassObjectiveNearbyDist.",
		},
		{
			"compassObjectiveMinHeight",
			"The minimum height that an objective is considered to be on this level",
		},
		{
			"compassObjectiveNearbyDist",
			"When an objective is closer than this distance (in meters), the icon will not be drawn on the tickertape.",
		},
		{
			"compassObjectiveNumRings",
			"The number of rings when a new objective appears",
		},
		{
			"compassObjectiveRingSize",
			"The maximum objective ring sige when a new objective appears on the compass",
		},
		{
			"compassObjectiveRingTime",
			"The amount of time between each ring when an objective appears",
		},
		{
			"compassObjectiveTextHeight",
			"Objective text height",
		},
		{
			"compassObjectiveTextScale",
			"Scale to apply to hud objectives",
		},
		{
			"compassObjectiveWidth",
			"",
		},
		{
			"compassObjectiveWraparoundTime",
			"How long it takes for the objective to wrap around the compass from one edge to the other",
		},
		{
			"compassPlayerHeight",
			"",
		},
		{
			"compassPlayerWidth",
			"",
		},
		{
			"compassRadarLineThickness",
			"Thickness, relative to the compass size, of the radar texture that sweeps across the map",
		},
		{
			"compassRadarPingFadeTime",
			"How long an enemy is visible on the compass after it is detected by radar",
		},
		{
			"compassRadarUpdateTime",
			"Time between radar updates for the normal radar mode",
		},
		{
			"compassRotation",
			"Style of compass",
		},
		{
			"compassSize",
			"Scale the compass",
		},
		{
			"compassSoundPingFadeTime",
			"The time in seconds for the sound overlay on the compass to fade",
		},
		{
			"compassTickertapeStretch",
			"How far the tickertape should stretch from its center.",
		},
		{
			"comscore_active",
			"Are we allowed to enable ComScore tracking or not",
		},
		{
			"con_default_console_filter",
			"Default channel filter for the console destination.",
		},
		{
			"con_errormessagetime",
			"Onscreen time for error messages in seconds",
		},
		{
			"con_inputBoxColor",
			"Color of the console input box",
		},
		{
			"con_inputHintBoxColor",
			"Color of the console input hint box",
		},
		{
			"con_matchPrefixOnly",
			"Only match the prefix when listing matching Dvars",
		},
		{
			"con_minicon",
			"Display the mini console on screen",
		},
		{
			"con_miniconlines",
			"Number of lines in the minicon message window",
		},
		{
			"con_minicontime",
			"Onscreen time for minicon messages in seconds",
		},
		{
			"con_outputBarColor",
			"Color of the console output slider bar",
		},
		{
			"con_outputSliderColor",
			"Color of the console slider",
		},
		{
			"con_outputWindowColor",
			"Color of the console output",
		},
		{
			"con_subtitleLeading",
			"Leading for subtitles, calculated as a percentage of the font height",
		},
		{
			"con_typewriterColorGlowCheckpoint",
			"",
		},
		{
			"con_typewriterColorGlowCompleted",
			"",
		},
		{
			"con_typewriterColorGlowFailed",
			"",
		},
		{
			"con_typewriterColorGlowUpdated",
			"",
		},
		{
			"con_typewriterColorInteriorCheckpoint",
			"",
		},
		{
			"con_typewriterColorInteriorCompleted",
			"",
		},
		{
			"con_typewriterColorInteriorFailed",
			"",
		},
		{
			"con_typewriterColorInteriorUpdated",
			"",
		},
		{
			"con_typewriterDecayDuration",
			"Time (in milliseconds) to spend disolving the line away.",
		},
		{
			"con_typewriterDecayStartTime",
			"Time (in milliseconds) to spend between the build and disolve phases.",
		},
		{
			"con_typewriterPrintSpeed",
			"Time (in milliseconds) to print each letter in the line.",
		},
		{
			"consoleGame",
			"True if running on a console",
		},
		{
			"content_download_should_auto_download",
			"Checks whether we should auto download PS4 content not already downloaded or not.",
		},
		{
			"content_download_timer_ms",
			"Number of milliseconds between calls to submit met players to the rest system.",
		},
		{
			"contentEnumerationAuto",
			"poll enumeration",
		},
		{
			"contentEnumerationTimer",
			"time between 2 automatic enumerations",
		},
		{
			"coop_matching_type_search",
			"Is the matching type set to search",
		},
		{
			"coop_showing_game_error_popup",
			"Are we showing a game error popup",
		},
		{
			"coopNameFontSize",
			"",
		},
		{
			"coopNameFontSizeSplitscreen",
			"",
		},
		{
			"counterDownloadInterval",
			"Number of minutes before all the global counters are uploaded",
		},
		{
			"counterUploadInterval",
			"Number of minutes before all the global counters are uploaded",
		},
		{
			"cSplineDebugRender",
			"Debug Render the csplines.",
		},
		{
			"cSplineDebugRenderCorridor",
			"Debug Render the cspline corridor.",
		},
		{
			"cSplineDebugRenderData",
			"Debug Render the cspline data.",
		},
		{
			"cSplineDebugRenderSplineId",
			"Select a cspline - 0 for all.",
		},
		{
			"damage_actorRangeFalloff",
			"When true, the damage the NPCs deal falls off the further their shot travels. Otherwise it deals basedamage regardless of range.",
		},
		{
			"damage_rangeLerp",
			"When true, lerp damage in between the max/min range by the max/min damage, otherwise just use the mid damage when in between max/min range.",
		},
		{
			"db_disableImageCacheEvictionQueue",
			"Disables image cache streaming queued eviction",
		},
		{
			"dc_lobbymerge",
			"Allows lobby merging across data centres",
		},
		{
			"debug_onlinePlayEvenIfDown",
			"Whether to allow people to play even if PSN is down",
		},
		{
			"debugOverlay",
			"Toggles the display of various debug info.",
		},
		{
			"debugOverlayOnly",
			"Toggles whether debug Overlay hides the rest of the UI.",
		},
		{
			"dedicated_dhclient",
			"True if we're a client playing on a DH server",
		},
		{
			"demigod_healthFloor",
			"Sets the health floor during demigod mode.",
		},
		{
			"depthSortViewmodel",
			"Enable depth sorting on the viewmodel.",
		},
		{
			"DEV_PROTOCOL_VERSION",
			"Dev only: temporarily override protocol version so as not to join other dev games in progress.",
		},
		{
			"developer",
			"Enable development options",
		},
		{
			"developer_looseFiles",
			"Enable loose file loading for gsc/lui (PC)",
		},
		{
			"developer_script",
			"Enable developer script comments: 0 disabled, 1 full developer script, 2 only dev scripts required by art/lighting tweaks.",
		},
		{
			"devgui_allowMouse",
			"Devgui allow mouse input",
		},
		{
			"devgui_bevelShade",
			"Bevel shade for the devgui",
		},
		{
			"devgui_colorBgnd",
			"Color background for the devgui",
		},
		{
			"devgui_colorBgndFocus",
			"Button color while the mouse hovers over it.",
		},
		{
			"devgui_colorBgndGray",
			"Grayed out background color for the devgui",
		},
		{
			"devgui_colorBgndGraySel",
			"Greyed out, selected background color for the devgui",
		},
		{
			"devgui_colorBgndHeld",
			"Button color when pressed by the mouse.",
		},
		{
			"devgui_colorBgndSel",
			"Selection background color for the devgui",
		},
		{
			"devgui_colorGraphKnotEditing",
			"Devgui color graph knot editing color",
		},
		{
			"devgui_colorGraphKnotNormal",
			"Devgiu Color graph knot normal color",
		},
		{
			"devgui_colorGraphKnotSelected",
			"Devgui color graph knot selected color",
		},
		{
			"devgui_colorSliderBgnd",
			"Color slider background for the devgui",
		},
		{
			"devgui_colorSliderBgndFocus",
			"Focused color slider background for the devgui",
		},
		{
			"devgui_colorSliderBgndHeld",
			"Held color slider background for the devgui",
		},
		{
			"devgui_colorSliderKnob",
			"Knob color for the devgui",
		},
		{
			"devgui_colorSliderKnobFocus",
			"Focused knob color for the devgui",
		},
		{
			"devgui_colorSliderKnobHeld",
			"Held knob color for the devgui",
		},
		{
			"devgui_colorSliderKnobSel",
			"Selected knob color for the devgui",
		},
		{
			"devgui_colorText",
			"Text color for the devgui",
		},
		{
			"devgui_colorTextGray",
			"Greyed out text color for the devgui",
		},
		{
			"devgui_colorTextGraySel",
			"Greyed out, selected text color for the devgui",
		},
		{
			"devgui_colorTextSel",
			"Selection text color for the devgui",
		},
		{
			"devgui_mouseScrollDelay",
			"Delay before scrolling when holding the slider bar with the left mouse button.",
		},
		{
			"devgui_mouseScrollSpeed",
			"Scroll speed when holding the slider bar with the left mouse button.",
		},
		{
			"disableDLC",
			"",
		},
		{
			"disableEliteDLCCheck",
			"",
		},
		{
			"discard_playerstats_on_suspend",
			"Forces stats discard on suspend",
		},
		{
			"disconnectOnSignOut",
			"If true, the player will be kicked back to the main menu if they sign out of their profile\n",
		},
		{
			"dw_addrHandleTimeout",
			"Delay before destroying an addrHandle after the connection is lost\n",
		},
		{
			"dw_environment_server",
			"Choose which Demonware environment to connect to. Use No Preference if you want to keep the default logic.",
		},
		{
			"dw_ignore_hack",
			"Which DemonWare ignore hack fix mode to use",
		},
		{
			"dw_leaderboard_write_active",
			"Are leaderboard writes enabled",
		},
		{
			"dw_memory_display_time",
			"Number of seconds between high watermark display.",
		},
		{
			"dw_neverDisconnect",
			"Set to true to avoid getting kick back to main menu when losing connection to dw.",
		},
		{
			"dw_presence_active",
			"Is the demonware presence system enabled",
		},
		{
			"dw_presence_coop_join_active",
			"Do we allow players to join on presence for private coop matches (post session to demonware",
		},
		{
			"dw_presence_get_delay",
			"Number of milliseconds to wait after booting the game to fetch demonware presence",
		},
		{
			"dw_presence_get_rate",
			"Number of milliseconds to wait between sending presence state to demonware",
		},
		{
			"dw_presence_put_delay",
			"Number of milliseconds to wait in a presence state before sending to demonware",
		},
		{
			"dw_presence_put_rate",
			"Number of milliseconds to wait between sending presence state to demonware",
		},
		{
			"dw_shared_presence_active",
			"Is the demonware shared presence system enabled",
		},
		{
			"dw_shared_presence_get_delay",
			"Number of milliseconds to wait after booting the game to fetch demonware presence",
		},
		{
			"dw_shared_presence_get_rate",
			"Number of milliseconds to wait between sending presence state to demonware",
		},
		{
			"dw_shared_presence_put_delay",
			"Number of milliseconds to wait in a shared presence state before sending to demonware",
		},
		{
			"dw_shared_presence_put_rate",
			"Number of milliseconds to wait between sending presence state to demonware",
		},
		{
			"dw_test_disconnect",
			"Simulate a demonware disconnect",
		},
		{
			"dw_test_retry_disconnect",
			"Simulate a demonware disconnect at the end of every connection retry",
		},
		{
			"dwBandwidthTestTaskTimeout",
			"default timeout for the bandwidth test task (in ms). 0 means no timeout",
		},
		{
			"dwInfoLogEnabled",
			"Toggle Demonware Info log",
		},
		{
			"dwStorageFakeError",
			"Fake errors from dwStorage",
		},
		{
			"dynEnt_bulletForce",
			"Force applied from bullet hit",
		},
		{
			"dynEnt_damageScale",
			"Scales damage applied to destructible dynents",
		},
		{
			"dynEnt_explodeForce",
			"Force applied from explosion hit",
		},
		{
			"dynEnt_explodeMaxEnts",
			"The maximum number of dynents that can be awakened by one explosion",
		},
		{
			"dynEnt_explodeMinForce",
			"Force below which dynents won't even bother waking up",
		},
		{
			"dynEnt_explodeSpinScale",
			"Scale of the random offset from the center of mass for explosion forces.",
		},
		{
			"dynEnt_explodeUpbias",
			"Upward bias applied to force directions from explosion hits",
		},
		{
			"dynEnt_explodingBulletForce",
			"Force applied from bullet explosion hit",
		},
		{
			"dynEnt_explodingBulletMaxEnts",
			"The maximum number of dynents that can be awakened by one explosion",
		},
		{
			"dynEnt_explodingBulletMinForce",
			"Force below which dynents won't even bother waking up",
		},
		{
			"dynEnt_explodingBulletSpinScale",
			"Scale of the random offset from the center of mass for explosion forces.",
		},
		{
			"dynEnt_explodingBulletUpbias",
			"Upward bias applied to force directions from explosion hits",
		},
		{
			"dynEnt_playerWakeUpRadius",
			"Determines threshold distance from player within which all dynents are woken up.",
		},
		{
			"dynEnt_playerWakeUpZOffset",
			"Determines vertical distance from player's feet from which wake up sphere is centered.",
		},
		{
			"dynEnt_showWakeUpSphere",
			"Shows the wakeup spheres that are trigger by WakeUpPhysicsSphere.",
		},
		{
			"elite_clan_active",
			"Are we allowed to show Elite Clans or not",
		},
		{
			"elite_clan_cool_off_time",
			"Cool off time between calls to fetch the elite clan",
		},
		{
			"elite_clan_delay",
			"Delay before the bdTeams calls start to Demonware. -1 means On-Demand and it will wait until the 'starteliteclan' menu call",
		},
		{
			"elite_clan_division_icon_active",
			"Are we allowed to show Elite Clan division icon or not",
		},
		{
			"elite_clan_get_blob_profile_max_retry_time",
			"Max time that the Elite Clan get private profile can retry",
		},
		{
			"elite_clan_get_blob_profile_retry_step",
			"Step in m/s for the Elite Clan get private profile retry",
		},
		{
			"elite_clan_get_clan_max_retry_time",
			"Max time that the Elite Clan get clan can retry",
		},
		{
			"elite_clan_get_clan_retry_step",
			"Step in m/s for the Elite Clan get clan retry",
		},
		{
			"elite_clan_get_members_max_retry_time",
			"Max time that the Elite Clan get members can retry",
		},
		{
			"elite_clan_get_members_retry_step",
			"Step in m/s for the Elite Clan get members retry",
		},
		{
			"elite_clan_get_private_member_profile_max_retry_time",
			"Max time that the Elite Clan get private profile can retry",
		},
		{
			"elite_clan_get_private_member_profile_retry_step",
			"Step in m/s for the Elite Clan get private profile retry",
		},
		{
			"elite_clan_get_public_profile_max_retry_time",
			"Max time that the Elite Clan get public profile can retry",
		},
		{
			"elite_clan_get_public_profile_retry_step",
			"Step in m/s for the Elite Clan get public profile retry",
		},
		{
			"elite_clan_get_team_stats_max_retry_time",
			"Max time that the Elite Clan get team stats can retry",
		},
		{
			"elite_clan_get_team_stats_retry_step",
			"Step in m/s for the Elite Clan get team stats retry",
		},
		{
			"elite_clan_motd_throttle_time",
			"Throttle time between motd update calls",
		},
		{
			"elite_clan_send_message_to_members_max_retry_time",
			"Max time that the Elite Clan send message to members can retry",
		},
		{
			"elite_clan_send_message_to_members_retry_step",
			"Step in m/s for the Elite Clan send message to members retry",
		},
		{
			"elite_clan_set_private_member_profile_max_retry_time",
			"Max time that the Elite Clan set private member profile can retry",
		},
		{
			"elite_clan_set_private_member_profile_retry_step",
			"Step in m/s for the Elite Clan set private member profile retry",
		},
		{
			"elite_clan_single_task_popup_text",
			"String to be displayed on popup when a single task is being performed",
		},
		{
			"elite_clan_using_title",
			"Stores whether the Elite Clan title is in use by the user",
		},
		{
			"enable_recordRecentActivity",
			"records the timestamp of when the player was recently active to the tracker leaderboards",
		},
		{
			"energyWeaponDebugDraw",
			"Draw energy weapon areas.",
		},
		{
			"entitlements_active",
			"Are we allowed to show Entitlements or not",
		},
		{
			"entitlements_config_file_max_retry_time",
			"Max time that the Entitlements config file read can retry",
		},
		{
			"entitlements_config_file_retry_step",
			"Step in m/s for the Entitlements config file read retry",
		},
		{
			"entitlements_cool_off_time",
			"Cool off time between calls to fetch the elite clan",
		},
		{
			"entitlements_delay",
			"Delay before the entitlement calls start to Demonware. -1 means On-Demand and it will wait until the 'startentitlements' menu call",
		},
		{
			"entitlements_key_archive_max_retry_time",
			"Max time that the Entitlements key archive read can retry",
		},
		{
			"entitlements_key_archive_retry_step",
			"Step in m/s for the Entitlements key archive read retry",
		},
		{
			"entitlementSystemOk",
			"Set by the game to inform that the entitlement system is initialised",
		},
		{
			"facebook_active",
			"Are we allowed to show Facebook or not",
		},
		{
			"facebook_delay",
			"Delay before the Facebook calls start to Demonware. -1 means On-Demand and it will wait until the 'startfacebook' menu call",
		},
		{
			"facebook_friends_active",
			"Are we allowed to show Facebook Friends or not",
		},
		{
			"facebook_friends_max_retry_time",
			"Max time that the Facebook friends read can retry",
		},
		{
			"facebook_friends_refresh_time",
			"Time in seconds between Facebook friend refreshes",
		},
		{
			"facebook_friends_retry_step",
			"Step in m/s for the Facebook friends read retry",
		},
		{
			"facebook_friends_showing_count",
			"Contains how many facebook friends are being shown in the UI.",
		},
		{
			"facebook_friends_throttle_time",
			"Throttle time between Facebook friend pages",
		},
		{
			"facebook_max_retry_time",
			"Max time that the Facebook authentication can retry",
		},
		{
			"facebook_password",
			"Facebook Password",
		},
		{
			"facebook_password_asterisk",
			"Facebook Password (Asterisk Version)",
		},
		{
			"facebook_popup_text",
			"Facebook Popup Text",
		},
		{
			"facebook_retry_step",
			"Step in m/s for the Facebook authentication retry",
		},
		{
			"facebook_upload_photo_active",
			"Are we allowed to Upload Photos to  Facebook or not",
		},
		{
			"facebook_upload_video_active",
			"Are we allowed to Upload Videos to Facebook or not",
		},
		{
			"facebook_username",
			"Facebook Username",
		},
		{
			"fastfile_loadDevelopment",
			"Enable/Disable loading of the development fast file.",
		},
		{
			"fixedtime",
			"Use a fixed time rate for each frame",
		},
		{
			"FoFIconMaxSize",
			"Maximum size a Friend-or-Foe icon should ever grow to.",
		},
		{
			"FoFIconMinSize",
			"Minimum size a Friend-or-Foe icon should ever shrink to.",
		},
		{
			"FoFIconScale",
			"Base scale of Friend-or-Foe icons.",
		},
		{
			"force_ranking",
			"Set to true to allow private or system link games to use the ranking system",
		},
		{
			"friction",
			"Player friction",
		},
		{
			"friendlyNameFontColor",
			"",
		},
		{
			"friendlyNameFontGlowColor",
			"",
		},
		{
			"friendlyNameFontSize",
			"Fontsize of the popup friendly names.",
		},
		{
			"friendlyNameFontSizeSplitscreen",
			"Fontsize of the popup friendly names, in splitscreen.",
		},
		{
			"fs_basegame",
			"Base game name",
		},
		{
			"fs_basepath",
			"Base game path",
		},
		{
			"fs_basepath_output",
			"Base game path",
		},
		{
			"fs_cdpath",
			"CD path",
		},
		{
			"fs_copyfiles",
			"Copy all used files to another location",
		},
		{
			"fs_debug",
			"Enable file system debugging information",
		},
		{
			"fs_game",
			"Game data directory. Must be \"\" or a sub directory of 'mods/'.",
		},
		{
			"fs_homepath",
			"Game home path",
		},
		{
			"fs_ignoreLocalized",
			"Ignore localized assets",
		},
		{
			"fx_alphaThreshold",
			"Don't draw billboard sprites, oriented sprites or tails with alpha below this threshold (0-256).",
		},
		{
			"fx_cast_shadow",
			"Enable transparency shadow mapping from script",
		},
		{
			"fx_count",
			"Debug effects count",
		},
		{
			"fx_cull_elem_draw",
			"Culls effect elems for drawing",
		},
		{
			"fx_cull_elem_draw_flicker",
			"Flicker DPVS culled effect elems",
		},
		{
			"fx_cull_elem_spawn",
			"Culls effect elems for spawning",
		},
		{
			"fx_debug3D",
			"Turn on effect system debug information, <int> parameter is culling distance",
		},
		{
			"fx_debugAssertQuat",
			"Turn on/off debug asserts for quaternions in the fx system",
		},
		{
			"fx_debugBolt",
			"Debug effects bolt",
		},
		{
			"fx_deferelem",
			"Toggles deferred processing of elements instead of effects",
		},
		{
			"fx_dpvs_cull_elem_draw",
			"Culls effect elems for drawing using DPVS(2: ignore per-effect portal culling flag)",
		},
		{
			"fx_draw",
			"",
		},
		{
			"fx_draw_omniLight",
			"",
		},
		{
			"fx_draw_simd",
			"Draw effects using SIMD / Vector code.",
		},
		{
			"fx_draw_spotLight",
			"",
		},
		{
			"fx_drawClouds",
			"Toggles the drawing of particle clouds",
		},
		{
			"fx_dump",
			"Sends debug info on PlayFX and PlayFXToTag calls within <int> units of player to the console",
		},
		{
			"fx_enable",
			"Toggles all effects processing",
		},
		{
			"fx_flare",
			"Toggles fx flare",
		},
		{
			"fx_forceEffectPass",
			"Forces effects to render in trans, emissive, or default mode",
		},
		{
			"fx_freeze",
			"Freeze effects",
		},
		{
			"fx_killEffectOnRewind",
			"Causes effects that have been marked for a soft kill (fade out) to be killed immediately on a rewind.",
		},
		{
			"fx_lightGridSampleOffset",
			"the length of effect sample's offset along X Axis",
		},
		{
			"fx_mark_profile",
			"Turn on FX profiling for marks (specify which local client, with '1' being the first.)",
		},
		{
			"fx_marks",
			"Toggles whether bullet hits leave marks",
		},
		{
			"fx_marks_ents",
			"Toggles whether bullet hits leave marks",
		},
		{
			"fx_marks_nearlimit",
			"Sets limit of number of decals that can exist at the same location (0 for unlimited)",
		},
		{
			"fx_marks_smodels",
			"Toggles whether bullet hits leave marks",
		},
		{
			"fx_overrideLightFrac",
			"Overrides every lighting-frac value, 0 is all unlit, to 1 that is 100% lit - the default of -1 restores segment lighting-frac values",
		},
		{
			"fx_physicsImpactVelocityThreshold",
			"Set the min normal velocity threshold in order for model physics fx to generate child impact effects.",
		},
		{
			"fx_profile",
			"Turn on FX profiling (specify which local client, with '1' being the first.)",
		},
		{
			"fx_profileFilter",
			"Only show effects with this as a substring in FX profile",
		},
		{
			"fx_profileFilterElemCountZero",
			"Do not include FX that have a zero element count",
		},
		{
			"fx_profileSkip",
			"Skip the first n lines in FX profile (to see ones off bottom of screen)",
		},
		{
			"fx_profileSort",
			"Choose sort criteria for FX profiling",
		},
		{
			"fx_showLightGridSampleOffset",
			"show light grid sample offset in CreateFX mode",
		},
		{
			"fx_visMinTraceDist",
			"Minimum visibility trace size",
		},
		{
			"g_ai",
			"Enable AI",
		},
		{
			"g_aiAnimscript",
			"Enable AI animscript",
		},
		{
			"g_aiEventDump",
			"Print AI events happening for this entity",
		},
		{
			"g_aiEventListenerDump",
			"Dump the AI event listeners once. Automatically reset to false after dump",
		},
		{
			"g_allowUnusedPak",
			"enable loading of texture mips from unused pak",
		},
		{
			"g_anim_mp_idle_turn_angle",
			"Rotation amount of 3rd-person pivot when player rotates in place while standing.",
		},
		{
			"g_anim_mp_idle_turn_anim_duration",
			"Duration of 3rd-person pivot animation.",
		},
		{
			"g_anim_mp_idle_turn_rotate_crouch_end",
			"Time during 3rd-person crouched pivot at which the player stops rotating in place.",
		},
		{
			"g_anim_mp_idle_turn_rotate_crouch_start",
			"Time during 3rd-person crouched pivot at which the player stops rotating in place.",
		},
		{
			"g_anim_mp_idle_turn_rotate_prone_end",
			"Time during 3rd-person prone pivot at which the player stops rotating in place.",
		},
		{
			"g_anim_mp_idle_turn_rotate_prone_start",
			"Time during 3rd-person prone pivot at which the player stops rotating in place.",
		},
		{
			"g_anim_mp_idle_turn_rotate_stand_end",
			"Time during 3rd-person standing pivot at which the player stops rotating in place.",
		},
		{
			"g_anim_mp_idle_turn_rotate_stand_start",
			"Time during 3rd-person standing pivot at which the player stops rotating in place.",
		},
		{
			"g_anim_mp_idle_turn_trigger_angle",
			"Aim yaw angle at which 3rd-person pivot triggers.",
		},
		{
			"g_animated_lean_blends",
			"Toggles between procedural (0) and blended (1) MP lean animation techniques.",
		},
		{
			"g_animsCommandsVerbose",
			"Dump all animation commands. If false, calls marked as verbose will not be printed",
		},
		{
			"g_atmosFogDistanceScaleReadOnly",
			"scale applied to scene distance used for atmospheric fog calculation",
		},
		{
			"g_atmosFogEnabledReadOnly",
			"use atmospheric fog",
		},
		{
			"g_atmosFogExtinctionStrengthReadOnly",
			"scale out scatter contribution of atmospheric fog",
		},
		{
			"g_atmosFogHalfPlaneDistanceReadOnly",
			"distance at which atmospheric fog contributes half the pixels color",
		},
		{
			"g_atmosFogHazeSpreadReadOnly",
			"directionality of haze (1ReadOnly = all forward scatter, 0ReadOnly = all back scatter)",
		},
		{
			"g_atmosFogHazeStrengthReadOnly",
			"portion of atmospheric fog density that is haze (0ReadOnly = all fog, 1ReadOnly = all haze)",
		},
		{
			"g_atmosFogHeightFogBaseHeightReadOnly",
			"height fog is full density at this world height and below",
		},
		{
			"g_atmosFogHeightFogEnabledReadOnly",
			"use height for atmospheric fog",
		},
		{
			"g_atmosFogHeightFogHalfPlaneDistanceReadOnly",
			"at this distance above g_atmosFogHeightFogBaseHeight, height fog density is half",
		},
		{
			"g_atmosFogInScatterStrengthReadOnly",
			"scale in scatter contribution of atmospheric fog",
		},
		{
			"g_atmosFogSkyAngularFalloffEnabledReadOnly",
			"use angular sky falloff for atmospheric fog",
		},
		{
			"g_atmosFogSkyDistanceReadOnly",
			"distance used for sky box when applying atmospheric fog",
		},
		{
			"g_atmosFogSkyFalloffAngleRangeReadOnly",
			"sky fog angular falloff angle range sky fog falls off over this range from the start angle",
		},
		{
			"g_atmosFogSkyFalloffStartAngleReadOnly",
			"sky fog angular falloff start angle (full strength fog at this angle)",
		},
		{
			"g_atmosFogStartDistanceReadOnly",
			"distance from camera at which fog contribution begins",
		},
		{
			"g_atmosFogSunDirectionReadOnly",
			"sun direction used when calculating atmospheric fog",
		},
		{
			"g_changelevel_time",
			"Time for change level fade out",
		},
		{
			"g_deathDelay",
			"Delay a level restart on death",
		},
		{
			"g_debugBullets",
			"Show debug information for bullets",
		},
		{
			"g_debugDamage",
			"Turn on debug information for damage",
		},
		{
			"g_debugLocDamage",
			"Display locational damage debug information for an entity",
		},
		{
			"g_debugLocHit",
			"Display locational damage info for an entity when the entity is hit",
		},
		{
			"g_debugLocHitTime",
			"Time duration of g_debugLocHit lines",
		},
		{
			"g_disableAnimTransitions",
			"Turns off the animation transition control tables",
		},
		{
			"g_disableBulletPenetration",
			"Removes all bullet penetration.",
		},
		{
			"g_dobjdump",
			"Write dobj debug info for this entity",
		},
		{
			"g_drawDefaultStaticModels",
			"Static default xmodels (the big red FX placeholder) are not rendered by default. Enabling this affects transient performance.",
		},
		{
			"g_drawEntBBoxes",
			"Draw entity bounding boxes",
		},
		{
			"g_drawGrenadeHints",
			"Draw debug information for grenades",
		},
		{
			"g_dumpAnims",
			"Write animation debug info for this entity",
		},
		{
			"g_dumpAnimsCommands",
			"Write animation commands debug info for this entity",
		},
		{
			"g_dumpAnimTransitions",
			"Write animation transition debug info for this entity",
		},
		{
			"g_earthquakeEnable",
			"Enable camera shake",
		},
		{
			"g_enableAccurateTrace",
			"Turn on accurate trace, for actors",
		},
		{
			"g_entinfo",
			"Display entity information",
		},
		{
			"g_entinfo_AItext",
			"Type of text information for AI entinfo",
		},
		{
			"g_entinfo_maxdist",
			"Maximum distance of an entity from the camera at which to show entity information",
		},
		{
			"g_entinfo_scale",
			"Scale of the entity information text",
		},
		{
			"g_entinfo_type",
			"Type of entities to display information",
		},
		{
			"g_fogColorIntensityReadOnly",
			"HDR Fog color intensity that was set in the most recent call to \"setexpfog\"",
		},
		{
			"g_fogColorReadOnly",
			"Fog color that was set in the most recent call to \"setexpfog\"",
		},
		{
			"g_fogHalfDistReadOnly",
			"",
		},
		{
			"g_fogMaxOpacityReadOnly",
			"Fog max opacity that was set in the most recent call to \"setexpfog\"",
		},
		{
			"g_fogStartDistReadOnly",
			"",
		},
		{
			"g_friendlyfireDamageScale",
			"Scales player damage from other players",
		},
		{
			"g_friendlyfireDist",
			"Maximum distance at which the player channot shoot while the crosshair is over a friendly",
		},
		{
			"g_friendlyNameDist",
			"Maximum distance at which a friendly name shows when the crosshairs is over them",
		},
		{
			"g_gameskill",
			"Game skill level",
		},
		{
			"g_giveAll",
			"Give all weapons",
		},
		{
			"g_gravity",
			"Gravity in inches per second per second",
		},
		{
			"g_grenadeDamageMaxHeight",
			"Maximum vertical distance at which a grenade will do damage when exploding",
		},
		{
			"g_heightFogBaseHeightReadOnly",
			"height fog is full density at this world height and below",
		},
		{
			"g_heightFogEnabledReadOnly",
			"use height for normal/sun fog, set in the most recent call to \"setexpfog\"",
		},
		{
			"g_heightFogHalfPlaneDistanceReadOnly",
			"at this distance above g_heightFogBaseHeight, height fog density is half, set in the most recent call to \"setexpfog\"",
		},
		{
			"g_hideMissingXmodels",
			"Hide missing xmodels, instead of drawing a big red FX. Must be set on boot command-line, not at runtime.",
		},
		{
			"g_knockback",
			"Maximum player knockback",
		},
		{
			"g_listEntity",
			"list all of the current entities",
		},
		{
			"g_minGrenadeDamageSpeed",
			"Minimum speed at which getting hit be a grenade will do damage (not the grenade explosion damage)",
		},
		{
			"g_notifydump",
			"Write notify debug info for this entity",
		},
		{
			"g_onlyPlayerAreaEntities",
			"Only checks client entities with movers. SP only.",
		},
		{
			"g_player_maxhealth",
			"Player's maximum health",
		},
		{
			"g_recordScriptPlace",
			"Records the file and line of the current script command",
		},
		{
			"g_reloading",
			"True if the game is reloading",
		},
		{
			"g_singleplayerAntilag",
			"Enable singleplayer/co-op antilag",
		},
		{
			"g_spawnai",
			"Enable AI spawning",
		},
		{
			"g_speed",
			"Maximum player speed",
		},
		{
			"g_streamingEnable",
			"enable loading of textures (priorities will still be calculated)",
		},
		{
			"g_sunFogBeginFadeAngleReadOnly",
			"Angle from the sun direction to start fade away from the sun fog color that was set in the most recent call to \"setexpfog\"",
		},
		{
			"g_sunFogColorIntensityReadOnly",
			"HDR Sun fog color intensity that was set in the most recent call to \"setexpfog\"",
		},
		{
			"g_sunFogColorReadOnly",
			"Sun fog color that was set in the most recent call to \"setexpfog\"",
		},
		{
			"g_sunFogDirReadOnly",
			"Sun fog direction that was set in the most recent call to \"setexpfog\"",
		},
		{
			"g_sunFogEnabledReadOnly",
			"Sun fog was enabled in the most recent call to \"setexpfog\"",
		},
		{
			"g_sunFogEndFadeAngleReadOnly",
			"Angle from the sun direction to end fade away from the sun fog color that was set in the most recent call to \"setexpfog\"",
		},
		{
			"g_sunFogScaleReadOnly",
			"Distance scale in the sun fog direction that was set in the most recent call to \"setexpfog\"",
		},
		{
			"g_useholdtime",
			"The time to hold down the 'use' button to activate a 'use' command on a gamepad",
		},
		{
			"g_vehicleDebug",
			"Turn on debug information for vehicles",
		},
		{
			"g_vehicleDrawPath",
			"Turn on debug information for vehicle paths",
		},
		{
			"gamedate",
			"The date compiled",
		},
		{
			"gamedvr_active",
			"Are we allowed to enable GameDVR or not",
		},
		{
			"gamedvr_prohibitrecording",
			"Allow user to capture video",
		},
		{
			"gamedvr_prohibitscreenshot",
			"Allow user to capture screenshots",
		},
		{
			"gamedvr_screenshotcomment",
			"Comment embedded in user screenshot",
		},
		{
			"gamedvr_screenshotgametitle",
			"Game name embedded in user screenshot",
		},
		{
			"gamedvr_screenshottitle",
			"Photo title embedded in user screenshot",
		},
		{
			"gamedvr_videocomment",
			"",
		},
		{
			"gamedvr_videocopyright",
			"",
		},
		{
			"gamedvr_videodescription",
			"",
		},
		{
			"gamedvr_videotitle",
			"Subtitle for recorded video",
		},
		{
			"gameMode",
			"Current gameMode",
		},
		{
			"glass_angular_vel",
			"Sets the range of angular velocities used by new glass pieces",
		},
		{
			"glass_beamDamage",
			"The amount of damage beam attacks do to glass",
		},
		{
			"glass_break",
			"Toggle whether or not glass breaks when shot",
		},
		{
			"glass_crack_pattern_scale",
			"The scale applied to the radius used for the crack pattern",
		},
		{
			"glass_damageToDestroy",
			"The amount of damage a piece of glass must take to look damaged",
		},
		{
			"glass_damageToWeaken",
			"The amount of damage a piece of glass must take to look damaged",
		},
		{
			"glass_debug",
			"Shows debug info for glass",
		},
		{
			"glass_edge_angle",
			"Sets the range of angle deflections used by new glass pieces on a supported edge",
		},
		{
			"glass_fall_delay",
			"Sets how long a heavy piece supported by a single edge waits before falling, based on glass_fall_ratio",
		},
		{
			"glass_fall_gravity",
			"Gravity for falling pieces of glass",
		},
		{
			"glass_fall_ratio",
			"Ratio of piece area to supporting edge length squared.  Below the min, the piece never falls.",
		},
		{
			"glass_fringe_maxcoverage",
			"The maximum portion of the original piece of glass that is allowed to remain after the glass shatters",
		},
		{
			"glass_fringe_maxsize",
			"The maximum area for an edge piece of glass when shattering. Pieces larger than this will be broken into smaller ones",
		},
		{
			"glass_fx_chance",
			"Chance to play an effect on a small piece of glass when it hits the ground",
		},
		{
			"glass_hinge_friction",
			"Friction used by moving glass pieces when joined like a hinge to a frame",
		},
		{
			"glass_linear_vel",
			"Sets the range of linear velocities used by new glass pieces",
		},
		{
			"glass_max_pieces_per_frame",
			"Maximum number of pieces to create in one frame. This is a guideline and not a hard limit.",
		},
		{
			"glass_max_shatter_fx_per_frame",
			"Maximum number of shatter effects to play in one frame This is a guideline and not a hard limit.",
		},
		{
			"glass_meleeDamage",
			"The amount of damage melee attacks do to glass",
		},
		{
			"glass_physics_chance",
			"The chance for a given shard of glass to use physics",
		},
		{
			"glass_physics_maxdist",
			"The maximum distance of a glass piece from the player to do physics",
		},
		{
			"glass_radiusDamageMultiplier",
			"The amount to scale damage to glass from grenades and other explosions",
		},
		{
			"glass_shard_maxsize",
			"The maximum area for a flying piece of glass when shattering. Pieces larger than this will be broken into smaller ones",
		},
		{
			"glass_shattered_scale",
			"The scale of the shattered glass material",
		},
		{
			"glass_simple_duration",
			"The time (in ms) that simple (non-physics) glass pieces live for",
		},
		{
			"glass_spam",
			"Toggle extra spam to help repro glass asserts",
		},
		{
			"glass_trace_interval",
			"The length of time, in milliseconds, between glass piece traces",
		},
		{
			"gpad_button_deadzone",
			"Game pad button deadzone threshhold",
		},
		{
			"gpad_button_rstick_deflect_max",
			"Maximum right stick deflection",
		},
		{
			"gpad_dpadDebounceTime",
			"",
		},
		{
			"gpad_menu_scroll_delay_first",
			"Menu scroll key-repeat delay, for the first repeat, in milliseconds",
		},
		{
			"gpad_menu_scroll_delay_rest_accel",
			"Menu scroll key-repeat delay acceleration from start to end, for repeats after the first, in milliseconds per repeat",
		},
		{
			"gpad_menu_scroll_delay_rest_end",
			"Menu scroll key-repeat delay end, for repeats after the first, in milliseconds",
		},
		{
			"gpad_menu_scroll_delay_rest_start",
			"Menu scroll key-repeat delay start, for repeats after the first, in milliseconds",
		},
		{
			"gpad_rumbleHighThreshold",
			"Game pad activation threshold for the high frequency rumble",
		},
		{
			"gpad_stick_deadzone_max",
			"Game pad maximum stick deadzone",
		},
		{
			"gpad_stick_deadzone_min",
			"Game pad minimum stick deadzone",
		},
		{
			"gpad_stick_pressed",
			"Game pad stick pressed threshhold",
		},
		{
			"gpad_stick_pressed_hysteresis",
			"Game pad stick pressed no-change-zone around gpad_stick_pressed to prevent bouncing",
		},
		{
			"gpad_touchpad_deadzone_max",
			"Touch pad maximum deadzone",
		},
		{
			"gpad_touchpad_deadzone_min",
			"Touch pad minimum deadzone",
		},
		{
			"gpad_vita_sensitivity",
			"Vita sensitivity scale",
		},
		{
			"grapple_magnet_draw",
			"Draw grapple magnet points",
		},
		{
			"grapple_magnet_draw_dist",
			"Draw grapple magnet point max range",
		},
		{
			"grapple_magnet_draw_fov",
			"Draw grapple magnet point max fov",
		},
		{
			"grapple_surface_dot_limit",
			"Surface normal dot limit that can be grappled to [cos(angle limit)]",
		},
		{
			"grenadeBounceRestitutionMax",
			"Cap to keep code from increasing bounce restitution too high.",
		},
		{
			"grenadeBumpFreq",
			"How likely (per server frame) a bump will occur",
		},
		{
			"grenadeBumpMag",
			"Size of bumps (as a fraction of the grenade's current speed)",
		},
		{
			"grenadeBumpMax",
			"Maximum upward speed of a bump (inches/sec)",
		},
		{
			"grenadeCurveMax",
			"Largest rolling curvature (will be random between +/- this value)",
		},
		{
			"grenadeFrictionHigh",
			"The amount of friction (0 to 1) for fast-moving grenades",
		},
		{
			"grenadeFrictionLow",
			"The amount of friction (0 to 1) for slower/rolling grenades",
		},
		{
			"grenadeFrictionMaxThresh",
			"The speed threshold that determines whether to use grenadeFrictionLow/High",
		},
		{
			"grenadeRestThreshold",
			"The speed threshold below which grenades will come to rest",
		},
		{
			"grenadeRollingEnabled",
			"Enables the new \"rolling\" grenade behavior",
		},
		{
			"grenadeWobbleFreq",
			"Wobble cycles per inch of rolling distance (approx)",
		},
		{
			"grenadeWobbleFwdMag",
			"The forward rolling speed will oscillate +/- this amount",
		},
		{
			"grenadeWobbleSideDamp",
			"The rate at which the amount of side-to-side wobbling decreases as overall grenade speed increases",
		},
		{
			"grenadeWobbleSideMag",
			"The distance to wobble left and right",
		},
		{
			"groupDownloadInterval",
			"Minimum interval to wait before getting new group counts",
		},
		{
			"groupUploadInterval",
			"Minimum interval to wait before setting new group counts",
		},
		{
			"hiDef",
			"True if the game video is running in high-def.",
		},
		{
			"hostileNameFontColor",
			"",
		},
		{
			"hostileNameFontGlowColor",
			"",
		},
		{
			"httpnetfs",
			"Stream fastfiles from the specified http server",
		},
		{
			"hud_bloodOverlayLerpRate",
			"Rate at which blood overlay fades out",
		},
		{
			"hud_deathQuoteFadeTime",
			"The time for the death quote to fade",
		},
		{
			"hud_drawHUD",
			"Draw HUD elements. Controlled from non-UI script",
		},
		{
			"hud_fade_ammodisplay",
			"The time for the ammo display to fade in seconds",
		},
		{
			"hud_fade_compass",
			"The time for the compass to fade in seconds",
		},
		{
			"hud_fade_healthbar",
			"The time for the health bar to fade in seconds",
		},
		{
			"hud_fade_offhand",
			"The time for the offhand weapons to fade in seconds",
		},
		{
			"hud_fade_sprint",
			"The time for the sprint meter to fade in seconds",
		},
		{
			"hud_fade_stance",
			"The time for the stance to fade in seconds",
		},
		{
			"hud_flash_period_offhand",
			"Offhand weapons flash period on changing weapon",
		},
		{
			"hud_flash_time_offhand",
			"Offhand weapons flash duration on changing weapon",
		},
		{
			"hud_forceMantleHint",
			"When true, forces the display of the mantle hint.  Can still be overridden by, for example, hud drawing being off.",
		},
		{
			"hud_health_pulserate_critical",
			"The pulse rate of the 'critical' pulse effect",
		},
		{
			"hud_health_pulserate_injured",
			"The pulse rate of the 'injured' pulse effect",
		},
		{
			"hud_health_startpulse_critical",
			"The health level at which to start the 'injured' pulse effect",
		},
		{
			"hud_health_startpulse_injured",
			"The health level at which to start the 'injured' pulse effect",
		},
		{
			"hud_letterBoxFadeTime",
			"The time for the letter box to fade after slam zoom",
		},
		{
			"hud_lui_hideshow_debug",
			"Debug the hide/show debug info",
		},
		{
			"hud_missionFailed",
			"Intended to be set by script and referenced by hud.menu elements.",
		},
		{
			"hud_showStance",
			"When true, allow player's stance indicator to draw.",
		},
		{
			"hudElemPausedBrightness",
			"Brightness of the hudelems when the game is paused.",
		},
		{
			"hudOutlineDuringADS",
			"Turn on the HUD outline (green for friendly, red for enemy) when you are pointing at a player while in ADS.",
		},
		{
			"igs_sosp",
			"Show Original Season Pass",
		},
		{
			"igs_td",
			"Show Trial DLC",
		},
		{
			"imageCache_allocationLimit",
			"Limit in GB image cache is allowed to allocate",
		},
		{
			"imageCache_ShowDebugPrints",
			"Enable image cache debug prints to aid in following images through the system.",
		},
		{
			"intro",
			"Intro movie should play",
		},
		{
			"inventoryHasAllItems",
			"Simulate all items being in the inventory",
		},
		{
			"inviteText",
			"Text to display for the game invite",
		},
		{
			"jump_auto_mantle",
			"Auto mantle when jumping",
		},
		{
			"jump_auto_mantle_max_z_vel",
			"Auto mantle when high jumping",
		},
		{
			"jump_height",
			"The maximum height of a player's jump",
		},
		{
			"jump_ladderPushVel",
			"The velocity of a jump off of a ladder",
		},
		{
			"jump_slowdownEnable",
			"Slow player movement after jumping",
		},
		{
			"jump_spreadAdd",
			"The amount of spread scale to add as a side effect of jumping",
		},
		{
			"jump_stepSize",
			"The maximum step up to the top of a jump arc",
		},
		{
			"laserDebug",
			"Enables the display of various debug info.",
		},
		{
			"laserForceOn",
			"Force laser sights on in all possible places (for debug purposes).",
		},
		{
			"lb_file",
			"Current Spec Ops leaderboard file",
		},
		{
			"lb_filter",
			"Filter applied to the leaderboard display: ('none','friends','facebook_friends')",
		},
		{
			"lb_filter_duration",
			"Current Spec Ops leaderboard filter duration",
		},
		{
			"lb_group",
			"GroupID applied to the leaderboard display",
		},
		{
			"lb_maxrows",
			"Maximum number of rows to fetch",
		},
		{
			"lb_minrefresh",
			"Minimum time (in seconds) between leaderboard fetches",
		},
		{
			"lb_readDelay",
			"Delay time between reads(in milliseconds) between leaderboard fetches",
		},
		{
			"lb_throttle_time",
			"Lobby throttling amount",
		},
		{
			"lb_times_in_window",
			"Lobby throttling window amount",
		},
		{
			"lb_window",
			"Lobby throttling window",
		},
		{
			"le_verbose",
			"Light edit system verbosity",
		},
		{
			"limited_mode",
			"",
		},
		{
			"live_qosec_firstupdatems",
			"MS to wait before deciding to early out qos",
		},
		{
			"live_qosec_lastupdatems",
			"MS since last update required to early out qos",
		},
		{
			"live_qosec_maxtime",
			"Maximum time to allow before qos early out, even if no results",
		},
		{
			"live_qosec_minpercent",
			"Minimum percentage of probe results required before early outing qos",
		},
		{
			"live_qosec_minprobes",
			"Minimum probe results required before early outing qos",
		},
		{
			"live_test_onlinedataoff",
			"Bit flags corresponding enum OnlineDataSyncFlags that indicate we do not have that piece of data",
		},
		{
			"liveanticheatunknowndvar",
			"Live Anti Cheat Unknown Dvar",
		},
		{
			"livestreaming_active",
			"Are we allowed to enable LiveStreaming or not",
		},
		{
			"livestreaming_bitrate",
			"Maximum bitrate of the stream",
		},
		{
			"livestreaming_enable",
			"Enable streaming video output.  Stream displays standin audio/video when disabled.",
		},
		{
			"livestreaming_enablearchive",
			"Allow live stream to be archived by the streaming service.",
		},
		{
			"livestreaming_metadata",
			"Metadata for livestreaming, visible to viewers.",
		},
		{
			"lmc",
			"Load my changes fast file on devmap.",
		},
		{
			"loading_sre_fatal",
			"Loading errors prevent level from loading.",
		},
		{
			"loc_language",
			"Language",
		},
		{
			"loc_translate",
			"Enable translations",
		},
		{
			"loc_warnings",
			"Enable localization warnings",
		},
		{
			"loc_warningsAsErrors",
			"Throw an error for any unlocalized string",
		},
		{
			"loc_warningsUI",
			"Enable localization warnings for UI",
		},
		{
			"lockAllItems",
			"Simulate all items being locked",
		},
		{
			"log_host_migration_chance",
			"The % chance of host migration results telemetry",
		},
		{
			"log_ImageLoadTime",
			"Toggle the output of images loading time",
		},
		{
			"log_mapvote_chance",
			"The % chance of sending map vote telemetry",
		},
		{
			"log_teambalance_chance",
			"The % chance of team balance results telemetry",
		},
		{
			"logfile",
			"Write to log file - 0 = disabled, 1 = async file write, 2 = Sync every write",
		},
		{
			"logScriptTimes",
			"Log times for every print called from script",
		},
		{
			"lowAmmoWarningColor1",
			"Color 1 of 2 to oscilate between",
		},
		{
			"lowAmmoWarningColor2",
			"Color 2 of 2 to oscilate between",
		},
		{
			"lowAmmoWarningNoAmmoColor1",
			"Like lowAmmoWarningColor1, but when no ammo.",
		},
		{
			"lowAmmoWarningNoAmmoColor2",
			"lowAmmoWarningColor2, but when no ammo.",
		},
		{
			"lowAmmoWarningNoReloadColor1",
			"Like lowAmmoWarningColor1, but when no ammo.",
		},
		{
			"lowAmmoWarningNoReloadColor2",
			"lowAmmoWarningColor2, but when no ammo.",
		},
		{
			"lowAmmoWarningPulseFreq",
			"Frequency of the pulse (oscilation between the 2 colors)",
		},
		{
			"lowAmmoWarningPulseMax",
			"Min of oscilation range: 0 is color1 and 1.0 is color2.  Can be < 0, and the wave will clip at 0.",
		},
		{
			"lowAmmoWarningPulseMin",
			"Max of oscilation range: 0 is color1 and 1.0 is color2.  Can be > 1.0, and the wave will clip at 1.0.",
		},
		{
			"ls_enableLevelStatTracking",
			"if true, level stat tracking can be turned on/off.  Set this on the command line before startup.",
		},
		{
			"lsp_enumertion_max_retry_time",
			"Max time that the LSP enumeration can retry",
		},
		{
			"lsp_enumertion_retry_step",
			"Step in m/s for the LSP enumeration retry",
		},
		{
			"lui_demoMode",
			"Check if the game is in demo mode.",
		},
		{
			"lui_disable_blur",
			"Disable LUI blur",
		},
		{
			"lui_drawdesigngrid",
			"Show design grid.",
		},
		{
			"lui_drawmemreport",
			"Show information about memory usage of the LUI system",
		},
		{
			"lui_enabled",
			"Enables LUI",
		},
		{
			"lui_FFotDLocalLoadEnabled",
			"Load the ffotd.lua file from the local patch for testing (Dev Only)",
		},
		{
			"lui_FFotDSupportEnabled",
			"Enables lui to update itself via the ffotd",
		},
		{
			"lui_forcelinedraws",
			"Force the minimum size of a quad to be at least 1 pixel",
		},
		{
			"lui_hud_motion_angle_ease_speed",
			"Hud motion ease percentage of degrees per second",
		},
		{
			"lui_hud_motion_bob_scale",
			"Hud motion bob scale",
		},
		{
			"lui_hud_motion_enabled",
			"Enable hud motion",
		},
		{
			"lui_hud_motion_perspective",
			"value for hud motion perspective transform in pixels",
		},
		{
			"lui_hud_motion_rotation_max",
			"Hud motion rotation max",
		},
		{
			"lui_hud_motion_rotation_scale",
			"Hud motion rotation scale",
		},
		{
			"lui_hud_motion_trans_ease_speed",
			"Hud motion ease percentage of pixels per second",
		},
		{
			"lui_hud_motion_translation_max",
			"Hud motion translation max",
		},
		{
			"lui_hud_motion_translation_scale",
			"Hud motion translation scale",
		},
		{
			"LUI_MemErrorsFatal",
			"Out of memory errors cause drops when true, reinits the UI system if false",
		},
		{
			"lui_menuFlowEnabled",
			"Enables LUI menu flow",
		},
		{
			"lui_pausemenu",
			"Use the lui version of the pausemenu",
		},
		{
			"lui_ReloadMenuRestore",
			"Restore the menu state after luiReload",
		},
		{
			"lui_splitscreenExtraMemory",
			"Increase the memory grab in splitscreen",
		},
		{
			"lui_splitscreensignin_menu",
			"Enables the LUI splitscreensignin menu",
		},
		{
			"lui_splitscreenupscaling",
			"Force splitscreen upscaling off/on (-1 off, 1 on) -- requires map change",
		},
		{
			"lui_systemlink_menu",
			"Enables the LUI systemlink menu",
		},
		{
			"lui_timescale",
			"Scale time of each frame of LUI animation",
		},
		{
			"lui_waitingforgavelmessagesconfirmed",
			"",
		},
		{
			"lui_waitingfornetworktype",
			"value is LuiWaitingForNetworkType enum",
		},
		{
			"lui_waitingforonlinedatafetch_controller",
			"the controller index that is fetching the online stats data",
		},
		{
			"LUI_WorkerCmdGC",
			"Dev-only flag to enable/disable LUI workerCmd GC thread",
		},
		{
			"lui_xboxlive_menu",
			"Enables the LUI xboxlive menu",
		},
		{
			"m_filter",
			"Allow mouse movement smoothing",
		},
		{
			"m_forward",
			"Forward speed in units per second",
		},
		{
			"m_pitch",
			"Default pitch",
		},
		{
			"m_side",
			"Sideways motion in units per second",
		},
		{
			"m_vehMouseSteerSensitivity",
			"Vehicle mouse steering sensitivity",
		},
		{
			"m_yaw",
			"Default yaw",
		},
		{
			"mangleDWStats",
			"0=do nothing, 1=pretend file doesn't exist--reset, 2=pretend file is corrupted--reset, 3=pretend you're a hacker and we reset your stats",
		},
		{
			"manifestfs",
			"Use a manifest file to read segmented fastfiles",
		},
		{
			"mantle_anim_rate_max",
			"Max anim rate when sprinting at mantle",
		},
		{
			"mantle_anim_rate_min",
			"Min anim rate when mantling.",
		},
		{
			"mantle_check_angle",
			"The minimum angle from the player to a mantle surface to allow a mantle",
		},
		{
			"mantle_check_radius",
			"The player radius to test against while mantling",
		},
		{
			"mantle_check_range",
			"",
		},
		{
			"mantle_check_range_smooth",
			"",
		},
		{
			"mantle_debug",
			"Show debug information for mantling",
		},
		{
			"mantle_debugLineTime",
			"How long to show lines when mantle_debug lines",
		},
		{
			"mantle_enable",
			"Enable player mantling",
		},
		{
			"mantle_max_time",
			"Max time (ms) to mantle. Can take less time depending on speed when approaching mantle. Must be greater than mantle_min_time",
		},
		{
			"mantle_min_over_dist",
			"The min mantle over distance a player can traverse horizontally. The over distance can increase depending on speed.",
		},
		{
			"mantle_on_dist_override",
			"If non-zero, overrides the minimum mantle on distance",
		},
		{
			"mantle_over_dist_21",
			"The max mantle over distance a player can traverse horizontally when sprinting for 57 height",
		},
		{
			"mantle_over_dist_27",
			"The max mantle over distance a player can traverse horizontally when sprinting for 57 height",
		},
		{
			"mantle_over_dist_33",
			"The max mantle over distance a player can traverse horizontally when sprinting for 57 height",
		},
		{
			"mantle_over_dist_39",
			"The max mantle over distance a player can traverse horizontally when sprinting for 57 height",
		},
		{
			"mantle_over_dist_45",
			"The max mantle over distance a player can traverse horizontally when sprinting for 57 height",
		},
		{
			"mantle_over_dist_51",
			"The max mantle over distance a player can traverse horizontally when sprinting for 57 height",
		},
		{
			"mantle_over_dist_57",
			"The max mantle over distance a player can traverse horizontally when sprinting for 57 height",
		},
		{
			"mantle_pitch_clamp_enabled",
			"Enable pitch clamping code",
		},
		{
			"mantle_pitch_default_contact_time",
			"When 3p animation doesn't have 'contact' notetrack, use this time to lerp into a fixed pitch",
		},
		{
			"mantle_pitch_default_return_time_on",
			"When 3p animation doesn't have 'return_pitch' notetrack on a mantle up, use this time to start lerping back to original pitch",
		},
		{
			"mantle_pitch_default_return_time_over",
			"When 3p animation doesn't have 'return_pitch' notetrack on a mantle up, use this time to start lerping back to original pitch",
		},
		{
			"mantle_pitch_return_lerp_time",
			"Time for pitch to return to original pitch when mantle started",
		},
		{
			"mantle_use_approach_angle",
			"Use the approach angle when mantling instead of surface normal",
		},
		{
			"mantle_view_yawcap",
			"The angle at which to restrict a sideways turn while mantling",
		},
		{
			"mapname",
			"current map name",
		},
		{
			"mapPackMask",
			"Mask of map packs that are available for debugging",
		},
		{
			"mapPackTwoEnabled",
			"",
		},
		{
			"marketing_active",
			"Are we allowed to enable Marketing Comms or not",
		},
		{
			"marketing_autorefresh",
			"automatically download new messages after reporting any message read",
		},
		{
			"marketing_refresh_time",
			"time in seconds to wait before refreshing marketing messages from demonware",
		},
		{
			"marketing_simulatefakemotd",
			"Simulate a fake Marketing MOTD",
		},
		{
			"match_making_telemetry_chance",
			"The % chance of sending match making telemetry",
		},
		{
			"matchdata_active",
			"Are match data uploads enabled",
		},
		{
			"matchdata_maxcompressionbuffer",
			"",
		},
		{
			"max_ping_threshold_good",
			"max ping value to be considered as good",
		},
		{
			"max_ping_threshold_medium",
			"max ping value to be considered as medium",
		},
		{
			"maxPrestigeOverride",
			"Overrides the maximum prestige level, disabled if 0.",
		},
		{
			"mdsd",
			"enable match data stat delta logging?",
		},
		{
			"meetPlayer_ListUpdateInterval",
			"Time in milliseconds since uploaded recent met player list.",
		},
		{
			"meetPlayer_ListUploadInterval",
			"This dvar is used to make sure that recent met player list get updates only when it changes.",
		},
		{
			"melee_debug",
			"Turn on debug lines for melee traces",
		},
		{
			"mis_cheat",
			"Set when level unlock cheat is performed",
		},
		{
			"missileCoastingSpeed",
			"Speed of missiles when coasting.",
		},
		{
			"missileDebugAttractors",
			"Draw the attractors and repulsors.  Attractors are green, and repulsors are yellow. 0 = off, 1 = show origin, 2 = draw sphere, 3 = draw sphere depth checked",
		},
		{
			"missileDebugDraw",
			"Draw guided missile trajectories.",
		},
		{
			"missileDebugText",
			"Print debug missile info to console.",
		},
		{
			"missileExplosionLiftDistance",
			"Distance to lift the explosion off the surface for \"big explosion\" weapons",
		},
		{
			"missileGlassShatterVel",
			"Velocity needed by a grenade or missile to shatter glass instead of bouncing off.",
		},
		{
			"missileHellfireMaxSlope",
			"This limits how steeply the hellfire missile can turn upward when climbing",
		},
		{
			"missileHellfireUpAccel",
			"The rate at which the hellfire missile curves upward",
		},
		{
			"missileJavAccelClimb",
			"Rocket acceleration when climbing.",
		},
		{
			"missileJavAccelDescend",
			"Rocket acceleration when descending towards target.",
		},
		{
			"missileJavClimbAngleDirect",
			"In direct-fire mode, the minimum angle between the rocket and target until the rocket stops climbing.  Smaller angles make for higher climbs.",
		},
		{
			"missileJavClimbAngleTop",
			"In top-fire mode, the minimum angle between the rocket and target until the rocket stops climbing.  Smaller angles make for higher climbs.",
		},
		{
			"missileJavClimbCeilingDirect",
			"In direct-fire mode, how high the missile needs to reach before it descends.",
		},
		{
			"missileJavClimbCeilingTop",
			"In top-fire mode, how high the missile needs to reach before it descends.",
		},
		{
			"missileJavClimbHeightDirect",
			"In direct-fire mode, how far above the target the rocket will aim for when climbing.",
		},
		{
			"missileJavClimbHeightTop",
			"In top-fire mode, how far above the target the rocket will aim for when climbing.",
		},
		{
			"missileJavClimbToOwner",
			"",
		},
		{
			"missileJavDuds",
			"If true, javelins that impact before their booster ignites will not explode, they will play their dud effects instead.",
		},
		{
			"missileJavSpeedLimitClimb",
			"Rocket's speed limit when climbing.",
		},
		{
			"missileJavSpeedLimitDescend",
			"Rocket's speed limit when descending towards target.",
		},
		{
			"missileJavTurnDecel",
			"",
		},
		{
			"missileJavTurnRateDirect",
			"In direct-fire mode, how sharp the rocket can turn, in angles/sec.",
		},
		{
			"missileJavTurnRateTop",
			"In top-fire mode, how sharp the rocket can turn, in angles/sec.",
		},
		{
			"missileMacross",
			"Swarmy goodness.",
		},
		{
			"missileRemoteFOV",
			"Remote missile-cam, FOV to use.",
		},
		{
			"missileRemoteSpeedDown",
			"Remote-controlled missile slowdown-factor.",
		},
		{
			"missileRemoteSpeedTargetRange",
			"Remote-controlled missile speeds.",
		},
		{
			"missileRemoteSpeedUp",
			"Remote-controlled missile speedup-factor.",
		},
		{
			"missileRemoteSteerPitchRange",
			"Remote-controlled missile allowed up/down range.  To keep players from steering missiles above the horizon.",
		},
		{
			"missileRemoteSteerPitchRate",
			"Remote-controlled missile up/down steering speed.",
		},
		{
			"missileRemoteSteerYawRate",
			"Remote-controlled missile left/right steering speed.",
		},
		{
			"missileWaterMaxDepth",
			"If a missile explodes deeper under water than this, they explosion effect/sound will not play.",
		},
		{
			"modPrvAnimApplyDelta",
			"Model previewer animation apply delta",
		},
		{
			"modPrvAnimBlendMode",
			"Model previewer animation blending mode",
		},
		{
			"modPrvAnimBlendWeight",
			"Model previewer animation blend weight",
		},
		{
			"modPrvAnimCrossBlendDuration",
			"Model previewer animation cross blend duration",
		},
		{
			"modPrvAnimCrossBlendTime",
			"Model previewer animation cross blending time",
		},
		{
			"modPrvAnimForceLoop",
			"Model Previewer - Force an animation loop",
		},
		{
			"modPrvAnimMruName0",
			"Model previewer most recently used anim name 0",
		},
		{
			"modPrvAnimMruName1",
			"Model previewer most recently used anim name 0",
		},
		{
			"modPrvAnimMruName2",
			"Model previewer most recently used anim name 0",
		},
		{
			"modPrvAnimMruName3",
			"Model previewer most recently used anim name 0",
		},
		{
			"modPrvAnimRate",
			"Model previewer - animation rate",
		},
		{
			"modPrvCenterOffset",
			"Model previewer center offset",
		},
		{
			"modPrvDisplayToggle",
			"Show model previewer overlay",
		},
		{
			"modPrvDrawAxis",
			"Draw the model previewer axes",
		},
		{
			"modPrvDrawBoneInfo",
			"Draw model previewer bone information",
		},
		{
			"modPrvDrawDistanceToModel",
			"Print viewer's distance to model.",
		},
		{
			"modPrvForceUpdate",
			"Force modPrvLoadModel to be re-evaluated.",
		},
		{
			"modPrvFromAnimMru",
			"Model previewer most recently used 'from' animation",
		},
		{
			"modPrvGamepadControlSpeed",
			"Model previewer game pad control speed",
		},
		{
			"modPrvHideModel",
			"Skip drawing the model.",
		},
		{
			"modPrvLoadFromAnim",
			"Model previewer loaded 'from' animation",
		},
		{
			"modPrvLoadModel",
			"Model previewer loaded model",
		},
		{
			"modPrvLoadToAnim",
			"Model previewer loaded 'to' animation",
		},
		{
			"modPrvLod",
			"Model previewer level of detail",
		},
		{
			"modPrvMatOverrideFrom",
			"This material in the current model will be replaced by modPrvMatOverrideTo",
		},
		{
			"modPrvMatOverrideTo",
			"This material will replace the material specified by modPrvMatOverrideFrom in the current model",
		},
		{
			"modPrvMatReplace",
			"Model previewer material replace",
		},
		{
			"modPrvMatSelect",
			"Model previewer material select",
		},
		{
			"modPrvModelMru",
			"Model previewer most recently used model",
		},
		{
			"modPrvModelMruName0",
			"Model previewer most recently used model",
		},
		{
			"modPrvModelMruName1",
			"Model previewer most recently used model",
		},
		{
			"modPrvModelMruName2",
			"Model previewer most recently used model",
		},
		{
			"modPrvModelMruName3",
			"Model previewer most recently used model",
		},
		{
			"modPrvOrigin",
			"Model previewer origin",
		},
		{
			"modPrvRotationAngles",
			"Model previwer rotation angles",
		},
		{
			"modPrvToAnimMru",
			"Model previewer most recently used 'to' animation",
		},
		{
			"monkeytoy",
			"Restrict console access",
		},
		{
			"motd",
			"Message of the day",
		},
		{
			"motd_store_link",
			"Add a link to the in-game store in the MOTD popup",
		},
		{
			"motionTrackerBlurDuration",
			"The motion blur duration for motion tracker dots",
		},
		{
			"motionTrackerCenterX",
			"",
		},
		{
			"motionTrackerCenterY",
			"",
		},
		{
			"motionTrackerPingFadeTime",
			"How long an enemy is visible on the motion tracker after being detected",
		},
		{
			"motionTrackerPingPitchAddPerEnemy",
			"The added percentage of pitch for each additional enemy that is detected (final pitch = base pitch * (1 + enemy count * this))",
		},
		{
			"motionTrackerPingPitchBase",
			"The pitch of the motion tracker sound for a nearby enemy",
		},
		{
			"motionTrackerPingPitchNearby",
			"The pitch of the motion tracker sound for a nearby enemy",
		},
		{
			"motionTrackerPingSize",
			"The width and height of the motion tracker's enemy indicators as a percentage of motion tracker scale",
		},
		{
			"motionTrackerRange",
			"The range, in world units, that the motion tracker displays",
		},
		{
			"motionTrackerSweepAngle",
			"The maximum angle from straight forward that the motion tracker detects enemies",
		},
		{
			"motionTrackerSweepInterval",
			"The time between motion tracker sweeps",
		},
		{
			"motionTrackerSweepSpeed",
			"The speed, in world units per second, of the motion tracker sweep",
		},
		{
			"moving_platform_display_antilag",
			"Displays mover parent entities that will be included in the antilag calculation for moving platforms. Children are not displayed.",
		},
		{
			"moving_platform_improved_aim",
			"Turns on improved moving platform aim system.",
		},
		{
			"moving_platform_keep_previous",
			"Allows jumping between moving platforms by remembering the player's previous platform while in mid-air.",
		},
		{
			"moving_platform_rotational_antilag",
			"Applies rotational antilag to script movers for moving platform system.",
		},
		{
			"nextmap",
			"The next map name",
		},
		{
			"nightVisionDisableEffects",
			"",
		},
		{
			"nightVisionFadeInOutTime",
			"How long the fade to/from black lasts when putting on or removing night vision goggles.",
		},
		{
			"nightVisionPowerOnTime",
			"How long the black-to-nightvision fade lasts when turning on the goggles.",
		},
		{
			"np_plus_callback_interval_allowed",
			"Amount of time we allow before a missed callback means a person no longer has plus access.\n",
		},
		{
			"np_status_callback_interval",
			"Amount of time between regular calls to sceNpCheckNpAvailability.\n",
		},
		{
			"np_status_retry_callback_interval",
			"Amount of time to wait after a failure in sceNpCheckNpAvailability before retrying.\n",
		},
		{
			"num_available_map_packs",
			"Number of map packs available for this platform",
		},
		{
			"objectiveAlpha",
			"Alpha value for objective waypoints.",
		},
		{
			"objectiveAlphaEnabled",
			"When true, dvar \"objectiveAlpha\" takes effect.",
		},
		{
			"objectiveArrowHeight",
			"Height of the objective pointer.",
		},
		{
			"objectiveArrowWidth",
			"Width of the objective pointer.",
		},
		{
			"objectiveFadeTimeGoingOff",
			"Onscreen Objective Pointer - How long to take to fade out.",
		},
		{
			"objectiveFadeTimeGoingOn",
			"Onscreen Objective Pointer - How long to take to fade out.",
		},
		{
			"objectiveFadeTimeWaitOff",
			"Onscreen Objective Pointer - How long to take to fade out.",
		},
		{
			"objectiveFadeTimeWaitOn",
			"Onscreen Objective Pointer - How long to take to fade out.",
		},
		{
			"objectiveFadeTooClose",
			"Onscreen Objective Pointer - Will not fade out if target ent is farther than this.",
		},
		{
			"objectiveFadeTooFar",
			"Onscreen Objective Pointer - Will not fade out if target ent is farther than this.",
		},
		{
			"objectiveFontSize",
			"Onscreen Objective Pointer - Fontsize of the icon's text.",
		},
		{
			"objectiveHide",
			"When enabled, objectives will not show.",
		},
		{
			"objectiveHideIcon",
			"When true, hides the objective pointer's icon, but will still show the arrow.",
		},
		{
			"objectiveTextOffsetY",
			"Onscreen Objective Pointer - Offset of the icon's text.",
		},
		{
			"overrideNVGModelWithKnife",
			"When true, nightvision animations will attach the weapDef's knife model instead of the night vision goggles.",
		},
		{
			"painVisionLerpInRate",
			"Rate at which pain vision effect lerps in",
		},
		{
			"painVisionLerpOutRate",
			"Rate at which pain vision effect lerps in",
		},
		{
			"partyChatDisallowed",
			"Whether to disallow ps4 Live Party Chat",
		},
		{
			"partyChatDisconnectTimer",
			"Time to wait after user responds to Party Chat dialog before kicking (msec)",
		},
		{
			"past_title_data_active",
			"Is the past title data system enabled",
		},
		{
			"past_title_data_read_failure_interval_hours",
			"default interval in which to try and read past title data if kicked off read failed.",
		},
		{
			"past_title_data_read_success_interval_hours",
			"default interval in which to try and read past title data if kicked off read failed.",
		},
		{
			"path_nodeInfoType",
			"Path node information type",
		},
		{
			"perk_armorVestDamageMultiplier",
			"Multiplier affecting damage while wearing armor vest",
		},
		{
			"perk_bulletPenetrationMultiplier",
			"Multiplier for extra bullet penetration",
		},
		{
			"perk_extendedMagsMGAmmo",
			"Number of extra bullets per clip for machine gun weapons with the extended mags perk.",
		},
		{
			"perk_extendedMagsPistolAmmo",
			"Number of extra bullets per clip for pistol weapons with the extended mags perk.",
		},
		{
			"perk_extendedMagsSMGAmmo",
			"Number of extra bullets per clip for sub machine gun weapons with the extended mags perk.",
		},
		{
			"perk_extendedMagsSpreadAmmo",
			"Number of extra bullets per clip for spread weapons with the extended mags perk.",
		},
		{
			"perk_extendedMeleeRange",
			"The range of the auto melee with the extened perk",
		},
		{
			"perk_extraBreath",
			"Number of extra seconds a player can hold his breath",
		},
		{
			"perk_fastOffhandMultiplier",
			"Percentage of switching and using offhand weapon time to use",
		},
		{
			"perk_fastSnipeScale",
			"Scales the recovery speed of the view kick when using a sniper.",
		},
		{
			"perk_flinchMultiplier",
			"Multiplier affecting flinch",
		},
		{
			"perk_footstepVolumeAlly",
			"",
		},
		{
			"perk_footstepVolumeEnemy",
			"",
		},
		{
			"perk_footstepVolumePlayer",
			"",
		},
		{
			"perk_footstepVolumeSelectiveHearingMin",
			"",
		},
		{
			"perk_improvedExtraBreath",
			"Number of extra seconds a player can hold his breath",
		},
		{
			"perk_lightWeightViewBobScale",
			"Scale for first person view movement while lightweight.",
		},
		{
			"perk_mantleSpeedMultiplier",
			"Multiplier affecting mantling speed",
		},
		{
			"perk_numExtraLethal",
			"Number of extra lethal grenades",
		},
		{
			"perk_numExtraTactical",
			"Number of extra tactical grenades",
		},
		{
			"perk_parabolicAngle",
			"Eavesdrop perk's effective FOV angle",
		},
		{
			"perk_parabolicIcon",
			"Eavesdrop icon to use when displaying eavesdropped voice chats",
		},
		{
			"perk_parabolicRadius",
			"Eavesdrop perk's effective radius",
		},
		{
			"perk_quickDrawSpeedScale",
			"Scales the 'Hip to ADS' transition speed.",
		},
		{
			"perk_recoilMultiplier",
			"Multiplier affecting recoil",
		},
		{
			"perk_resistExplosionDamageMultiplier",
			"Multiplier affecting damage with resist explosion",
		},
		{
			"perk_resistShellShockMultiplier",
			"Multiplier affecting damage with resist explosion",
		},
		{
			"perk_sprintMultiplier",
			"Multiplier for player_sprinttime",
		},
		{
			"perk_sprintRecoveryMultiplierActual",
			"",
		},
		{
			"perk_sprintRecoveryMultiplierVisual",
			"",
		},
		{
			"perk_weapRateMultiplier",
			"Percentage of weapon firing rate to use",
		},
		{
			"perk_weapReloadMultiplier",
			"Percentage of weapon reload time to use",
		},
		{
			"perk_weapSpreadMultiplier",
			"Percentage of weapon spread to use",
		},
		{
			"perk_weapSwapMultiplier",
			"Percentage of weapon swap time to use",
		},
		{
			"phsyVeh_enableBalanceLoad",
			"Force the load at each wheel to share the entire weight of the vehicle equally.  (Requires map restart)",
		},
		{
			"phys_autoDisableLinear",
			"A body must have linear velocity less than this to be considered idle.",
		},
		{
			"phys_autoDisableTime",
			"The amount of time a body must be idle for it to go to sleep.",
		},
		{
			"phys_bulletSpinScale",
			"Scale of the effective offset from the center of mass for the bullet impacts.",
		},
		{
			"phys_bulletUpBias",
			"Up Bias for the direction of the bullet impact.",
		},
		{
			"phys_dragAngular",
			"The amount of angular drag, applied globally",
		},
		{
			"phys_dragLinear",
			"The amount of linear drag, applied globally",
		},
		{
			"phys_drawAwake",
			"Debug draw a box indicating which bodies are disabled",
		},
		{
			"phys_drawBroadPhase",
			"Debug draw broadphase AABBs",
		},
		{
			"phys_drawCenterOfMass",
			"Debug draw center of masses",
		},
		{
			"phys_drawCollision",
			"Draw collision for specified world",
		},
		{
			"phys_drawContacts",
			"Debug draw contact points",
		},
		{
			"phys_drawDebugInfo",
			"Print info about the physics objects",
		},
		{
			"phys_drawDistance",
			"Size of region centered around player to draw debug physics information",
		},
		{
			"phys_drawJoints",
			"Debug draw joints",
		},
		{
			"phys_dumpWorld",
			"Debug dump out physics world",
		},
		{
			"phys_earthquakeStrengthScale",
			"The earthquake force overall strength multiplier.  Multiplies the scale passed into Earthquake script function.",
		},
		{
			"phys_earthquakeWaveFrequency",
			"The earthquake force wave frequency of oscillation",
		},
		{
			"phys_earthquakeWaveLength",
			"The earthquake force wave length",
		},
		{
			"phys_enableContinuousCollision",
			"Enable continuous collision detection",
		},
		{
			"phys_enablePicking",
			"Enable picking up physics bodies.  Hover over dynamic physics body, then press and hold L3 to activate.",
		},
		{
			"phys_enableSleep",
			"Enable rigid body deactivation",
		},
		{
			"phys_gravity",
			"Physics gravity in units/sec^2.",
		},
		{
			"phys_gravity_ragdoll",
			"Physics gravity used by ragdolls in units/sec^2.",
		},
		{
			"phys_gravityChangeWakeupRadius",
			"The radius around the player within which objects get awakened when gravity changes",
		},
		{
			"phys_jitterMaxMass",
			"Maximum mass to jitter - jitter will fall off up to this mass",
		},
		{
			"phys_maxTimeStep",
			"Max physics time step in seconds",
		},
		{
			"phys_rollingResistance",
			"The amount of rolling resistance, applied globally",
		},
		{
			"phys_solverPositionIterations",
			"Number of solver position iterations used for constraint solving.",
		},
		{
			"phys_solverVelocityIterations",
			"Number of solver velocity iterations used for constraint solving.",
		},
		{
			"phys_solverWarmStarting",
			"Enable solver warm starting for better stability",
		},
		{
			"physVeh_antiRollScale",
			"Amount to scale antiroll force (Requires map restart)",
		},
		{
			"physVeh_dampingCoefficientScale",
			"Scale applied to the suspension damping strength.  (Requires map restart)",
		},
		{
			"physVeh_downForceScale",
			"Max amount of downforce proportional to gravity that is applied.  (Requires map restart)",
		},
		{
			"physVeh_explodeForce",
			"The force applied to physics vehicles due to explosions",
		},
		{
			"physVeh_explodeSpinScale",
			"The max (random) offset from the center of mass at which splash damage applies its force",
		},
		{
			"physVeh_jump",
			"Set to 1 to make a vehicle jump.",
		},
		{
			"physVeh_lateralFrictionScale",
			"Scale applied to lateral/side friction.  (Requires map restart)",
		},
		{
			"physVeh_minContactImpulse",
			"The minimum impulse needed to register a contact notification",
		},
		{
			"physVeh_minImpactMomentum",
			"The minimum collision momentum needed to register an impact",
		},
		{
			"physVeh_sideToFrontFrictionScale",
			"Ratio between longitudinal friction and lateral friction.  (Requires map restart)",
		},
		{
			"physVeh_speedIntegralMaxError",
			"The absolute accumulated error between desired - current speed over time is clamped by this value over time.  Lower this value to limit overshoot of target speed due to integral windup.",
		},
		{
			"physVeh_springCoefficientScale",
			"Scale applied to the suspension spring strength.  (Requires map restart)",
		},
		{
			"physVeh_StepsPerFrame",
			"The number of physics timesteps that the server frame will be divided into.",
		},
		{
			"pickupPrints",
			"Print a message to the game window when picking up ammo, etc.",
		},
		{
			"player_adsExitDelayGamepad",
			"Delay before exiting aim down sight with mouse",
		},
		{
			"player_adsExitDelayMouse",
			"Delay before exiting aim down sight with mouse",
		},
		{
			"player_backSpeedScale",
			"The scale applied to the player speed when strafing",
		},
		{
			"player_breath_fire_delay",
			"The amount of time subtracted from the player remaining breath time when a weapon is fired",
		},
		{
			"player_breath_gasp_lerp",
			"The interpolation rate for the target waver amplitude when gasping",
		},
		{
			"player_breath_gasp_scale",
			"Scale value to apply to the target waver during a gasp",
		},
		{
			"player_breath_gasp_time",
			"The amount of time a player will gasp once they can breath again",
		},
		{
			"player_breath_hold_lerp",
			"The interpolation rate for the target waver amplitude when gasping",
		},
		{
			"player_breath_hold_time",
			"The maximum time a player can hold his breath",
		},
		{
			"player_breath_snd_delay",
			"The delay before playing the breathe in sound",
		},
		{
			"player_breath_snd_lerp",
			"The interpolation rate for the player hold breath sound",
		},
		{
			"player_damageMultiplier",
			"Player damage is scaled by this amount; used by script for setting difficulty level",
		},
		{
			"player_deathInvulnerableToMelee",
			"The player is invulnerable to melee attacks during death invulnerability",
		},
		{
			"player_deathInvulnerableToProjectile",
			"The player is invulnerable to projectile attacks during death invulnerability",
		},
		{
			"player_debugHealth",
			"Turn on debugging info for player health",
		},
		{
			"player_debugTrace",
			"Show player trace debug planes",
		},
		{
			"player_dmgtimer_flinchTime",
			"Maximum time to play flinch animations",
		},
		{
			"player_dmgtimer_maxTime",
			"The maximum time that the player is slowed due to damage",
		},
		{
			"player_dmgtimer_minScale",
			"The minimum scale value to slow the player by when damaged",
		},
		{
			"player_dmgtimer_stumbleTime",
			"Maximum time to play stumble animations",
		},
		{
			"player_dmgtimer_timePerPoint",
			"The time in milliseconds that the player is slowed down per point of damage",
		},
		{
			"player_footstepsThreshhold",
			"The minimum speed at which the player makes loud footstep noises",
		},
		{
			"player_footstepsThreshholdCrouch",
			"The minimum speed at which the player makes loud footstep noises",
		},
		{
			"player_footstepsThreshholdProne",
			"The minimum speed at which the player makes loud footstep noises",
		},
		{
			"player_lastStandCrawlSpeedScale",
			"The scale applied to the player speed when strafing",
		},
		{
			"player_lastStandDebug",
			"Forces players into last stand for debugging purposes.",
		},
		{
			"player_lateralPlane",
			"The plane used for the player's lateral movement.",
		},
		{
			"player_meleeChargeFriction",
			"Friction used during melee charge",
		},
		{
			"player_meleeChargeHeightTolerance",
			"Height tolerance to allow charging to a victim.  If victim and attacker heights are greater than this value, attack will always do swipe",
		},
		{
			"player_meleeChargeMaxSpeed",
			"Max speed to clamp when doing a charge melee",
		},
		{
			"player_meleeChargeMaxSpeedUp",
			"Max vertical (up) speed to clamp when doing a charge melee",
		},
		{
			"player_meleeChargePlayerLockTime",
			"The amount of time, in milliseconds, to lock the player's movement when performing a melee charge",
		},
		{
			"player_meleeDamageMultiplier",
			"Melee damage to the player is scaled by this amount; used by script for setting difficulty level",
		},
		{
			"player_meleeForceAlternate",
			"Forces the player to use alternate melee even if the PWF is not set.",
		},
		{
			"player_meleeForceServerMiss",
			"Force the server to always miss melee",
		},
		{
			"player_meleeHeight",
			"The height of the player's melee attack",
		},
		{
			"player_meleeIdealEndDistance",
			"Ideal distance that the melee attacker and victim should be apart from each other when melee ends. -1 means off",
		},
		{
			"player_meleeMaxQuickRaiseTime",
			"After melee is done, max quickraise time. Allows raising weapon quicker to give more responsive feel after melee.",
		},
		{
			"player_meleeMinimumResponseGraceTime",
			"Melee Combo: max input roundtrip time (ms) before considering player to have skipped combo input",
		},
		{
			"player_meleeRange",
			"The maximum range of the player's melee attack",
		},
		{
			"player_meleeRangeCharge",
			"The maximum range of the player's melee attack",
		},
		{
			"player_meleeSwipePlayerLockTime",
			"When swiping, lock player view and movement for this number of milliseconds",
		},
		{
			"player_meleeSwipeViewLockReleaseTime",
			"When swiping, lock player view until this much weapon time left.",
		},
		{
			"player_meleeWidth",
			"The width of the player's melee attack",
		},
		{
			"player_MGUseRadius",
			"The radius within which a player can mount a machine gun",
		},
		{
			"player_moveThreshhold",
			"The speed at which the player is considered to be moving for the purposes of \nview model bob and multiplayer model movement",
		},
		{
			"player_on_vehicle_aim_fix",
			"Turns on/off aim fix while player is on a vehicle or platform. Defaults to on.",
		},
		{
			"player_prone_view_pitch_down",
			"Maximum angle that the player can look up",
		},
		{
			"player_prone_view_pitch_up",
			"Maximum angle that the player can look up",
		},
		{
			"player_radiusDamageMultiplier",
			"Radius damage to the player is scaled by this amount; used by script for setting difficulty level",
		},
		{
			"player_runThreshhold",
			"The speed threshold before a player is considered to be running forwards",
		},
		{
			"player_scope_prototype",
			"Enables the 'scope drift' system that takes the place of the current 'hold breath' system",
		},
		{
			"player_scopeExitOnDamage",
			"Exit the scope if the player takes damage",
		},
		{
			"player_shield_postUseWeaponDelay",
			"The delay between putting down the exo-shield and being able to fire your weapon again.",
		},
		{
			"player_spaceCapsuleHeight",
			"Player capsule height. 70 = standing, 50 = crouch, 30 = prone",
		},
		{
			"player_spaceEnabled",
			"True if player space logic is enabled. (Player must also be swim enabled)",
		},
		{
			"player_spaceViewHeight",
			"Camera view height. 60 = standing, 40 = crouch, 11 = prone",
		},
		{
			"player_spectateSpeedScale",
			"The scale applied to the player speed when strafing",
		},
		{
			"player_sprintCameraBob",
			"The speed the camera bobs while you sprint",
		},
		{
			"player_sprintForwardMinimum",
			"The minimum forward deflection required to maintain a sprint",
		},
		{
			"player_sprintJumpAnimRate",
			"If > 0, allow sprint to persist through jumps. The value also determins anim rate to play sprint loop at while jumping.",
		},
		{
			"player_sprintJumpDropWeaponScaler",
			"If 'player_sprintJumpAnimRate' is set, this dvar scales down the time sprint drop timer when player is in the air and wants to fire.",
		},
		{
			"player_sprintMinTime",
			"The minimum sprint time needed in order to start sprinting",
		},
		{
			"player_sprintRechargePause",
			"The length of time the meter will pause before starting to recharge after a player sprints",
		},
		{
			"player_sprintSpeedScale",
			"The scale applied to the player speed when strafing",
		},
		{
			"player_sprintStrafeSpeedScale",
			"The speed at which you can strafe while sprinting",
		},
		{
			"player_sprintTime",
			"The base length of time a player can sprint",
		},
		{
			"player_sprintUnlimited",
			"Whether players can sprint forever or not",
		},
		{
			"player_strafeAnimCosAngle",
			"Cosine of the angle which player starts using strafe animations",
		},
		{
			"player_strafeSpeedScale",
			"The scale applied to the player speed when strafing",
		},
		{
			"player_sustainAmmo",
			"Firing weapon will not decrease clip ammo.",
		},
		{
			"player_swimAcceleration",
			"Forward/lateral swim acceleration",
		},
		{
			"player_swimCombatTimer",
			"Time (ms) at which viewmodel switches from combat to relaxed animation state when not firing",
		},
		{
			"player_swimDragHandEnabled",
			"Enable/disable left hand drag during swimming",
		},
		{
			"player_swimDragHandFrictionMax",
			"Max friction value to bring the dragging hand back to default idle position",
		},
		{
			"player_swimDragHandFrictionMin",
			"Min friction value to bring the dragging hand back to default idle position",
		},
		{
			"player_swimDragHandLookAtOffset",
			"Forward axis offset to compare against last look at position for hand drag anims",
		},
		{
			"player_swimForwardAnimCatchupMax",
			"Lerped max speed to extend or retract arms if player is moving forward or back",
		},
		{
			"player_swimForwardAnimCatchupMin",
			"Lerped min speed to extend or retract arms if player is moving forward or back",
		},
		{
			"player_swimForwardMinAngle",
			"Min angle (relative to forward axis) for player to be in swim forward state",
		},
		{
			"player_swimForwardMinSpeed",
			"Min speed (relative to forward axis) for player to be in swim forward state",
		},
		{
			"player_swimForwardSettleTime",
			"Time (ms) that player will stay in swim forward state even if outside parameters when already in forward state",
		},
		{
			"player_swimForwardWarmupTime",
			"Time (ms) that player has to wait before moving into swim forward state once speed and angle are met",
		},
		{
			"player_swimFriction",
			"Friction value applied to velocity when swimming and no input is given",
		},
		{
			"player_swimSpeed",
			"Max forward/lateral swim speed",
		},
		{
			"player_swimVerticalAcceleration",
			"Vertical swim acceleration",
		},
		{
			"player_swimVerticalFriction",
			"Vertical friction value applied to velocity when swimming and no input is given",
		},
		{
			"player_swimVerticalSpeed",
			"Max vertical swim speed",
		},
		{
			"player_swimWaterSurface",
			"Water surface Z value",
		},
		{
			"player_swimWaterSurfaceEnabled",
			"Water surface Z value movement restriction enabled",
		},
		{
			"player_throwbackInnerRadius",
			"The radius to a live grenade player must be within initially to do a throwback",
		},
		{
			"player_throwbackOuterRadius",
			"The radius player is allow to throwback a grenade once the player has been in the inner radius",
		},
		{
			"player_useRadius",
			"The radius within which a player can use things",
		},
		{
			"player_view_pitch_down",
			"Maximum angle that the player can look up",
		},
		{
			"player_view_pitch_up",
			"Maximum angle that the player can look up",
		},
		{
			"player_walkCameraBob",
			"The speed the camera bobs while you walk.",
		},
		{
			"playercard_cache_download_max_retry_time",
			"Max time that the player cache download can retry",
		},
		{
			"playercard_cache_download_retry_step",
			"Step in m/s for the player cache download retry",
		},
		{
			"playercard_cache_show_cached",
			"Shows whether a playercard is in the cache or not",
		},
		{
			"playercard_cache_upload_max_retry_time",
			"Max time that the player cache upload can retry",
		},
		{
			"playercard_cache_upload_retry_step",
			"Step in m/s for the player cache upload retry",
		},
		{
			"playercard_cache_validity_life",
			"The life of a cached gamercard (it can be re-downloaded after this)",
		},
		{
			"playerPositionRecordSampleTime",
			"How often to sample player positions and save them into match data.",
		},
		{
			"prestige_shop_active",
			"Are we allowed to show the Prestige Shop or not",
		},
		{
			"prestige30EasterEggEnabled",
			"Enables the easter egg for prestige 30 if 1, disabled if 0.",
		},
		{
			"printSnapshotDetails",
			"Log snapshot details (can slow down the game a lot)",
		},
		{
			"prof_probe0",
			"",
		},
		{
			"prof_probe1",
			"",
		},
		{
			"prof_probe2",
			"",
		},
		{
			"prof_probe3",
			"",
		},
		{
			"prof_probe4",
			"",
		},
		{
			"prof_probeMaxMsec",
			"Height of each profile probe graph, in milliseconds",
		},
		{
			"prof_sortTime",
			"Time in seconds between resort profiles",
		},
		{
			"profile",
			"",
		},
		{
			"profile_rowcount",
			"Profile row count",
		},
		{
			"profile_script",
			"Enable profile scripts",
		},
		{
			"profile_script_by_file",
			"Enable profile scripts by source file",
		},
		{
			"profile_script_hierarchical",
			"Toggle hierarchical drawing of script profiling",
		},
		{
			"profile_show_loading",
			"Show map load profiler",
		},
		{
			"profile_thread",
			"Thread being profiled",
		},
		{
			"profile2",
			"",
		},
		{
			"profile2_frames",
			"Profile frame count",
		},
		{
			"profile2_mode",
			"Reporting mode",
		},
		{
			"profileMenuOption_blacklevel",
			"",
		},
		{
			"profileMenuOption_offensiveContentMode",
			"Mode of the offensive content warning at startup - 0, skip and turn on; 1, skip and turn off; 2, ask user",
		},
		{
			"profileMenuOption_safeAreaHorz",
			"",
		},
		{
			"profileMenuOption_safeAreaVert",
			"",
		},
		{
			"profileMenuOption_sensitivity",
			"",
		},
		{
			"profileMenuOption_volume",
			"",
		},
		{
			"prologue_select",
			"Toggles intro.map between the prologue and the persona non grata setup.",
		},
		{
			"ps4_dw_disconnect_test",
			"Turns on automatic random disconnects from demonware to test connectivity code.\n",
		},
		{
			"ps4_how_long_to_wait_on_join_ms",
			"Delay in milliseconds we're willing to wait before shutting down a join in progress due to lack of friend.",
		},
		{
			"ps4_meetplayer_rest_submit_delay",
			"Number of milliseconds between calls to submit met players to the rest system.",
		},
		{
			"ps4_num_session_slots_override",
			"Override the max number of player slots in a ps4 session.  0 will use the proper number.",
		},
		{
			"ps4_presence_put_delay",
			"Delay in milliseconds after presence changes before sending updated presence through np",
		},
		{
			"ps4_presence_put_rate",
			"Minimum delay in milliseconds before sending updated presence through np",
		},
		{
			"ps4_signin_notify_delay",
			"Delay in milliseconds after a sign in status change before we change our sign-in status (in-game)",
		},
		{
			"ps4Game",
			"True if running on PS4",
		},
		{
			"publisherFileFetchTimeout",
			"default timeout for publisher files FETCH tasks (in seconds)",
		},
		{
			"r_adaptiveSubdiv",
			"Enables screen space Catmull Clark adaptive tessellation.  If disabled, models tessellate to their designed subdivision level.",
		},
		{
			"r_adaptiveSubdivBaseFactor",
			"Screen space Catmull Clark adaptive tessellation factor for the base model.  Smaller values mean more tessellation.",
		},
		{
			"r_adaptiveSubdivPatchFactor",
			"Screen space Catmull Clark adaptive tessellation factor for the base model.  Smaller values mean more tessellation.",
		},
		{
			"r_allCells",
			"Draw all cells.  Most useful for seeing if portals or cells are hiding things they should not..",
		},
		{
			"r_animBoundsScale",
			"Scale animated xmodel bounds by the given factor",
		},
		{
			"r_animBoundsWarn",
			"Warn when model bounds are estimated far off from their actual animated values.",
		},
		{
			"r_animEstimatedBoundsScale",
			"Scale animated xmodel estimated bounds by the given factor",
		},
		{
			"r_aoBlurSharpness",
			"Controls the tolerance for depth discontinuities during the bilateral blur step.  Larger values reduce the depth tolerance and effectively sharpen more edges.",
		},
		{
			"r_aoBlurStep",
			"Step scale applied to sample offsets during the bilateral blur.  A value of 1 results in a normal gaussian blur.  Increasing it to 2 or 3 makes the filter larger but causes fine dithering patterns.",
		},
		{
			"r_aoDiminish",
			"Decrease the effect of occlusion on brighter colors",
		},
		{
			"r_aoPower",
			"Power curve applied to AO factor",
		},
		{
			"r_aoStrength",
			"Strength of Ambient Occlusion effect",
		},
		{
			"r_aoUseTweaks",
			"Use r_ao* dvars instead of the current light set values for AO common params",
		},
		{
			"r_artTweaksLastVisionSet",
			"Tells the script which vision set was last set in code",
		},
		{
			"r_artUseTweaks",
			"Tells the game that art tweaks is enabled and script is in control (as opposed to ColorEd).",
		},
		{
			"r_asyncCompute",
			"Enables scheduling GPU compute shader work prior to the graphics frame, improving overlap.",
		},
		{
			"r_atlasAnimFPS",
			"Speed to animate atlased 2d materials",
		},
		{
			"r_balanceLightmapOpaqueLists",
			"Split lightmap opaque into multiple draw lists.",
		},
		{
			"r_balanceOpaqueLists",
			"Split opaque into multiple draw lists.",
		},
		{
			"r_blacklevel",
			"Black level (negative brightens output)",
		},
		{
			"r_blendshape_debug",
			"Entity number to debug blendshape weights",
		},
		{
			"r_blendshape_debug_index",
			"Blendshape weight index to debug/force; all others will be 0",
		},
		{
			"r_blendshape_debug_value",
			"Blendshape weight value to use for debug/force",
		},
		{
			"r_blendshape_emulation",
			"Enables blendshape emulation",
		},
		{
			"r_blendshape_enable",
			"Toggles blendshapes",
		},
		{
			"r_blur",
			"Dev tweak to blur the screen",
		},
		{
			"r_blurdstGaussianBlurLevel",
			"MIP level to start gaussian blur at",
		},
		{
			"r_blurdstGaussianBlurRadius",
			"Amount to gaussian blur blur distortion render target",
		},
		{
			"r_blurdstShowOverlay",
			"Toggles blur distortion overlay",
		},
		{
			"r_brightness",
			"Brightness adjustment",
		},
		{
			"r_cacheModelLighting",
			"Speed up model lighting by caching previous results",
		},
		{
			"r_cacheSModelLighting",
			"Speed up static model lighting by caching previous results",
		},
		{
			"r_clampLodScale",
			"Clamps the amount that the engine can adjust the LOD distance. 0 the engine can fully adjust. 1 the engine cannot adjust it at all. 0.5 the maximum the engine can adjust the LOD distance is 50% or the default.",
		},
		{
			"r_clear",
			"Controls how the color buffer is cleared",
		},
		{
			"r_clearColor",
			"Color to clear the screen to when clearing the frame buffer",
		},
		{
			"r_clearColor2",
			"Color to clear every second frame to (for use during development)",
		},
		{
			"r_clutCompositeVisionSet",
			"Composite clut with vision set.",
		},
		{
			"r_clutDebugOverlay",
			"Enable Clut Debug Overlay",
		},
		{
			"r_clutDumpAssets",
			"Dump Assets",
		},
		{
			"r_clutEnable",
			"Enable/Disable Clut",
		},
		{
			"r_cmdbuf_handoff",
			"Allow the backend thread to take over partially executed command buffer jobs.",
		},
		{
			"r_cmdbuf_worker",
			"Process command buffer in a separate thread",
		},
		{
			"r_colorblindMode",
			"Selects the Colorblind simulation mode",
		},
		{
			"r_colorGradingEnable",
			"Enable color grading.",
		},
		{
			"r_colorGradingFilmTweaks",
			"Enable/Disable Vision Set Film Tweaks",
		},
		{
			"r_colorGradingForceReinhard",
			"Enable/Disable Force Reinhard",
		},
		{
			"r_colorMap",
			"Replace all color maps with pure black or pure white",
		},
		{
			"r_colorScaleUseTweaks",
			"Override color scale LightSet settings with tweak dvar values. (MP)",
		},
		{
			"r_combinePostOpaqueFx",
			"",
		},
		{
			"r_contrast",
			"Contrast adjustment",
		},
		{
			"r_daltonizeIntensity",
			"Daltonize correction strength.",
		},
		{
			"r_daltonizeMode",
			"Selects the Daltonize mode",
		},
		{
			"r_darkBlur",
			"Apply blur (decrease of visual acuity) when dark",
		},
		{
			"r_darkBlurPower",
			"Power curve of blurring (decrease of visual acuity) when dark",
		},
		{
			"r_darkBlurRadius",
			"Radius of blurring (decrease of visual acuity) when dark",
		},
		{
			"r_darkColor",
			"Reduce color sensitivity when dark",
		},
		{
			"r_darkColorPower",
			"Power curve of color sensitivity when dark",
		},
		{
			"r_debugDrawLights",
			"Draw light debug info",
		},
		{
			"r_debugDrawLightsDrawRadius",
			"Max distance from camera to draw debug light info",
		},
		{
			"r_debugDrawLightsIndex",
			"Draw light index",
		},
		{
			"r_debugLineWidth",
			"Width of server side debug lines",
		},
		{
			"r_debugRefHeight",
			"Reference Image's Height",
		},
		{
			"r_debugRefImageLeft",
			"Reference Image's Left Screen Pos.",
		},
		{
			"r_debugRefImagePrePostFx",
			"Draw Debug Reference before postFx.",
		},
		{
			"r_debugRefImageTop",
			"Reference Image's Top Screen Pos.",
		},
		{
			"r_debugRefWidth",
			"Reference Image's Width ",
		},
		{
			"r_debugShader",
			"Enable shader debugging information",
		},
		{
			"r_defaultPatchCount",
			"Patches per thread group for all other surfaces.",
		},
		{
			"r_delayAddSceneModels",
			"Add DObjs and brushes to GfxScene in a separate worker command",
		},
		{
			"r_depthPrepass",
			"Enable depth prepass for various geometries",
		},
		{
			"r_depthSortDebug",
			"Enable depth sort debug visualization.",
		},
		{
			"r_depthSortEnable",
			"Enable sorting of transparent surfaces.",
		},
		{
			"r_depthSortRange",
			"Range to consider depth sort,",
		},
		{
			"r_desaturation",
			"Desaturation adjustment",
		},
		{
			"r_detailMap",
			"Replace all detail maps with an image that effectively disables them",
		},
		{
			"r_diffuseColorScale",
			"Globally scale the diffuse color of all point lights",
		},
		{
			"r_disableLightSets",
			"Disable LightSets",
		},
		{
			"r_displacementMap",
			"Replace all displacement maps with an image that effectively disables them",
		},
		{
			"r_displacementPatchCount",
			"Patches per thread group for displacement surfaces.",
		},
		{
			"r_distortion",
			"Enable distortion",
		},
		{
			"r_distortion_script_force_off",
			"Force distortion off in script",
		},
		{
			"r_dlightLimit",
			"Maximum number of dynamic lights drawn simultaneously",
		},
		{
			"r_dof_bias",
			"Depth of field bias as a power function (like gamma); less than 1 is sharper",
		},
		{
			"r_dof_enable",
			"Enable the depth of field effect",
		},
		{
			"r_dof_farBlur",
			"",
		},
		{
			"r_dof_farEnd",
			"Depth of field far end distance, in inches",
		},
		{
			"r_dof_farStart",
			"Depth of field far start distance, in inches",
		},
		{
			"r_dof_nearBlur",
			"",
		},
		{
			"r_dof_nearEnd",
			"Depth of field near end distance, in inches",
		},
		{
			"r_dof_nearStart",
			"Depth of field near start distance, in inches",
		},
		{
			"r_dof_physical_accurateFov",
			"Enable physical fov (but still based on cg_fov). This will make the fov to subtlety change depending on the focus distance",
		},
		{
			"r_dof_physical_adsFocusSpeed",
			"ADS focus speed (focus dist. far to near, focus dist. near to far, aperture opening, aperture closing)",
		},
		{
			"r_dof_physical_adsMaxFstop",
			"ADS maximum f-stop (optimal aperture and focus distance are automatically calculated for this mode)",
		},
		{
			"r_dof_physical_adsMinFstop",
			"ADS minimum f-stop (optimal aperture and focus distance are automatically calculated for this mode)",
		},
		{
			"r_dof_physical_bokehEnable",
			"Enable the bokeh depth of field effect",
		},
		{
			"r_dof_physical_bokehPreset",
			"Changes dof sampling quality",
		},
		{
			"r_dof_physical_bokehRotation",
			"Bokeh shape rotation in degrees (hexagonal and octogonal only)",
		},
		{
			"r_dof_physical_bokehShape",
			"Changes the bokeh shape",
		},
		{
			"r_dof_physical_bokehSharpness",
			"Bokeh shape sharpness, trades sharpness for stability (circular only)",
		},
		{
			"r_dof_physical_distanceMeter",
			"Enable physical depth of field debug information",
		},
		{
			"r_dof_physical_enable",
			"enable physical camera controls (using aperture priority)",
		},
		{
			"r_dof_physical_filmDiagonal",
			"Diagonal size of the film/sensor (mm). The bigger the sensor size, the bigger the circle of confusion (which means stronger blurring at all distances). Defaults to full-frame 35mm",
		},
		{
			"r_dof_physical_focusDistance",
			"Distance to the plane in focus for the scene",
		},
		{
			"r_dof_physical_fstop",
			"Aperture of the camera for the scene. Lower f-stop yields a shallower depth of field. Typical values range from f/1 to f/22. Rare extremes are f/0.75 and f/32",
		},
		{
			"r_dof_physical_hipEnable",
			"Enable hyperfocal mode",
		},
		{
			"r_dof_physical_hipFocusSpeed",
			"Hyperfocal mode focus speed (focus dist. far to near, focus dist. near to far, aperture opening, aperture closing)",
		},
		{
			"r_dof_physical_hipFstop",
			"Aperture of the camera for the scene in the hyperfocal mode",
		},
		{
			"r_dof_physical_hipSharpCocDiameter",
			"Defines what circle of confusion can be considered sharp (mm). Defaults to 0.03mm, generally accepted value for 35mm",
		},
		{
			"r_dof_physical_maxCocDiameter",
			"Maximum circle of confusion diameter (virtual units, might be clamped for bokeh dof)",
		},
		{
			"r_dof_physical_minFocusDistance",
			"Minimum focus distance (inches)",
		},
		{
			"r_dof_physical_viewModelFocusDistance",
			"Distance to the plane in focus for the scene",
		},
		{
			"r_dof_physical_viewModelFstop",
			"Aperture of the camera for the view model. Lower f-stop yields a shallower depth of field. Typical values range from f/1 to f/22. Rare extremes are f/0.75 and f/32",
		},
		{
			"r_dof_tweak",
			"Use dvars to set the depth of field effect; overrides r_dof_enable",
		},
		{
			"r_dof_viewModelEnd",
			"Depth of field viewmodel end distance, in inches",
		},
		{
			"r_dof_viewModelStart",
			"Depth of field viewmodel start distance, in inches",
		},
		{
			"r_dpvsFilterDebug",
			"Filter all entities to all cells (debug)",
		},
		{
			"r_draw_frustum",
			"Draw the frustum using debug lines",
		},
		{
			"r_drawBModels",
			"Enable brush model rendering",
		},
		{
			"r_drawDebugRefImage",
			"Draw Debug Reference Image.",
		},
		{
			"r_drawDynEnts",
			"Enable dynamic entity rendering",
		},
		{
			"r_drawEntities",
			"Enable entity rendering",
		},
		{
			"r_drawPoly",
			"Enable poly rendering",
		},
		{
			"r_drawPrimHistogram",
			"Draws a histogram of the sizes of each primitive batch",
		},
		{
			"r_drawRigidModels",
			"Enable rigid model rendering",
		},
		{
			"r_drawSkinnedModels",
			"Enable skinned model rendering",
		},
		{
			"r_drawSModels",
			"Fraction of static models to render",
		},
		{
			"r_drawStreaming",
			"Makes materials with textures that are not yet loaded draw in pink",
		},
		{
			"r_drawSun",
			"Enable sun effects",
		},
		{
			"r_drawTessellatedWorld",
			"Draw tessellated world surfaces",
		},
		{
			"r_drawWater",
			"Enable water animation",
		},
		{
			"r_drawWorld",
			"Enable world rendering",
		},
		{
			"r_drawXModels",
			"Enable xmodel rendering",
		},
		{
			"r_dumpViewInfo",
			"Dump all the info in the GfxViewInfo structure",
		},
		{
			"r_dynamicLight",
			"Toggle VFX ( omni & spot ) light, debug only",
		},
		{
			"r_dynamicOPL",
			"Enable drawing vfx lights as overlapping primary light for saving gpu performance.",
		},
		{
			"r_dynamicSpotLightShadows",
			"Enable shadows for dynamic/VFX spot lights, you should set this dvar then spawn the new light.",
		},
		{
			"r_emblemBrightnessScale",
			"Modifier that scales the brightness of the emblem on model materials",
		},
		{
			"r_emissiveMap",
			"Replace all emissive maps with pure black or pure white",
		},
		{
			"r_enableNoTessBuckets",
			"Enables placing triangles that don't need tessellation into additional draw calls using non-tessellated shaders.",
		},
		{
			"r_envBrdfLutMap",
			"Replace environment BRDF lookup table with pure black (no secondary specular) or pure white (maximum secondary specular)",
		},
		{
			"r_envMapExponent",
			"Reflection exponent.",
		},
		{
			"r_envMapMaxIntensity",
			"Max reflection intensity based on glancing angle.",
		},
		{
			"r_envMapMinIntensity",
			"",
		},
		{
			"r_envMapOverride",
			"",
		},
		{
			"r_envMapSunIntensity",
			"Max sun specular intensity intensity with env map materials.",
		},
		{
			"r_eyePupil",
			" Change eye's pupil Size.",
		},
		{
			"r_eyeRedness",
			" Change eye's redness.",
		},
		{
			"r_eyeWetness",
			" Change eye's wetness.",
		},
		{
			"r_fastModelPrimaryLightCheck",
			"Reduce R_GetNonSunPrimaryLightForSphere/R_GetNonSunPrimaryLightForBox function calls",
		},
		{
			"r_fastModelPrimaryLightLink",
			"Speed up R_LinkSphereEntityToPrimaryLights and R_LinkBoxEntityToPrimaryLights.",
		},
		{
			"r_filmAltShader",
			"Use alternate shader (with middle tint and dark desat) for film color.",
		},
		{
			"r_filmTweakBrightness",
			"Tweak dev var; film color brightness",
		},
		{
			"r_filmTweakContrast",
			"Tweak dev var; film color contrast",
		},
		{
			"r_filmTweakDesaturation",
			"Tweak dev var; Desaturation applied after all 3D drawing to light areas",
		},
		{
			"r_filmTweakDesaturationDark",
			"Tweak dev var; Additional desaturation applied after all 3D drawing to dark areas",
		},
		{
			"r_filmTweakEnable",
			"Tweak dev var; enable film color effects",
		},
		{
			"r_filmTweakInvert",
			"Tweak dev var; enable inverted video",
		},
		{
			"r_filmUseTweaks",
			"Overide film effects with tweak dvar values.",
		},
		{
			"r_floatzCopyCompressed",
			"Use a compute shader to copy compressed depth data to $floatz",
		},
		{
			"r_fog",
			"Set to 0 to disable fog",
		},
		{
			"r_fog_depthhack_scale",
			"Fog scale for depth hack surfaces",
		},
		{
			"r_fog_ev_adjust",
			"Fog color ev adjustment (+2 means fog color is 2 stops brighter)",
		},
		{
			"r_fogDebugOverlay",
			"Fog Debug Overlay",
		},
		{
			"r_font_cache_debug_display",
			"Display the current fontcache texture on the HUD for debug purposes",
		},
		{
			"r_forceLetterbox4x3",
			"Force 16:9 -> 4:3 letterbox mode on, regardless of aspect ratio.",
		},
		{
			"r_forceLod",
			"Force all level of detail to this level",
		},
		{
			"r_fullbright",
			"Toggles rendering without lighting",
		},
		{
			"r_fxaaSubpixel",
			"FXAA sub-pixel amount, lower values have more aliasing and less blur",
		},
		{
			"r_FXAverageColorFunc",
			"How to compute FX system average color? 0 = use IWrad equation, 1 = legacy equation, 2 = spherical harmonics 1 coefficient.",
		},
		{
			"r_globalGenericMaterialScale",
			"Hack global generic material constants",
		},
		{
			"r_glow",
			"Enable glow.",
		},
		{
			"r_glow_allowed",
			"Allow glow.",
		},
		{
			"r_glow_allowed_script_forced",
			"Force 'allow glow' to be treated as true, by script.",
		},
		{
			"r_glowTweakAltColorScaleB",
			"Tweak dev var; Alt cutoff intensity B",
		},
		{
			"r_glowTweakAltColorScaleG",
			"Tweak dev var; Alt cutoff intensity G",
		},
		{
			"r_glowTweakAltColorScaleR",
			"Tweak dev var; Alt cutoff intensity R",
		},
		{
			"r_glowTweakAltCutoff",
			"Tweak dev var; Alt cutoff",
		},
		{
			"r_glowTweakBloomCutoff",
			"Tweak dev var; Glow bloom cut off fraction",
		},
		{
			"r_glowTweakBloomDesaturation",
			"Tweak dev var; Glow bloom desaturation",
		},
		{
			"r_glowTweakBloomIntensity0",
			"Tweak dev var; Glow bloom intensity",
		},
		{
			"r_glowTweakBloomPinch",
			"Tweak dev var; Pinch the bloom horizontally ( positive ) or vertically ( negative )",
		},
		{
			"r_glowTweakEnable",
			"Tweak dev var; Enable glow",
		},
		{
			"r_glowTweakRadius0",
			"Tweak dev var; Glow radius in pixels at 640x480",
		},
		{
			"r_glowTweakUseAltCutoff",
			"Tweak dev var; Enable glow alt cutoff",
		},
		{
			"r_glowUseTweaks",
			"Overide glow with tweak dvar values.",
		},
		{
			"r_gpuFrameHistogram",
			"Enables a visual histogram of time between page flips.",
		},
		{
			"r_gpuTimers",
			"GPU Timers to display",
		},
		{
			"r_gpuTimersShowPrepass",
			"Show prepass in GPU Timers",
		},
		{
			"r_gradientAdjust",
			"Enable mipmap gradient adjust (required by checkerboard upsample)",
		},
		{
			"r_gunSightColorEntityScale",
			"Scale the gun sight color when over an entity.",
		},
		{
			"r_gunSightColorNoneScale",
			"Scale the gun sight color when not over an entity.",
		},
		{
			"r_hdrSkyIntensity",
			"Vision set intensity for HDR sky.",
		},
		{
			"r_hdrSkyIntensityUseTweaks",
			"Enables HDR sky intensity tweaks",
		},
		{
			"r_hdrSpotmeter",
			"Perform HDR spot meter measurement of luminance and color in center of screen",
		},
		{
			"r_hemiAoBlurTolerance",
			"Hemi SSAO Blur Tolerance (log10)",
		},
		{
			"r_hemiAoCombineResolutionsBeforeBlur",
			"The higher quality modes blend wide and narrow sampling patterns. The wide pattern is due to deinterleaving and requires blurring. The narrow pattern is not on a deinterleaved buffer, but it only samples every other pixel. The blur on it is optional. If you combine the two before blurring, the narrow will get blurred as well. This creates a softer effect but can remove any visible noise from having 50% sample coverage.",
		},
		{
			"r_hemiAoCombineResolutionsWithMul",
			"When combining the wide and narrow patterns, a mul() operation can be used or a min() operation. Multiplication exaggerates the result creating even darker creases. This is an artistic choice. I think it looks less natural, but often art teams prefer more exaggerated contrast. For me, it's more about having the right AO falloff so that it's a smooth gradient rather than falling off precipitously and forming overly dark recesses.",
		},
		{
			"r_hemiAoDepthSquash",
			"Hemi SSAO depth squash. Value is rcp.",
		},
		{
			"r_hemiAoEnable",
			"Enable Hemi SSAO",
		},
		{
			"r_hemiAoHierarchyDepth",
			"Hemi SSAO recursion depth (filter width)",
		},
		{
			"r_hemiAoMaxDepthDownsample",
			"Use max depth value when downsampling, instead of pseudo-randomly picking a depth sample? Leaving this at the default false may produce more stable results.",
		},
		{
			"r_hemiAoNoiseFilterTolerance",
			"This is necessary to filter out pixel shimmer due to bilateral upsampling with too much lost resolution. High frequency detail can sometimes not be reconstructed, and the noise filter fills in the missing pixels with the result of the higher resolution SSAO. Value is log10.",
		},
		{
			"r_hemiAoPower",
			"Power curve applied to Hemi SSAO factor, not applied in game yet",
		},
		{
			"r_hemiAoQualityLevel",
			"Hemi SSAO quality setting",
		},
		{
			"r_hemiAoRejectionFalloff",
			"Controls how aggressive to fade off samples that occlude spheres but by so much as to be unreliable. This is what gives objects a dark halo around them when placed in front of a wall. If you want to fade off the halo, boost your rejection falloff. The tradeoff is that it reduces overall AO. Value is rcp.",
		},
		{
			"r_hemiAoStrength",
			"Strength of Hemi Screen Space Ambient Occlusion effect",
		},
		{
			"r_hemiAoUpsampleTolerance",
			"Hemi SSAO Upsample Tolerance (log10)",
		},
		{
			"r_heroLighting",
			"Enable hero-only lighting",
		},
		{
			"r_hideAmbientSModels",
			"Hide ambient static models.",
		},
		{
			"r_hideDepthHack",
			"Hide depth hackS to make it easier to find floating decals.",
		},
		{
			"r_hideLitTSplit0",
			"Hide lit transparent prior to split",
		},
		{
			"r_hideLitTSplit1",
			"Hide lit transparent post split",
		},
		{
			"r_hideModelLMapSModels",
			"Hide model lightmap static models.",
		},
		{
			"r_hideOpaque",
			"Hide opaque to make it easier to find floating decals.",
		},
		{
			"r_hideReactiveMotionSModels",
			"Hide reactive motion static models.",
		},
		{
			"r_hideVertexLitSModels",
			"Hide vertex lit static models.",
		},
		{
			"r_highLodDist",
			"Distance for high level of detail",
		},
		{
			"r_hudFx",
			"Draw HUD Effects",
		},
		{
			"r_ignore",
			"",
		},
		{
			"r_ignoreDoplFlags",
			"Debug only dvar: Enable it then all vfx spot light becomes dopls depend on r_dynamicOPL.",
		},
		{
			"r_ignoref",
			"",
		},
		{
			"r_inGameVideo",
			"Allow in game cinematics",
		},
		{
			"r_lateAllocParamCacheAllowed",
			"Enable late allocation of parameter cache for VS stage.",
		},
		{
			"r_lateAllocParamCacheDefault",
			"Late allocation of parameter cache value for sub-div materials.",
		},
		{
			"r_lateAllocParamCacheDisplacement",
			"Late allocation of parameter cache value for sub-div materials.",
		},
		{
			"r_lateAllocParamCacheSubdiv",
			"Late allocation of parameter cache value for sub-div materials.",
		},
		{
			"r_letterbox4x3",
			"Use 16:9 aspect ratio always, with conversion to letterbox 4:3 if necessary.",
		},
		{
			"r_lightCacheLessFrequentMaxDistance",
			"Adjust the distance fx models (and models tagged as less-frequently-lit by script) move before immediately being relit",
		},
		{
			"r_lightCacheLessFrequentPeriod",
			"Adjust how frequently fx models (and models tagged as less-frequently-lit by script) get relit on average (1 is every frame, 8 is every 8th frame)",
		},
		{
			"r_lightGridAvgApplyPrimaryLight",
			"apply primary light color onto r_showLightGridAvgProbes boxes",
		},
		{
			"r_lightGridAvgFollowCamera",
			"allow the r_showLightGridAvgProbes boxes following current camera position",
		},
		{
			"r_lightGridAvgProbeCount",
			"how many light grid avg color probes will show up)",
		},
		{
			"r_lightGridAvgTraceGround",
			" lock boxes to ground ",
		},
		{
			"r_lightGridContrast",
			"Adjust the contrast of light color from the light grid",
		},
		{
			"r_lightGridDefaultFXLightingLookup",
			"Default FX lighting lookup location\n",
		},
		{
			"r_lightGridDefaultModelLightingLookup",
			"Default model lighting lookup location",
		},
		{
			"r_lightGridEnableTweaks",
			"Enable tweaks of the light color from the light grid",
		},
		{
			"r_lightGridIntensity",
			"Adjust the intensity of light color from the light grid",
		},
		{
			"r_lightGridSHBands",
			"Spherical harmonics bands being used for evaluating current-gen light grids colors. 0 = default, 1 = 1 band, 2 = 2 bands, 3 = 3 bands.\n",
		},
		{
			"r_lightGridUseTweakedValues",
			"Use tweaked values instead of default",
		},
		{
			"r_lightMap",
			"Replace all lightmaps with pure black or pure white",
		},
		{
			"r_lightSetDebugOverlay",
			"Enable LightSets Debug Overlay",
		},
		{
			"r_lightTweakBoneSpaceIndex",
			"Tweak origin and angles appear in the space of this entity",
		},
		{
			"r_lightTweakColor",
			"Light color",
		},
		{
			"r_lightTweakEnable",
			"Apply light tweaks to the active primary light",
		},
		{
			"r_lightTweakEntSpaceIndex",
			"Tweak origin and angles appear in the space of this entity",
		},
		{
			"r_lightTweakHeading",
			"Light heading in degrees",
		},
		{
			"r_lightTweakIndex",
			"Light index to tweak. Works with r_lightTweakEnable.",
		},
		{
			"r_lightTweakInnerCone",
			"Light tweak inner cone angle",
		},
		{
			"r_lightTweakIntensity",
			"Light strength",
		},
		{
			"r_lightTweakOuterCone",
			"Light tweak outer cone angle",
		},
		{
			"r_lightTweakPitch",
			"Light pitch in degrees",
		},
		{
			"r_lightTweakRadius",
			"Light radius",
		},
		{
			"r_limitCUCount",
			"1 limits the PS4 CU count to 12 matching Xbox One, 0 restores default CU limit.",
		},
		{
			"r_litSurfaceHDRScalar",
			"Vision set based scalar applied to lit surfaces",
		},
		{
			"r_lockPvs",
			"Lock the viewpoint used for determining what is visible to the current position and direction",
		},
		{
			"r_lod4Dist",
			"Distance for lowest level of detail 4",
		},
		{
			"r_lod5Dist",
			"Distance for lowest level of detail 5",
		},
		{
			"r_lowestLodDist",
			"Distance for lowest level of detail",
		},
		{
			"r_lowLodDist",
			"Distance for low level of detail",
		},
		{
			"r_mbEnable",
			"Set of objects which will be enabled for motion blur",
		},
		{
			"r_mbFastEnable",
			"Toggle on/off fast high quality motion blur",
		},
		{
			"r_mbFastPreset",
			"Changes motion blur quality",
		},
		{
			"r_mdao",
			"Enable the medium distance ambient occlusion feature",
		},
		{
			"r_mdaoAsyncOccluderGen",
			"The occluder generation step is performed via async compute",
		},
		{
			"r_mdaoBoneInfluenceRadiusScale",
			"Scale for the bone influence radius for mdao",
		},
		{
			"r_mdaoCapsuleStrength",
			"MDAO strength for capsule occluders",
		},
		{
			"r_mdaoDrawOccluders",
			"Draws the entity occluder ellipsoids used as MDAO casters",
		},
		{
			"r_mdaoMinBoneBoundsToOcclude",
			"Minimum volume of the bone collider to create occluders for",
		},
		{
			"r_mdaoOccluderCullDistance",
			"Culling distance for mdao occluders",
		},
		{
			"r_mdaoOccluderFadeOutStartDistance",
			"Fade out distance for mdao occluders",
		},
		{
			"r_mdaoUseTweaks",
			"Use r_mdao* dvars instead of the current light set values for MDAO",
		},
		{
			"r_mdaoVolumeStrength",
			"MDAO strength for volume occluders",
		},
		{
			"r_mediumLodDist",
			"Distance for medium level of detail",
		},
		{
			"r_mode",
			"Display mode",
		},
		{
			"r_modelLightingMap",
			"Replace all model lighting maps (light grid) with pure black",
		},
		{
			"r_mpRimColor",
			"Change character's rim color for multiplayer",
		},
		{
			"r_mpRimDiffuseTint",
			"Change character's rim diffuse tint for multiplayer.",
		},
		{
			"r_mpRimStrength",
			"Change character's rim color for multiplayer",
		},
		{
			"r_mrtEnable",
			"Enable setting of multiple render targets.",
		},
		{
			"r_normalMap",
			"Replace all normal maps with a flat normal map",
		},
		{
			"r_occlusionQueryDebugOverlay",
			"Enable OcclusionQuery Debug Overlay",
		},
		{
			"r_offchipTessellationAllowed",
			"Enable off-chip tessellation support.",
		},
		{
			"r_offchipTessellationTfThreshold",
			"Tessellation factor threshold for off-chip.",
		},
		{
			"r_offchipTessellationWaveThreshold",
			"Domain shader wave threshold for off-chip.",
		},
		{
			"r_opaqueUnlit",
			"Toggles opaque rendering without lighting",
		},
		{
			"r_outdoor",
			"Prevents snow from going indoors",
		},
		{
			"r_outdoorDebugOverlay",
			"Enable Outdoor Debug Overlay",
		},
		{
			"r_outdoorFeather",
			"Outdoor z-feathering value",
		},
		{
			"r_overrideDrawList",
			"Replace textures for this draw list",
		},
		{
			"r_p4xCapture",
			"P4X capture",
		},
		{
			"r_particleHdr",
			"Enable Hdr Particle Features",
		},
		{
			"r_patchCountAllowed",
			"Enable run-time setting of patch count per draw call.",
		},
		{
			"r_pix_material",
			"Enable profile material names. Set to 1 to see material and technique names as markers. Set to 2 to also see shader names.",
		},
		{
			"r_polygonOffsetBias",
			"Offset bias for decal polygons; bigger values z-fight less but poke through walls more",
		},
		{
			"r_polygonOffsetClamp",
			"Offset clamp for decal polygons; bigger values z-fight less but poke through walls more",
		},
		{
			"r_polygonOffsetScale",
			"Offset scale for decal polygons; bigger values z-fight less but poke through walls more",
		},
		{
			"r_portalBevels",
			"Helps cull geometry by angles of portals that are acute when projected onto the screen, value is the cosine of the angle",
		},
		{
			"r_portalBevelsOnly",
			"Use screen-space bounding box of portals rather than the actual shape of the portal projected onto the screen",
		},
		{
			"r_portalMinClipArea",
			"Don't clip child portals by a parent portal smaller than this fraction of the screen area.",
		},
		{
			"r_portalMinRecurseDepth",
			"Ignore r_portalMinClipArea for portals with fewer than this many parent portals.",
		},
		{
			"r_portalWalkLimit",
			"Stop portal recursion after this many iterations.  Useful for debugging portal errors.",
		},
		{
			"r_postAA",
			"Post process antialiasing mode",
		},
		{
			"r_postfx_enable",
			"Enable post-processing effects such as color correction, bloom, depth-of-field, etc.",
		},
		{
			"r_primaryLightTweakDiffuseStrength",
			"Tweak the diffuse intensity for primary lights",
		},
		{
			"r_primaryLightTweakSpecularStrength",
			"Tweak the specular intensity for primary lights",
		},
		{
			"r_primaryLightUseTweaks",
			"",
		},
		{
			"r_primBegin",
			"Beginning of the range of drawn primitives, inclusive.",
		},
		{
			"r_primBisect",
			"Enables limiting the range of drawn primitives, for debugging.",
		},
		{
			"r_primEnd",
			"End of the range range of drawn primitives, inclusive.",
		},
		{
			"r_PS4HardwareGamma",
			"Controls the hardware gamma curve used (higher is darker)",
		},
		{
			"r_randomFailCommandBuffer",
			"Command buffer failure rate",
		},
		{
			"r_randomFailConstantBuffer",
			"Constant buffer failure rate",
		},
		{
			"r_reactiveMotionActorRadius",
			"Radial distance from the ai characters that influences reactive motion models (inches)",
		},
		{
			"r_reactiveMotionActorVelocityMax",
			"AI velocity considered the maximum when determining the length of motion tails (inches/sec)",
		},
		{
			"r_reactiveMotionActorZOffset",
			"Distance from the actor origin along Z direction where the actor's reactive motion effector sphere is centered at.",
		},
		{
			"r_reactiveMotionEffectorDebugDraw",
			"Enable debug drawing of effector spheres",
		},
		{
			"r_reactiveMotionEffectorStrengthScale",
			"Additional scale for the effector influence, as a factor of the model part distance from the effector center and model part extents",
		},
		{
			"r_reactiveMotionHelicopterLimit",
			"Maximum number of helicopter entities that actively influence reactive motion.  Can increase CPU cost of the scene.",
		},
		{
			"r_reactiveMotionHelicopterRadius",
			"Radial distance from the helicopter that influences reactive motion models (inches)",
		},
		{
			"r_reactiveMotionHelicopterStrength",
			"Scales the influence of helicopter wind tunnel motion",
		},
		{
			"r_reactiveMotionPlayerHeightAdjust",
			"Amount to adjust the vertical distance of the effector from the player position (inches)",
		},
		{
			"r_reactiveMotionPlayerRadius",
			"Radial distance from the player that influences reactive motion models (inches)",
		},
		{
			"r_reactiveMotionPlayerZOffset",
			"Distance from the player origin along Z direction where the player's reactive motion effector sphere is centered at.",
		},
		{
			"r_reactiveMotionVelocityTailScale",
			"Additional scale for the velocity-based motion tails, as a factor of the effector radius",
		},
		{
			"r_reactiveMotionWindAmplitudeScale",
			"Scales amplitude of wind wave motion",
		},
		{
			"r_reactiveMotionWindAreaScale",
			"Scales distribution of wind motion",
		},
		{
			"r_reactiveMotionWindDir",
			"Controls the global wind direction",
		},
		{
			"r_reactiveMotionWindFrequencyScale",
			"Scales frequency of wind wave motion",
		},
		{
			"r_reactiveMotionWindStrength",
			"Scale of the global wind direction (inches/sec)",
		},
		{
			"r_reflectionProbeMap",
			"Replace all reflection probes with pure black",
		},
		{
			"r_reflectionProbeNmlLuminance",
			"Enable/disable shader code for computing luminance during reflection probe denormalization. This is just an experiment.\n",
		},
		{
			"r_rimLight0Heading",
			"Rim Light 0 heading in degrees",
		},
		{
			"r_rimLight0Pitch",
			"Rim Light 0 pitch in degrees -90 is noon.",
		},
		{
			"r_rimLightBias",
			"How much to bias the n.l calculation",
		},
		{
			"r_rimLightDiffuseIntensity",
			"Strength of the diffuse component of the rim light.",
		},
		{
			"r_rimLightFalloffMaxDistance",
			"Distance at which the rim light hits intensity of 100%.",
		},
		{
			"r_rimLightFalloffMinDistance",
			"Distance at which the rim light hits intensity of 100%.",
		},
		{
			"r_rimLightFalloffMinIntensity",
			"Intensity of the effect at and before minDistance.",
		},
		{
			"r_rimLightPower",
			"Power to raise the n.l calculation",
		},
		{
			"r_rimLightSpecIntensity",
			"Strength of the spec ( additive) component of the rim light",
		},
		{
			"r_rimLightUseTweaks",
			"Turn on rim lighting tweaks",
		},
		{
			"r_scaleViewport",
			"Scale 3D viewports by this fraction.  Use this to see if framerate is pixel shader bound.",
		},
		{
			"r_sceneMipShowOverlay",
			"Toggles scene mip rendertarget overlay",
		},
		{
			"r_showAabbTrees",
			"Show axis-aligned bounding box trees used in potentially visibile set determination. 1 shows hierarchy, 2 shows world surfaces, 3 shows both.",
		},
		{
			"r_showCollision",
			"Show the collision surfaces for the selected mask types",
		},
		{
			"r_showCollisionDepthTest",
			"Select whether to use depth test in collision surfaces display",
		},
		{
			"r_showCollisionDist",
			"Maximum distance to show collision surfaces",
		},
		{
			"r_showCollisionGroups",
			"Select whether to show the terrain, brush or all collision surface groups",
		},
		{
			"r_showCollisionPolyType",
			"Select whether to display the collision surfaces as wireframe, poly interiors, or both",
		},
		{
			"r_showCullBModels",
			"Show culled brush models",
		},
		{
			"r_showCullSModels",
			"Show culled static models",
		},
		{
			"r_showCullXModels",
			"Show culled xmodels",
		},
		{
			"r_showFbColorDebug",
			"Show front buffer color debugging information",
		},
		{
			"r_showFloatZDebug",
			"Show float z buffer used to eliminate hard edges on particles near geometry",
		},
		{
			"r_showLightGrid",
			"Show light grid debugging information (2: detailed, 3: detailed for this box only)",
		},
		{
			"r_showLightGridAvgProbes",
			"show an array of boxes which are using the light grid average color at its location",
		},
		{
			"r_showLightGridDetailInfo",
			"Show more details for light grid debugging.",
		},
		{
			"r_showLightProbes",
			"Show the light probes at the light grid sample locations in world space centered around the camera.",
		},
		{
			"r_showMissingLightGrid",
			"Use rainbow colors for entities that are outside the light grid",
		},
		{
			"r_showModelLightingLowWaterMark",
			"",
		},
		{
			"r_showModelList",
			"Display models that are in the ShowModelList.txt",
		},
		{
			"r_showModelLods",
			"Currently drawn LOD displayed with each model",
		},
		{
			"r_showModelNames",
			"Model names displayed with each model",
		},
		{
			"r_showPortals",
			"Show portals for debugging",
		},
		{
			"r_showPortalsOverview",
			"Render 2d XY portal overlay scaled to fit to this distance.  Useful for debugging portal errors.",
		},
		{
			"r_showPrimCounts",
			"Prim count for each rendered entity",
		},
		{
			"r_showReflectionProbeSelection",
			"Show reflection probe selection",
		},
		{
			"r_showSModelNames",
			"Show static model names",
		},
		{
			"r_showSurfCounts",
			"Surface count for each rendered entity",
		},
		{
			"r_showTess",
			"Show details for each surface",
		},
		{
			"r_showTexelDensity",
			"Enable shader overrides for texel density.",
		},
		{
			"r_showTriCountMin",
			"Minimum triangle count to show when showing tri counts",
		},
		{
			"r_showTriCounts",
			"Triangle count for each rendered entity",
		},
		{
			"r_showTris",
			"Show triangle outlines",
		},
		{
			"r_showVertCountMin",
			"Minimum vertex count to show when showing vert counts",
		},
		{
			"r_showVertCounts",
			"",
		},
		{
			"r_singleCell",
			"Only draw things in the same cell as the camera.  Most useful for seeing how big the current cell is.",
		},
		{
			"r_skipPvs",
			"Skipt the determination of what is in the potentially visible set (disables most drawing)",
		},
		{
			"r_sky_fog_intensity",
			"Amount of sky fog fading",
		},
		{
			"r_sky_fog_max_angle",
			"End of angular sky fog fading",
		},
		{
			"r_sky_fog_min_angle",
			"Start of angular sky fog fading",
		},
		{
			"r_skyFogUseTweaks",
			"Enable dvar control of sky fog",
		},
		{
			"r_smodelInstancedRenderer",
			"Render static models with instanced renderer",
		},
		{
			"r_smodelInstancedThreshold",
			"Minimum number of static model instances before instanced rendering is used",
		},
		{
			"r_smp_backend",
			"Process renderer back end in a separate thread",
		},
		{
			"r_smp_worker",
			"Process renderer front end in a separate thread",
		},
		{
			"r_smp_worker_thread0",
			"",
		},
		{
			"r_smp_worker_thread1",
			"",
		},
		{
			"r_smp_worker_thread2",
			"",
		},
		{
			"r_specOccMap",
			"Replace all specular occlusion maps with pure black (fully occluded) or pure white (not occluded)",
		},
		{
			"r_specularColorScale",
			"Set greater than 1 to brighten specular highlights",
		},
		{
			"r_specularMap",
			"Replace all specular maps with pure black (off) or pure white (super shiny)",
		},
		{
			"r_spotLightEntityShadows",
			"Enable entity shadows for spot lights.",
		},
		{
			"r_spotLightShadows",
			"Enable shadows for spot lights.",
		},
		{
			"r_ssao",
			"Screen Space Ambient Occlusion mode",
		},
		{
			"r_ssaoDebug",
			"Render calculated or blurred Screen Space Ambient Occlusion values",
		},
		{
			"r_ssaoDebugMip",
			"Selects which mip level to render when r_ssaoDebug is enabled. If 0 and r_ssaoDownsample is enabled, will render mip 1.",
		},
		{
			"r_ssaoDepthScale",
			"Scale applied to depth values used for occlusion tests.",
		},
		{
			"r_ssaoDepthScaleViewModel",
			"Scale applied to depth values used for occlusion tests.",
		},
		{
			"r_ssaoDownsample",
			"Screen Space Ambient Occlusion calculation occurs at half linear resolution",
		},
		{
			"r_ssaoFadeDepth",
			"Depth at which the SSAO begins to fade out.  It fades at even increments of this distance (e.g. it's at 1 for depth r_ssaoFadeDepth, 1/2 for depth 2*r_ssaoFadeDepth, etc.)",
		},
		{
			"r_ssaoGapFalloff",
			"Falloff used to blend between creases (that should darken) and silhouettes (that should not darken).  Lower values falloff more quickly.",
		},
		{
			"r_ssaoGradientFalloff",
			"Falloff used to fade out the effect for steep depth gradients (i.e. surfaces nearly parallel to the camera direction).  This fixes sampling artifacts that appear for surfaces nearly parallel to the camera direction (commonly occuring for flat ground planes).",
		},
		{
			"r_ssaoMaxStrengthDepth",
			"Depth at which SSAO strength is at its maximum",
		},
		{
			"r_ssaoMethod",
			"Screen Space Ambient Occlusion method (original or IW6, both are volumetric obscurance)",
		},
		{
			"r_ssaoMinPixelWidth",
			"Minimum pixel width of the effect.  When the effect is smaller than this, it is culled entirely.",
		},
		{
			"r_ssaoMinStrengthDepth",
			"Depth at which SSAO strength is zero, effectively disabled",
		},
		{
			"r_ssaoMultiRes",
			"Screen Space Ambient Occlusion calculation occurs at half linear resolution",
		},
		{
			"r_ssaoPower",
			"Power curve applied to SSAO factor",
		},
		{
			"r_ssaoRejectDepth",
			"Depth at which the SSAO is disabled.  Smaller values result in more rejected pixels which is faster, but limits the distance at which the effect is visible.",
		},
		{
			"r_ssaoSampleCount",
			"Selects the number of samples used for SSAO",
		},
		{
			"r_ssaoScriptScale",
			"Allows script to lerp to disable or enable the SSAO.  This applies a scalar value to the SSAO strength.  When set to 0, this effectively disables SSAO.",
		},
		{
			"r_ssaoStrength",
			"Strength of Screen Space Ambient Occlusion effect",
		},
		{
			"r_ssaoUseTweaks",
			"Use r_ssao* dvars instead of the current light set values for SSAO",
		},
		{
			"r_ssaoWidth",
			"The width of the SSAO effect, in pixels at 720p.  Larger values increase area but lower effective quality.",
		},
		{
			"r_ssrBlendScale",
			"Add extra scale to ssr weight versus reflection probe weight, >1 value will make ssr more obvious.",
		},
		{
			"r_ssrDepthTraceFakeOff",
			"toggle on/off to see ssr depth trace material performance impact.",
		},
		{
			"r_ssrFadeInDuration",
			"Duration of the screen-space reflection fade-in, which occurs whenever the reflection source buffer is invalidated due to view changes (in particular, dual-view scope transitions).",
		},
		{
			"r_ssrRoughnessMipParameters",
			"X: mirror mip; Y: roughest mip; Z: roughness middle point, may need different value for different screen resolution on PC.",
		},
		{
			"r_ssrShowOverlay",
			"Toggles overlay drawing of screen space reflection occlusion buffers.",
		},
		{
			"r_sssBlendWeight",
			"Controls the blend between the wide (zero) and narrow (one) gaussians",
		},
		{
			"r_sssDebugMaterial",
			"Debug Feature: toggle materials with SSS",
		},
		{
			"r_sssEnable",
			"Enables the subsurface scattering effect (note that disabling SSS will not prevent the filter from running)",
		},
		{
			"r_sssGlobalRadius",
			"Controls the global radius (in inches)",
		},
		{
			"r_sssJitterRadius",
			"Percentage of the kernel to be jittered",
		},
		{
			"r_sssNarrowRadius",
			"Controls the narrow Gaussian radius",
		},
		{
			"r_sssPreset",
			"Changes subsurface scattering quality",
		},
		{
			"r_sssWideRadius",
			"Controls the wide Gaussian radius",
		},
		{
			"r_staticModelDumpLodInfo",
			"Dump static model info for the next frame.",
		},
		{
			"r_subdiv",
			"Enables Catmull Clark surface subdivision.",
		},
		{
			"r_subdivLimit",
			"Set the maximum Catmull Clark subdivision level.",
		},
		{
			"r_subdivPatchCount",
			"Patches per thread group for sub-division surfaces.",
		},
		{
			"r_subdomainLimit",
			"Maximum number of extra tessellation subdivisions using instancing (max tess amts are 0:64, 1:128, 2:192, 3:256, max instances used are 0:1, 1:4, 2:9, 3:12)",
		},
		{
			"r_subdomainScale",
			"Debug only: Scales the extra subdivision amount (for values < 1, not all instanced sub triangles will draw).",
		},
		{
			"r_subwindow",
			"subwindow to draw: left, right, top, bottom",
		},
		{
			"r_sun_from_dvars",
			"Set sun flare values from dvars rather than the level",
		},
		{
			"r_sun_fx_position",
			"Position in degrees of the sun effect",
		},
		{
			"r_sunblind_fadein",
			"time in seconds to fade blind from 0% to 100%",
		},
		{
			"r_sunblind_fadeout",
			"time in seconds to fade blind from 100% to 0%",
		},
		{
			"r_sunblind_max_angle",
			"angle from sun in degrees inside which effect is max",
		},
		{
			"r_sunblind_max_darken",
			"0-1 fraction for how black the world is at max blind",
		},
		{
			"r_sunblind_min_angle",
			"angle from sun in degrees outside which effect is 0",
		},
		{
			"r_sunflare_fadein",
			"time in seconds to fade alpha from 0% to 100%",
		},
		{
			"r_sunflare_fadeout",
			"time in seconds to fade alpha from 100% to 0%",
		},
		{
			"r_sunflare_max_alpha",
			"0-1 vertex color and alpha of sun at max effect",
		},
		{
			"r_sunflare_max_angle",
			"angle from sun in degrees inside which effect is max",
		},
		{
			"r_sunflare_max_size",
			"largest size of flare effect in pixels at 640x480",
		},
		{
			"r_sunflare_min_angle",
			"angle from sun in degrees outside which effect is 0",
		},
		{
			"r_sunflare_min_size",
			"smallest size of flare effect in pixels at 640x480",
		},
		{
			"r_sunflare_shader",
			"name for flare effect; can be any material",
		},
		{
			"r_sunglare_fadein",
			"time in seconds to fade glare from 0% to 100%",
		},
		{
			"r_sunglare_fadeout",
			"time in seconds to fade glare from 100% to 0%",
		},
		{
			"r_sunglare_max_angle",
			"angle from sun in degrees inside which effect is max",
		},
		{
			"r_sunglare_max_lighten",
			"0-1 fraction for how white the world is at max glare",
		},
		{
			"r_sunglare_min_angle",
			"angle from sun in degrees inside which effect is max",
		},
		{
			"r_sunInfDist",
			"Sun infinite distance used to place sun fx",
		},
		{
			"r_sunshadowmap_cmdbuf_worker",
			"Process shadowmap command buffer in a separate thread",
		},
		{
			"r_sunsprite_shader",
			"name for static sprite; can be any material",
		},
		{
			"r_sunsprite_size",
			"diameter in pixels at 640x480 and 80 fov",
		},
		{
			"r_sunsprite_size_override",
			"override the .sun file for the sunsprite size; diameter in pixels at 640x480 and 80 fov, -1 means off",
		},
		{
			"r_surfaceHDRScalarUseTweaks",
			"Enables lit and unlit surface scalar tweaks",
		},
		{
			"r_tension_enable",
			"Toggles tension",
		},
		{
			"r_tessellation",
			"Enables tessellation of world geometry, with an optional cutoff distance.",
		},
		{
			"r_tessellationCutoffDistance",
			"Distance at which world geometry ceases to tessellate.",
		},
		{
			"r_tessellationCutoffFalloff",
			"Range over which tessellation is faded out, up to the cutoff.",
		},
		{
			"r_tessellationEyeScale",
			"Scale applied due to eye * object normal for less tessellation on facing polygons.",
		},
		{
			"r_tessellationFactor",
			"Target edge length, based on dividing full window height by this factor, for dynamic tessellation.  Use zero to disable tessellation.",
		},
		{
			"r_tessellationGPUSync",
			"Sync GPU between tessellated world surfaces (temporary hack for GPU hang)",
		},
		{
			"r_tessellationHeightAuto",
			"Correctly auto scale displacement heights for layers to grow as texture is stretched over larger surface areas to preserve feature proportions.",
		},
		{
			"r_tessellationHeightScale",
			"Displacement height scale factor.",
		},
		{
			"r_tessellationHybrid",
			"Hybrid per pixel displacement scale.",
		},
		{
			"r_tessellationLodBias",
			"Displacement map lod bias.",
		},
		{
			"r_texFilterAnisoBias",
			"Bias used when computing anisotropy ratio.  Lower values are more efficient at the expense of filtering quality.",
		},
		{
			"r_texFilterAnisoMax",
			"Maximum anisotropy to use for texture filtering",
		},
		{
			"r_texFilterAnisoMin",
			"Minimum anisotropy to use for texture filtering (overridden by max)",
		},
		{
			"r_texFilterDisable",
			"Disables all texture filtering (uses nearest only.)",
		},
		{
			"r_texFilterMipBias",
			"Change the mipmap bias",
		},
		{
			"r_texFilterMipMode",
			"Forces all mipmaps to use a particular blend between levels (or disables mipping.)",
		},
		{
			"r_texFilterProbeBilinear",
			"Force reflection probe to use bilinear filter",
		},
		{
			"r_texFilterTrilinearWindow",
			"Window over which trilinear filtering blends between mips.  A value of 1.0 is a linear blend, and a value of 0 is the minimal blend.  Smaller values are more efficient but result in a sharper transition between mip levels.",
		},
		{
			"r_texShowMipMode",
			"Forces textures with the specified mip filtering to draw black.",
		},
		{
			"r_thermal",
			"Select thermal effect mode",
		},
		{
			"r_thermalColorOffset",
			"Offset of the thermal colors (offset + scale*color)",
		},
		{
			"r_thermalColorScale",
			"Scale of the thermal colors (offset + scale*color)",
		},
		{
			"r_thermalDetailScale",
			"Scale of the detail that is added to the thermal map from the normal map (multiplies the detail amount from AssetManager)",
		},
		{
			"r_thermalFadeColor",
			"Color the thermal fades to at distance.",
		},
		{
			"r_thermalFadeControl",
			"Select thermal fade mode",
		},
		{
			"r_thermalFadeMax",
			"Distance at which thermal stops fading",
		},
		{
			"r_thermalFadeMin",
			"Distance at which thermal starts fading",
		},
		{
			"r_tonemap",
			"HDR Tonemapping mode",
		},
		{
			"r_tonemapAdaptSpeed",
			"HDR Tonemap exposure adaptation speed",
		},
		{
			"r_tonemapAuto",
			"HDR Tonemapping performs auto-exposure",
		},
		{
			"r_tonemapAutoExposureAdjust",
			"HDR Tonemap Auto Exposure Adjust value (set to 0.0 for automatic adjustment)",
		},
		{
			"r_tonemapBlack",
			"HDR Filmic Tonemap black point",
		},
		{
			"r_tonemapBlend",
			"HDR Tonemapping blends between exposures",
		},
		{
			"r_tonemapCaptureExposurePoint",
			"Capture the specified exposure point (Negative value means no capture).",
		},
		{
			"r_tonemapCaptureMode",
			"Tonemap Exposure Capture Mode.",
		},
		{
			"r_tonemapCaptureTweakExposureAdjust",
			"HDR Tonemap Exposure Adjust Tweak",
		},
		{
			"r_tonemapCrossover",
			"HDR Filmic Tonemap crossover point",
		},
		{
			"r_tonemapDarkEv",
			"HDR Tonemap Dark EV",
		},
		{
			"r_tonemapDarkExposureAdjust",
			"HDR Tonemap Dark Exposure Adjust",
		},
		{
			"r_tonemapDebug",
			"Tonemap curve debug",
		},
		{
			"r_tonemapExposure",
			"HDR Tonemap exposure (in EV) override (only works in non-auto mode)",
		},
		{
			"r_tonemapExposureAdjust",
			"HDR Tonemap exposure adjustment (in EV, 0 is no adjustment, works like a camera where +1 reduces EV by 1)",
		},
		{
			"r_tonemapGamma",
			"HDR Tonemap gamma curve power",
		},
		{
			"r_tonemapHighlightRange",
			"HDR Tonemap dynamic range, which determines white point luminance",
		},
		{
			"r_tonemapLightEv",
			"HDR Tonemap Light EV",
		},
		{
			"r_tonemapLightExposureAdjust",
			"HDR Tonemap Light Exposure Adjust",
		},
		{
			"r_tonemapLockAutoExposureAdjust",
			"HDR Tonemapping lock auto exposure adjust",
		},
		{
			"r_tonemapMaxExposure",
			"HDR Tonemap maximum exposure (in EV)",
		},
		{
			"r_tonemapMaxExposureAdjust",
			"HDR Tonemap Max Exposure Adjust",
		},
		{
			"r_tonemapMidEv",
			"HDR Tonemap Mid EV",
		},
		{
			"r_tonemapMidExposureAdjust",
			"HDR Tonemap Mid Exposure Adjust",
		},
		{
			"r_tonemapMinExposureAdjust",
			"HDR Tonemap Min Exposure Adjust",
		},
		{
			"r_tonemapShoulder",
			"HDR Filmic Tonemap shoulder control (0 is linear)",
		},
		{
			"r_tonemapToe",
			"HDR Filmic Tonemap toe control  (0 is linear)",
		},
		{
			"r_tonemapUseCS",
			"HDR Tonemapping uses compute shader.",
		},
		{
			"r_tonemapUseTweaks",
			"Override tone map LightSet settings with tweak dvar values.",
		},
		{
			"r_tonemapWhite",
			"HDR Filmic Tonemap white point",
		},
		{
			"r_trace",
			"Razor GPU trace capture",
		},
		{
			"r_uhdMode",
			"UHD up-scale mode",
		},
		{
			"r_ui3d_debug_display",
			"Show UI3D debug overlay",
		},
		{
			"r_ui3d_h",
			"ui3d texture window height",
		},
		{
			"r_ui3d_use_debug_values",
			"Use UI debug values",
		},
		{
			"r_ui3d_w",
			"ui3d texture window width",
		},
		{
			"r_ui3d_x",
			"ui3d texture window x",
		},
		{
			"r_ui3d_y",
			"ui3d texture window y",
		},
		{
			"r_uiBlurDstMode",
			"UI blur distortion mode. Fast uses the scene mip map render target, PostSun uses a downsampled post sun resolve buffer, PostSun HQ uses a gaussian blurred post sun resolve buffer.",
		},
		{
			"r_umbra",
			"Enables Umbra-based portal culling.",
		},
		{
			"r_umbraAccurateOcclusionThreshold",
			"The distance (in inches) to which accurate occlusion information is gathered. -1.0 = deduced automatically.",
		},
		{
			"r_umbraExclusive",
			"Toggle Umbra for exclusive static culling (disables static portal dpvs)",
		},
		{
			"r_umbraQueryParts",
			"The number of parts the Umbra query frustum is broken into for async query processing as an M x N grid (0, 0 = all queries are synchronous).",
		},
		{
			"r_umbraUseDpvsCullDist",
			"Use cull distance from the DPVS instead of the far plane distance.",
		},
		{
			"r_unlitSurfaceHDRScalar",
			"Vision set based scalar applied to unlit surfaces to balance those surfaces with the luminance of the scene",
		},
		{
			"r_upsampleMode",
			"Checkerboard upsample mode",
		},
		{
			"r_useComputeSkinning",
			"Enables compute shader (GPU) skinning.",
		},
		{
			"r_useLightGridDefaultFXLightingLookup",
			"Enable/disable default fx lighting lookup\n",
		},
		{
			"r_useLightGridDefaultModelLightingLookup",
			"Enable/disable default model lighting lookup\n",
		},
		{
			"r_useShadowGeomOpt",
			"Enable iwRad shadow geometry optimization. It only works when we have the data generated in iwRad.",
		},
		{
			"r_useSunShadowPortals",
			"Enable sun shadow portals when dir light change and not using cached shadow.",
		},
		{
			"r_useXAnimIK",
			"Enables IK animation.",
		},
		{
			"r_validateCmdBuf",
			"Enable validation of command buffer during kick.",
		},
		{
			"r_vc_makelog",
			"Enable logging of light grid points for the vis cache.  1 starts from scratch, 2 appends.",
		},
		{
			"r_vc_showlog",
			"Show this many rows of light grid points for the vis cache",
		},
		{
			"r_veil",
			"Apply veiling luminance (HDR glow)",
		},
		{
			"r_veilAntialiasing",
			"Veil antialiasing mode (downsample technique used for first mip).",
		},
		{
			"r_veilBackgroundStrength",
			"Strength of background when applying veiling luminance (HDR glow)",
		},
		{
			"r_veilFalloffScale1",
			"Controls the size of individual Gaussians (Gaussians 4-6 in XYZ, where Gaussian 6 is the wider one)",
		},
		{
			"r_veilFalloffScale2",
			"Controls the size of individual Gaussians (Gaussians 4-6 in XYZ, where Gaussian 6 is the wider one)",
		},
		{
			"r_veilFalloffWeight1",
			"Controls the weight of individual Gaussians (Gaussians 4-6 in XYZ, where Gaussian 6 is the wider one)",
		},
		{
			"r_veilFalloffWeight2",
			"Controls the weight of individual Gaussians (Gaussians 4-6 in XYZ, where Gaussian 6 is the wider one)",
		},
		{
			"r_veilFilter",
			"Changes the veil filtering mode",
		},
		{
			"r_veilPreset",
			"Changes veil sampling quality",
		},
		{
			"r_veilRadius",
			"Controls the radius of the first Gaussian in virtual pixels (remaining Gaussians follow proportionally).",
		},
		{
			"r_veilStrength",
			"Strength of veiling luminance (HDR glow)",
		},
		{
			"r_veilUseTweaks",
			"Override veil LightSet settings with tweak dvar values.",
		},
		{
			"r_velocityPrepass",
			"Perform velocity rendering during the depth prepass",
		},
		{
			"r_viewModelPrimaryLightTweakDiffuseStrength",
			"Tweak the diffuse intensity for view model primary lights",
		},
		{
			"r_viewModelPrimaryLightTweakSpecularStrength",
			"Tweak the specular intensity for view model primary lights",
		},
		{
			"r_viewModelPrimaryLightUseTweaks",
			"",
		},
		{
			"r_volumeLightScatter",
			"Enables volumetric light scattering",
		},
		{
			"r_volumeLightScatterAngularAtten",
			"Distance of sun from center of screen before angular attenuation starts for god rays",
		},
		{
			"r_volumeLightScatterBackgroundDistance",
			"Distance at which pixels are considered background for volume light scatter effect",
		},
		{
			"r_volumeLightScatterDebug",
			"Enables volumetric light scattering debug rendering",
		},
		{
			"r_volumeLightScatterDepthAttenFar",
			"Pixels >= than this depth recieve full volume light scatter.",
		},
		{
			"r_volumeLightScatterDepthAttenNear",
			"Pixels <= than this depth recieve no volume light scatter.",
		},
		{
			"r_volumeLightScatterEv",
			"Light intensity (in EV) for volumetric light scattering",
		},
		{
			"r_volumeLightScatterLinearAtten",
			"Coefficient of linear attenuation of god rays",
		},
		{
			"r_volumeLightScatterQuadraticAtten",
			"Coefficient of quadratic attenuation of god rays)",
		},
		{
			"r_volumeLightScatterUseTweaks",
			"Enables volumetric light scattering tweaks",
		},
		{
			"r_vsync",
			"Enable v-sync before drawing the next frame to avoid 'tearing' artifacts.",
		},
		{
			"r_warningRepeatDelay",
			"Number of seconds after displaying a \"per-frame\" warning before it will display again",
		},
		{
			"r_wideTessFactorsThreshold",
			"If a surface has more than this many triangles, process triangles in parallel instead of surfaces.",
		},
		{
			"r_xdebug",
			"xmodel/xanim debug rendering",
		},
		{
			"r_zfar",
			"Change the distance at which culling fog reaches 100% opacity; 0 is off",
		},
		{
			"r_znear",
			"Things closer than this aren't drawn.  Reducing this increases z-fighting in the distance.",
		},
		{
			"r_znear_depthhack",
			"Viewmodel near clip plane",
		},
		{
			"radarjamDistMax",
			"",
		},
		{
			"radarjamDistMin",
			"",
		},
		{
			"radarjamSinCurve",
			"",
		},
		{
			"radius_damage_debug",
			"Turn on debug lines for radius damage traces",
		},
		{
			"ragdoll_baselerp_time",
			"Default time ragdoll baselerp bones take to reach the base pose",
		},
		{
			"ragdoll_bullet_force",
			"Bullet force applied to ragdolls",
		},
		{
			"ragdoll_bullet_upbias",
			"Upward bias applied to ragdoll bullet effects",
		},
		{
			"ragdoll_debug_trace",
			"Draw traces from penetration test",
		},
		{
			"ragdoll_dump_anims",
			"Dump animation data when ragdoll fails",
		},
		{
			"ragdoll_enable",
			"Turn on ragdoll death animations",
		},
		{
			"ragdoll_explode_force",
			"Explosive force applied to ragdolls",
		},
		{
			"ragdoll_explode_upbias",
			"Upwards bias applied to ragdoll explosion effects",
		},
		{
			"ragdoll_exploding_bullet_force",
			"Force applied to ragdolls from explosive bullets",
		},
		{
			"ragdoll_exploding_bullet_upbias",
			"Upwards bias applied to ragdoll from explosive bullets",
		},
		{
			"ragdoll_idle_min_velsq",
			"Minimum squared speed a ragdoll body needs to be moving before it will shut down due to time",
		},
		{
			"ragdoll_jitter_scale",
			"Scale up or down the effect of physics jitter on ragdolls",
		},
		{
			"ragdoll_jointlerp_time",
			"Default time taken to lerp down ragdoll joint friction",
		},
		{
			"ragdoll_max_life",
			"Max lifetime of a ragdoll system in msec",
		},
		{
			"ragdoll_max_simulating",
			"Max number of simultaneous active ragdolls - archived",
		},
		{
			"ragdoll_max_stretch_pct",
			"Force ragdoll limbs to not stretch more than this percentage in one frame",
		},
		{
			"ragdoll_resolve_penetration_bias",
			"Bias value on force to push ragdolls out of environment.",
		},
		{
			"ragdoll_rotvel_scale",
			"Ragdoll rotational velocity estimate scale",
		},
		{
			"ragdoll_self_collision_scale",
			"Scale the size of the collision capsules used to prevent ragdoll limbs from interpenetrating",
		},
		{
			"ragdoll_stretch_iters",
			"Iterations to run the alternate limb solver",
		},
		{
			"rankedPlayEndMatchKeepLobby",
			"keep the lobby if the lobby host is in our private party.",
		},
		{
			"recon_serverPort",
			"Recon Server Port (IP:Port)",
		},
		{
			"RemoteCameraSounds_DryLevel",
			"",
		},
		{
			"RemoteCameraSounds_RoomType",
			"",
		},
		{
			"RemoteCameraSounds_WetLevel",
			"",
		},
		{
			"replay_asserts",
			"Enable/Disable replay aborts due to inconsistency",
		},
		{
			"replay_autosave",
			"Use autosaves as part of demos - will make demo access faster but will cause hitches",
		},
		{
			"replay_autosaveOnError",
			"Auto save replay on error",
		},
		{
			"replay_time",
			"Draw replay time",
		},
		{
			"reportUserVoteInterval",
			"The interval in minutes you wait before getting another vote on this console as long as the console is turned on.",
		},
		{
			"requirestats",
			"Whether stats are currently required",
		},
		{
			"reset_mm_data",
			"reset data with dlc popup",
		},
		{
			"riotshield_bullet_damage_scale",
			"Value to scale bullet damage to deployed riotshield.",
		},
		{
			"riotshield_debug",
			"Riotshield debug.",
		},
		{
			"riotshield_deploy_extents",
			"Trace extents of the riotshield when planting",
		},
		{
			"riotshield_deploy_forward_offset",
			"Forward offset from the player when planting",
		},
		{
			"riotshield_deploy_limit_radius",
			"Min distance deployed riotshields must be from each other.",
		},
		{
			"riotshield_deploy_pitch_max",
			"Max surface pitch angle to allow riotshield deployment",
		},
		{
			"riotshield_deploy_roll_max",
			"Max surface roll angle to allow riotshield deployment",
		},
		{
			"riotshield_deploy_trace_parallel",
			"Report collisions when riotshield traces are parallel to plane of triangle.  If disabled traces parallel to triangle planes do not report collisions at all.",
		},
		{
			"riotshield_deploy_trigger_yoffset",
			"Y offset from the shield to place the trigger",
		},
		{
			"riotshield_deploy_trigger_zoffset",
			"Z offset from the shield to place the trigger",
		},
		{
			"riotshield_deploy_zdiff_max",
			"Max height difference allowed between plant surface and player origin",
		},
		{
			"riotshield_deployed_health",
			"Deployed riotshield health.",
		},
		{
			"riotshield_destroyed_cleanup_time",
			"Time (in seconds) destroyed riotshield model persists before disappearing",
		},
		{
			"riotshield_explosive_damage_scale",
			"Value to scale explosive damage to deployed riotshield..",
		},
		{
			"riotshield_melee_damage_scale",
			"Value to scale melee damage to deployed riotshield.",
		},
		{
			"riotshield_projectile_damage_scale",
			"Value to scale projectile damage to deployed riotshield.",
		},
		{
			"riotshield_use_mindot",
			"The minimum angle dot in order to pick up a deployed riot shield",
		},
		{
			"riotshield_use_radius",
			"The radius within which the player can pick up a deployed riot shield",
		},
		{
			"RunForTime",
			"Time for the server to run",
		},
		{
			"s_scriptable_physBaseExplosionScale",
			"Base multiplier applied to the explosion force calculations (consider this a 'range scaler').",
		},
		{
			"safeArea_adjusted_horizontal",
			"User-adjustable horizontal safe area as a fraction of the screen width",
		},
		{
			"safeArea_adjusted_vertical",
			"User-adjustable vertical safe area as a fraction of the screen height",
		},
		{
			"safeArea_horizontal",
			"Horizontal safe area as a fraction of the screen width",
		},
		{
			"safeArea_vertical",
			"Vertical safe area as a fraction of the screen height",
		},
		{
			"savegame_profile",
			"Profile the size of savegames.  Prints results to console after every save.",
		},
		{
			"scr_dof_enable",
			"enable dof",
		},
		{
			"screenshots_active",
			"Are we allowed to enable Screenshots or not",
		},
		{
			"scriptable_debug",
			"Debug mode for scriptables",
		},
		{
			"scriptable_enable",
			"Debug option to enable/disable spawning scriptable instances at run time.",
		},
		{
			"scriptable_explCamShakeDuration",
			"Duration, in MS, that a camera shake should last for during an explosion",
		},
		{
			"scriptable_explCamShakeRadiusMultiplier",
			"Multiplier applied to the outer radius of an explosion event to trigger the shake within.",
		},
		{
			"scriptable_explCamShakeScale",
			"Scale applied to the camera shake. Higher means more shaking.",
		},
		{
			"scriptIgnoreInfiniteLoops",
			"Allows script to loop infinitely without any script errors or warnings.",
		},
		{
			"sensitivity",
			"Mouse sensitivity",
		},
		{
			"sentry_placement_debug",
			"Enables sentry placement debug lines",
		},
		{
			"sentry_placement_feet_offset",
			"Position of the feet from the center axis.",
		},
		{
			"sentry_placement_feet_trace_dist_z",
			"Max distance for a foot to be considered touching the ground",
		},
		{
			"sentry_placement_trace_dist",
			"Distance along the trace axis where the sentry will attempt to position itself",
		},
		{
			"sentry_placement_trace_min_normal",
			"Minimum normal to accept a sentry position",
		},
		{
			"sentry_placement_trace_parallel",
			"Enable turret traces that are parallel to plane of triangle.  If 0, traces parallel to triangle planes do not report collisions at all.  If 2 (debug-only), then trace code ping pongs between new and old.",
		},
		{
			"sentry_placement_trace_pitch",
			"Pitch used for the trace axis",
		},
		{
			"sentry_placement_trace_radius",
			"Radius of the bound used for the placement trace",
		},
		{
			"sentry_placement_trace_radius_canon_safety",
			"Extra radius used in the forward direction to compensate for the canon length",
		},
		{
			"serverCulledSounds",
			"Enable culling of sounds on the server thread",
		},
		{
			"session_join_min_time",
			"Minimum number of milliseconds between join attempts",
		},
		{
			"session_nonblocking",
			"Non-blocking Session code",
		},
		{
			"shieldBlastDamageProtection_120",
			"How much protection a shield has against an explosion.  0.0 is none, 1.0 is full.  For explosions within a 30-degree arc of the shield.",
		},
		{
			"shieldBlastDamageProtection_180",
			"How much protection a shield has against an explosion.  0.0 is none, 1.0 is full.  For explosions within a 30-degree arc of the shield.",
		},
		{
			"shieldBlastDamageProtection_30",
			"How much protection a shield has against an explosion.  0.0 is none, 1.0 is full.  For explosions within a 30-degree arc of the shield.",
		},
		{
			"shieldBlastDamageProtection_60",
			"How much protection a shield has against an explosion.  0.0 is none, 1.0 is full.  For explosions within a 30-degree arc of the shield.",
		},
		{
			"shieldDeployShakeDuration",
			"Viewmodel shake duration for riotshield deploy.",
		},
		{
			"shieldDeployShakeScale",
			"Viewmodel shake scale for riotshield deploy.",
		},
		{
			"shieldImpactBulletShakeDuration",
			"Viewmodel shake duration for bullet impacts.",
		},
		{
			"shieldImpactBulletShakeScale",
			"Viewmodel shake scale for bullet impacts.",
		},
		{
			"shieldImpactExplosionHighShakeDuration",
			"Viewmodel shake duration for strong splash damage.",
		},
		{
			"shieldImpactExplosionHighShakeScale",
			"Viewmodel shake scale for strong splash damage.",
		},
		{
			"shieldImpactExplosionLowShakeDuration",
			"Viewmodel shake duration for weak splash damage.",
		},
		{
			"shieldImpactExplosionLowShakeScale",
			"Viewmodel shake scale for weak splash damage.",
		},
		{
			"shieldImpactExplosionThreshold",
			"Pre-shield splash damage that is above this will be 'strong'.",
		},
		{
			"shieldImpactMissileShakeDuration",
			"Viewmodel shake duration for direct missile impacts.",
		},
		{
			"shieldImpactMissileShakeScale",
			"Viewmodel shake scale for direct missile impacts.",
		},
		{
			"shieldPlayerBulletProtectionDegrees",
			"Bullets fired at a riotshield-bearing player will be blocked if they are within this many degrees of the player's forward direction.  Higher value means wider protection.",
		},
		{
			"showDebugAmmoCounter",
			"Show the debug ammo counter when unable to show ar ammo counter",
		},
		{
			"showPlaylistTotalPlayers",
			"Toggle the display of the total number of players in a playlist and online",
		},
		{
			"si_validateFileAccess",
			"Validate accessed files against install file list.",
		},
		{
			"sm_cacheSpotShadows",
			"Cache spot shadow maps, improves shadow map performance at the cost of memory (requires vid_restart)",
		},
		{
			"sm_cacheSpotShadowsEnabled",
			"Enables caching of spot shadows.",
		},
		{
			"sm_cacheSunShadow",
			"Cache sun shadow map, improves shadow map performance at the cost of memory (requires vid_restart)",
		},
		{
			"sm_cacheSunShadowEnabled",
			"Enables caching of sun-based shadows.",
		},
		{
			"sm_cameraOffset",
			"",
		},
		{
			"sm_debugFastSunShadow",
			"Debug fast sun shadow",
		},
		{
			"sm_dynlightAllSModels",
			"Enable, from script, rendering all static models in dynamic light volume when shadow mapping",
		},
		{
			"sm_enable",
			"Enable shadow mapping",
		},
		{
			"sm_fastSunShadow",
			"Fast sun shadow",
		},
		{
			"sm_forceLinear",
			"Force shadow sampling to be linear",
		},
		{
			"sm_lightScore_eyeProjectDist",
			"When picking shadows for primary lights, measure distance from a point this far in front of the camera.",
		},
		{
			"sm_lightScore_spotProjectFrac",
			"When picking shadows for primary lights, measure distance from a point this far in front of the camera.",
		},
		{
			"sm_maxLightsWithShadows",
			"Limits how many primary lights can have shadow maps",
		},
		{
			"sm_minSpotLightScore",
			"Minimum score (based on intensity, radius, and position relative to the camera) for a spot light to have shadow maps.",
		},
		{
			"sm_polygonOffsetBias",
			"Shadow map offset bias",
		},
		{
			"sm_polygonOffsetClamp",
			"Shadow map offset clamp",
		},
		{
			"sm_polygonOffsetPreset",
			"Shadow map polygon offset preset.",
		},
		{
			"sm_polygonOffsetScale",
			"Shadow map offset scale",
		},
		{
			"sm_projStepSize",
			"",
		},
		{
			"sm_qualitySpotShadow",
			"Fast spot shadow",
		},
		{
			"sm_shadowUseTweaks",
			"Override shadow LightSet settings with tweak dvar values.",
		},
		{
			"sm_showCachedSunOverlay",
			"Show cached sun shadow map overlay; dvar value picks the partition.",
		},
		{
			"sm_showOverlay",
			"Show shadow map overlay",
		},
		{
			"sm_showOverlayDepthBounds",
			"Near and far depth values for the shadow map overlay",
		},
		{
			"sm_showOverlayTrans",
			"Show transparency shadow map overlay",
		},
		{
			"sm_spotDistCull",
			"Distance cull spot shadows",
		},
		{
			"sm_spotEnable",
			"Enable spot shadow mapping from script",
		},
		{
			"sm_spotFilterRadius",
			"Spot soft shadows filter radius",
		},
		{
			"sm_spotLightScoreModelScale",
			"Scale the calculated spot light score by this value if the light currently only affects static or script brush models.",
		},
		{
			"sm_spotLightScoreRadiusPower",
			"Power to apply to light radius when determining spot light shadowing score (1.0 means radius scales up score a lot, 0.0 means don't scale score using radius)",
		},
		{
			"sm_spotLimit",
			"Limit number of spot shadows from script",
		},
		{
			"sm_spotShadowFadeTime",
			"How many seconds it takes for a primary light shadow map to fade in or out",
		},
		{
			"sm_strictCull",
			"Strict shadow map cull",
		},
		{
			"sm_sunEnable",
			"Enable sun shadow mapping from script",
		},
		{
			"sm_sunFilterRadius",
			"Sun soft shadows filter radius",
		},
		{
			"sm_sunSampleSizeNear",
			"Shadow sample size",
		},
		{
			"sm_sunShadowBoundsMax",
			"Max Shadow Bounds",
		},
		{
			"sm_sunShadowBoundsMin",
			"Min Shadow Bounds",
		},
		{
			"sm_sunShadowBoundsOverride",
			"Override Shadow Bounds",
		},
		{
			"sm_sunShadowCenter",
			"Sun shadow center, 0 0 0 means don't override",
		},
		{
			"sm_sunShadowCenterMode",
			"When false center value only used for far map, when true sets both maps",
		},
		{
			"sm_sunShadowScale",
			"Sun shadow scale optimization",
		},
		{
			"sm_usedSunCascadeCount",
			"How many shadow cascade we are using",
		},
		{
			"snd_allowHeadphoneHRTF",
			"Enable HRTF over headphones",
		},
		{
			"snd_announcerVoicePrefix",
			"Local mp announcer voice to use",
		},
		{
			"snd_autoSim",
			"turn on client side simulation of automatic gun sounds",
		},
		{
			"snd_cinematicVolumeScale",
			"Scales the volume of Bink videos.",
		},
		{
			"snd_debugAlias",
			"Print out tracking information about a particular alias",
		},
		{
			"snd_debugReplace",
			"Print out information about when we stop a playing sound to play another",
		},
		{
			"snd_detectedSpeakerConfig",
			"speaker configuration:\n0: autodetect\n1: mono\n2: stereo\n4: quadrophonic\n6: 5.1 surround\n8: 7.1 surround",
		},
		{
			"snd_dolbyPanning",
			"Enables experimental Dolby-standard -3dB pan laws and speaker angles",
		},
		{
			"snd_dopplerAuditionEnable",
			"Enables doppler calculation preview mode",
		},
		{
			"snd_dopplerBaseSpeedOfSound",
			"The base speed of sound used in doppler calculation",
		},
		{
			"snd_dopplerEnable",
			"Enables doppler calculation",
		},
		{
			"snd_dopplerPitchMax",
			"Maximum pitch that can be legally applied by doppler",
		},
		{
			"snd_dopplerPitchMin",
			"Minimum pitch that can be legally applied by doppler",
		},
		{
			"snd_dopplerPlayerVelocityScale",
			"The scale of the player velocity, relative the the sound source velocity, when applied to the doppler calculation",
		},
		{
			"snd_dopplerSmoothing",
			"Smoothing factor applied to doppler to eliminate jitter or sudden acceleration changes",
		},
		{
			"snd_draw3D",
			"Draw the position and info of world sounds",
		},
		{
			"snd_drawEqChannels",
			"Draw overlay of EQ settings for each channel",
		},
		{
			"snd_drawEqEnts",
			"Show which ents can have EQ turned on/off, which ones are on (green) and off (magenta)",
		},
		{
			"snd_drawInfo",
			"Draw debugging information for sounds",
		},
		{
			"snd_drawSubmixInfo",
			"Displays a list of active sound submixes",
		},
		{
			"snd_enable2D",
			"Enable 2D sounds",
		},
		{
			"snd_enable3D",
			"Enable 3D sounds",
		},
		{
			"snd_enableEq",
			"Enable equalization filter",
		},
		{
			"snd_enableReverb",
			"Enable sound reverberation",
		},
		{
			"snd_enableStream",
			"Enable streamed sounds",
		},
		{
			"snd_envFollowerBuffScale",
			"Amount of buffer to use for envelope follower. Smaller value indicates faster envelope.",
		},
		{
			"snd_errorOnMissing",
			"Cause a Com_Error if a sound file is missing.",
		},
		{
			"snd_focusGlobalAngleMax",
			"",
		},
		{
			"snd_focusGlobalAngleMin",
			"Angle describing the conde where focus attenuation begins (no attenuation).",
		},
		{
			"snd_focusGlobalAttenuation",
			"",
		},
		{
			"snd_focusGlobalEnabled",
			"Enable the global sound focus.",
		},
		{
			"snd_focusWeaponAngleMax",
			"",
		},
		{
			"snd_focusWeaponAngleMin",
			"Angle describing the cone where focus attenuation ends (full attenuation).",
		},
		{
			"snd_focusWeaponAttenuation",
			"",
		},
		{
			"snd_focusWeaponEnableDVars",
			"Enable the weapon sound focus parameter override with DVars.",
		},
		{
			"snd_focusWeaponFadeTime_ms",
			"Time in milliseconds until which the feature is totally in effect after activation.",
		},
		{
			"snd_hitsoundDisabled",
			"Disable the hit indicator sound",
		},
		{
			"snd_inheritSecondaryPitchVol",
			"Set to true for secondary aliases to inherit the pitch of the parent",
		},
		{
			"snd_levelFadeTime",
			"The amout of time in milliseconds for all audio to fade in at the start of a level",
		},
		{
			"snd_loadFadeTime",
			"Fade time for loading from a checkpoint after death.",
		},
		{
			"snd_logAudioVoices",
			"Log to a file, every frame, the info on all voices playing",
		},
		{
			"snd_logPlayback",
			"Logs all alias playback to the console",
		},
		{
			"snd_loopFadeTime",
			"Fade-in time for looping sounds.",
		},
		{
			"snd_loudVO",
			"Doubles the headroom for non-dialogue volmods",
		},
		{
			"snd_musicDisabledForCustomSoundtrack",
			"Disable all in-game music due to user playing a custom soundtrack",
		},
		{
			"snd_muteAlias",
			"Mute every alias that contain those strings",
		},
		{
			"snd_occlusionDelay",
			"Minimum delay in (ms) between occlusion updates",
		},
		{
			"snd_occlusionEnabled",
			"Enable occlusion",
		},
		{
			"snd_occlusionLerpTime",
			"Time to lerp to target occlusion lerp when occluded",
		},
		{
			"snd_peakLimiterCompression",
			"Peak limiter compression factor. The output data is scaled by this and then normalized: F < 1 = disabled; F >= 1 enabled.",
		},
		{
			"snd_peakLimiterDecay",
			"Peak limiter compression decay ratio.",
		},
		{
			"snd_peakLimiterSustainFrames",
			"Number of frames to sustain the limiter peak. 1 frame = 10 msec.",
		},
		{
			"snd_premixVolume",
			"Game sound pre-mix volume",
		},
		{
			"snd_realtimeAliasUpdate",
			"Update a specific parameter for a particular alias",
		},
		{
			"snd_realtimeCurveUpdate",
			"Update a specific curve (LPF, Reverb, Volume fallof)",
		},
		{
			"snd_realtimeDumpSubmixChanges",
			"Activating this dvar will create a debug file containing every tweaks made to submix, done with the snd_realtimeSubmixUpdate dvar",
		},
		{
			"snd_realtimeSubmixUpdate",
			"Update a submix",
		},
		{
			"snd_reverbZoneOutsideFactor",
			"When a 3d sound is played in a different reverb zone than the player, this factor will be applied to its wet level.",
		},
		{
			"snd_slaveFadeTime",
			"The amount of time in milliseconds for a 'slave' sound\nto fade its volumes when a master sound starts or stops",
		},
		{
			"snd_soloAlias",
			"Mute every alias that do not contain those strings",
		},
		{
			"snd_sortOverlay",
			"It will sort the channels shown when using snd_drawinfo",
		},
		{
			"snd_speakerConfig",
			"speaker configuration:\n0: autodetect\n1: mono\n2: stereo\n4: quadrophonic\n6: 5.1 surround\n8: 7.1 surround",
		},
		{
			"snd_sustainPeakVU",
			"Display the peak value on the VU for this period of time before resetting.",
		},
		{
			"snd_useOldPanning",
			"Use old and busted panning",
		},
		{
			"snd_virtualChannelInfo",
			"Display virtual voice info.",
		},
		{
			"snd_virtualForce",
			"Forces all sounds to be initially virtual.",
		},
		{
			"snd_virtualMinDur",
			"The minimum duration (in seconds) of a sound if it is to be added to the virtual voice buffer.",
		},
		{
			"snd_virtualMinPri",
			"The minimum priority of an alias if it is to be added to the virtual voice buffer.",
		},
		{
			"snd_virtualMinTimeLeftToRevive",
			"The minimum time (in ms) left in a sample in order to attempt to revive it.",
		},
		{
			"snd_virtualReviveVoices",
			"Whether or not to restore virtual voices.",
		},
		{
			"snd_virtualWaitToReviveTime",
			"The minimum time (in ms) to wait before trying to revive the voice.",
		},
		{
			"snd_volume",
			"Game sound master volume",
		},
		{
			"so_survival",
			"special ops - survival mode",
		},
		{
			"solo_play",
			"User is in solo Spec Ops.",
		},
		{
			"sp_matchdata_active",
			"Are SP match data uploads enabled",
		},
		{
			"sp_matchdata_maxcompressionbuffer",
			"",
		},
		{
			"specialops",
			"special ops",
		},
		{
			"specops_map_groupnum",
			"Level challenge group number",
		},
		{
			"specops_map_itemnum",
			"Level challenge item number",
		},
		{
			"speech_active",
			"Are we allowed to enable Speech or not",
		},
		{
			"splitscreen",
			"Current game is a splitscreen game",
		},
		{
			"stopspeed",
			"The player deceleration",
		},
		{
			"stringtable_debug",
			"spam debug info for stringtable lookups",
		},
		{
			"sv_cheats",
			"Turn on cheats: do not access directly - access through CheatsOk for server demo",
		},
		{
			"sv_debugTrackServerTime",
			"Enable server time track debugging.",
		},
		{
			"sv_loadingsavegame",
			"Is loading up a previous save",
		},
		{
			"sv_running",
			"Server is running",
		},
		{
			"sv_saveDeviceAvailable",
			"True if the save device is currently available",
		},
		{
			"sv_saveGameAvailable",
			"True if the save game is currently available",
		},
		{
			"sv_saveGameNotReadable",
			"True if the save game is not readable",
		},
		{
			"sv_savegametransients",
			"Comma-separated list of transients that should be loaded with this savegame (check sv_loadingsavegame first!)",
		},
		{
			"sv_saveOnStartMap",
			"Save at the start of a level",
		},
		{
			"sv_trackFrameMsecThreshold",
			"server frame time that will trigger script time tracking.",
		},
		{
			"sv_znear",
			"Things closer than this aren't drawn.  Reducing this increases z-fighting in the distance.",
		},
		{
			"svwp",
			"playerdata server write protection: 0 = disable, 1 = silent, 2 = kick",
		},
		{
			"syncTimeTimeout",
			"default timeout for sync time task (in seconds)",
		},
		{
			"sys_language",
			"",
		},
		{
			"theater_active",
			"Are we allowed to show theater or not.",
		},
		{
			"thermalBlurFactorNoScope",
			"Amount of blur to use when drawing blur through a weapon's thermal scope.",
		},
		{
			"thermalBlurFactorScope",
			"Amount of blur to use when drawing blur through a weapon's thermal scope.",
		},
		{
			"timescale",
			"",
		},
		{
			"tokensEnabled",
			"Is token economy enabled",
		},
		{
			"tracer_debugDraw",
			"Draw debug lines where the tracers should be visible.",
		},
		{
			"tracer_disable",
			"Disable all weapon tracers.",
		},
		{
			"tracer_firstPersonMaxWidth",
			"The maximum width our OWN tracers can be when looking through our ADS",
		},
		{
			"tracer_thermalWidthMult",
			"The multiplier applied to the base width when viewed in thermal vision",
		},
		{
			"traceScheduler_debugThrottleTraces",
			"Displays debug info for trace throttling.",
		},
		{
			"traceScheduler_throttleTraces",
			"Throttles locational traces on the server.",
		},
		{
			"transients_load_delay",
			"Enables an artificial delay (in seconds) that will be added to SP transient fastfile load times. Useful to figure out if transient syncs are missing.",
		},
		{
			"transients_verbose",
			"Verbose logging information for transient fastfiles.",
		},
		{
			"trigger_draw",
			"Draw trigger geometry",
		},
		{
			"trigger_drawDepthTest",
			"Display trigger geo with depth information",
		},
		{
			"trigger_drawDistance",
			"Trigger draw distance",
		},
		{
			"triggerDLCEnumerationOnSocialConfigLoad",
			"Triggers a new DLC enumeration after social config has loaded.",
		},
		{
			"turret_adsEnabled",
			"Enable/Disable ADS on turrets",
		},
		{
			"turret_adsTime",
			"Time (msec) to transition to ADS",
		},
		{
			"turretConvergenceHeightDebug",
			"When true, turns on debugging for turret converging to enemy head",
		},
		{
			"turretPlayerAvoidScale",
			"Auto turrets will try to avoid the player.  They will not choose a target that is within a cone around the player.  The diameter of the cone is the player's height, so the cone is smaller, the farther the player is from the turret.  Use this dvar to scale the cone size.",
		},
		{
			"turretSentryRestrictUsageToOwner",
			"When true, only players that own the sentry turret are allowed to interact with it.",
		},
		{
			"turretTargetDebug",
			"When true, turns on turret target debugging",
		},
		{
			"ufoHitsTriggers",
			"ufo/noclip will hit triggers when enabled",
		},
		{
			"ui_allowSkip",
			"Overrides unskippable cutscenes",
		},
		{
			"ui_autoContinue",
			"Automatically 'click to continue' after loading a level",
		},
		{
			"ui_blurAmount",
			"Max amount to blur background menu items.",
		},
		{
			"ui_blurDarkenAmount",
			"Amount to darken blurred UI.",
		},
		{
			"ui_blurTime",
			"Time in milliseconds to fade in/out the blur.",
		},
		{
			"ui_borderLowLightScale",
			"Scales the border color for the lowlight color on certain UI borders",
		},
		{
			"ui_buildLocation",
			"Where to draw the build number",
		},
		{
			"ui_buildSize",
			"Font size to use for the build number",
		},
		{
			"ui_campaign",
			"The current campaign",
		},
		{
			"ui_chyronGameMessages",
			"True if game messages should be sent through script to get the chyron treatment",
		},
		{
			"ui_cinematicsTimestamp",
			"Shows cinematics timestamp on subtitle UI elements.",
		},
		{
			"ui_contextualMenuLocation",
			"Contextual menu location from where you entered the store.",
		},
		{
			"ui_currentLevelIndex",
			"The index of the current level in script",
		},
		{
			"ui_debugMode",
			"Shows ui debug information on screen.",
		},
		{
			"ui_disableInGameStore",
			"This will disable the ingame store button on the xbox live menu.",
		},
		{
			"ui_disableTokenRedemption",
			"This will disable the token redemption option in the in-game store menu.",
		},
		{
			"ui_gametype",
			"Current game type",
		},
		{
			"ui_hideMap",
			"Meant to be set by script and referenced by menu files to determine if minimap should be drawn.",
		},
		{
			"ui_inGameStoreOpen",
			"is the InGameStore open",
		},
		{
			"ui_inSpecOpsLeaderboards",
			"User is in Spec Ops Leaderboards.",
		},
		{
			"ui_is_so_dlc",
			"Is the user is attempting to play only spec ops DLC maps",
		},
		{
			"ui_isSaving",
			"True if the game is currently saving",
		},
		{
			"ui_loadMenuName",
			"Frontend menu will start on this level instead of lockout",
		},
		{
			"ui_mapname",
			"Current map name",
		},
		{
			"ui_mousePitch",
			"Invert mouse pitch",
		},
		{
			"ui_multiplayer",
			"True if the game is multiplayer",
		},
		{
			"ui_myPartyColor",
			"Player name font color when in the same party as the local player",
		},
		{
			"ui_nextMission",
			"Next mission",
		},
		{
			"ui_online_coop",
			"Did the user choose to enter an online coop game",
		},
		{
			"ui_play_credits",
			"Should we play the credits",
		},
		{
			"ui_safearea",
			"Show the Screen Safe overlay",
		},
		{
			"ui_savegame",
			"Save game name",
		},
		{
			"ui_saveMessageMinTime",
			"Minumum time for the save message to be on screen in seconds",
		},
		{
			"ui_selectedFeederMap",
			"Current preview game type",
		},
		{
			"ui_showList",
			"Show list of currently visible menus",
		},
		{
			"ui_showMenuOnly",
			"If set, only menus using this name will draw.",
		},
		{
			"ui_skip_initial_game_mode",
			"Forces boot flow to ignore short circuit to default choice",
		},
		{
			"ui_skipMainLockout",
			"True if the page that restricts player control to a single controller is skipped",
		},
		{
			"ui_sliderSteps",
			"The number of steps for a slider itemdef",
		},
		{
			"ui_startupActiveController",
			"default active game pad on start up",
		},
		{
			"ui_textScrollFadeTime",
			"Text scrolling takes this long (seconds) to fade out at the end before restarting",
		},
		{
			"ui_textScrollPauseEnd",
			"Text scrolling waits this long (seconds) before starting",
		},
		{
			"ui_textScrollPauseStart",
			"Text scrolling waits this long (seconds) before starting",
		},
		{
			"ui_textScrollSpeed",
			"Speed at which text scrolls vertically",
		},
		{
			"uiscript_debug",
			"spam debug info for the ui script",
		},
		{
			"uiscript_verbose",
			"Turns on extra ui script debugging console prints",
		},
		{
			"unattended",
			"Allows the game to be run without user input.",
		},
		{
			"unlockAllAttachments",
			"Simulate all attachments being unlocked",
		},
		{
			"unlockAllEntitlements",
			"Development dvar to simulate having entitlements unlocked",
		},
		{
			"unlockAllItems",
			"Simulate all items being unlocked",
		},
		{
			"unlockAllLootItems",
			"Simulate all loot items being locked",
		},
		{
			"unlockDevAttachments",
			"Unlocks all dev-only attachments in the front-end",
		},
		{
			"unlockDevWeapons",
			"Unlocks all dev weapons in the front-end",
		},
		{
			"uno_current_tos_version",
			"Current Uno Terms of Service Version",
		},
		{
			"use_new_sva_system",
			"Temp flag to help depricate the old scripted viewmodel system. This will be removed when all of the old data is gone.",
		},
		{
			"useCPMarkerForCPOwnership",
			"If set, we will check the player inventory to see if he owns the redeemedItem for a contentPack if this contentPack is not available for the player",
		},
		{
			"useonlinestats",
			"Whether to use online stats when in offline modes",
		},
		{
			"userFileFetchTimeout",
			"default timeout for user files FETCH tasks (in seconds)",
		},
		{
			"userGroup_active",
			"Are we allowed to show Usergroups or not",
		},
		{
			"userGroup_cool_off_time",
			"Cool off time between calls to fetch the elite clan",
		},
		{
			"userGroup_coop_delay",
			"Delay between a player joining a coop lobby and the DW user group task starting",
		},
		{
			"userGroup_max_retry_time",
			"Max time that the usergroup read find can retry",
		},
		{
			"userGroup_refresh_time_secs",
			"Time in seconds between re-sending lobby group data to confirmed users.",
		},
		{
			"userGroup_retry_step",
			"Step in m/s for the usegroup read retry",
		},
		{
			"userGroup_RetryTime",
			"Time in ms between sending lobby group data retrys.",
		},
		{
			"useStatsGroups",
			"If true then StatsGroups are in use for all playerdata.ddl accessing.",
		},
		{
			"useTagFlashSilenced",
			"When true, silenced weapons will use \"tag_flash_silenced\" instead of \"tag_flash\".",
		},
		{
			"using_mlg",
			"MLG feature on/off",
		},
		{
			"veh_aiOverSteerScale",
			"Scaler used to cause ai vehicles to over steer",
		},
		{
			"veh_boneControllerLodDist",
			"Distance at which bone controllers are not updated.",
		},
		{
			"veh_boneControllerUnLodDist",
			"Distance at which bone controllers start updating when not moving.",
		},
		{
			"veh_drawTrack",
			"Debug draw the vehicle tracks",
		},
		{
			"vehAudio_inAirPitchDownLerp",
			"Rate at which the pitch lerps down",
		},
		{
			"vehAudio_inAirPitchUpLerp",
			"Rate at which the pitch lerps up",
		},
		{
			"vehAudio_spawnVolumeTime",
			"Seconds it takes for spawned vehicles to reach full volume.",
		},
		{
			"vehBoatRockingScale",
			"Scale of the boat rocking (usually used to turn off rocking during cinematics)",
		},
		{
			"vehCam_angles",
			"Camera angles from vehicle",
		},
		{
			"vehCam_angles3P",
			"Camera angles from vehicle (3rd person)",
		},
		{
			"vehCam_chaseAngleOffset",
			"Angle offset from vehicle angles",
		},
		{
			"vehCam_chaseDist",
			"Camera distance from vehicle",
		},
		{
			"vehCam_chaseLateralInfluence",
			"Vehicle's lateral velocity's influence on the camera's left and right movement.  This will be multiplied with the chase distance.",
		},
		{
			"vehCam_chaseMinRollForLateralMovement",
			"The minimum roll required before the camera will begin to move laterally",
		},
		{
			"vehCam_chaseMinRollTime",
			"The minimum time for the plane to be rolling in one direction before the camera reacts to it",
		},
		{
			"vehCam_chaseOffset",
			"",
		},
		{
			"vehCam_chasePitchLerpRateModifier",
			"Multiplied by the frame time to determine the yaw lerp rate",
		},
		{
			"vehCam_chaseVerticalInfluence",
			"Vehicle's vertical velocity's influence on the camera's up and down movement",
		},
		{
			"vehCam_chaseYawLerpRateModifier",
			"Multiplied by the frame time to determine the yaw lerp rate",
		},
		{
			"vehCam_freeLook",
			"Enables free look mode",
		},
		{
			"vehCam_mode",
			"Camera modes: 1st person, 3rd person, or bot",
		},
		{
			"vehCam_offset",
			"",
		},
		{
			"vehCam_offset3P",
			"Focus offset from vehicle origin (3rd person)",
		},
		{
			"vehCam_pitchClamp",
			"Pitch clamp for user adjustment",
		},
		{
			"vehCam_pitchClamp3P",
			"Pitch clamp for user adjustment (3rd person)",
		},
		{
			"vehCam_pitchTurnRate",
			"Pitch turn rate for user adjustment",
		},
		{
			"vehCam_pitchTurnRate3P",
			"Pitch turn rate for user adjustment (3rd person)",
		},
		{
			"vehCam_radius",
			"Camera radius from vehicle",
		},
		{
			"vehCam_radius3P",
			"Camera radius from vehicle (3rd person)",
		},
		{
			"vehCam_speedInfluence",
			"Controls how much the vehicle's speed effects the camera.",
		},
		{
			"vehCam_speedInfluence3P",
			"Controls how much the vehicle's speed effects the camera.",
		},
		{
			"vehCam_yawClamp",
			"Yaw clamp for user adjustment",
		},
		{
			"vehCam_yawClamp3P",
			"Yaw clamp for user adjustment (3rd person)",
		},
		{
			"vehCam_yawTurnRate",
			"Yaw turn rate for user adjustment",
		},
		{
			"vehCam_yawTurnRate3P",
			"Yaw turn rate for user adjustment (3rd person)",
		},
		{
			"vehCam_zOffsetMode",
			"Camera offset mode for Z axis",
		},
		{
			"vehCam_zOffsetMode3P",
			"Camera offset mode for Z axis (3rd person)",
		},
		{
			"vehDiveboatControlScheme",
			"0: like the jetbike.  1: like the pitbull.  2: like the pitbull, but with dive on LTRIG.",
		},
		{
			"vehHelicopterBoundsRadius",
			"The radius of the collision volume to be used when colliding with world geometry.",
		},
		{
			"vehHelicopterControlsAltitude",
			"Determines how to control altitude",
		},
		{
			"vehHelicopterControlSystem",
			"Determines how the helicopter will be controlled",
		},
		{
			"vehHelicopterDecelerationFwd",
			"Set the deceleration of the player helicopter (as a fraction of acceleration) in the direction the chopper is facing.  So 1.0 makes it equal to the acceleration.",
		},
		{
			"vehHelicopterDecelerationSide",
			"Set the side-to-side deceleration of the player helicopter (as a fraction of acceleration).  So 1.0 makes it equal to the acceleration.",
		},
		{
			"vehHelicopterDecelerationUp",
			"Set the vertical deceleration of the player helicopter (as a fraction of acceleration).  So 1.0 makes it equal to the acceleration.",
		},
		{
			"vehHelicopterForwardDrag",
			"The forward deceleration caused by drag in control system 2",
		},
		{
			"vehHelicopterFreeLookReleaseSpeed",
			"The rate that the player's view moves back to center when freelook is released",
		},
		{
			"vehHelicopterHeadSwayDontSwayTheTurret",
			"If set, the turret will not fire through the crosshairs, but straight ahead of the vehicle, when the player is not freelooking.",
		},
		{
			"vehHelicopterHoverSpeedThreshold",
			"The speed below which the player helicopter begins to jitter the tilt, for hovering",
		},
		{
			"vehHelicopterInvertUpDown",
			"Invert the altitude control on the player helicopter.",
		},
		{
			"vehHelicopterJitterJerkyness",
			"Specifies how jerky the tilt jitter should be",
		},
		{
			"vehHelicopterLateralDrag",
			"The lateral deceleration caused by drag in control system 2",
		},
		{
			"vehHelicopterLookaheadTime",
			"How far ahead (in seconds) the player helicopter looks ahead, to avoid hard collisions.  (Like driving down the highway, you should keep 2 seconds distance between you and the vehicle in front of you)",
		},
		{
			"vehHelicopterMaxAccel",
			"Maximum horizontal acceleration of the player helicopter (in MPH per second)",
		},
		{
			"vehHelicopterMaxAccelVertical",
			"Maximum vertical acceleration of the player helicopter (in MPH per second)",
		},
		{
			"vehHelicopterMaxAltitude",
			"Determines the maximum altitude at which a helicopter can fly",
		},
		{
			"vehHelicopterMaxPitch",
			"Maximum pitch of the player helicopter",
		},
		{
			"vehHelicopterMaxRoll",
			"Maximum roll of the player helicopter",
		},
		{
			"vehHelicopterMaxSpeed",
			"Maximum horizontal speed of the player helicopter (in MPH)",
		},
		{
			"vehHelicopterMaxSpeedVertical",
			"Maximum vertical speed of the player helicopter (in MPH)",
		},
		{
			"vehHelicopterMaxYawAccel",
			"Maximum yaw acceleration of the player helicopter",
		},
		{
			"vehHelicopterMaxYawRate",
			"Maximum yaw speed of the player helicopter",
		},
		{
			"vehHelicopterMinAltitude",
			"Determines the minimum altitude at which a helicopter can fly",
		},
		{
			"vehHelicopterPitchLock",
			"True if the view pitches with the body of the helicopter",
		},
		{
			"vehHelicopterPitchOffset",
			"The resting pitch of the helicopter",
		},
		{
			"vehHelicopterPitchRate",
			"The pitch speed of the helicopter in control system 2",
		},
		{
			"vehHelicopterPitchReturn",
			"The rate the pitch returns to the rest pitch of the helicopter in control system 2",
		},
		{
			"vehHelicopterPitchThrust",
			"The forward acceleration produced by pitching forward in control system 2",
		},
		{
			"vehHelicopterRightStickDeadzone",
			"Dead-zone for the axes of the right thumbstick.  This helps to better control the two axes separately.",
		},
		{
			"vehHelicopterRollRate",
			"The roll speed of the helicopter in control system 2",
		},
		{
			"vehHelicopterRollReturn",
			"The rate the roll returns to the rest roll of the helicopter in control system 2",
		},
		{
			"vehHelicopterRollThrust",
			"The lateral acceleration produced by rolling to the side in control system 2",
		},
		{
			"vehHelicopterScaleMovement",
			"Scales down the smaller of the left stick axes.",
		},
		{
			"vehHelicopterSoftCollisions",
			"Player helicopters have soft collisions (slow down before they collide).",
		},
		{
			"vehHelicopterStrafeDeadzone",
			"Dead-zone so that you can fly straight forward easily without accidentally strafing (and thus rolling).",
		},
		{
			"vehHelicopterTiltFromAcceleration",
			"The amount of tilt caused by acceleration",
		},
		{
			"vehHelicopterTiltFromControllerAxes",
			"The amount of tilt caused by the desired velocity (i.e., the amount of controller stick deflection)",
		},
		{
			"vehHelicopterTiltFromDeceleration",
			"The amount of tilt caused by deceleration",
		},
		{
			"vehHelicopterTiltFromFwdAndYaw",
			"The amount of roll caused by yawing while moving forward.",
		},
		{
			"vehHelicopterTiltFromFwdAndYaw_VelAtMaxTilt",
			"The forward speed (as a fraction of top speed) at which the tilt due to yaw reaches is maximum value.",
		},
		{
			"vehHelicopterTiltFromLook",
			"The contribution in degrees of view pitch to vehicle pitch in control system 1",
		},
		{
			"vehHelicopterTiltFromLookRate",
			"The rate the vehicle pitches due to view pitch in control system 1",
		},
		{
			"vehHelicopterTiltFromVelocity",
			"The amount of tilt caused by the current velocity",
		},
		{
			"vehHelicopterTiltMomentum",
			"The amount of rotational momentum the helicopter has with regards to tilting.",
		},
		{
			"vehHelicopterTiltSpeed",
			"The rate at which the player helicopter's tilt responds",
		},
		{
			"vehHelicopterVerticalDrag",
			"The vertical deceleration caused by drag in control system 2",
		},
		{
			"vehHelicopterYawOnLeftStick",
			"The yaw speed created by the left stick when pushing the stick diagonally (e.g., moving forward and strafing slightly).",
		},
		{
			"vehHudDrawPipOnStickWhenFreelooking",
			"Set to 0 to not draw the pip-on-a-stick reticle when the player is freelooking",
		},
		{
			"vehHudLineWidth",
			"The width of the line used by code to draw elements on the vehicle HUD",
		},
		{
			"vehHudReticleBouncingDiamondSize",
			"The size of the bouncing diamond quad.",
		},
		{
			"vehHudReticleBouncingRadius",
			"The radius of the circle in which the diamond bounces.",
		},
		{
			"vehHudReticleBouncingSpeed",
			"The rate at which the bouncing diamond moves",
		},
		{
			"vehHudReticlePipOnAStickCenterCircle",
			"The diameter of the small, center circle in the pip-on-a-stick reticle",
		},
		{
			"vehHudReticlePipOnAStickCenterCircleBuffer",
			"Tweaks how close the stick is drawn to the center circle.  Positive numbers makes the line longer.",
		},
		{
			"vehHudReticlePipOnAStickMovingCircle",
			"The diameter of the large, moving circle in the pip-on-a-stick reticle",
		},
		{
			"vehHudReticlePipOnAStickMovingCircleBuffer",
			"Tweaks how close the stick is drawn to the center circle.  Positive numbers makes the line longer.",
		},
		{
			"vehHudTargetScreenEdgeClampBufferBottom",
			"",
		},
		{
			"vehHudTargetScreenEdgeClampBufferLeft",
			"",
		},
		{
			"vehHudTargetScreenEdgeClampBufferRight",
			"",
		},
		{
			"vehHudTargetScreenEdgeClampBufferTop",
			"",
		},
		{
			"vehHudTargetSize",
			"The width of the enemy target indicator on the hud.",
		},
		{
			"vehicle_debug_render_spline_plane",
			"Do we want to render the spline plane data",
		},
		{
			"vehicle_pathsmooth",
			"Smoothed vehicle pathing.",
		},
		{
			"vehicle_pathsmoothdebug",
			"Debug smoothed vehicle pathing.",
		},
		{
			"vehJetbikeControlScheme",
			"Which control scheme to use",
		},
		{
			"vehPlaneAiLiftModifier",
			"ai planes' wing areas are multiplied by this value",
		},
		{
			"vehPlaneAiNoClip",
			"true if planes in AI mode should ignore collision when moving.",
		},
		{
			"vehPlaneAiPitchResponseRate",
			"ai pitch fraction per degree of pitch error",
		},
		{
			"vehPlaneAiRollResponseRate",
			"ai roll fraction per degree of roll error",
		},
		{
			"vehPlaneAiThrottleModifier",
			"ai planes can control throttle up to this multiplier of the plane's actual max thrust",
		},
		{
			"vehPlaneAiThrottleResponseRate",
			"ai throttle fraction per second per knot of speed error",
		},
		{
			"vehPlaneAiYawResponseRate",
			"ai pitch fraction per degree of yaw error",
		},
		{
			"vehPlaneChordPitchCenter",
			"the wing's aoa when the body is level.",
		},
		{
			"vehPlaneChordPitchMax",
			"the max AOA the auto pitch trim will attempt to drive us to",
		},
		{
			"vehPlaneChordPitchMin",
			"the min AOA the auto pitch trim will attempt to drive us to",
		},
		{
			"vehPlaneCollisionLerpRate",
			"Collision lerp rate",
		},
		{
			"vehPlaneCollisionLookAheadLerpRate",
			"Collision look ahead lerp rate",
		},
		{
			"vehPlaneCollisionLookAheadRollLerpRate",
			"Collision look ahead roll lerp rate",
		},
		{
			"vehPlaneCollisionLookAheadTime",
			"Collision look ahead time",
		},
		{
			"vehPlaneCollisionRollLerpRate",
			"Collision roll lerp rate",
		},
		{
			"vehPlaneControlExponent",
			"an exponent on the joystick deflection, higher for sub-linear response",
		},
		{
			"vehPlaneControlForceReferenceSpeed",
			"speed at which control forces will reach their advertised values (knots)",
		},
		{
			"vehPlaneControlLowpassCoeff",
			"how much of the last frame control value to mix in (higher values for more sluggishness)",
		},
		{
			"vehPlaneControlReduceRoll",
			"in control scheme 1, how much to reduce roll by when throttle is up or down",
		},
		{
			"vehPlaneControlRollYawCoupling",
			"how much yaw input affects roll input (doesn't go the other way)",
		},
		{
			"vehPlaneControlScheme",
			"Which control scheme to use",
		},
		{
			"vehPlaneControlSquaring",
			"how much to stretch the circular range of the joystick into a square",
		},
		{
			"vehPlaneControlYawRollCoupling",
			"how much roll input affects yaw input (doesn't go the other way)",
		},
		{
			"vehPlaneDampingPitch",
			"how much pitch rate decays from frame to frame, lower for more momentum",
		},
		{
			"vehPlaneDampingRoll",
			"how much roll rate decays from frame to frame, lower for more momentum",
		},
		{
			"vehPlaneDampingYaw",
			"how much yaw rate decays from frame to frame, lower for more momentum",
		},
		{
			"vehPlaneDeadZonePercent",
			"Percent of stick dead zone",
		},
		{
			"vehPlaneDihedralCoeff",
			"how much sideslip affects roll.  higher values for stronger tendency to level out",
		},
		{
			"vehPlaneFuselageLoading",
			"fuselage loading, lower values decrease sideslip (kg / m^2)",
		},
		{
			"vehPlaneGravity",
			"the strength of gravity, (inch/s^2)",
		},
		{
			"vehPlaneGravityVelocity",
			"This velocity will be added to the plane's Z-velocity",
		},
		{
			"vehPlaneInducedDragCoeffExponent",
			"induced drag is modeled as sin(aoa)^e, this is the e",
		},
		{
			"vehPlaneMass",
			"mass (kg)",
		},
		{
			"vehPlaneMaxControlForcePitch",
			"strength of pitch force compared to other forces",
		},
		{
			"vehPlaneMaxControlForceRoll",
			"strength of roll force compared to other forces",
		},
		{
			"vehPlaneMaxControlForceScale",
			"scales all the control forces",
		},
		{
			"vehPlaneMaxControlForceYaw",
			"strength of yaw force compared to other forces",
		},
		{
			"vehPlaneMaxInducedDragCoeff",
			"the maximum induced drag coefficient (reached at 90 degrees aoa)",
		},
		{
			"vehPlaneMaxLiftCoeff",
			"the wing's stalling lift coefficient",
		},
		{
			"vehPlaneMaxLiftCoeffAoA",
			"the wing's stall angle of attack (degrees)",
		},
		{
			"vehPlaneMaxPitch",
			"Maximum pitch",
		},
		{
			"vehPlaneMaxPitchDiffPerSec",
			"Maximum pitch delta per second",
		},
		{
			"vehPlaneMaxRightingForcePitch",
			"strength of pitch righting force compared to other forces",
		},
		{
			"vehPlaneMaxRightingForceScale",
			"scales all the aerodynamic righting forces (which align you with your direction of motion)",
		},
		{
			"vehPlaneMaxRightingForceYaw",
			"strength of yaw righting force compared to other forces",
		},
		{
			"vehPlaneMaxRoll",
			"Maximum roll",
		},
		{
			"vehPlaneMaxYawRatePerSec",
			"Maximum yaw rate per second",
		},
		{
			"vehPlaneParasiticDragCoeff",
			"parasitic drag - higher values will slow you down faster",
		},
		{
			"vehPlanePathAllowance",
			"The angle that the plane is allowed to turn away from the expected path",
		},
		{
			"vehPlanePathAngle",
			"Angle of the current expected path segment in the level that will limit yaw controls",
		},
		{
			"vehPlanePitchDeadZoneWhileRolling",
			"The percent of stick dead zone applied to pitch when rolling",
		},
		{
			"vehPlanePitchLerpRate",
			"Pitch lerp rate",
		},
		{
			"vehPlanePitchVelocityModifer",
			"Modifies the velocity based on the pitch (faster when pitched down, slower when pitched up)",
		},
		{
			"vehPlaneRightingForceReferenceSpeed",
			"speed at which righting forces will reach their advertised values (knots)",
		},
		{
			"vehPlaneRollLerpRate",
			"Roll lerp rate",
		},
		{
			"vehPlaneRollModifierExponent",
			"Applies the exponent to the current roll of the plane",
		},
		{
			"vehPlaneSecondZeroLiftCoeffAoA",
			"the angle of attack, after stalling, at which the lift coefficient goes back to zero",
		},
		{
			"vehPlaneStickPusherEngageAoA",
			"when angle of attack reaches this, you will fully lose pitch-up authority",
		},
		{
			"vehPlaneStickPusherFullAoA",
			"when angle of attack reaches this, you will fully lose pitch-up authority",
		},
		{
			"vehPlaneSwapSticks",
			"Swaps the flight stick from left to right when enabled",
		},
		{
			"vehPlaneThrustToWeightRatio",
			"thrust to weight ratio - higher increases acceleration and max speed",
		},
		{
			"vehPlaneTurbulenceSpatialFrequency",
			"how quickly the wind varies from place to place (1 / inch)",
		},
		{
			"vehPlaneTurbulenceStrength",
			"the strength of the random wind (kts)",
		},
		{
			"vehPlaneVelocityLerpRate",
			"velocity lerp rate",
		},
		{
			"vehPlaneWingLeveling",
			"fraction of control input player's plane will use to automatically roll towards wings-level",
		},
		{
			"vehPlaneWingLoading",
			"wing loading, lower values lower stall speed (kg / m^2)",
		},
		{
			"vehPlaneYawLerpRate",
			"Yaw lerp rate",
		},
		{
			"vehSubmarineAllowInSolid",
			"Allow in solid.  Dangerous! Used to push the submarine with a collision brushmodel.",
		},
		{
			"vehSubmarineBodyRelRotation",
			"Submarine Body Relative Rotation (0 == world, 1 == body coupled, 2 == horz is roll only, 3 == horz is world yaw, add 4 for camera relative)",
		},
		{
			"vehSubmarineControls",
			"Submarine controls (0==org, 1=trigger accel, 2=right stick pitch/yaw)",
		},
		{
			"vehSubmarineForwardDampening",
			"The submarine forward dampening fraction",
		},
		{
			"vehSubmarineFwdCollMaxAccel",
			"Submarine forward probe max accel",
		},
		{
			"vehSubmarineFwdCollMaxAngAccel",
			"Submarine forward probe max angular accel",
		},
		{
			"vehSubmarineFwdProbeTime",
			"The time for the submarine forward probe",
		},
		{
			"vehSubmarineHorizControlGamma",
			"Submarine gamma for input on horizontal input",
		},
		{
			"vehSubmarineInvertUpDown",
			"Invert the attitude control on the player submarine.",
		},
		{
			"vehSubmarineLateralDampening",
			"The submarine lateral dampening fraction",
		},
		{
			"vehSubmarineMaxDownPitch",
			"The max pitch down allowed for the submarine",
		},
		{
			"vehSubmarineMaxForwardAccel",
			"The max forward acceleration allowed for the submarine",
		},
		{
			"vehSubmarineMaxForwardPitchAccel",
			"The max pitch acceleration when moving forward for the submarine",
		},
		{
			"vehSubmarineMaxForwardVel",
			"The max forward velocity allowed for the submarine",
		},
		{
			"vehSubmarineMaxForwardYawAccel",
			"The max yaw acceleration when moving forward for the submarine",
		},
		{
			"vehSubmarineMaxFwdVelRef",
			"Submarine max forward velocity reference for coll adjust",
		},
		{
			"vehSubmarineMaxLateralVel",
			"The max lateral velocity allowed for the submarine",
		},
		{
			"vehSubmarineMaxNegativeBuoyancy",
			"The max negative buoyancy allowed for the submarine",
		},
		{
			"vehSubmarineMaxPositiveBuoyancy",
			"The max positive buoyancy allowed for the submarine",
		},
		{
			"vehSubmarineMaxReverseAccel",
			"The max reverse acceleration allowed for the submarine",
		},
		{
			"vehSubmarineMaxReversePitchAccel",
			"The max pitch acceleration when moving forward for the submarine",
		},
		{
			"vehSubmarineMaxReverseVel",
			"The max reverse velocity allowed for the submarine",
		},
		{
			"vehSubmarineMaxReverseYawAccel",
			"The max yaw acceleration when moving backwards for the submarine",
		},
		{
			"vehSubmarineMaxRoll",
			"The max roll allowed for the submarine",
		},
		{
			"vehSubmarineMaxRollAccel",
			"The max roll acceleration for the submarine",
		},
		{
			"vehSubmarineMaxStoppedPitchAccel",
			"The max pitch acceleration when stopped for the submarine",
		},
		{
			"vehSubmarineMaxStoppedYawAccel",
			"The max yaw acceleration when stopped for the submarine",
		},
		{
			"vehSubmarineMaxUpPitch",
			"The max pitch up allowed for the submarine",
		},
		{
			"vehSubmarineMinForwardVel",
			"The min forward velocity allowed for the submarine (only active when non-zero)",
		},
		{
			"vehSubmarineMinVelNoRestore",
			"The minimum velocity the submarine has when restore is enabled",
		},
		{
			"vehSubmarinePitchDampening",
			"The submarine pitch dampening fraction",
		},
		{
			"vehSubmarinePitchRestore",
			"The submarine pitch restore fraction when not moving",
		},
		{
			"vehSubmarineRollDampening",
			"The submarine roll dampening fraction",
		},
		{
			"vehSubmarineRollDrivenYaw",
			"Submarine roll drives applied yaw (0 mean don't scale yaw)",
		},
		{
			"vehSubmarineRollRestore",
			"The submarine roll restore fraction when not moving",
		},
		{
			"vehSubmarineSideCollMaxAccel",
			"Submarine side probe max accel",
		},
		{
			"vehSubmarineSideCollMaxAngAccel",
			"Submarine side probe max angular accel",
		},
		{
			"vehSubmarineSideProbeMaxDist",
			"Max distance for the submarine side probe",
		},
		{
			"vehSubmarineSideProbeMinDist",
			"Min distance for the submarine side probe",
		},
		{
			"vehSubmarineStoppedVel",
			"The threshold velocity to determine the submarine is stopped",
		},
		{
			"vehSubmarineVertControlGamma",
			"Submarine gamma for input on vertical input",
		},
		{
			"vehSubmarineYawDampening",
			"The submarine yaw dampening fraction",
		},
		{
			"vehUGVPitchTrack",
			"UGV body pitch orientation speed",
		},
		{
			"vehUGVRollTrack",
			"UGV body roll orientation speed",
		},
		{
			"vehUGVWheelInfluence",
			"UGV wheel influence on the orientation of the body",
		},
		{
			"vehWalkerControlMode",
			"Walker controls (0==move no turn, 1=move and turn, 2=move relative(tank))",
		},
		{
			"version",
			"Game version",
		},
		{
			"viewangNow",
			"",
		},
		{
			"viewModelDebugNotetracks",
			"Enable display of viewmodel notetrack debug info.",
		},
		{
			"viewModelHacks",
			"Enabled depth hack and remove viewmodel from shadows.",
		},
		{
			"viewposNow",
			"",
		},
		{
			"virtualLobbyActive",
			"Indicates the VL is actively being displayed.",
		},
		{
			"virtualLobbyAllocated",
			"Indicates the first VL zone has been loaded.",
		},
		{
			"waypointAerialIconMaxSize",
			"Max size of aerial targeting waypoints.",
		},
		{
			"waypointAerialIconMinSize",
			"Min size of aerial targeting waypoints.",
		},
		{
			"waypointAerialIconScale",
			"Base scale of aerial targeting waypoints.",
		},
		{
			"waypointDebugDraw",
			"",
		},
		{
			"waypointDistScaleRangeMax",
			"Distance from player that icon distance scaling ends.",
		},
		{
			"waypointDistScaleRangeMin",
			"Distance from player that icon distance scaling ends.",
		},
		{
			"waypointDistScaleSmallest",
			"Smallest scale that the distance effect uses.",
		},
		{
			"waypointIconHeight",
			"",
		},
		{
			"waypointIconWidth",
			"",
		},
		{
			"waypointOffscreenCircleRadius",
			"Radius of circle for circular offscreen waypoints.",
		},
		{
			"waypointOffscreenCornerRadius",
			"Size of the rounded corners.",
		},
		{
			"waypointOffscreenDistanceThresholdAlpha",
			"Distance from the threshold over which offscreen objective icons lerp their alpha.",
		},
		{
			"waypointOffscreenPadBottom",
			"",
		},
		{
			"waypointOffscreenPadLeft",
			"",
		},
		{
			"waypointOffscreenPadRight",
			"",
		},
		{
			"waypointOffscreenPadTop",
			"",
		},
		{
			"waypointOffscreenPointerDistance",
			"Distance from the center of the offscreen objective icon to the center its arrow.",
		},
		{
			"waypointOffscreenPointerHeight",
			"",
		},
		{
			"waypointOffscreenPointerWidth",
			"",
		},
		{
			"waypointOffscreenRoundedCorners",
			"Off-screen icons take rounded corners when true.  90-degree corners when false.",
		},
		{
			"waypointOffscreenScaleLength",
			"How far the offscreen icon scale travels from full to smallest scale.",
		},
		{
			"waypointOffscreenScaleSmallest",
			"Smallest scale that the offscreen effect uses.",
		},
		{
			"waypointPlayerOffsetCrouch",
			"For waypoints pointing to players, how high to offset off of their origin when they are prone.",
		},
		{
			"waypointPlayerOffsetProne",
			"For waypoints pointing to players, how high to offset off of their origin when they are prone.",
		},
		{
			"waypointPlayerOffsetStand",
			"For waypoints pointing to players, how high to offset off of their origin when they are prone.",
		},
		{
			"waypointScreenCenterFadeAdsMin",
			"When 'waypointScreenCenterFadeRadius' enabled, minimum amount that waypoint will fade when in ads",
		},
		{
			"waypointScreenCenterFadeHipMin",
			"When 'waypointScreenCenterFadeRadius' enabled, minimum amount that waypoint will fade when in ads",
		},
		{
			"waypointScreenCenterFadeRadius",
			"Radius from screen center that a waypoint will start fading out. Setting to 0 will turn this off",
		},
		{
			"waypointSplitscreenScale",
			"Scale applied to waypoint icons in splitscreen views.",
		},
		{
			"waypointTweakY",
			"",
		},
		{
			"weap_printSharedAmmo",
			"Print out shared ammo between weapons on level load",
		},
		{
			"weap_printSharedClips",
			"Print out shared clip ammo between weapons on level load",
		},
		{
			"weapon_drop_validate_time",
			"Time between checks to see if a weapon position is valid. Will drop the weapon when invalid. Default = 0 which will never test weapon.",
		},
		{
			"whitelist_fakeNoPingServer",
			"Simulate no access to ATVI ping servers",
		},
		{
			"wideScreen",
			"True if the game video is running in 16x9 aspect, false if 4x3.",
		},
		{
			"xanim_blendshape_raw_anim",
			"Toggles uncompressed blendshape animations",
		},
		{
			"xanim_disableIK",
			"Disable inverse kinematics solvers",
		},
		{
			"xanim_disableLookAt",
			"Disable head tracking",
		},
		{
			"xblive_loggedin",
			"User is logged into xbox live",
		},
		{
			"xenon_maxVoicePacketsPerSec",
			"Max voice packets per second a client will send",
		},
		{
			"xenon_maxVoicePacketsPerSecForServer",
			"Max voice packets per second the server will send",
		},
		{
			"xenon_voiceDebug",
			"Debug voice communication",
		},
		{
			"xenon_voiceDegrade",
			"Degrade voice quality",
		},
		{
			"xphys_enable",
			"Turn on dynamic physics joints",
		},
		{
			"xphys_matchAnimationPose",
			"Force position/orientation of physics bones to animation pose",
		},
		{
			"xphys_teleportAngle",
			"If physics body is rotation deviates more than this angle difference from original animation angle, then physics body is teleported",
		},
		{
			"xphys_teleportDistance",
			"If physics body is further than this distance from original animation position, then physics body is manually teleported to be within at least distance from the animation target",
		}
	};

	std::string dvar_get_description(const std::string& name)
	{
		const auto lower = utils::string::to_lower(name);
		for (std::uint32_t i = 0; i < dvar_list.size(); i++)
		{
			if (utils::string::to_lower(dvar_list[i].name) == lower)
			{
				return dvar_list[i].description;
			}
		}

		return {};
	}

	bool can_add_dvar_to_list(std::string name)
	{
		const auto lower = utils::string::to_lower(name);
		for (std::uint32_t i = 0; i < dvar_list.size(); i++)
		{
			if (utils::string::to_lower(dvar_list[i].name) == lower)
			{
				return false;
			}
		}

		return true;
	}

	std::optional<dvar_info> get_dvar_info_from_hash(const int hash)
	{
		for (std::uint32_t i = 0; i < dvar_list.size(); i++)
		{
			if (dvar_list[i].hash == hash)
			{
				return {dvar_list[i]};
			}
		}

		return {};
	}

	std::string hash_to_string(const int hash)
	{
		return utils::string::va("0x%lX", hash);
	}

	game::dvar_t* register_int(const std::string& name, int value, int min, int max,
		unsigned int flags, const std::string& description)
	{
		const auto hash = game::generateHashValue(name.data());

		if (can_add_dvar_to_list(name))
		{
			dvar_list.push_back({name, description});
		}

		return game::Dvar_RegisterInt(hash, "", value, min, max, flags);
	}

	game::dvar_t* register_bool(const std::string& name, bool value,
		unsigned int flags, const std::string& description)
	{
		const auto hash = game::generateHashValue(name.data());

		if (can_add_dvar_to_list(name))
		{
			dvar_list.push_back({name, description});
		}

		return game::Dvar_RegisterBool(hash, "", value, flags);
	}

	game::dvar_t* register_string(const std::string& name, const char* value,
		unsigned int flags, const std::string& description)
	{
		const auto hash = game::generateHashValue(name.data());

		if (can_add_dvar_to_list(name))
		{
			dvar_list.push_back({name, description});
		}

		return game::Dvar_RegisterString(hash, "", value, flags);
	}

	game::dvar_t* register_float(const std::string& name, float value, float min,
		float max, unsigned int flags, const std::string& description)
	{
		const auto hash = game::generateHashValue(name.data());

		if (can_add_dvar_to_list(name))
		{
			dvar_list.push_back({name, description});
		}

		return game::Dvar_RegisterFloat(hash, "", value, min, max, flags);
	}

	game::dvar_t* register_vec4(const std::string& name, float x, float y, float z,
		float w, float min, float max, unsigned int flags, const std::string& description)
	{
		const auto hash = game::generateHashValue(name.data());

		if (can_add_dvar_to_list(name))
		{
			dvar_list.push_back({name, description});
		}

		return game::Dvar_RegisterVec4(hash, "", x, y, z, w, min, max, flags);
	}

	game::dvar_t* register_enum(const std::string& name, const char** value_list, int default_index, 
		unsigned int flags, const std::string& description)
	{
		const auto hash = game::generateHashValue(name.data());

		if (can_add_dvar_to_list(name))
		{
			dvar_list.push_back({name, description});
		}

		return game::Dvar_RegisterEnum(hash, "", value_list, default_index, flags);
	}
}
