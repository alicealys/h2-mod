#include <std_include.hpp>

// This file has been generated.
// Do not touch!

// based on https://github.com/xensik/gsc-tool/blob/dev/src/iw6/xsk/resolver.cpp

namespace scripting
{
    std::unordered_map<std::string, unsigned> function_map =
    {
        {"precacheturret", 0x001}, // 0x4EF520
        {"getweaponarray", 0x002}, // 0x4E2AF0
        {"createprintchannel", 0x003}, // 0x4EB6E0 empty
        {"updategamerprofileall", 0x004}, // 0x502950
        {"clearlocalizedstrings", 0x005}, // 0x504F40
        {"setphysicsgravitydir", 0x006}, // 0x4FF120
        {"gettimescale", 0x007}, // 0x508680
        {"settimescale", 0x008}, // 0x5086A0
        {"setslowmotionview", 0x009}, // 0x5087A0
        {"teleportscene", 0x00A}, // 0x5086C0
        {"forcesharedammo", 0x00B}, // 0x5085A0
        {"refreshhudcompass", 0x00C}, // 0x504FD0
        {"refreshhudammocounter", 0x00D}, // 0x505000
        {"notifyoncommand", 0x00E}, // 0x505060
        {"setprintchannel", 0x00F}, // 0x4EB750 empty
        {"_func_10", 0x010}, // 0x4EB820, returns param count
        {"_func_11", 0x011}, // 0x4EB9C0, ^
        {"_func_12", 0x012}, // 0x4EC560, empty
        {"_func_13", 0x013}, // 0x4EC570, ^
        {"print", 0x014}, // 0x4EC640 empty
        {"println", 0x015}, // 0x4EC860 empty
        {"print3d", 0x016}, // 0x4EC870 empty
        {"line", 0x017}, // 0x4EC880 empty
        {"spawnturret", 0x018}, // 0x4EF180
        {"canspawnturret", 0x019}, // 0x4EF3D0
        {"assert", 0x01A}, // 0x4EC890
        {"pausecinematicingame", 0x01B}, // 0x502660
        {"drawcompassfriendlies", 0x01C}, // 0x502690
        {"bulletspread", 0x01D}, // 0x5021C0
        {"bullettracer", 0x01E}, // 0x5023E0
        {"badplace_delete", 0x01F}, // 0x4E6F00
        {"badplace_cylinder", 0x020}, // 0x4E7120
        {"badplace_arc", 0x021}, // 0x4E76D0
        {"badplace_brush", 0x022}, // 0x4E7F10
        {"clearallcorpses", 0x023}, // 0x503F20
        {"setturretnode", 0x024}, // 0x525640
        {"unsetturretnode", 0x025}, // 0x5256E0
        {"setnodepriority", 0x026}, // 0x525560
        {"isnodeoccupied", 0x027}, // 0x525420
        {"setdebugorigin", 0x02A}, // 0x504630 empty
        {"setdebugangles", 0x02B}, // 0x504640 empty
        {"updategamerprofile", 0x02C}, // 0x5046B0
        {"assertex", 0x02D}, // 0x4EC920
        {"assertmsg", 0x02E}, // 0x4EC980
        {"isdefined", 0x02F}, // 0x4ED1F0
        {"isvalidmissile", 0x030}, // 0x4ED5F0
        {"isstring", 0x031}, // 0x4ED850
        {"setomnvar", 0x032}, // 0x4EE990
        {"getomnvar", 0x033}, // 0x4EEC00
        {"setdvar", 0x034}, // 0x4EF040
        {"setdynamicdvar", 0x036}, // 0x4EFA60
        {"setdvarifuninitialized", 0x037}, // 0x4EF560
        {"setdevdvar", 0x038}, // 0x4EFCA0
        {"getdvar", 0x039}, // 0x4F0260
        {"getdvarint", 0x03A}, // 0x4F0730
        {"getdvarfloat", 0x03B}, // 0x4F1500
        {"getdvarvector", 0x03C}, // 0x4F1A20
        {"gettime", 0x03D}, // 0x4F2180
        {"gettimeutc", 0x03E}, // 0x4F21C0
        {"getculldist", 0x03F}, // 0x4F2330
        {"getentbynum", 0x040}, // 0x4F2400
        {"getscreenwidth", 0x041}, // 0x4F2620
        {"getscreenheight", 0x042}, // 0x4F27A0
        {"getweaponmodel", 0x043}, // 0x4F28B0
        {"_func_44", 0x044}, // 0x502BF0, sets config string
        {"_func_45", 0x045}, // 0x502C50, ^
        {"_func_46", 0x046}, // 0x502C90, bunch of arrays
        {"_func_47", 0x047}, // 0x502CF0, sets config string
        {"_func_48", 0x048}, // 0x502D80, ^
        {"setsunlight", 0x049}, // 0x502DB0
        {"resetsunlight", 0x04A}, // 0x502E40
        {"getmapsundirection", 0x04B}, // 0x502E70
        {"getmapsunangles", 0x04C}, // 0x502EA0
        {"setsundirection", 0x04D}, // 0x502FD0
        {"lerpsundirection", 0x04E}, // 0x5030E0
        {"lerpsunangles", 0x04F}, // 0x503380
        {"resetsundirection", 0x050}, // 0x5036E0
        {"enableforcedsunshadows", 0x051}, // 0x503740
        {"enableforcednosunshadows", 0x052}, // 0x503760
        {"disableforcedsunshadows", 0x053}, // 0x5037B0
        {"enableouterspacemodellighting", 0x054}, // 0x5037D0
        {"disableouterspacemodellighting", 0x055}, // 0x5038F0
        {"remapstage", 0x056}, // 0x504B50
        {"changelevel", 0x057}, // 0x501A80
        {"missionsuccess", 0x058}, // 0x501C40
        {"missionfailed", 0x059}, // 0x501C70
        {"cinematic", 0x05A}, // 0x501CD0
        {"cinematicingame", 0x05B}, // 0x501EB0
        {"cinematicingamesync", 0x05C}, // 0x501F50
        {"cinematicingameloop", 0x05D}, // 0x5020C0
        {"cinematicingameloopresident", 0x05E}, // 0x502140
        {"iscinematicplaying", 0x05F}, // 0x502390
        {"stopcinematicingame", 0x060}, // 0x502640
        {"getweapondisplayname", 0x061}, // 0x4F32A0
        {"getweaponbasename", 0x062}, // 0x4F36D0
        {"getweaponattachments", 0x063}, // 0x4F3D20
        {"getweaponattachmentdisplaynames", 0x064}, // 0x4F4440
        {"getweaponcamoname", 0x065}, // 0x4F51C0
        {"getweaponreticlename", 0x066}, // 0x4F55D0
        {"getanimlength", 0x067}, // 0x4F7960
        {"animhasnotetrack", 0x068}, // 0x4F7B90
        {"getnotetracktimes", 0x069}, // 0x4F8020
        {"spawn", 0x06A}, // 0x4EEDD0
        {"spawnloopsound", 0x06B}, // 0x4EEDD0
        {"spawnloopingsound", 0x06C}, // 0x6B56B0
        {"bullettrace", 0x06D}, // 0x4F1BD0
        {"target_setmaxsize", 0x06E}, // 0x5140B0
        {"target_setcolor", 0x06F}, // 0x513BC0
        {"target_setdelay", 0x070}, // 0x513D50
        {"getstartorigin", 0x071}, // 0x4AF5F0
        {"getstartangles", 0x072}, // 0x4AF4E0
        {"getcycleoriginoffset", 0x073}, // 0x4AF180
        {"magicgrenade", 0x074}, // 0x501DB0
        {"magicgrenademanual", 0x075}, // 0x4F0130
        {"setblur", 0x076}, // 0x507470
        {"musicplay", 0x077}, // 0x507570
        {"musicstop", 0x078}, // 0x507770
        {"soundfade", 0x079}, // 0x507890
        {"addsubmix", 0x07A}, // 0x4F6940, server command name
        {"clearsubmix", 0x07B}, // 0x4F6990, ^
        {"clearallsubmixes", 0x07C}, // 0x4F6A20, ^
        {"blendsubmix", 0x07D}, // 0x4F6C90, ^
        {"makestickysubmix", 0x07E}, // 0x4F6DA0, ^
        {"makeunstickysubmix", 0x07F}, // 0x4F6DD0, ^
        {"soundtime", 0x080}, // 0x507A70, ^
        {"soundtimereset", 0x081}, // 0x4FF720, ^
        {"levelsoundfade", 0x082}, // 0x507960
        {"precachenightvisioncodeassets", 0x083}, // 0x5019A0
        {"precachedigitaldistortcodeassets", 0x085}, // 0x5019C0
        {"precacheminimapsentrycodeassets", 0x086}, // 0x501A60
        {"savegame", 0x087}, // 0x4FF7B0
        {"issavesuccessful", 0x088}, // 0x500380
        {"issaverecentlyloaded", 0x089}, // 0x5003A0
        {"savegamenocommit", 0x08A}, // 0x500210
        {"commitsave", 0x08B}, // 0x5003C0
        {"commitwouldbevalid", 0x08C}, // 0x500B40
        {"getfxvisibility", 0x08D}, // 0x5026B0
        {"setculldist", 0x08E}, // 0x502AE0
        {"bullettracepassed", 0x08F}, // 0x4F2260
        {"sighttracepassed", 0x090}, // 0x4F2480
        {"physicstrace", 0x091}, // 0x4F2C80
        {"playerphysicstrace", 0x092}, // 0x4F3190
        {"getgroundposition", 0x093}, // 0x4F5780
        {"getangledelta", 0x094}, // 0x4F9570
        {"getmovedelta", 0x095}, // 0x4F9E10
        {"getnorthyaw", 0x096}, // 0x4E2840
        {"getcommandfromkey", 0x097}, // 0x506E40
        {"getsticksconfig", 0x098}, // 0x506F60
        {"_func_99", 0x099}, // 0x507060, gamepad related
        {"_func_9a", 0x09A}, // 0x5070A0, returns 0
        {"_func_9b", 0x09B}, // 0x507140, gamepad related
        {"weaponfightdist", 0x09C}, // 0x500BF0
        {"weaponmaxdist", 0x09D}, // 0x500C40
        {"isturretactive", 0x09E}, // 0x500BB0
        {"getstarttime", 0x09F}, // 0x506DA0
        {"target_alloc", 0x0A0}, // 0x513350
        {"target_flush", 0x0A1}, // 0x513780
        {"target_set", 0x0A2}, // 0x513A60
        {"target_remove", 0x0A3}, // 0x513A20
        {"target_setshader", 0x0A4}, // 0x514750
        {"target_setoffscreenshader", 0x0A5}, // 0x514380
        {"target_isinrect", 0x0A6}, // 0x513900
        {"target_isincircle", 0x0A7}, // 0x513890
        {"target_startreticlelockon", 0x0A8}, // 0x5148A0
        {"target_clearreticlelockon", 0x0A9}, // 0x513370
        {"target_getarray", 0x0AA}, // 0x513810
        {"target_istarget", 0x0AB}, // 0x5139A0
        {"target_setattackmode", 0x0AC}, // 0x513A80
        {"target_setjavelinonly", 0x0AD}, // 0x513F60
        {"target_hidefromplayer", 0x0AE}, // 0x513880
        {"target_showtoplayer", 0x0AF}, // 0x514890
        {"target_setscaledrendermode", 0x0B0}, // 0x514600
        {"target_drawcornersonly", 0x0B1}, // 0x5133A0
        {"target_drawsquare", 0x0B2}, // 0x513610
        {"target_drawsingle", 0x0B3}, // 0x5134F0
        {"target_setminsize", 0x0B4}, // 0x5141F0
        {"setnorthyaw", 0x0B5}, // 0x4E2B60
        {"setslowmotion", 0x0B6}, // 0x4F4320
        {"randomint", 0x0B7}, // 0x4F5A60
        {"randomfloat", 0x0B8}, // 0x4F5BE0
        {"randomintrange", 0x0B9}, // 0x4F5E40
        {"randomfloatrange", 0x0BA}, // 0x4F5ED0
        {"sin", 0x0BB}, // 0x4F5FE0
        {"cos", 0x0BC}, // 0x4F6030
        {"tan", 0x0BD}, // 0x4F6130
        {"asin", 0x0BE}, // 0x4F6390
        {"acos", 0x0BF}, // 0x4F6470
        {"atan", 0x0C0}, // 0x4F6880
        {"int", 0x0C1}, // 0x4F69B0
        {"float", 0x0C2}, // 0x4F6E00
        {"abs", 0x0C3}, // 0x4F7000
        {"min", 0x0C4}, // 0x4F7200
        {"objective_additionalcurrent", 0x0C5}, // 0x50F290
        {"objective_ring", 0x0C6}, // 0x50F3D0
        {"objective_setpointertextoverride", 0x0C7}, // 0x50F490
        {"getnode", 0x0C8}, // 0x524E60
        {"getnodearray", 0x0C9}, // 0x524F70
        {"getallnodes", 0x0CA}, // 0x524C40
        {"getnodesinradius", 0x0CB}, // 0x525080
        {"getnodesinradiussorted", 0x0CC}, // 0x525090
        {"getclosestnodeinsight", 0x0CD}, // 0x524C90
        {"getreflectionlocs", 0x0CE}, // 0x504F60
        {"getreflectionreferencelocs", 0x0CF}, // 0x504F70
        {"getvehicletracksegment", 0x0D0}, // 0x6DC180
        {"getvehicletracksegmentarray", 0x0D1}, // 0x6DC260
        {"getallvehicletracksegments", 0x0D2}, // 0x6DC130
        {"isarray", 0x0D3}, // 0x4EA580
        {"isai", 0x0D4}, // 0x504740
        {"getindexforluincstring", 0x0D5}, // 0x4EE030
        {"issentient", 0x0D6}, // 0x4ED980
        {"isgodmode", 0x0D7}, // 0x504790
        {"getdebugdvar", 0x0D8}, // 0x504AA0
        {"getdebugdvarint", 0x0D9}, // 0x504B20
        {"getdebugdvarfloat", 0x0DA}, // 0x504C30
        {"setsaveddvar", 0x0DB}, // 0x504C60
        {"getfreeaicount", 0x0DC}, // 0x505030
        {"getaicount", 0x0DD}, // 0x5050A0
        {"getaiarray", 0x0DE}, // 0x505360
        {"getaispeciesarray", 0x0DF}, // 0x505520
        {"getspawnerarray", 0x0E0}, // 0x505840
        {"getcorpsearray", 0x0E1}, // 0x505730
        {"getspawnerteamarray", 0x0E2}, // 0x505A40
        {"getweaponclipmodel", 0x0E3}, // 0x505B40
        {"getbrushmodelcenter", 0x0E4}, // 0x506D90
        {"getkeybinding", 0x0E5}, // 0x506DB0
        {"max", 0x0E6}, // 0x4F7270
        {"floor", 0x0E7}, // 0x4F7300
        {"ceil", 0x0E8}, // 0x4F7430
        {"round", 0x0E9}, // 0x4F7470
        {"exp", 0x0EA}, // 0x4F7570
        {"log", 0x0EB}, // 0x4F75F0
        {"sqrt", 0x0EC}, // 0x4F7750
        {"squared", 0x0ED}, // 0x4F78D0
        {"clamp", 0x0EE}, // 0x4F7A80
        {"angleclamp", 0x0EF}, // 0x4F7AD0
        {"angleclamp180", 0x0F0}, // 0x4F7BF0
        {"vectorfromlinetopoint", 0x0F1}, // 0x4FA300
        {"pointonsegmentnearesttopoint", 0x0F2}, // 0x4E20E0
        {"distance", 0x0F3}, // 0x4E2C00
        {"distance2d", 0x0F4}, // 0x4E2E70
        {"distancesquared", 0x0F5}, // 0x4E2FA0
        {"length", 0x0F6}, // 0x4E3330
        {"length2d", 0x0F7}, // 0x4E3530
        {"lengthsquared", 0x0F8}, // 0x4E3590
        {"length2dsquared", 0x0F9}, // 0x4E36E0
        {"closer", 0x0FA}, // 0x4E3730
        {"vectordot", 0x0FB}, // 0x4E3900
        {"vectorcross", 0x0FC}, // 0x4E3A10
        {"axistoangles", 0x0FD}, // 0x4E4680
        {"visionsetthermal", 0x0FE}, // 0x502A40
        {"visionsetpain", 0x0FF}, // 0x502A50
        {"incrementcounter", 0x104}, // 0x5056E0
        {"getcountertotal", 0x105}, // 0x5057A0
        {"getlevelticks", 0x106}, // 0x4F2380
        {"perlinnoise2d", 0x107}, // 0x4F7D00
        {"calcrockingangles", 0x108}, // 0x4F81C0
        {"reconevent", 0x109}, // 0x7C1910
        {"reconspatialevent", 0x10A}, // 0x7C1920
        {"setsunflareposition", 0x10B}, // 0x4E4760
        {"createthreatbiasgroup", 0x10C}, // 0x531770
        {"threatbiasgroupexists", 0x10D}, // 0x5317A0
        {"getthreatbias", 0x10E}, // 0x5317E0
        {"setthreatbias", 0x10F}, // 0x531840
        {"setthreatbiasagainstall", 0x110}, // 0x5318C0
        {"setignoremegroup", 0x111}, // 0x531910
        {"isenemyteam", 0x112}, // 0x531060
        {"objective_additionalentity", 0x113}, // 0x50FA70
        {"objective_state_nomessage", 0x114}, // 0x50EFE0
        {"objective_string", 0x115}, // 0x50EFF0
        {"objective_string_nomessage", 0x116}, // 0x50F000
        {"objective_additionalposition", 0x117}, // 0x50F200
        {"objective_current_nomessage", 0x118}, // 0x50F280
        {"vectornormalize", 0x119}, // 0x4E3E90
        {"vectortoangles", 0x11A}, // 0x4E40A0
        {"vectortoyaw", 0x11B}, // 0x4E42A0
        {"vectorlerp", 0x11C}, // 0x4E4430
        {"anglestoup", 0x11D}, // 0x4E4AD0
        {"anglestoright", 0x11E}, // 0x4E4C70
        {"anglestoforward", 0x11F}, // 0x4E4DD0
        {"anglesdelta", 0x120}, // 0x4E4F80
        {"combineangles", 0x121}, // 0x4E5940
        {"transformmove", 0x122}, // 0x4E5C10
        {"rotatevector", 0x123}, // 0x4E7C20
        {"rotatepointaroundvector", 0x124}, // 0x4E7FC0
        {"issubstr", 0x125}, // 0x4E9170
        {"isendstr", 0x126}, // 0x4E91B0
        {"getsubstr", 0x127}, // 0x4E9440
        {"tolower", 0x128}, // 0x4E9E20
        {"strtok", 0x129}, // 0x4EA2D0
        {"stricmp", 0x12A}, // 0x4EA980
        {"ambientplay", 0x12B}, // 0x4F2E00
        {"_func_145", 0x145}, // 0x5046A0
        {"visionsetnaked", 0x148}, // 0x502940
        {"visionsetnight", 0x149}, // 0x502A20
        {"visionsetmissilecam", 0x14A}, // 0x502A30
        {"ambientstop", 0x14B}, // 0x4F52C0
        {"precachemodel", 0x14C}, // 0x4EB430
        {"precacheshellshock", 0x14D}, // 0x4EB670
        {"precacheitem", 0x14E}, // 0x4F1FC0
        {"precacheshader", 0x14F}, // 0x4F2120
        {"precachestring", 0x150}, // 0x4F2390
        {"precachemenu", 0x151}, // 0x4F25F0
        {"precacherumble", 0x152}, // 0x4F2770
        {"precachelocationselector", 0x153}, // 0x4EB550
        {"precacheleaderboards", 0x154}, // 0x4F2190
        {"loadfx", 0x155}, // 0x4E3480
        {"playfx", 0x156}, // 0x4E44F0
        {"playfxontag", 0x157}, // 0x4E5DB0
        {"stopfxontag", 0x158}, // 0x4E7A60
        {"killfxontag", 0x159}, // 0x4E8220
        {"playloopedfx", 0x15A}, // 0x6E8E00
        {"spawnfx", 0x15B}, // 0x6E9130
        {"triggerfx", 0x15C}, // 0x6E9390
        {"playfxontagforclients", 0x15D}, // 0x4E6360
        {"_func_15e", 0x15E}, // 0x6E90C0, sets some entity flag
        {"_func_15f", 0x15F}, // 0x4E7420, fx related
        {"_func_160", 0x160}, // 0x4E61E0, calls Scr_ErrorInternal
        {"precacheheadicon", 0x170}, // 0x4F2350
        {"physicsexplosionsphere", 0x184}, // 0x4E8920
        {"physicsexplosioncylinder", 0x185}, // 0x4EA120
        {"physicsjolt", 0x186}, // 0x4E9610
        {"physicsjitter", 0x187}, // 0x4E9C60
        {"setexpfog", 0x188}, // 0x4ECC20
        {"setexpfogext", 0x189}, // 0x4ECDE0, server command name
        {"setexpfogdvarsonly", 0x18A}, // 0x4ED510, ^
        {"setexpfogextdvarsonly", 0x18B}, // 0x4ED580, ^
        {"setatmosfog", 0x18C}, // 0x4ED760, ^
        {"setatmosfogdvarsonly", 0x18D}, // 0x4EDAA0, ^
        {"isexplosivedamagemod", 0x18E}, // 0x4F6330
        {"radiusdamage", 0x18F}, // 0x4F7460
        {"setplayerignoreradiusdamage", 0x190}, // 0x4F8550
        {"glassradiusdamage", 0x191}, // 0x4F7620
        {"earthquake", 0x192}, // 0x4ED2F0
        {"getnumparts", 0x193}, // 0x4EDDA0
        {"_func_194", 0x194}, // 0x50EEA0, eartquake func; args: (magnitude, duration, origin, radius)
        {"iprintln", 0x19E}, // 0x504120
        {"iprintlnbold", 0x19F}, // 0x5041A0
        {"logstring", 0x1A0}, // 0x5071B0
        {"getent", 0x1A1}, // 0x511680
        {"getentarray", 0x1A2}, // 0x5116F0
        {"spawnstruct", 0x1A5}, // 0x5C6B00
        {"isalive", 0x1A7}, // 0x5042F0
        {"isspawner", 0x1A8}, // 0x504380
        {"missile_createattractorent", 0x1A9}, // 0x4D73E0
        {"missile_createattractororigin", 0x1AA}, // 0x4D73F0
        {"missile_createrepulsorent", 0x1AB}, // 0x4D7710
        {"missile_createrepulsororigin", 0x1AC}, // 0x4D7720
        {"missile_deleteattractor", 0x1AD}, // 0x4D7730
        {"newhudelem", 0x1AF}, // 0x4C2570
        {"newclienthudelem", 0x1B0}, // 0x4C2510
        {"resettimeout", 0x1B2}, // 0x5C8350, time related
        {"isplayer", 0x1B3}, // 0x504650
        {"getpartname", 0x1B5}, // 0x4EDE60
        {"weaponfiretime", 0x1B6}, // 0x4EF400
        {"weaponclipsize", 0x1B7}, // 0x4EC650
        {"weaponisauto", 0x1B8}, // 0x4ED640
        {"weaponissemiauto", 0x1B9}, // 0x4ED8F0
        {"weaponisboltaction", 0x1BA}, // 0x4EDDE0
        {"weaponinheritsperks", 0x1BB}, // 0x4EDFA0
        {"weaponburstcount", 0x1BC}, // 0x4EDBC0
        {"weapontype", 0x1BD}, // 0x4EE190
        {"weaponclass", 0x1BE}, // 0x4EE450
        {"getnextarraykey", 0x1BF}, // 0x4F73A0
        {"sortbydistance", 0x1C0}, // 0x4F7D90
        {"tablelookup", 0x1C1}, // 0x4EFD40
        {"tablelookupbyrow", 0x1C2}, // 0x4F2020
        {"tablelookupistring", 0x1C3}, // 0x4F0940
        {"tablelookupistringbyrow", 0x1C4}, // 0x4F21E0
        {"tablelookuprownum", 0x1C5}, // 0x4F1590
        {"tableexists", 0x1C6}, // 0x4F1DC0
        {"getmissileowner", 0x1C7}, // 0x4ECBC0
        {"magicbullet", 0x1C8}, // 0x4E98D0
        {"getweaponflashtagname", 0x1C9}, // 0x4EA890
        {"averagepoint", 0x1CA}, // 0x4F2630
        {"averagenormal", 0x1CB}, // 0x4F2A50
        {"vehicle_getspawnerarray", 0x1CC}, // 0x6DA010
        {"playrumbleonposition", 0x1CD}, // 0x4EC020
        {"playrumblelooponposition", 0x1CE}, // 0x4EC050
        {"stopallrumbles", 0x1CF}, // 0x4EC0F0
        {"soundexists", 0x1D0}, // 0x4F8DC0
        {"openfile", 0x1D1}, // 0x4F5E20, empty
        {"closefile", 0x1D2}, // 0x4F5E30, ^
        {"fprintln", 0x1D3}, // 0x4F5EC0, ^
        {"fprintfields", 0x1D4}, // 0x4F5F20, ^
        {"freadln", 0x1D5}, // 0x4F5F70, ^
        {"fgetarg", 0x1D6}, // 0x4F5F80, ^
        {"setminimap", 0x1D7}, // 0x4F6A70
        {"setthermalbodymaterial", 0x1D8}, // 0x4F6EE0
        {"getarraykeys", 0x1D9}, // 0x4F7090
        {"getfirstarraykey", 0x1DA}, // 0x4F7240
        {"getglass", 0x1DB}, // 0x4F2C30
        {"getglassarray", 0x1DC}, // 0x4F30B0
        {"getglassorigin", 0x1DD}, // 0x4F35B0
        {"isglassdestroyed", 0x1DE}, // 0x4F37E0
        {"destroyglass", 0x1DF}, // 0x4F3A90
        {"deleteglass", 0x1E0}, // 0x4F3E00
        {"getentchannelscount", 0x1E1}, // 0x4F46B0
        {"getentchannelname", 0x1E2}, // 0x4F4760
        {"objective_add", 0x1E3}, // 0x50F580
        {"objective_delete", 0x1E4}, // 0x50FA30
        {"objective_state", 0x1E5}, // 0x50EFD0
        {"objective_icon", 0x1E6}, // 0x50F010
        {"_func_1e7", 0x1E7}, // 0x50F110
        {"objective_position", 0x1E8}, // 0x50F1B0
        {"objective_current", 0x1E9}, // similar to objective_current_nomessage
        {"weaponinventorytype", 0x1EA}, // 0x4EE760
        {"weaponstartammo", 0x1EB}, // 0x4EE920
        {"weaponmaxammo", 0x1EC}, // 0x4EEB40
        {"weaponaltweaponname", 0x1ED}, // 0x4EEF70
        {"isweaponcliponly", 0x1EE}, // 0x4EF9E0
        {"isweapondetonationtimed", 0x1EF}, // 0x4EFB10
        {"weaponhasthermalscope", 0x1F0}, // 0x4F0080
        {"weaponhasexplosivedamage", 0x1F1}, // 0x4F0550, made up name
        {"getvehiclenode", 0x1F2}, // 0x6CFCC0
        {"getvehiclenodearray", 0x1F3}, // 0x6CFDB0
        {"getallvehiclenodes", 0x1F4}, // 0x6CFC50
        {"getnumvehicles", 0x1F5}, // 0x6D9D40
        {"precachevehicle", 0x1F6}, // 0x6DA090
        {"spawnvehicle", 0x1F7}, // 0x6DA130
        {"vehicle_getarray", 0x1F8}, // 0x6D9D80
        {"pow", 0x1F9}, // 0x4F79C0
        {"_func_1fa", 0x1FA}, // 0x4F6950
        {"_func_1fb", 0x1FB}, // 0x501980
        {"_func_1fc", 0x1FC}, // 0x501C90
        {"_func_20c", 0x20C}, // 0x525490
        {"_func_20d", 0x20D}, // 0x5250A0, path node related
        {"_func_216", 0x216}, // 0x506D20, weapon related
        {"getlinkednodes", 0x21D}, // 0x524DA0
        {"disconnectnodepair", 0x21E}, // 0x4E5D30
        {"connectnodepair", 0x21F}, // 0x4E6160
        {"_func_220", 0x220}, // 0x505420
        {"_func_223", 0x223}, // 0x5035C0, empty
        {"loadtransient", 0x225}, // 0x5CD370
        {"unloadtransient", 0x226}, // 0x5CD5B0
        {"unloadalltransients", 0x227}, // 0x5CD4F0
        {"synctransients", 0x228}, // 0x5CD490
        {"istransientqueued", 0x229}, // 0x5CD070
        {"istransientloaded", 0x22A}, // 0x5CD0F0
        {"loadstartpointtransient", 0x22B}, // 0x5CD0A0
        {"_func_22c", 0x22C}, // 0x5CD130, transient related
        {"distance2dsquared", 0x22D}, // 0x4E2F40
        {"getangledelta3d", 0x22E}, // 0x4E1EB0
        {"_func_22f", 0x22F}, // 0x4E8A30, useless function
        {"trajectorycalculateinitialvelocity", 0x230}, // 0x4E8D40
        {"trajectorycalculateminimumvelocity", 0x231}, // 0x4E9240
        {"trajectorycalculateexitangle", 0x232}, // 0x4E9570
        {"trajectorycomputedeltaheightattime", 0x233}, // 0x4E97C0
        {"trajectorycanattemptaccuratejump", 0x234}, // 0x4E9F80
        {"_func_235", 0x235}, // 0x4EA100, useless function
        {"_func_236", 0x236}, // 0x4ECA00, empty
        {"ispointinvolume", 0x237}, // 0x4E2A30
        {"cinematicgettimeinmsec", 0x238}, // 0x5024C0
        {"cinematicgetframe", 0x239}, // 0x5024A0
        {"iscinematicloaded", 0x23A}, // 0x5023C0
        {"bbprint", 0x23C}, // 0x4EBC50, empty
        {"getscriptablearray", 0x23E}, // 0x4EBE50
        {"clearfog", 0x23F}, // 0x4EB140
        {"setleveldopplerpreset", 0x240}, // 0x508500
        {"screenshake", 0x241}, // 0x500C90
        {"_func_242", 0x242}, // 0x4EA700, returns 1
        {"_func_243", 0x243}, // 0x4EAAD0, returns 1
        {"_func_245", 0x245}, // 0x4F3620, capsule trace ?
        {"_func_246", 0x246}, // 0x4F3920, ^
        {"_func_247", 0x247}, // 0x4F14E0, return 0.0
        {"_func_248", 0x248}, // 0x4F14F0, ^
        {"_func_249", 0x249}, // 0x4F1580, returns 0
        {"_func_24a", 0x24A}, // 0x4F1640, ^
        {"_func_24b", 0x24B}, // 0x4F1820, ^
        {"luinotifyevent", 0x24D}, // 0x4EB420
        {"physwakeup", 0x252}, // 0x4FF230, server command name
        {"ragdollwakeup", 0x253}, // 0x4FF350, ^
        {"_func_254", 0x254}, // 0x4E6B60
        {"_func_255", 0x255}, // 0x4F97C0
        {"anglestoaxis", 0x256}, // 0x4E48A0
        {"visionsetwater", 0x257}, // 0x502AD0
        {"sendscriptusageanalysisdata", 0x258}, // 0x4EBBA0, empty
        {"resetscriptusageanalysisdata", 0x259}, // 0x4EBBB0, ^
        {"instantlylogusageanalysisdata", 0x25A}, // 0x4EBC20, ^
        {"invertangles", 0x25B}, // 0x4E60E0
        {"rotatevectorinverted", 0x25C}, // 0x4E6250
        {"_func_25d", 0x25D}, // 0x4E6800, vector related
        {"_func_25f", 0x25F}, // 0x504A20, sets some entity flag
        {"_func_260", 0x260}, // 0x4F2990, sets some config string
        {"_func_261", 0x261}, // 0x4F29C0, string/font related ?
        {"getcsplinecount", 0x262}, // 0x4EC960
        {"getcsplinepointcount", 0x263}, // 0x4ECA10
        {"getcsplinelength", 0x264}, // 0x4ECD80
        {"getcsplinepointid", 0x265}, // 0x4ECF30
        {"getcsplinepointlabel", 0x266}, // 0x4ED440
        {"getcsplinepointtension", 0x267}, // 0x4ED6D0
        {"getcsplinepointposition", 0x268}, // 0x4ED9E0
        {"getcsplinepointcorridordims", 0x269}, // 0x4EDCE0
        {"getcsplinepointtangent", 0x26A}, // 0x4EDEE0
        {"getcsplinepointdisttonextpoint", 0x26B}, // 0x4EE210
        {"calccsplinetangent", 0x26C}, // 0x4EE530
        {"calccsplinecorridor", 0x26D}, // 0x4EE850
        {"calccsplinecorridor", 0x26E}, // 0x4EECB0
        {"setnojipscore", 0x26F}, // 0x4EEF30, useless function
        {"setnojiptime", 0x270}, // 0x4EEF50, ^
        {"getpredictedentityposition", 0x272}, // 0x4EF3C0, empty
        {"gamedvrprohibitrecording", 0x273}, // 0x4EF3F0, ^
        {"gamedvrstartrecording", 0x274}, // 0x4EF550, ^
        {"gamedvrstoprecording", 0x275}, // 0x4EF670, ^
        {"gamedvrsetvideometadata", 0x276}, // 0x4EF780, ^
        {"gamedvrprohibitscreenshots", 0x277}, // 0x4EFA50, ^
        {"gamedvrsetscreenshotmetadata", 0x279}, // 0x505B30, ^
        {"queuedialog", 0x27A}, // 0x505BC0, ^
        {"speechenablegrammar", 0x27B}, // 0x4EFAF0, ^
        {"speechenable", 0x27C}, // 0x4EFB00, ^
        {"livestreamingenable", 0x27D}, // 0x4EFC90, ^
        {"livestreamingsetbitrate", 0x27E}, // 0x4EFD30, ^
        {"triggerportableradarping", 0x284}, // 0x4EC710, ^
        {"setglaregrimematerial", 0x285}, // 0x4C2560, calls Scr_ErrorInternal
        {"botgetteamlimit", 0x286}, // 0x4EDF90, empty
        {"spawnfxforclient", 0x287}, // 0x4EE020, ^
        {"botgetteamdifficulty", 0x288}, // 0x4EE0A0, ^
        {"getsquadassaultelo", 0x289}, // 0x4EBC80, ^
        {"loadluifile", 0x28A}, // 0x4EE0B0
        {"_func_290", 0x290}, // 0x4EBCE0, empty
        {"_func_293", 0x293}, // 0x4F08C0, returns 0
        {"_func_294", 0x294}, // 0x4E82E0
        {"_func_295", 0x295}, // 0x4E8A60
        {"enablecontext", 0x29A}, // 0x4F6E70, server command name
        {"disablecontext", 0x29B}, // 0x4F6EC0, ^
        {"_func_29c", 0x29C}, // 0x5051E0
        {"_func_29d", 0x29D}, // 0x4F0D80, sound related
        {"_func_29e", 0x29E}, // 0x4F49C0, ^
        {"_func_29f", 0x29F}, // 0x4EEA20
        {"_func_2a0", 0x2A0}, // 0x4EF270
        {"_func_2a1", 0x2A1}, // 0x4EF680
        {"_func_2a2", 0x2A2}, // 0x502500
        {"_func_2a3", 0x2A3}, // 0x4E8D30, empty
        {"_func_2a4", 0x2A4}, // 0x4E2730
        {"_func_2a5", 0x2A5}, // 0x4ED4D0
        {"_func_2a6", 0x2A6}, // 0x4F2440
        {"_func_2a7", 0x2A7}, // 0x4F27B0
        {"_func_2aa", 0x2AA}, // 0x4F03B0, weapon related
        {"_func_2ab", 0x2AB}, // 0x502710
        {"_func_2ac", 0x2AC}, // 0x4F3A00
        {"_func_2ad", 0x2AD}, // 0x4E8C40, useless function
        {"_func_2ae", 0x2AE}, // 0x4F67C0
        {"_func_2af", 0x2AF}, // 0x4F6F00, empty
        {"_func_2b0", 0x2B0}, // 0x4F6EB0, ^
        {"_func_2b2", 0x2B2}, // 0x505BD0, useless function
        {"_func_2b3", 0x2B3}, // 0x4EE110
        {"_func_2b4", 0x2B4}, // 0x4F7450, empty
        {"_func_2b5", 0x2B5}, // 0x4F6FF0, returns 0
        {"_func_2b6", 0x2B6}, // 0x4F72A0, empty
        {"_func_2b7", 0x2B7}, // 0x4F7390, ^
        {"_func_2b8", 0x2B8}, // 0x4F7490, ^
        {"_func_2bd", 0x2BD}, // 0x4E8130, ^
        {"_func_2be", 0x2BE}, // 0x4E87E0, ^
        {"_func_2bf", 0x2BF}, // 0x4EA560
        {"challengemessage", 0x2C0}, // 0x508310, server command name
        {"_func_2c5", 0x2C5}, // 0x4F5C30, weapon related
        {"_func_2c6", 0x2C6}, // 0x508340
        {"_func_2c7", 0x2C7}, // 0x4EB9E0
        {"_func_2c9", 0x2C9}, // 0x4F7590, empty
        {"_func_2ca", 0x2CA}, // 0x4F7610, ^
        {"_func_2cb", 0x2CB}, // 0x4EA680
        {"_func_2cc", 0x2CC}, // 0x4F2000
        {"_func_2cd", 0x2CD}, // 0x4F2160
        {"_func_2ce", 0x2CE}, // 0x508550
        {"_func_2d7", 0x2D7}, // 0x5070C0
        {"_func_2dd", 0x2DD}, // 0x507B60
        {"_func_2de", 0x2DE}, // 0x4F6CA0
        {"_func_2e0", 0x2E0}, // 0x4F6F70, empty
        {"_func_2e1", 0x2E1}, // 0x502A60
        {"_func_2e3", 0x2E3}, // 0x5085B0, empty
        {"_func_2e8", 0x2E8}, // 0x4F8070, returns 1
        {"_func_2e9", 0x2E9}, // 0x4EAC20
        {"_func_2eb", 0x2EB}, // 0x4E6980, empty
        {"_func_2ec", 0x2EC}, // 0x4E8150, ^
        {"_func_2ed", 0x2ED}, // 0x4E8880, ^
        {"_func_2ee", 0x2EE}, // 0x508430
        {"_func_2ef", 0x2EF}, // 0x4E9010
        {"_func_2f0", 0x2F0}, // 0x4E92D0
        {"_func_2f1", 0x2F1}, // 0x503950
        {"_func_2f2", 0x2F2}, // 0x503C40, empty
        {"_func_2f3", 0x2F3}, // 0x4F1FB0
        {"_func_2f7", 0x2F7}, // 0x4F6010, returns 0
        {"_func_2f8", 0x2F8}, // 0x4F6020, returns ""
        {"_func_2f9", 0x2F9}, // 0x4F60C0
        {"_func_2fa", 0x2FA}, // 0x4F6190, returns -1
        {"_func_2fb", 0x2FB}, // 0x4F6320, returns 0
        {"_func_2fc", 0x2FC}, // 0x4F63E0, returns ""
        {"_func_2fd", 0x2FD}, // 0x4F6400
        {"_func_2fe", 0x2FE}, // 0x4F64D0, returns -1
        {"_func_2ff", 0x2FF}, // 0x4F67B0, returns ""
        {"_func_300", 0x300}, // 0x4F6810
        {"_func_301", 0x301}, // 0x4F69A0, returns -1
        {"_func_302", 0x302}, // 0x4F48A0, weapon related
        {"_func_303", 0x303}, // 0x4E3AD0, calls Scr_ErrorInternal
        {"_func_304", 0x304}, // 0x4E3B30, ^
        {"_func_305", 0x305}, // 0x4F08D0, weapon related
        {"_func_306", 0x306}, // 0x4ECC50
        {"_func_307", 0x307}, // 0x4ECE10
        {"_func_308", 0x308}, // 0x4ECFC0
        {"_func_30a", 0x30A}, // 0x4F8160
        {"_func_30c", 0x30C}, // 0x4EF3B0, empty
        {"_func_30e", 0x30E}, // 0x4F02E0
        {"_func_30f", 0x30F}, // 0x4F1970
        {"_func_313", 0x313}, // 0x4E3E80, calls Scr_ErrorInternal
        {"_func_314", 0x314}, // 0x4ED7A0
        {"_func_315", 0x315}, // 0x4AF3F0
        {"fast_restart", 0x316}, // 0x4ECC10
        {"_func_317", 0x317}, // 0x4AF2D0
        {"_func_319", 0x319}, // 0x4F4300
        {"enablepg", 0x31A}, // 0x4EA930, server command name
        {"_func_31b", 0x31B}, // 0x503C50
        {"_func_31c", 0x31C}, // 0x503D60
        {"_func_31d", 0x31D}, // 0x503E40
        {"_func_31e", 0x31E}, // 0x503F00
        {"_func_31f", 0x31F}, // 0x5024E0
        {"_func_320", 0x320}, // 0x4F9110
    };

    std::unordered_map<std::string, unsigned> method_map =
    {
        {"thermaldrawdisable", 0x8000}, // 0x4ED5B0
        {"playerforcedeathanim", 0x8001}, // 0x4FF0A0
        {"isonladder", 0x801A}, // 0x4E2590
        {"attach", 0x801D}, // 0x4EFBC0
        {"getlightfovinner", 0x801E}, // 0x504220
        {"getlightfovouter", 0x801F}, // 0x504350
        {"setlightfovrange", 0x8020}, // 0x5044C0
        {"getlightexponent", 0x8021}, // 0x504A80
        {"setlightexponent", 0x8022}, // 0x504AE0
        {"startragdoll", 0x8023}, // 0x504E00
        {"startragdollfromimpact", 0x8024}, // 0x504E40
        {"_meth_8025", 0x8025}, // 0x4E7A50, empty
        {"_meth_8026", 0x8026}, // 0x5071C0, ^
        {"_meth_8028", 0x8028}, // 0x505220
        {"thermaldrawenable", 0x8029}, // 0x4ED540
        {"detach", 0x802A}, // 0x4F0510
        {"detachall", 0x802B}, // 0x4F07B0
        {"getattachsize", 0x802C}, // 0x4F27F0
        {"getattachmodelname", 0x802D}, // 0x4F2D90
        {"getattachtagname", 0x802E}, // 0x4F3120
        {"setturretcanaidetach", 0x802F}, // 0x5058C0
        {"setturretfov", 0x8030}, // 0x505940
        {"setturretfov2", 0x8031}, // 0x506CA0
        {"lerpfov", 0x8032}, // 0x506E70
        {"lerpfovscale", 0x8033}, // 0x506FA0
        {"getvalidcoverpeekouts", 0x8034}, // 0x524B80
        {"gethighestnodestance", 0x8035}, // 0x524B10
        {"doesnodeallowstance", 0x8036}, // 0x5249E0
        {"_meth_8037", 0x8037}, // 0x524A90
        {"getgunangles", 0x8038}, // 0x5019E0
        {"magicgrenade", 0x8039}, // 0x501B20
        {"magicgrenademanual", 0x803A}, // 0x501FD0
        {"getentnum", 0x803B}, // 0x503F30
        {"launch", 0x803C}, // 0x503F50
        {"setsoundblend", 0x803D}, // 0x504250
        {"makefakeai", 0x803E}, // 0x507170
        {"spawndrone", 0x803F}, // 0x5071D0
        {"setcorpseremovetimer", 0x8040}, // 0x507320
        {"setlookattext", 0x8041}, // 0x507380
        {"setspawnerteam", 0x8042}, // 0x507410
        {"addaieventlistener", 0x8043}, // 0x508710
        {"removeaieventlistener", 0x8044}, // 0x508750
        {"getlightcolor", 0x8045}, // 0x4F9B00
        {"setlightcolor", 0x8046}, // 0x4F9CC0
        {"_meth_8047", 0x8047}, // 0x4F9F60
        {"_meth_8048", 0x8048}, // 0x4FA1D0
        {"_meth_8049", 0x8049}, // 0x4E1D90
        {"_meth_804a", 0x804A}, // 0x4E2050
        {"getlightradius", 0x804B}, // 0x503B60
        {"setlightradius", 0x804C}, // 0x503B80
        {"getattachignorecollision", 0x804D}, // 0x4F3530
        {"hidepart", 0x804E}, // 0x4F3830
        {"hidepartallinstances", 0x804F}, // 0x4F3E40
        {"hideallparts", 0x8050}, // 0x4F4610
        {"showpart", 0x8051}, // 0x4F47B0
        {"showallparts", 0x8052}, // 0x4F4FC0
        {"linkto", 0x8053}, // 0x4F5BC0
        {"linktoblendtotag", 0x8054}, // 0x4F5EA0
        {"unlink", 0x8055}, // 0x4F5F30
        {"setnormalhealth", 0x8056}, // 0x5043D0
        {"dodamage", 0x8057}, // 0x4F64E0
        {"kill", 0x8058}, // 0x5047E0
        {"show", 0x8059}, // 0x504DB0
        {"hide", 0x805A}, // 0x504E20
        {"_meth_805b", 0x805B}, // 0x504DD0
        {"laserforceon", 0x805C}, // 0x504EF0
        {"laserforceoff", 0x805D}, // 0x504F80
        {"disconnectpaths", 0x805E}, // 0x4E5070
        {"connectpaths", 0x805F}, // 0x4E58C0
        {"disconnectnode", 0x8060}, // 0x4E5A90
        {"connectnode", 0x8061}, // 0x4E5BF0
        {"startusingheroonlylighting", 0x8062}, // 0x507450
        {"stopusingheroonlylighting", 0x8063}, // 0x507510
        {"startusinglessfrequentlighting", 0x8064}, // 0x507530
        {"stopusinglessfrequentlighting", 0x8065}, // 0x507550
        {"setmovingplatformplayerturnrate", 0x8066}, // 0x4FF730
        {"setthermalfog", 0x8067}, // 0x4FF9A0
        {"setnightvisionfog", 0x8068}, // 0x500460
        {"clearthermalfog", 0x8069}, // 0x5012B0
        {"clearnightvisionfog", 0x806A}, // 0x501320
        {"setmode", 0x806C}, // 0x505270
        {"getmode", 0x806D}, // 0x505620
        {"setturretignoregoals", 0x806E}, // 0x5057F0
        {"islinked", 0x806F}, // 0x4F5F90
        {"enablelinkto", 0x8070}, // 0x4F7320
        {"playsoundatviewheight", 0x8071}, // 0x503A90
        {"prefetchsound", 0x8073}, // 0x4E4870
        {"setpitch", 0x8074}, // 0x4E4730
        {"scalepitch", 0x8075}, // 0x4E4C40
        {"setvolume", 0x8076}, // 0x4E4AA0
        {"scalevolume", 0x8077}, // 0x4EA790
        {"setspeakermapmonotostereo", 0x8078}, // 0x503360
        {"setspeakermapmonoto51", 0x8079}, // 0x503370
        {"setdistributed2dsound", 0x807A}, // 0x503570
        {"playsoundasmaster", 0x807B}, // 0x503AD0
        {"playloopsound", 0x807C}, // 0x503AF0
        {"eqon", 0x807D}, // 0x503C10
        {"eqoff", 0x807E}, // 0x503C90
        {"haseq", 0x807F}, // 0x503D20
        {"iswaitingonsound", 0x8080}, // 0x503DA0
        {"foley", 0x8081}, // 0x503E50, server command name
        {"getnormalhealth", 0x8082}, // 0x5040A0
        {"playerlinkto", 0x8083}, // 0x4F8600
        {"playerlinktodelta", 0x8084}, // 0x4F8610
        {"playerlinkweaponviewtodelta", 0x8085}, // 0x4F87E0
        {"playerlinktoabsolute", 0x8086}, // 0x4F8820
        {"playerlinktoblend", 0x8087}, // 0x4E3B40
        {"playerlinkedoffsetenable", 0x8088}, // 0x4F8D50
        {"setwaypointedgestyle_secondaryarrow", 0x8089}, // 0x4C0980
        {"setwaypointiconoffscreenonly", 0x808A}, // 0x4C09D0
        {"fadeovertime", 0x808B}, // 0x4C0D90
        {"scaleovertime", 0x808C}, // 0x4C0ED0
        {"moveovertime", 0x808D}, // 0x4C0FB0
        {"reset", 0x808E}, // 0x4C10F0
        {"destroy", 0x808F}, // 0x4C1130
        {"setpulsefx", 0x8090}, // 0x4C1270
        {"setplayernamestring", 0x8091}, // 0x4C11B0
        {"changefontscaleovertime", 0x8092}, // 0x4C0E30
        {"startignoringspotlight", 0x8093}, // 0x5073D0
        {"stopignoringspotlight", 0x8094}, // 0x5073F0
        {"dontcastshadows", 0x8095}, // 0x5085C0
        {"castshadows", 0x8096}, // 0x5085E0
        {"setstablemissile", 0x8097}, // 0x505180
        {"playersetgroundreferenceent", 0x8098}, // 0x4FF530
        {"dontinterpolate", 0x8099}, // 0x4F9F10
        {"dospawn", 0x809A}, // 0x501390
        {"stalingradspawn", 0x809B}, // 0x501430
        {"getorigin", 0x809C}, // 0x501800
        {"getcentroid", 0x809D}, // 0x501880
        {"getshootatpos", 0x809E}, // 0x5018D0
        {"getdebugeye", 0x809F}, // 0x502A70
        {"useby", 0x80A0}, // 0x502B30
        {"playsound", 0x80A1}, // 0x5035A0
        {"playerlinkedoffsetdisable", 0x80A6}, // 0x4F90A0
        {"playerlinkedsetviewznear", 0x80A7}, // 0x4F9360
        {"playerlinkedsetusebaseangleforviewclamp", 0x80A8}, // 0x4F9940
        {"lerpviewangleclamp", 0x80A9}, // 0x4E6410
        {"setviewangleresistance", 0x80AA}, // 0x4E6FA0
        {"springcamenabled", 0x80AB}, // 0x4E7DB0
        {"springcamdisabled", 0x80AC}, // 0x4E8760
        {"linktoplayerview", 0x80AD}, // 0x4EA000
        {"unlinkfromplayerview", 0x80AE}, // 0x4EA8D0
        {"geteye", 0x80AF}, // 0x4F0840
        {"istouching", 0x80B0}, // 0x4F8630
        {"stoploopsound", 0x80B1}, // 0x4F89A0
        {"stopsounds", 0x80B2}, // 0x4EB760
        {"playrumbleonentity", 0x80B3}, // 0x4F9200
        {"playrumblelooponentity", 0x80B4}, // 0x4EBC30
        {"_meth_80b5", 0x80B5}, // 0x4EBC60
        {"_meth_80b6", 0x80B6}, // 0x4EC140
        {"delete", 0x80B7}, // 0x4F0460
        {"setmodel", 0x80B8}, // 0x4EE2A0
        {"laseron", 0x80B9}, // 0x4EC720
        {"laseroff", 0x80BA}, // 0x4EC8B0
        {"laseraltviewon", 0x80BD}, // 0x4EC9A0
        {"laseraltviewoff", 0x80BE}, // 0x4ED190
        {"thermalvisionon", 0x80BF}, // 0x4EE4D0
        {"thermalvisiononshadowoff", 0x80C0}, // 0x4EE7F0
        {"setcontents", 0x80C5}, // 0x4EAA70
        {"makeusable", 0x80C6}, // 0x4EAC90
        {"makeunusable", 0x80C7}, // 0x4EAE90
        {"_meth_80c8", 0x80C8}, // 0x4E3270
        {"_meth_80c9", 0x80C9}, // 0x4E3800
        {"setwhizbyprobabilities", 0x80CA}, // 0x4B9F70
        {"visionsetnakedforplayer_lerp", 0x80CB}, // 0x4B7190
        {"setwaitnode", 0x80CC}, // 0x6D74C0
        {"returnplayercontrol", 0x80CD}, // 0x6D4DF0
        {"vehphys_starttrack", 0x80CE}, // 0x6D7320
        {"vehphys_clearautodisable", 0x80CF}, // 0x6D73B0
        {"vehicleusealtblendedaudio", 0x80D0}, // 0x6D65E0
        {"settext", 0x80D1}, // 0x4C1D90
        {"clearalltextafterhudelem", 0x80D2}, // 0x4C1E80
        {"setshader", 0x80D3}, // 0x4C1E90
        {"settargetent", 0x80D4}, // 0x4C2200
        {"cleartargetent", 0x80D5}, // 0x4C23A0
        {"settimer", 0x80D6}, // 0x4C0200
        {"settimerup", 0x80D7}, // 0x4C02D0
        {"settimerstatic", 0x80D8}, // 0x4C03A0
        {"settenthstimer", 0x80D9}, // 0x4C0470
        {"settenthstimerup", 0x80DA}, // 0x4C0540
        {"settenthstimerstatic", 0x80DB}, // 0x4C0610
        {"setclock", 0x80DC}, // 0x4C06E0
        {"setclockup", 0x80DD}, // 0x4C0700
        {"setvalue", 0x80DE}, // 0x4C0720
        {"setwaypoint", 0x80DF}, // 0x4C07C0
        {"setwaypointedgestyle_rotatingicon", 0x80E0}, // 0x4C0930
        {"setcursorhint", 0x80E1}, // 0x4EB300
        {"sethintstring", 0x80E2}, // 0x4EBA30
        {"forceusehinton", 0x80E4}, // 0x4EBF10
        {"forceusehintoff", 0x80E5}, // 0x4EC080
        {"makesoft", 0x80E6}, // 0x4EAF40
        {"makehard", 0x80E7}, // 0x4EB0F0
        {"willneverchange", 0x80E8}, // 0x4EB190
        {"startfiring", 0x80E9}, // 0x4F9400
        {"stopfiring", 0x80EA}, // 0x4F9740
        {"isfiringturret", 0x80EB}, // 0x4F9C60
        {"startbarrelspin", 0x80EC}, // 0x4F9D60
        {"stopbarrelspin", 0x80ED}, // 0x4FA020
        {"getbarrelspinrate", 0x80EE}, // 0x4FA260
        {"shootturret", 0x80F1}, // 0x4E3F80
        {"getturretowner", 0x80F2}, // 0x4E4120
        {"enabledeathshield", 0x80F3}, // 0x4BBCF0
        {"nightvisiongogglesforceon", 0x80F4}, // 0x4BBDC0
        {"nightvisiongogglesforceoff", 0x80F5}, // 0x4BBE10
        {"enableinvulnerability", 0x80F6}, // 0x4BBE60
        {"disableinvulnerability", 0x80F7}, // 0x4BBEC0
        {"forceviewmodelanimation", 0x80F8}, // 0x4BBF20
        {"_meth_80f9", 0x80F9}, // 0x4BBFB0
        {"_meth_80fa", 0x80FA}, // 0x4BC320
        {"disableturretdismount", 0x80FB}, // 0x4BC560
        {"enableturretdismount", 0x80FC}, // 0x4BC5C0
        {"uploadscore", 0x80FD}, // 0x4BC620
        {"uploadtime", 0x80FE}, // 0x4BC6B0
        {"uploadleaderboards", 0x80FF}, // 0x4BC760
        {"giveachievement", 0x8100}, // 0x4B6240, empty
        {"hidehud", 0x8101}, // 0x4BC0C0
        {"showhud", 0x8102}, // 0x4BC110
        {"mountvehicle", 0x8103}, // 0x4BC1E0
        {"dismountvehicle", 0x8104}, // 0x4BC270
        {"enableslowaim", 0x8105}, // 0x4B4880
        {"disableslowaim", 0x8106}, // 0x4B4AC0
        {"_meth_8107", 0x8107}, // 0x4BC160, some button pressed
        {"_meth_8108", 0x8108}, // 0x4B3AF0, ^
        {"_meth_8109", 0x8109}, // 0x4B3BE0, ^
        {"vehicleattackbuttonpressed", 0x810A}, // 0x4B3CF0
        {"setwhizbyoffset", 0x810B}, // 0x4B9EF0
        {"setsentryowner", 0x810C}, // 0x4E42F0
        {"setsentrycarrier", 0x810D}, // 0x4E47E0
        {"setturretminimapvisible", 0x810E}, // 0x4E4B30
        {"settargetentity", 0x810F}, // 0x4E4E20
        {"snaptotargetentity", 0x8110}, // 0x4E57B0
        {"cleartargetentity", 0x8111}, // 0x4E5B60
        {"getturrettarget", 0x8112}, // 0x4E5EA0
        {"setplayerspread", 0x8113}, // 0x4E69A0
        {"setaispread", 0x8114}, // 0x4E6F40
        {"setsuppressiontime", 0x8115}, // 0x4E7220
        {"setflaggedanimknobrestart", 0x8116}, // 0x4DF260
        {"setflaggedanimknoblimitedrestart", 0x8117}, // 0x4DF280
        {"setflaggedanimknoball", 0x8118}, // 0x4DF2A0
        {"setflaggedanimknoballrestart", 0x8119}, // 0x4DF2C0
        {"setflaggedanim", 0x811A}, // 0x4DF2E0
        {"setflaggedanimlimited", 0x811B}, // 0x4DF300
        {"setflaggedanimrestart", 0x811C}, // 0x4DF310
        {"setflaggedanimlimitedrestart", 0x811D}, // 0x4DF330
        {"useanimtree", 0x811E}, // 0x4DF350
        {"stopuseanimtree", 0x811F}, // 0x4DF430
        {"setanimtime", 0x8120}, // 0x4DF4B0
        {"allowstand", 0x8121}, // 0x4B4100
        {"allowcrouch", 0x8122}, // 0x4B4370
        {"allowprone", 0x8123}, // 0x4B4510
        {"allowlean", 0x8124}, // 0x4B4980
        {"allowswim", 0x8125}, // 0x4BB0E0
        {"setocclusion", 0x8126}, // 0x4BB590
        {"deactivateocclusion", 0x8128}, // 0x4BB320
        {"deactivateallocclusion", 0x8129}, // 0x4BB3A0
        {"isocclusionenabled", 0x812A}, // 0x4BB730
        {"iseqenabled", 0x812F}, // 0x4BB780
        {"seteq", 0x8130}, // 0x4BB3C0
        {"seteqbands", 0x8131}, // 0x4BB7D0
        {"deactivateeq", 0x8132}, // 0x4BB930
        {"seteqlerp", 0x8133}, // 0x4BB240
        {"islookingat", 0x8134}, // 0x4BB9E0
        {"isthrowinggrenade", 0x8135}, // 0x4BA2C0
        {"isfiring", 0x8136}, // 0x4B20C0
        {"ismeleeing", 0x8137}, // 0x4B6AF0
        {"setautopickup", 0x8138}, // 0x4BBC20
        {"allowmelee", 0x8139}, // 0x4B3DD0
        {"allowfire", 0x813A}, // 0x4B3FC0
        {"enablehealthshield", 0x813B}, // 0x4BBC90
        {"setconvergencetime", 0x813C}, // 0x4E75E0
        {"setconvergenceheightpercent", 0x813D}, // 0x4E7D20
        {"setturretteam", 0x813E}, // 0x4E8160
        {"maketurretsolid", 0x813F}, // 0x4E8710
        {"_meth_8140", 0x8140}, // 0x4E87F0
        {"maketurretoperable", 0x8141}, // 0x4E8BF0
        {"maketurretinoperable", 0x8142}, // 0x4E8CE0
        {"makeentitysentient", 0x8143}, // 0x531180
        {"freeentitysentient", 0x8144}, // 0x531290
        {"isindoor", 0x8145}, // 0x5313C0
        {"getdroptofloorposition", 0x8146}, // 0x531420
        {"isbadguy", 0x8147}, // 0x531500
        {"animscripted", 0x8148}, // 0x4DFA90
        {"animrelative", 0x814A}, // 0x4DFAD0
        {"stopanimscripted", 0x814B}, // 0x4DF9C0
        {"clearanim", 0x814C}, // 0x4DFB10
        {"setanimknob", 0x814D}, // 0x4DFB90
        {"setanimknoblimited", 0x814E}, // 0x4DFBB0
        {"setanimknobrestart", 0x814F}, // 0x4DFBC0
        {"setanimknoblimitedrestart", 0x8150}, // 0x4DFBE0
        {"setanimknoball", 0x8151}, // 0x4DFC00
        {"setanimknoballlimited", 0x8152}, // 0x4DFC20
        {"setanimknoballrestart", 0x8153}, // 0x4DFC30
        {"setanimknoballlimitedrestart", 0x8154}, // 0x4DFC50
        {"setanim", 0x8155}, // 0x4DF0C0
        {"setanimlimited", 0x8156}, // 0x4DF0E0
        {"setanimrestart", 0x8157}, // 0x4DF0F0
        {"setanimlimitedrestart", 0x8158}, // 0x4DF110
        {"getanimtime", 0x8159}, // 0x4DF130
        {"getanimweight", 0x815A}, // 0x4DF1A0
        {"getanimassettype", 0x815B}, // 0x4DF1E0
        {"setflaggedanimknob", 0x815C}, // 0x4DF230
        {"setflaggedanimknoblimited", 0x815D}, // 0x4DF250
        {"setturretaccuracy", 0x815E}, // 0x4E9000, empty
        {"setrightarc", 0x815F}, // 0x4E9860
        {"setleftarc", 0x8160}, // 0x4E9F20
        {"settoparc", 0x8161}, // 0x4EA4F0
        {"setbottomarc", 0x8162}, // 0x4EA6A0
        {"setautorotationdelay", 0x8163}, // 0x4EA800
        {"setdefaultdroppitch", 0x8164}, // 0x4EACF0
        {"restoredefaultdroppitch", 0x8165}, // 0x4EB090
        {"turretfiredisable", 0x8166}, // 0x4EB280
        {"getfixednodesafevolume", 0x8167}, // 0x49AA00
        {"clearfixednodesafevolume", 0x8168}, // 0x49AA60
        {"isingoal", 0x8169}, // 0x49AB00
        {"setruntopos", 0x816A}, // 0x49AC00
        {"nearnode", 0x816B}, // 0x49AD20
        {"nearclaimnode", 0x816C}, // 0x49ADD0
        {"nearclaimnodeandangle", 0x816D}, // 0x49AE60
        {"atdangerousnode", 0x816E}, // 0x49AF30
        {"_meth_816f", 0x816F}, // 0x49B020
        {"_meth_8170", 0x8170}, // 0x49B0D0
        {"getenemyinfo", 0x8171}, // 0x4B52F0
        {"clearenemy", 0x8172}, // 0x49B190
        {"setentitytarget", 0x8173}, // 0x49B2F0
        {"clearentitytarget", 0x8174}, // 0x49B520
        {"setpotentialthreat", 0x8175}, // 0x49B6A0
        {"clearpotentialthreat", 0x8176}, // 0x49B700
        {"setflashbanged", 0x8177}, // 0x49B7C0
        {"setengagementmindist", 0x8178}, // 0x49B910
        {"setengagementmaxdist", 0x8179}, // 0x49B980
        {"isknownenemyinradius", 0x817A}, // 0x49BB70
        {"isknownenemyinvolume", 0x817B}, // 0x49BC00
        {"settalktospecies", 0x817C}, // 0x49BC70
        {"invisiblenotsolid", 0x817D}, // 0x49BE50
        {"visiblesolid", 0x817E}, // 0x49BF50
        {"setdefaultaimlimits", 0x817F}, // 0x49BFB0
        {"initriotshieldhealth", 0x8180}, // 0x49C0D0
        {"getenemysqdist", 0x8181}, // 0x531560
        {"getclosestenemysqdist", 0x8182}, // 0x5315A0
        {"setthreatbiasgroup", 0x8183}, // 0x531970
        {"getthreatbiasgroup", 0x8184}, // 0x5319E0
        {"turretfireenable", 0x8185}, // 0x4EB470
        {"setturretmodechangewait", 0x8186}, // 0x4EB7B0
        {"usetriggerrequirelookat", 0x8187}, // 0x4EADE0
        {"getstance", 0x8188}, // 0x4EF4A0
        {"setstance", 0x8189}, // 0x4EF790
        {"itemweaponsetammo", 0x818A}, // 0x4ECA70
        {"getammocount", 0x818B}, // 0x4ED240
        {"gettagorigin", 0x818C}, // 0x4F3370
        {"gettagangles", 0x818D}, // 0x4F3440
        {"shellshock", 0x818E}, // 0x4F8BC0
        {"stunplayer", 0x818F}, // 0x4F9120
        {"stopshellshock", 0x8190}, // 0x4ED870
        {"fadeoutshellshock", 0x8191}, // 0x4EDAD0
        {"setdepthoffield", 0x8192}, // 0x4F3B40
        {"setviewmodeldepthoffield", 0x8193}, // 0x4F4550
        {"setmotionblurmovescale", 0x8194}, // 0x4F5A30
        {"pickupgrenade", 0x8195}, // 0x498F10
        {"useturret", 0x8196}, // 0x498F60
        {"stopuseturret", 0x8197}, // 0x498FD0
        {"canuseturret", 0x8198}, // 0x499010
        {"traversemode", 0x8199}, // 0x499080
        {"animmode", 0x819A}, // 0x499110
        {"orientmode", 0x819B}, // 0x4992E0
        {"getmotionangle", 0x819C}, // 0x499640
        {"shouldfacemotion", 0x819D}, // 0x499740
        {"getanglestolikelyenemypath", 0x819E}, // 0x499790
        {"setturretanim", 0x819F}, // 0x4997E0
        {"getturret", 0x81A0}, // 0x499840
        {"getgroundenttype", 0x81A1}, // 0x499890
        {"animcustom", 0x81A6}, // 0x499960
        {"isinscriptedstate", 0x81A7}, // 0x499A10
        {"canattackenemynode", 0x81A8}, // 0x499A60
        {"getnegotiationstartnode", 0x81A9}, // 0x499B70
        {"getnegotiationendnode", 0x81AA}, // 0x499BE0
        {"getnegotiationnextnode", 0x81AB}, // 0x499C50
        {"getdoorpathnode", 0x81AC}, // 0x499CD0
        {"comparenodedirtopathdir", 0x81AD}, // 0x499D70
        {"checkprone", 0x81AE}, // 0x499E60
        {"pushplayer", 0x81AF}, // 0x49A050
        {"checkgrenadethrowpos", 0x81B0}, // 0x498850
        {"setgoalnode", 0x81B1}, // 0x49A1B0
        {"setgoalpos", 0x81B2}, // 0x49A410
        {"setgoalentity", 0x81B3}, // 0x49A480
        {"setgoalvolume", 0x81B4}, // 0x49A520
        {"setgoalvolumeauto", 0x81B5}, // 0x49A650
        {"getgoalvolume", 0x81B6}, // 0x49A7C0
        {"cleargoalvolume", 0x81B7}, // 0x49A810
        {"setfixednodesafevolume", 0x81B8}, // 0x49A8D0
        {"setmotionblurturnscale", 0x81B9}, // 0x4F5A40
        {"setmotionblurzoomscale", 0x81BA}, // 0x4F5A50
        {"viewkick", 0x81BB}, // 0x4F5AB0
        {"localtoworldcoords", 0x81BC}, // 0x4ECFE0
        {"getentitynumber", 0x81BD}, // 0x4EBB50
        {"getentityvelocity", 0x81BE}, // 0x4EBBC0
        {"enablegrenadetouchdamage", 0x81BF}, // 0x4EBC90
        {"disablegrenadetouchdamage", 0x81C0}, // 0x4EBFD0
        {"enableaimassist", 0x81C1}, // 0x4ECD20
        {"setlookatyawlimits", 0x81C2}, // 0x49C1A0
        {"stoplookat", 0x81C3}, // 0x49C220
        {"getmuzzlepos", 0x81C4}, // 0x49C6E0
        {"getmuzzleangle", 0x81C5}, // 0x49C7D0
        {"getmuzzlesideoffsetpos", 0x81C6}, // 0x49C930
        {"getaimangle", 0x81C7}, // 0x49CCE0
        {"canshoot", 0x81C8}, // 0x49CE30
        {"canshootenemy", 0x81C9}, // 0x49CF20
        {"cansee", 0x81CA}, // 0x49CFA0
        {"seerecently", 0x81CB}, // 0x49D060
        {"lastknowntime", 0x81CC}, // 0x5312E0
        {"lastknownpos", 0x81CD}, // 0x531340
        {"dropweapon", 0x81CE}, // 0x49D1B0
        {"maymovetopoint", 0x81CF}, // 0x497FE0
        {"maymovefrompointtopoint", 0x81D0}, // 0x498150
        {"teleport", 0x81D1}, // 0x498270
        {"forceteleport", 0x81D2}, // 0x498280
        {"safeteleport", 0x81D3}, // 0x4982A0
        {"withinapproxpathdist", 0x81D4}, // 0x4982C0
        {"ispathdirect", 0x81D5}, // 0x498320
        {"allowedstances", 0x81D6}, // 0x498390
        {"isstanceallowed", 0x81D7}, // 0x498460
        {"issuppressionwaiting", 0x81D8}, // 0x498550
        {"issuppressed", 0x81D9}, // 0x498590
        {"ismovesuppressed", 0x81DA}, // 0x4985D0
        {"isgrenadepossafe", 0x81DB}, // 0x498610
        {"checkgrenadethrow", 0x81DC}, // 0x4986C0
        {"checkgrenadelaunch", 0x81DD}, // 0x498D40
        {"checkgrenadelaunchpos", 0x81DE}, // 0x498E40
        {"throwgrenade", 0x81DF}, // 0x498A50
        {"disableaimassist", 0x81E0}, // 0x4ECED0
        {"radiusdamage", 0x81E1}, // 0x4F7530
        {"detonate", 0x81E2}, // 0x4F8080
        {"damageconetrace", 0x81E3}, // 0x4F8800
        {"sightconetrace", 0x81E4}, // 0x4F8AF0
        {"missile_settargetent", 0x81E5}, // 0x4EC260
        {"missile_settargetpos", 0x81E6}, // 0x4EC580
        {"missile_cleartarget", 0x81E7}, // 0x4EC7B0
        {"missile_setflightmodedirect", 0x81E8}, // 0x4EC940
        {"missile_setflightmodetop", 0x81E9}, // 0x4EC990
        {"getlightintensity", 0x81EA}, // 0x4F8DF0
        {"setlightintensity", 0x81EB}, // 0x4F9020
        {"_meth_81ec", 0x81EC}, // 0x4F91B0, similar to 81ea/81eb
        {"_meth_81ed", 0x81ED}, // 0x4F9260, ^
        {"_meth_81ee", 0x81EE}, // 0x4F9510, ^
        {"_meth_81ef", 0x81EF}, // 0x4F96D0, ^
        {"isragdoll", 0x81F0}, // 0x4E2530
        {"setmovespeedscale", 0x81F1}, // 0x4F5260
        {"cameralinkto", 0x81F2}, // 0x4E3650
        {"cameraunlink", 0x81F3}, // 0x4E3840
        {"startcoverarrival", 0x81F4}, // 0x49A010
        {"starttraversearrival", 0x81F5}, // 0x49A240
        {"checkcoverexitposwithpath", 0x81F6}, // 0x49A280
        {"shoot", 0x81F7}, // 0x49A300
        {"shootblank", 0x81F8}, // 0x49A5E0
        {"melee", 0x81F9}, // 0x49A730
        {"updateplayersightaccuracy", 0x81FA}, // 0x49A980
        {"findshufflecovernode", 0x81FB}, // 0x49AAB0
        {"findnearbycovernode", 0x81FC}, // 0x49ABA0
        {"findcovernode", 0x81FD}, // 0x49ACE0
        {"findbestcovernode", 0x81FE}, // 0x49AD80
        {"getcovernode", 0x81FF}, // 0x49AE10
        {"usecovernode", 0x8200}, // 0x49AEA0
        {"iscovervalidagainstenemy", 0x8201}, // 0x49AFB0
        {"reacquirestep", 0x8202}, // 0x49B070
        {"findreacquiredirectpath", 0x8203}, // 0x49B120
        {"trimpathtoattack", 0x8204}, // 0x49B420
        {"reacquiremove", 0x8205}, // 0x49B480
        {"findreacquireproximatepath", 0x8206}, // 0x49B280
        {"flagenemyunattackable", 0x8207}, // 0x49B4E0
        {"enterprone", 0x8208}, // 0x49B5D0
        {"exitprone", 0x8209}, // 0x49B760
        {"setproneanimnodes", 0x820A}, // 0x49B810
        {"updateprone", 0x820B}, // 0x49B9F0
        {"clearpitchorient", 0x820C}, // 0x49BD40
        {"setlookatanimnodes", 0x820D}, // 0x49BEB0
        {"setlookat", 0x820E}, // 0x49C020
        {"setlookatentity", 0x820F}, // 0x49C2F0
        {"controlslinkto", 0x8210}, // 0x49C660
        {"controlsunlink", 0x8211}, // 0x4E3FF0
        {"makevehiclesolidcapsule", 0x8212}, // 0x4E4380
        {"teleportentityrelative", 0x8214}, // 0x501020
        {"drivevehicleandcontrolturret", 0x821C}, // 0x4E6720
        {"drivevehicleandcontrolturretoff", 0x821D}, // 0x4E6AA0
        {"getplayersetting", 0x821E}, // 0x4E7320
        {"getlocalplayerprofiledata", 0x821F}, // 0x4E84A0
        {"setlocalplayerprofiledata", 0x8220}, // 0x4E8DF0
        {"remotecamerasoundscapeon", 0x8221}, // 0x4EAAE0
        {"remotecamerasoundscapeoff", 0x8222}, // 0x4EABD0
        {"setmotiontrackervisible", 0x8223}, // 0x4EAD70
        {"getmotiontrackervisible", 0x8224}, // 0x4EAF90
        {"worldpointinreticle_circle", 0x8225}, // 0x4F05C0
        {"worldpointinreticle_rect", 0x8226}, // 0x4F1650
        {"getpointinbounds", 0x8227}, // 0x4F1B20
        {"transfermarkstonewscriptmodel", 0x8228}, // 0x4F20B0
        {"setwatersheeting", 0x8229}, // 0x4E3980
        {"setweaponhudiconoverride", 0x822C}, // 0x4EB1A0
        {"getweaponhudiconoverride", 0x822D}, // 0x4EB8E0
        {"setempjammed", 0x822E}, // 0x4EBCF0
        {"playersetexpfogext", 0x822F}, // 0x4ED110, server command name
        {"playersetexpfog", 0x8230}, // 0x4ECE50
        {"playersetatmosfog", 0x8231}, // 0x4ED7C0, server command name
        {"isitemunlocked", 0x8232}, // 0x4F1830
        {"getplayerdata", 0x8233}, // 0x4F99C0
        {"vehicleturretcontroloff", 0x8234}, // 0x6D56D0
        {"isturretready", 0x8235}, // 0x6D5740
        {"vehicledriveto", 0x8236}, // 0x6D5990
        {"vehicle_dospawn", 0x8237}, // 0x6D5C20
        {"vehicle_isphysveh", 0x8238}, // 0x6D5D50
        {"vehphys_crash", 0x8239}, // 0x6D5DC0
        {"_meth_823a", 0x823A}, // 0x6D5F30, vehicle
        {"vehphys_launch", 0x823B}, // 0x6D60A0
        {"vehphys_disablecrashing", 0x823C}, // 0x6D61E0
        {"vehphys_enablecrashing", 0x823D}, // 0x6D6290
        {"vehphys_setspeed", 0x823E}, // 0x6D6310
        {"vehphys_setconveyorbelt", 0x823F}, // 0x6D64E0
        {"playerlinkedturretanglesenable", 0x8241}, // 0x4F9DB0
        {"playerlinkedturretanglesdisable", 0x8242}, // 0x4FA490
        {"_meth_8243", 0x8243}, // 0x4E25E0, enable/disable
        {"_meth_8244", 0x8244}, // 0x4E2A80, ^
        {"playersetstreamorigin", 0x8245}, // 0x4E3020
        {"playerclearstreamorigin", 0x8246}, // 0x4E35F0
        {"nightvisionviewon", 0x8247}, // 0x4EE9D0
        {"nightvisionviewoff", 0x8248}, // 0x4EED80
        {"painvisionon", 0x8249}, // 0x4EEEE0
        {"painvisionoff", 0x824A}, // 0x4EF360
        {"getplayerintelisfound", 0x824B}, // 0x4E7B30
        {"setplayerintelfound", 0x824C}, // 0x4E8050
        {"sethuddynlight", 0x824E}, // 0x4E41B0
        {"startscriptedanim", 0x824F}, // 0x499B10
        {"startcoverbehavior", 0x8250}, // 0x499F80
        {"setplayerdata", 0x8251}, // 0x4FA070
        {"getcacplayerdata", 0x8252}, // 0x4E22F0
        {"setcacplayerdata", 0x8253}, // 0x4E2890
        {"trackerupdate", 0x8254}, // 0x4E4960
        {"buttonpressed", 0x8256}, // 0x4BB030
        {"dropitem", 0x825A}, // 0x4B7A30
        {"setjitterparams", 0x825C}, // 0x6D84D0
        {"sethoverparams", 0x825D}, // 0x6D85A0
        {"joltbody", 0x825E}, // 0x6D86C0
        {"freevehicle", 0x825F}, // 0x6D88C0
        {"getwheelsurface", 0x8260}, // 0x6D8970
        {"getvehicleowner", 0x8261}, // 0x6D8BC0
        {"setvehiclelookattext", 0x8262}, // 0x6D8C40
        {"setvehicleteam", 0x8263}, // 0x6D8CD0
        {"neargoalnotifydist", 0x8264}, // 0x6D8D60
        {"setvehgoalpos", 0x8265}, // 0x6D8DD0
        {"setgoalyaw", 0x8266}, // 0x6D8EE0
        {"cleargoalyaw", 0x8267}, // 0x6D8F70
        {"settargetyaw", 0x8268}, // 0x6D8FF0
        {"cleartargetyaw", 0x8269}, // 0x6D9080
        {"vehicle_helisetai", 0x826A}, // 0x6D9100
        {"setturrettargetvec", 0x826B}, // 0x6D46C0
        {"setturrettargetent", 0x826C}, // 0x6D47C0
        {"clearturrettargetent", 0x826D}, // 0x6D48C0
        {"vehicle_canturrettargetpoint", 0x826E}, // 0x6D4950
        {"setlookatent", 0x826F}, // 0x6D4C60
        {"clearlookatent", 0x8270}, // 0x6D4D10
        {"setvehweapon", 0x8271}, // 0x6D4E70
        {"fireweapon", 0x8272}, // 0x6D4EE0
        {"vehicleturretcontrolon", 0x8273}, // 0x6D55F0
        {"physicslaunchserver", 0x8280}, // 0x50A760
        {"physicslaunchserveritem", 0x8281}, // 0x50A990
        {"clonebrushmodeltoscriptmodel", 0x8282}, // 0x50B0A0
        {"vehicle_teleport", 0x8286}, // 0x6D6C80
        {"attachpath", 0x8287}, // 0x6D6EA0
        {"getattachpos", 0x8288}, // 0x6D6FC0
        {"startpath", 0x8289}, // 0x6D7190
        {"setswitchnode", 0x828A}, // 0x6D7430
        {"setwaitspeed", 0x828B}, // 0x6D7530
        {"vehicle_setspeed", 0x828D}, // 0x6D75B0
        {"vehicle_setspeedimmediate", 0x828E}, // 0x6D7620
        {"vehicle_rotateyaw", 0x828F}, // 0x6D77D0
        {"vehicle_getspeed", 0x8290}, // 0x6D78F0
        {"vehicle_getvelocity", 0x8291}, // 0x6D79E0
        {"vehicle_getbodyvelocity", 0x8292}, // 0x6D7A50
        {"vehicle_getsteering", 0x8293}, // 0x6D7AC0
        {"vehicle_getthrottle", 0x8294}, // 0x6D7BB0
        {"vehicle_turnengineoff", 0x8295}, // 0x6D7C20
        {"vehicle_turnengineon", 0x8296}, // 0x6D7C80
        {"vehicle_orientto", 0x8297}, // 0x6D7CE0
        {"getgoalspeedmph", 0x8298}, // 0x6D7DC0
        {"setacceleration", 0x8299}, // 0x6D7E30
        {"setdeceleration", 0x829A}, // 0x6D7EA0
        {"resumespeed", 0x829B}, // 0x6D7F10
        {"setyawspeed", 0x829C}, // 0x6D7FA0
        {"setyawspeedbyname", 0x829D}, // 0x6D8100
        {"setmaxpitchroll", 0x829E}, // 0x6D8310
        {"setairresitance", 0x829F}, // 0x6D83D0
        {"setturningability", 0x82A0}, // 0x6D8460
        {"_meth_82a1", 0x82A1}, // 0x4B45F0
        {"weaponlocknoclearance", 0x82AD}, // 0x4B3D60
        {"showhudsplash", 0x82AF}, // 0x4B6B50
        {"setperk", 0x82B0}, // 0x4B9D40
        {"hasperk", 0x82B1}, // 0x4BA190
        {"clearperks", 0x82B2}, // 0x4B2220
        {"unsetperk", 0x82B3}, // 0x4BA5B0
        {"noclip", 0x82B6}, // 0x4B2310
        {"ufo", 0x82B7}, // 0x4B2320
        {"moveto", 0x82B8}, // 0x50B590
        {"movex", 0x82B9}, // 0x50BAC0
        {"movey", 0x82BA}, // 0x50BAD0
        {"movez", 0x82BB}, // 0x50BAF0
        {"movegravity", 0x82BC}, // 0x50B730
        {"moveslide", 0x82BD}, // 0x50B8B0
        {"stopmoveslide", 0x82BE}, // 0x50BA40
        {"rotateto", 0x82BF}, // 0x50BB90
        {"rotatepitch", 0x82C0}, // 0x50C010
        {"rotateyaw", 0x82C1}, // 0x50C020
        {"rotateroll", 0x82C2}, // 0x50C040
        {"addpitch", 0x82C3}, // 0x50BF80
        {"addyaw", 0x82C4}, // 0x50BFD0
        {"addroll", 0x82C5}, // 0x50BFF0
        {"vibrate", 0x82C6}, // 0x50C060
        {"rotatevelocity", 0x82C7}, // 0x50C2C0
        {"solid", 0x82C8}, // 0x50B180
        {"notsolid", 0x82C9}, // 0x50B230
        {"setcandamage", 0x82CA}, // 0x50C460
        {"setcanradiusdamage", 0x82CB}, // 0x50C4C0
        {"physicslaunchclient", 0x82CC}, // 0x50A3E0
        {"disableweaponpickup", 0x82D5}, // 0x4B6300
        {"enableweaponpickup", 0x82D6}, // 0x4B6470
        {"issplitscreenplayer", 0x82D7}, // 0x4B6560
        {"getweaponslistoffhands", 0x82D9}, // 0x4B5AC0
        {"getweaponslistitems", 0x82DA}, // 0x4B5BD0
        {"getweaponslistexclusives", 0x82DB}, // 0x4B5D70
        {"getweaponslist", 0x82DC}, // 0x4B5E80
        {"canplayerplacesentry", 0x82DD}, // 0x4B9550
        {"canplayerplacetank", 0x82DE}, // 0x4B9A10
        {"_meth_82df", 0x82DF}, // 0x4B6FF0
        {"visionsetnakedforplayer", 0x82E0}, // 0x4B76A0
        {"visionsetnightforplayer", 0x82E1}, // 0x4B7710
        {"visionsetmissilecamforplayer", 0x82E2}, // 0x4B77C0
        {"visionsetthermalforplayer", 0x82E3}, // 0x4B77E0
        {"setblurforplayer", 0x82E4}, // 0x4B8EA0
        {"getplayerweaponmodel", 0x82E5}, // 0x4B9500
        {"getplayerknifemodel", 0x82E6}, // 0x4B9520
        {"updateplayermodelwithweapons", 0x82E7}, // 0x502840
        {"notifyonplayercommand", 0x82E8}, // 0x4B23C0
        {"canmantle", 0x82E9}, // 0x4B2700
        {"forcemantle", 0x82EA}, // 0x4B2B20
        {"ismantling", 0x82EB}, // 0x4B2CA0
        {"playfx", 0x82EC}, // 0x4B3100
        {"player_recoilscaleon", 0x82ED}, // 0x4B34A0
        {"player_recoilscaleoff", 0x82EE}, // 0x4B3570
        {"weaponlockstart", 0x82EF}, // 0x4B3630
        {"weaponlockfinalize", 0x82F0}, // 0x4B3870
        {"weaponlockfree", 0x82F1}, // 0x4B3B80
        {"weaponlocktargettooclose", 0x82F2}, // 0x4B3C50
        {"getviewmodel", 0x82F8}, // 0x4B5450
        {"fragbuttonpressed", 0x82F9}, // 0x4B4170
        {"secondaryoffhandbuttonpressed", 0x82FA}, // 0x4B4240
        {"getcurrentweaponclipammo", 0x82FB}, // 0x4B6870
        {"setvelocity", 0x82FC}, // 0x4B2330
        {"getplayerviewheight", 0x82FD}, // 0x4B2B60
        {"getnormalizedmovement", 0x82FE}, // 0x4B2DD0
        {"playlocalsound", 0x82FF}, // 0x4B2670
        {"stoplocalsound", 0x8300}, // 0x4B2750
        {"setweaponammoclip", 0x8301}, // 0x4B7EC0
        {"setweaponammostock", 0x8302}, // 0x4B82B0
        {"getweaponammoclip", 0x8303}, // 0x4B87B0
        {"getweaponammostock", 0x8304}, // 0x4B8990
        {"hasreloadmultipleanim", 0x8305}, // 0x4B8C30
        {"regweaponforfxremoval", 0x8306}, // 0x4B9480
        {"setclientdvar", 0x8307}, // 0x4B9AD0
        {"setclientdvars", 0x8308}, // 0x4B9BD0
        {"allowads", 0x830B}, // 0x4B3770
        {"allowjump", 0x830C}, // 0x4B3A90
        {"allowsprint", 0x830D}, // 0x4B4A60
        {"_meth_830e", 0x830E}, // 0x4B4BC0, allow something
        {"_meth_830f", 0x830F}, // 0x4B4D50, equal to ^
        {"setspreadoverride", 0x8310}, // 0x4B5CE0
        {"resetspreadoverride", 0x8311}, // 0x4B6020
        {"setaimspreadmovementscale", 0x8312}, // 0x4B6130
        {"setactionslot", 0x8313}, // 0x4B5130
        {"setviewkickscale", 0x8314}, // 0x4B3EE0
        {"getviewkickscale", 0x8315}, // 0x4B40B0
        {"getweaponslistall", 0x8316}, // 0x4B58B0
        {"getweaponslistprimaries", 0x8317}, // 0x4B59B0
        {"getnormalizedcameramovement", 0x8318}, // 0x4B2F90
        {"giveweapon", 0x8319}, // 0x4B5500
        {"takeweapon", 0x831A}, // 0x4B60A0
        {"takeallweapons", 0x831B}, // 0x4B61C0
        {"getcurrentweapon", 0x831C}, // 0x4B6350
        {"getcurrentprimaryweapon", 0x831D}, // 0x4B6600
        {"getcurrentoffhand", 0x831E}, // 0x4B6A50
        {"hasweapon", 0x831F}, // 0x4B7290
        {"switchtoweapon", 0x8320}, // 0x4B7730
        {"switchtoweaponimmediate", 0x8321}, // 0x4B7890
        {"_meth_8322", 0x8322}, // 0x4B8580
        {"switchtooffhand", 0x8323}, // 0x4B86A0
        {"setoffhandsecondaryclass", 0x8324}, // 0x4B6EE0
        {"getoffhandsecondaryclass", 0x8325}, // 0x4B7110
        {"beginlocationselection", 0x8326}, // 0x4B3350
        {"endlocationselection", 0x8327}, // 0x4B35C0
        {"disableweapons", 0x8328}, // 0x4B7010
        {"enableweapons", 0x8329}, // 0x4B7380
        {"disableoffhandweapons", 0x832A}, // 0x4B74C0
        {"enableoffhandweapons", 0x832B}, // 0x4B7590
        {"disableweaponswitch", 0x832C}, // 0x4B7820
        {"enableweaponswitch", 0x832D}, // 0x4B79E0
        {"openpopupmenu", 0x832E}, // 0x4B6260
        {"openpopupmenunomouse", 0x832F}, // 0x4B64C0
        {"closepopupmenu", 0x8330}, // 0x4B65B0
        {"openmenu", 0x8331}, // 0x4B6780
        {"closemenu", 0x8332}, // 0x4B69B0
        {"freezecontrols", 0x8334}, // 0x4B6DA0
        {"disableusability", 0x8335}, // 0x4B6E90
        {"enableusability", 0x8336}, // 0x4B6FA0
        {"setwhizbyspreads", 0x8337}, // 0x4BA0C0
        {"setwhizbyradii", 0x8338}, // 0x4BA360
        {"setreverb", 0x8339}, // 0x4BA470
        {"deactivatereverb", 0x833A}, // 0x4B2150
        {"setvolmod", 0x833B}, // 0x4B2430
        {"givestartammo", 0x833C}, // 0x4B8D10
        {"givemaxammo", 0x833D}, // 0x4B9030
        {"getfractionstartammo", 0x833E}, // 0x4B9320
        {"getfractionmaxammo", 0x833F}, // 0x4B9810
        {"isdualwielding", 0x8340}, // 0x4B9CB0
        {"isreloading", 0x8341}, // 0x4B9E80
        {"isswitchingweapon", 0x8342}, // 0x4BA040
        {"setorigin", 0x8343}, // 0x4B2270
        {"getvelocity", 0x8344}, // 0x4B2560
        {"setplayerangles", 0x8345}, // 0x4B28E0
        {"getplayerangles", 0x8346}, // 0x4B2AD0
        {"usebuttonpressed", 0x8347}, // 0x4B36F0
        {"attackbuttonpressed", 0x8348}, // 0x4B3A10
        {"adsbuttonpressed", 0x8349}, // 0x4B3E40
        {"meleebuttonpressed", 0x834A}, // 0x4B4030
        {"playerads", 0x834B}, // 0x4B4DB0
        {"isonground", 0x834C}, // 0x4B4FD0
        {"isusingturret", 0x834D}, // 0x4B5290
        {"setviewmodel", 0x834E}, // 0x4B5370
        {"setoffhandprimaryclass", 0x834F}, // 0x4B6C30
        {"getoffhandprimaryclass", 0x8350}, // 0x4B6E10
        {"enablefocus", 0x8351}, // 0x4B2960, server command name
        {"disablefocus", 0x8352}, // 0x4B2C00, ^
        {"enablezoom", 0x8353}, // 0x4B2E80, ^
        {"disablezoom", 0x8354}, // 0x4B3060, ^
        {"_meth_8355", 0x8355}, // 0x4C1FB0, hud elem
        {"_meth_8356", 0x8356}, // 0x4B9540, empty
        {"_meth_8359", 0x8359}, // 0x4B2CF0
        {"_meth_835b", 0x835B}, // 0x4B4800
        {"_meth_8391", 0x8391}, // 0x4E6990, calls Scr_ErrorInternal
        {"_meth_8394", 0x8394}, // 0x4B53F0
        {"_meth_83b0", 0x83B0}, // 0x4C0B00
        {"_meth_83b1", 0x83B1}, // 0x4C0BB0
        {"_meth_83b2", 0x83B2}, // 0x4C0C20
        {"_meth_83b3", 0x83B3}, // 0x4C0C90
        {"_meth_83b4", 0x83B4}, // 0x507660
        {"_meth_83b5", 0x83B5}, // 0x4B42C0
        {"_meth_83b6", 0x83B6}, // 0x4B4450
        {"_meth_83b7", 0x83B7}, // 0x4BBBD0
        {"_meth_83b8", 0x83B8}, // 0x5079E0
        {"_meth_83b9", 0x83B9}, // 0x507B10
        {"_meth_83ba", 0x83BA}, // 0x4BBA80
        {"_meth_83bb", 0x83BB}, // 0x4BBB60
        {"_meth_83bc", 0x83BC}, // 0x4BBAF0
        {"_meth_83bf", 0x83BF}, // 0x4B3BD0, empty
        {"_meth_83c0", 0x83C0}, // 0x4B2D70
        {"_meth_83c1", 0x83C1}, // 0x4B2F50
        {"_meth_83c2", 0x83C2}, // 0x6D6E70
        {"_meth_83c3", 0x83C3}, // 0x6D6E80
        {"_meth_83c5", 0x83C5}, // 0x4EE040
        {"_meth_83c6", 0x83C6}, // 0x4EB9A0, returns 0.0
        {"_meth_83c7", 0x83C7}, // 0x4EB9D0, returns 0
        {"_meth_83c8", 0x83C8}, // 0x4B4780
        {"_meth_83c9", 0x83C9}, // 0x6D9360
        {"_meth_83ca", 0x83CA}, // 0x6D9410
        {"_meth_83cb", 0x83CB}, // 0x6D9490
        {"_meth_83cc", 0x83CC}, // 0x4DF6B0
        {"lightset", 0x83CD}, // 0x4B7BC0, server command name
        {"lightset2", 0x83CE}, // 0x4B7CB0, ^
        {"lightset3", 0x83CF}, // 0x4B84D0, ^
        {"_meth_83d0", 0x83D0}, // 0x50A5A0
        {"_meth_83d1", 0x83D1}, // 0x531100
        {"_meth_83d2", 0x83D2}, // 0x6D8A10, vehicle
        {"_meth_83d3", 0x83D3}, // 0x4B4BA0, empty
        {"_meth_83d4", 0x83D4}, // 0x4DF640
        {"setdoghandler", 0x83D5}, // 0x49C410
        {"setdogcommand", 0x83D6}, // 0x49C4A0
        {"setdogattackradius", 0x83D7}, // 0x49C760
        {"isdogfollowinghandler", 0x83D8}, // 0x49C860
        {"setdogmaxdrivespeed", 0x83D9}, // 0x49CAA0
        {"isdogbeingdriven", 0x83DA}, // 0x49CA30
        {"setdogautoattackwhendriven", 0x83DB}, // 0x49CB10
        {"getdogattackbeginnode", 0x83DC}, // 0x49CB70
        {"pushplayervector", 0x83E4}, // 0x505440
        {"issprinting", 0x83E5}, // 0x4B32F0
        {"playerlinkeduselinkedvelocity", 0x83E6}, // 0x4E1FD0
        {"shootstopsound", 0x83E7}, // 0x49A5A0
        {"setclothtype", 0x83E8}, // 0x4F9480
        {"getclothmovesound", 0x83E9}, // 0x4FF2D0
        {"getequipmovesound", 0x83EA}, // 0x4FF3F0
        {"jumpbuttonpressed", 0x83EB}, // 0x4B43D0
        {"rotateby", 0x83EC}, // 0x50BD60
        {"_meth_83ef", 0x83EF}, // 0x4F93F0, calls Scr_ErrorInternal
        {"setsurfacetype", 0x83F1}, // 0x4F9BC0
        {"aiphysicstrace", 0x83F2}, // 0x4F3F60
        {"aiphysicstracepassed", 0x83F3}, // 0x4F5490
        {"setdevtext", 0x83F4}, // 0x4C1E70, empty
        {"forcemovingplatformentity", 0x83F5}, // 0x507230
        {"setmovingplatformtrigger", 0x83F6}, // 0x507360
        {"visionsetstage", 0x83F7}, // 0x4B6CF0
        {"linkwaypointtotargetwithoffset", 0x83F8}, // 0x4C22B0
        {"getlinkedparent", 0x83F9}, // 0x4F6060
        {"getmovingplatformparent", 0x83FA}, // 0x4F6BE0
        {"retargetscriptmodellighting", 0x83FC}, // 0x5053E0
        {"clearclienttriggeraudiozone", 0x83FE}, // 0x4F8F20
        {"setclienttriggeraudiozone", 0x83FF}, // 0x4F7C60
        {"makevehiclenotcollidewithplayers", 0x8400}, // 0x4E59E0
        {"getbobrate", 0x8401}, // 0x4BB180
        {"setbobrate", 0x8402}, // 0x4BB1E0
        {"setscriptablepartstate", 0x8403}, // 0x4E2480
        {"stopsliding", 0x8404}, // 0x4E2C90, empty
        {"cancelrocketcorpse", 0x8405}, // 0x4F9560, calls Scr_ErrorInternal
        {"setdronegoalpos", 0x8406}, // 0x6D8D50, ^
        {"hudoutlineenable", 0x8407}, // 0x4EDC40, empty
        {"hudoutlinedisable", 0x8408}, // 0x4EDCD0, empty
        {"motionblurhqenable", 0x8409}, // 0x4EE150
        {"motionblurhqdisable", 0x840A}, // 0x4EE360
        {"screenshakeonentity", 0x840B}, // 0x5014D0
        {"_meth_840c", 0x840C}, // 0x49BD90
        {"_meth_840d", 0x840D}, // 0x49BDD0
        {"worldpointtoscreenpos", 0x840E}, // 0x4EC370
        {"_meth_840f", 0x840F}, // 0x4EA200
        {"shouldplaymeleedeathanim", 0x8410}, // 0x49A860
        {"visionsetwaterforplayer", 0x8412}, // 0x4B7870
        {"setwatersurfacetransitionfx", 0x8413}, // 0x4BC7B0
        {"linktoplayerviewfollowwatersurface", 0x8414}, // 0x4EA600
        {"linktoplayerviewattachwatersurfacetransitioneffects", 0x8415}, // 0x4EA710
        {"playersetwaterfog", 0x8416}, // 0x4EDC50
        {"enableforceviewmodeldof", 0x8418}, // 0x4BC020
        {"disableforceviewmodeldof", 0x8419}, // 0x4BC070
        {"_meth_841a", 0x841A}, // 0x5070F0
        {"isenemyaware", 0x841B}, // 0x505900
        {"hasenemybeenseen", 0x841C}, // 0x505AE0
        {"physicssetmaxlinvel", 0x841D}, // 0x50AB00
        {"physicssetmaxangvel", 0x841E}, // 0x50ABB0
        {"physicsgetlinvel", 0x841F}, // 0x50AC60
        {"physicsgetlinspeed", 0x8420}, // 0x50AD20
        {"physicsgetangvel", 0x8421}, // 0x50AE00
        {"physicsgetangspeed", 0x8422}, // 0x50AEC0
        {"disablemissileboosting", 0x842A}, // 0x4ED790, empty
        {"enablemissileboosting", 0x842B}, // 0x4ED840, ^
        {"canspawntestclient", 0x842C}, // 0x4ED9D0, ^
        {"spawntestclient", 0x842D}, // 0x4EDA90, ^
        {"turretsetbarrelspinenabled", 0x842E}, // 0x4EB580
        {"setclienttriggeraudiozonelerp", 0x8431}, // 0x4F8470
        {"setclienttriggeraudiozonepartial", 0x8432}, // 0x4F8680
        {"rotatetolinked", 0x8434}, // 0x50BC10
        {"rotatebylinked", 0x8435}, // 0x50BDE0
        {"setlinkedangles", 0x8436}, // 0x50BE80
        {"_meth_8438", 0x8438}, // 0x502800
        {"disableautoreload", 0x843E}, // 0x4B6730
        {"enableautoreload", 0x843F}, // 0x4B6820
        {"_meth_8440", 0x8440}, // 0x4B6970
        {"_meth_8441", 0x8441}, // 0x4BBC10, empty
        {"getlinkedchildren", 0x8442}, // 0x4F68B0
        {"cancelmantle", 0x8445}, // 0x4B2BB0
        {"setscriptabledamageowner", 0x8448}, // 0x4E1E50
        {"_meth_8449", 0x8449}, // 0x4E27E0
        {"_meth_844b", 0x844B}, // 0x49CBE0
        {"_meth_844c", 0x844C}, // 0x49CC80
        {"_meth_844d", 0x844D}, // 0x4B8480
        {"_meth_844e", 0x844E}, // 0x49D100
        {"_meth_844f", 0x844F}, // 0x4F3760
        {"_meth_8450", 0x8450}, // 0x4B4BB0
        {"_meth_8451", 0x8451}, // 0x6D4D80
        {"_meth_8452", 0x8452}, // 0x6D9550
        {"_meth_8453", 0x8453}, // 0x4F5C10
        {"_meth_8454", 0x8454}, // 0x4B96F0
        {"_meth_8455", 0x8455}, // 0x508600
        {"_meth_8456", 0x8456}, // 0x508640
        {"_meth_8457", 0x8457}, // 0x4E30A0
        {"_meth_8458", 0x8458}, // 0x4EDD90
        {"_meth_8459", 0x8459}, // 0x4EDE50
        {"_meth_845a", 0x845A}, // 0x4B3200
        {"_meth_845b", 0x845B}, // 0x4E61F0
        {"_meth_845c", 0x845C}, // 0x4E6920
        {"_meth_845d", 0x845D}, // 0x6D96D0
        {"_meth_845e", 0x845E}, // 0x6D96F0
        {"_meth_845f", 0x845F}, // 0x6D9710
        {"_meth_8460", 0x8460}, // 0x6D9730
        {"_meth_8461", 0x8461}, // 0x5035D0
        {"_meth_8462", 0x8462}, // 0x503910
        {"_meth_8463", 0x8463}, // 0x6D9610
        {"_meth_8464", 0x8464}, // 0x4B4E00
        {"_meth_8465", 0x8465}, // 0x4B3990
        {"_meth_8466", 0x8466}, // 0x504050
        {"_meth_8469", 0x8469}, // 0x4F09F0
        {"_meth_846a", 0x846A}, // 0x500080
        {"_meth_846b", 0x846B}, // 0x500150
        {"_meth_846c", 0x846C}, // 0x4B8AD0
        {"_meth_846f", 0x846F}, // 0x4FF640
        {"_meth_8470", 0x8470}, // 0x4F6F80
        {"_meth_8474", 0x8474}, // 0x4BA6E0
        {"_meth_8475", 0x8475}, // 0x4B3510
        {"_meth_8476", 0x8476}, // 0x4E5F30
        {"_meth_8477", 0x8477}, // 0x4B49F0
        {"_meth_8479", 0x8479}, // 0x50B340
        {"_meth_847a", 0x847A}, // 0x50B510
        {"_meth_847b", 0x847B}, // 0x4E4CC0
        {"_meth_847c", 0x847C}, // 0x4E5750
        {"_meth_847d", 0x847D}, // 0x4E5AB0
        {"_meth_847e", 0x847E}, // 0x4E5FD0
        {"_meth_847f", 0x847F}, // 0x4E6A00
        {"_meth_8480", 0x8480}, // 0x4B41F0
        {"_meth_8481", 0x8481}, // 0x5076F0
        {"_meth_8482", 0x8482}, // 0x6D97B0
        {"_meth_8483", 0x8483}, // 0x6D97F0
        {"_meth_8484", 0x8484}, // 0x6D9850
        {"_meth_8485", 0x8485}, // 0x6D98A0
        {"_meth_8486", 0x8486}, // 0x6D98C0
        {"_meth_8487", 0x8487}, // 0x4E1D10
        {"_meth_8488", 0x8488}, // 0x4E2270
        {"_meth_8489", 0x8489}, // 0x4B7920
        {"_meth_848a", 0x848A}, // 0x507810
        {"_meth_848b", 0x848B}, // 0x507910
        {"_meth_848c", 0x848C}, // 0x4B32D0
        {"_meth_848d", 0x848D}, // 0x4B32E0
        {"_meth_848e", 0x848E}, // 0x6D8AC0
        {"_meth_848f", 0x848F}, // 0x6D8B40
        {"_meth_8490", 0x8490}, // 0x4B54B0
        {"_meth_8491", 0x8491}, // 0x4B57F0
        {"_meth_8492", 0x8492}, // 0x4EE600
        {"_meth_8493", 0x8493}, // 0x4E2ED0
        {"_meth_8494", 0x8494}, // 0x4B3DC0
        {"_meth_8495", 0x8495}, // 0x4B3CE0
        {"_meth_8497", 0x8497}, // 0x6D6640
        {"_meth_8498", 0x8498}, // 0x6D9760
        {"_meth_849b", 0x849B}, // 0x50BCB0
        {"_meth_849c", 0x849C}, // 0x4B3CD0
        {"_meth_849d", 0x849D}, // 0x6D66C0
        {"_meth_849e", 0x849E}, // 0x6D66B0
        {"_meth_849f", 0x849F}, // 0x4B85D0
        {"_meth_84a0", 0x84A0}, // 0x4B88C0
        {"_meth_84a1", 0x84A1}, // 0x4B8B80
        {"_meth_84a2", 0x84A2}, // 0x4BB520
        {"_meth_84a3", 0x84A3}, // 0x4E2580
        {"_meth_84a6", 0x84A6}, // 0x503CC0
        {"_meth_84a7", 0x84A7}, // 0x503DC0
        {"_meth_84a8", 0x84A8}, // 0x4BC4A0
        {"_meth_84ab", 0x84AB}, // 0x4B2500
        {"_meth_84ac", 0x84AC}, // 0x4B27E0
        {"_meth_84ad", 0x84AD}, // 0x4B3CC0
        {"_meth_84b8", 0x84B8}, // 0x4F46D0
        {"_meth_84b9", 0x84B9}, // 0x4F4950
        {"_meth_84ba", 0x84BA}, // 0x4F5050
        {"_meth_84bb", 0x84BB}, // 0x4F5900
        {"_meth_84bc", 0x84BC}, // 0x6D7960
        {"_meth_84bd", 0x84BD}, // 0x4EA0A0
        {"_meth_84be", 0x84BE}, // 0x4EA5B0
        {"_meth_84bf", 0x84BF}, // 0x4B4AE0
        {"_meth_84c0", 0x84C0}, // 0x6D8170
        {"_meth_84c1", 0x84C1}, // 0x6D8270
        {"_meth_84c3", 0x84C3}, // 0x4EF770
        {"_meth_84c4", 0x84C4}, // 0x4E2650
        {"_meth_84c5", 0x84C5}, // 0x4E33A0
        {"_meth_84c6", 0x84C6}, // 0x4E3890
        {"_meth_84c7", 0x84C7}, // 0x4E3AE0
        {"_meth_84c8", 0x84C8}, // 0x4F0070
        {"_meth_84c9", 0x84C9}, // 0x4F63F0
        {"_meth_84ca", 0x84CA}, // 0x6D72B0
        {"_meth_84cb", 0x84CB}, // 0x4F56B0
        {"_meth_84ce", 0x84CE}, // 0x4B7650
        {"_meth_84cf", 0x84CF}, // 0x4B76C0
        {"_meth_84d2", 0x84D2}, // 0x4B7D90
        {"_meth_84d3", 0x84D3}, // 0x4B8120
        {"_meth_84d5", 0x84D5}, // 0x4F0A60
        {"_meth_84d6", 0x84D6}, // 0x6D45F0
        {"_meth_84d9", 0x84D9}, // 0x4B26B0
        {"_meth_84da", 0x84DA}, // 0x4B25D0
        {"_meth_84db", 0x84DB}, // 0x4B2620
        {"_meth_84dc", 0x84DC}, // 0x6D63B0
        {"_meth_84dd", 0x84DD}, // 0x6D6450
        {"_meth_84df", 0x84DF}, // 0x6D4610
        {"_meth_84e0", 0x84E0}, // 0x4F92D0
        {"_meth_84e1", 0x84E1}, // 0x4F9670
        {"_meth_84e2", 0x84E2}, // 0x4F72B0
        {"_meth_84e3", 0x84E3}, // 0x4F73E0
        {"_meth_84e4", 0x84E4}, // 0x4F74A0
        {"_meth_84e5", 0x84E5}, // 0x4F75A0
        {"_meth_84e6", 0x84E6}, // 0x4F7770
        {"_meth_84e7", 0x84E7}, // 0x4F78F0
        {"_meth_84e8", 0x84E8}, // 0x4F7A00
        {"_meth_84e9", 0x84E9}, // 0x4F7B30
        {"_meth_84eb", 0x84EB}, // 0x4BA420
        {"_meth_84ec", 0x84EC}, // 0x50AFA0
        {"_meth_84ee", 0x84EE}, // 0x499910
        {"_meth_84ef", 0x84EF}, // 0x6D8640
        {"_meth_84f2", 0x84F2}, // 0x4E7290
        {"_meth_84f3", 0x84F3}, // 0x4C1060
        {"_meth_84f4", 0x84F4}, // 0x6D4650
        {"_meth_84f6", 0x84F6}, // 0x4B91D0
        {"_meth_84f7", 0x84F7}, // 0x4E67F0
        {"_meth_84f8", 0x84F8}, // 0x508450
        {"_meth_84f9", 0x84F9}, // 0x4F64C0
        {"_meth_84fa", 0x84FA}, // 0x4B4570
        {"_meth_84fb", 0x84FB}, // 0x4B7B60
        {"_meth_84fc", 0x84FC}, // 0x4B7C50
        {"_meth_84fd", 0x84FD}, // 0x4DF760
        {"_meth_84fe", 0x84FE}, // 0x4EE3A0
        {"hidepartvm", 0x84FF}, // 0x4E2D70, server command name
        {"openluimenu", 0x8500}, // 0x4EB830, ^
        {"_meth_8501", 0x8501}, // 0x4EFDF0
        {"_meth_8502", 0x8502}, // 0x4EDED0
        {"_meth_8503", 0x8503}, // 0x6D57F0
        {"_meth_8504", 0x8504}, // 0x6D5900
        {"_meth_8505", 0x8505}, // 0x4E8490
        {"_meth_8506", 0x8506}, // 0x4E8670
        {"_meth_8507", 0x8507}, // 0x4E76B0
        {"_meth_8508", 0x8508}, // 0x4E7910
        {"_meth_8509", 0x8509}, // 0x4E7A40
        {"_meth_850a", 0x850A}, // 0x4E7B20
        {"_meth_850b", 0x850B}, // 0x4E7C10
        {"_meth_850c", 0x850C}, // 0x4E7F00
        {"_meth_850d", 0x850D}, // 0x4E8140
        {"_meth_850e", 0x850E}, // 0x4F77E0
        {"_meth_850f", 0x850F}, // 0x4E4A00
        {"_meth_8510", 0x8510}, // 0x4E4BC0
        {"_meth_8514", 0x8514}, // 0x4F70C0
        {"_meth_8515", 0x8515}, // 0x4F5980
        {"_meth_8517", 0x8517}, // 0x4E8C70
        {"_meth_851d", 0x851D}, // 0x503580
        {"_meth_851e", 0x851E}, // 0x4B8E00
        {"_meth_851f", 0x851F}, // 0x4C0A20
        {"_meth_8521", 0x8521}, // 0x4F6F10
        {"_meth_8522", 0x8522}, // 0x4F7030
        {"_meth_8523", 0x8523}, // 0x4ECCA0
        {"_meth_8524", 0x8524}, // 0x4B2690
        {"_meth_8525", 0x8525}, // 0x4E4D60
        {"_meth_8526", 0x8526}, // 0x6D4680
        {"_meth_8527", 0x8527}, // 0x6D46A0
        {"_meth_8528", 0x8528}, // 0x4B3040
        {"_meth_8529", 0x8529}, // 0x4B3050
        {"_meth_852a", 0x852A}, // 0x4EC1D0
        {"_meth_852c", 0x852C}, // 0x4C23F0
        {"_meth_852e", 0x852E}, // 0x4B3260
        {"_meth_852f", 0x852F}, // 0x4F7C40
        {"_meth_8530", 0x8530}, // 0x4F7230
        {"_meth_8531", 0x8531}, // 0x50B020
        {"_meth_8532", 0x8532}, // 0x4DF400
        {"_meth_8535", 0x8535}, // 0x4B7800, vision
        {"_meth_8537", 0x8537}, // 0x4F5E90
        {"_meth_8538", 0x8538}, // 0x4F61A0
        {"_meth_853b", 0x853B}, // 0x4C0A90
        {"_meth_853d", 0x853D}, // 0x4E9090
        {"_meth_853e", 0x853E}, // 0x4EAFF0
        {"_meth_8541", 0x8541}, // 0x4E8210
        {"_meth_8550", 0x8550}, // 0x4E8680
        {"_meth_8551", 0x8551}, // 0x4B3CB0
        {"_meth_8554", 0x8554}, // 0x4F1E10
        {"_meth_8555", 0x8555}, // 0x4E8890
        {"_meth_855c", 0x855C}, // 0x4B50C0
        {"_meth_8561", 0x8561}, // 0x4F8E40
        {"_meth_8564", 0x8564}, // 0x50B2C0
        {"_meth_8565", 0x8565}, // 0x4EB8D0
        {"_meth_856d", 0x856D}, // 0x4B4C20
        {"_meth_856e", 0x856E}, // 0x4B4C30
        {"_meth_856f", 0x856F}, // 0x4B4C40
        {"_meth_8571", 0x8571}, // 0x4FA2B0
        {"_meth_8572", 0x8572}, // 0x4E50F0
        {"_meth_8573", 0x8573}, // 0x4F8180
        {"_meth_8574", 0x8574}, // 0x4F8570
        {"_meth_8575", 0x8575}, // 0x4EB4F0
        {"_meth_8576", 0x8576}, // 0x4EB610
        {"_meth_8577", 0x8577}, // 0x50BB10
        {"_meth_8578", 0x8578}, // 0x524AD0
        {"_meth_8579", 0x8579}, // 0x498A00
        {"_meth_857a", 0x857A}, // 0x4FF940
        {"_meth_857b", 0x857B}, // 0x49A0B0
        {"_meth_857c", 0x857C}, // 0x4E3FD0
        {"_meth_857d", 0x857D}, // 0x4EA9F0
        {"_meth_857e", 0x857E}, // 0x4EAB80
        {"_meth_857f", 0x857F}, // 0x4E9100
        {"_meth_8580", 0x8580}, // 0x4E93B0
        {"_meth_8581", 0x8581}, // 0x4E9750
        {"_meth_8582", 0x8582}, // 0x4DF820
        {"_meth_8583", 0x8583}, // 0x4DF840
        {"_meth_8584", 0x8584}, // 0x4DF8C0
        {"_meth_8585", 0x8585}, // 0x49A0F0
        {"_meth_8586", 0x8586}, // 0x49C290
        {"_meth_8587", 0x8587}, // 0x4B2BF0
        {"_meth_8588", 0x8588}, // 0x4B8F80
        {"_meth_8589", 0x8589}, // 0x4B9270
        {"_meth_858c", 0x858C}, // 0x4EAB30
        {"_meth_858d", 0x858D}, // 0x4EB6F0
        {"_meth_8590", 0x8590}, // 0x4F67A0
        {"_meth_8595", 0x8595}, // 0x4B3F60
        {"_meth_8596", 0x8596}, // 0x4BBD50
        {"_meth_8597", 0x8597}, // 0x6D7B40
        {"_meth_8598", 0x8598}, // 0x4DF9E0
        {"_meth_8599", 0x8599}, // 0x4FF6C0
        {"_meth_859a", 0x859A}, // 0x4B46E0
        {"_meth_859b", 0x859B}, // 0x4BC500
        {"_meth_859c", 0x859C}, // 0x4B4E40
        {"_meth_859d", 0x859D}, // 0x49C600
        {"_meth_859e", 0x859E}, // 0x6D66F0
        {"_meth_859f", 0x859F}, // 0x503610
        {"_meth_85a0", 0x85A0}, // 0x503710
        {"_meth_85a1", 0x85A1}, // 0x6D5B10
        {"_meth_85a2", 0x85A2}, // 0x6D6760
        {"_meth_85a3", 0x85A3}, // 0x4B5040
        {"_meth_85a4", 0x85A4}, // 0x4B4F70
        {"_meth_85a5", 0x85A5}, // 0x6D6B80
        {"_meth_85a6", 0x85A6}, // 0x5082B0
        {"_meth_85a7", 0x85A7}, // 0x5083E0
        {"_meth_85a8", 0x85A8}, // 0x4E2CA0
        {"_meth_85a9", 0x85A9}, // 0x503780
        {"_meth_85aa", 0x85AA}, // 0x4E7930
        {"_meth_85ab", 0x85AB}, // 0x4DF920
        {"_meth_85ac", 0x85AC}, // 0x4E4A50
        {"_meth_85b0", 0x85B0}, // 0x4E4100
        {"_meth_85b1", 0x85B1}, // 0x4F20A0
        {"_meth_85b2", 0x85B2}, // 0x4EAEE0
        {"_meth_85b3", 0x85B3}, // 0x4F6200
        {"_meth_85b4", 0x85B4}, // 0x4F6CF0
        {"_meth_85b5", 0x85B5}, // 0x4FF5C0
        {"_meth_85b6", 0x85B6}, // 0x4B8070
        {"_meth_85b7", 0x85B7}, // 0x4B8230
        {"_meth_85b8", 0x85B8}, // 0x4B73F0
        {"_meth_85b9", 0x85B9}, // 0x4B7510
        {"_meth_85ba", 0x85BA}, // 0x4B75E0
        {"_meth_85bb", 0x85BB}, // 0x4B5740
        {"_meth_85bc", 0x85BC}, // 0x4EB3E0
        {"_meth_85bd", 0x85BD}, // 0x6D6870
        {"_meth_85be", 0x85BE}, // 0x5038B0
        {"_meth_85bf", 0x85BF}, // 0x6D67E0
        {"_meth_85c0", 0x85C0}, // 0x6D6C00
        {"_meth_85c1", 0x85C1}, // 0x6D6920
        {"_meth_85c2", 0x85C2}, // 0x4DF7A0
        {"_meth_85c3", 0x85C3}, // 0x6D6DF0
        {"_meth_85c4", 0x85C4}, // 0x4F8ED0
        {"_meth_85c5", 0x85C5}, // 0x6D69A0
        {"_meth_85c6", 0x85C6}, // 0x6D6A10
        {"_meth_85c7", 0x85C7}, // 0x6D6A90
        {"_meth_85c8", 0x85C8}, // 0x6D6B00
        {"_meth_85c9", 0x85C9}, // 0x4DF7E0
        {"_meth_85ca", 0x85CA}, // 0x4DF890
        {"_meth_85cb", 0x85CB}, // 0x4DF980
        {"_meth_85cc", 0x85CC}, // 0x4B6250
        {"_meth_85cd", 0x85CD}, // 0x4E9350
        {"_meth_85ce", 0x85CE}, // 0x4B4C50
        {"_meth_85cf", 0x85CF}, // 0x4B4EA0
        {"_meth_85d0", 0x85D0}, // 0x4B37F0
        {"_meth_85d1", 0x85D1}, // 0x4F8B30
        {"_meth_85d2", 0x85D2}, // 0x4E97B0
    };

    std::unordered_map<std::string, unsigned> token_map =
    {
        {"maps/_utility", 42407},
        {"common_scripts/utility", 42237},
        {"maps/_load", 42323},
        {"maps/_compass", 42272},
        {"maps/_spawner", 42372},
        {"maps/_vehicle", 42411},
        {"maps/_attack_heli", 42508},
        {"maps/_slowmo_breach", 42367},

        {"init", 521},
        {"main", 616},
        {"player", 794},
        {"default_start", 10126},
        {"setupminimap", 33575},
        {"set_player_viewhand_model", 32417},

        // built-in entity fields
        {"code_classname", 172},
        {"classname", 170},
        {"model", 669},
        {"count", 216},
        {"health", 486},
        {"dmg", 293},
        {"maxhealth", 626},
        {"anglelerprate", 64},
        {"activator", 19},
        {"slidevelocity", 974},
        {"disableplayeradsloscheck", 291},
        {"accuracy", 10},
        {"lookforward", 604},
        {"lookright", 605},
        {"lookup", 607},
        {"fovcosine", 411},
        {"fovcosinebusy", 412},
        {"fovcosinez", 413},
        {"upaimlimit", 1252},
        {"downaimlimit", 307},
        {"rightaimlimit", 894},
        {"leftaimlimit", 590},
        {"maxsightdistsqrd", 628},
        {"sightlatency", 967},
        {"defaultsightlatency", 968},
        {"ignoreclosefoliage", 508},
        {"interval", 525},
        {"teammovewaittime", 1199},
        {"damagetaken", 257},
        {"damagedir", 252},
        {"damageyaw", 259},
        {"damagelocation", 253},
        {"damageweapon", 258},
        {"damagemod", 254},
        {"proneok", 841},
        {"walkdistfacingmotion", 1299},
        {"walkdist", 1298},
        {"desiredangle", 278},
        {"pacifist", 744},
        {"pacifistwait", 745},
        {"footstepdetectdist", 398},
        {"footstepdetectdistwalk", 400},
        {"footstepdetectdistsprint", 399},
        {"reactiontargetpos", 859},
        {"newenemyreactiondistsq", 686},
        {"ignoreexplosionevents", 509},
        {"ignoresuppression", 513},
        {"suppressionwait", 1060},
        {"suppressionduration", 1056},
        {"suppressionstarttime", 1058},
        {"suppressionmeter", 1057},
        {"ignoreplayersuppression", 514},
        {"name", 680},
        {"weapon", 1302},
        {"dontavoidplayer", 304},
        {"grenadeawareness", 465},
        {"grenade", 458},
        {"grenadeweapon", 470},
        {"grenadeammo", 464},
        {"grenadetargetpos", 467},
        {"grenadetargetvalid", 468},
        {"grenadetossvel", 469},
        {"favoriteenemy", 377},
        {"highlyawareradius", 495},
        {"minpaindamage", 642},
        {"allowpain", 52},
        {"allowdeath", 49},
        {"delayeddeath", 274},
        {"diequietly", 287},
        {"forceragdollimmediate", 405},
        {"providecoveringfire", 842},
        {"doingambush", 302},
        {"combatmode", 199},
        {"alertlevel", 38},
        {"alertlevelint", 39},
        {"useable", 1257},
        {"ignoretriggers", 515},
        {"pushable", 846},
        {"script_pushable", 926},
        {"dropweapon", 309},
        {"drawoncompass", 308},
        {"groundtype", 474},
        {"anim_pose", 68},
        {"goalradius", 452},
        {"goalheight", 450},
        {"goalpos", 451},
        {"nodeoffsetpos", 705},
        {"ignoreforfixednodesafecheck", 510},
        {"fixednode", 381},
        {"fixednodesaferadius", 382},
        {"pathgoalpos", 762},
        {"pathrandompercent", 764},
        {"usechokepoints", 1258},
        {"stopanimdistsq", 1044},
        {"lastenemysightpos", 584},
        {"pathenemylookahead", 761},
        {"pathenemyfightdist", 760},
        {"meleeattackdist", 633},
        {"movemode", 675},
        {"script_move_distance_override", 31298},
        {"usecombatscriptatcover", 1259},
        {"safetochangescript", 906},
        {"keepclaimednode", 561},
        {"keepclaimednodeifvalid", 562},
        {"keepnodeduringscriptedanim", 563},
        {"dodangerreact", 295},
        {"dangerreactduration", 260},
        {"nododgemove", 706},
        {"noteammove", 707},
        {"leanamount", 587},
        {"pitchamount", 788},
        {"turnrate", 1230},
        {"turnanimactive", 1229},
        {"badplaceawareness", 107},
        {"damageshield", 256},
        {"nogrenadereturnthrow", 709},
        {"noattackeraccuracymod", 698},
        {"frontshieldanglecos", 426},
        {"lookaheaddir", 601},
        {"lookaheaddist", 602},
        {"lookaheadhitsstairs", 603},
        {"velocity", 1283},
        {"prevanimdelta", 821},
        {"exposedduration", 356},
        {"requestarrivalnotify", 875},
        {"scriptedarrivalent", 938},
        {"goingtoruntopos", 455},
        {"engagemindist", 334},
        {"engageminfalloffdist", 335},
        {"engagemaxdist", 332},
        {"engagemaxfalloffdist", 333},
        {"usingcovermoveup", 42987},
        {"finalaccuracy", 378},
        {"facemotion", 373},
        {"gunblockedbywall", 475},
        {"relativedir", 866},
        {"lockorientation", 597},
        {"maxfaceenemydist", 625},
        {"stairsstate", 1012},
        {"script", 912},
        {"prevscript", 823},
        {"headicon", 483},
        {"headiconteam", 484},
        {"coversearchinterval", 219},
        {"threatupdateinterval", 37018},
        {"canclimbladders", 150},
        {"swimmer", 1063},
        {"space", 986},
        {"doghandler", 301},
        {"sharpturnlookaheaddist", 961},
        {"postsharpturnlookaheaddist", 813},
        {"sharpturntooclosetodestdist", 963},
        {"usepathsmoothingvalues", 1262},
        {"pathlookaheaddist", 763},
        {"maxturnspeed", 629},
        {"sharpturn", 960},
        {"disablesightandthreatupdate", 54743},
        {"team", 1194},
        {"threatbias", 1204},
        {"threatbiasgroup", 1205},
        {"node", 700},
        {"prevnode", 822},
        {"enemy", 322},
        {"syncedmeleetarget", 1065},
        {"lastattacker", 583},
        {"lastpusher", 42997},
        {"ignoreme", 511},
        {"ignoreall", 507},
        {"maxvisibledist", 630},
        {"surprisedbymedistsq", 1062},
        {"attackeraccuracy", 86},
        {"ignorerandombulletdamage", 512},
        {"dodamagetoall", 294},
        {"turretinvulnerability", 1240},
        {"useorcaavoidance", 1261},
        {"reciprocality", 863},
        {"avoidanceboundshalfsize", 94},
        {"onlygoodnearestnodes", 735},
        {"playername", 803},
        {"deathinvulnerabletime", 266},
        {"criticalbulletdamagedist", 222},
        {"attackercount", 87},
        {"damagemultiplier", 255},
        {"laststand", 586},
        {"motiontrackerenabled", 672},
        {"veh_speed", 1276},
        {"veh_pathspeed", 1273},
        {"veh_transmission", 1279},
        {"veh_pathdir", 1272},
        {"veh_pathtype", 1274},
        {"veh_topspeed", 1278},
        {"veh_brake", 1266},
        {"veh_throttle", 1277},
        {"x", 1331},
        {"y", 1339},
        {"z", 1342},
        {"fontscale", 393},
        {"font", 392},
        {"alignx", 44},
        {"aligny", 45},
        {"horzalign", 499},
        {"vertalign", 1284},
        {"color", 196},
        {"alpha", 55},
        {"label", 578},
        {"sort", 983},
        {"foreground", 408},
        {"lowresbackground", 612},
        {"hidewhendead", 491},
        {"hidewheninmenu", 493},
        {"glowcolor", 445},
        {"glowalpha", 444},
        {"positioninworld", 812},
        {"relativeoffset", 867},
        {"enablehudlighting", 315},
        {"enableinputprogressicon", 42996},
        {"rotation", 902},
        {"targetname", 1193},
        {"target", 1191},
        {"animscript", 71},
        {"script_linkname", 920},
        {"script_noteworthy", 922},
        {"origin", 740},
        {"angles", 65},
        {"minusedistsq", 643},
        {"parentname", 749},
        {"spawnflags", 989},
        {"type", 1244},
        {"owner", 743},
        {"radius", 851},
        {"customangles", 9555},
        {"speed", 997},
        {"lookahead", 600},
        {"script_vehicle_anim", 40318},
    };
}
