#pragma once
#include <d3d11.h>

namespace game
{
	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

	struct gclient_s
	{
		char __pad0[140];
		vec3_t velocity;
		char __pad1[112];
		vec3_t angles;
		char __pad2[59380];
		char flags;
	};

	static_assert(sizeof(gclient_s) == 59660);
	static_assert(offsetof(gclient_s, flags) == 59656);
	static_assert(offsetof(gclient_s, velocity) == 140);
	static_assert(offsetof(gclient_s, angles) == 264);

	struct client_t
	{
		char __pad0[13508];
	};

	enum $219904913BC1E6DB920C78C8CC0BD8F1
	{
		FL_GODMODE = 0x1,
		FL_DEMI_GODMODE = 0x2,
		FL_NOTARGET = 0x4,
		FL_NO_KNOCKBACK = 0x8,
		FL_NO_RADIUS_DAMAGE = 0x10,
		FL_SUPPORTS_LINKTO = 0x20,
		FL_NO_AUTO_ANIM_UPDATE = 0x40,
		FL_GRENADE_TOUCH_DAMAGE = 0x80,
		FL_STABLE_MISSILES = 0x100,
		FL_REPEAT_ANIM_UPDATE = 0x200,
		FL_VEHICLE_TARGET = 0x400,
		FL_GROUND_ENT = 0x800,
		FL_CURSOR_HINT = 0x1000,
		FL_MISSILE_ATTRACTOR_OR_REPULSOR = 0x2000,
		FL_WEAPON_BEING_GRABBED = 0x4000,
		FL_DELETE = 0x8000,
		FL_BOUNCE = 0x10000,
		FL_MOVER_SLIDE = 0x20000,
		FL_MOVING = 0x40000,
		FL_DONT_AUTOBOLT_MISSILE_EFFECTS = 0x80000,
		FL_DISABLE_MISSILE_STICK = 0x100000,
		FL_NO_MELEE_TARGET = 0x2000000,
		FL_DYNAMICPATH = 0x8000000,
		FL_AUTO_BLOCKPATHS = 0x10000000,
		FL_OBSTACLE = 0x20000000,
		FL_BADPLACE_VOLUME = 0x80000000,
	};

	enum
	{
		PMF_TIME_HARDLANDING = 1 << 7,
		PMF_TIME_KNOCKBACK = 1 << 8,
	};

	struct EntityState
	{
		uint16_t entityNum;
	};

	enum scr_string_t
	{
		scr_string_t_dummy = 0x0,
	};

	struct Bounds
	{
		vec3_t midPoint;
		vec3_t halfSize;
	};

	struct gentity_s
	{
		char __pad0[26];
		vec3_t origin;
		char __pad1[100];
		EntityState s;
		char __pad2[50];
		Bounds box;
		char __pad3[4];
		Bounds absBox;
		char __pad4[36];
		gclient_s* client;
		char __pad5[48];
		scr_string_t script_classname;
		char __pad6[24];
		char flags;
		char __pad7[395];
	}; // size = 760

	//auto a = sizeof(gentity_s);

	static_assert(sizeof(gentity_s) == 760);
	static_assert(offsetof(gentity_s, origin) == 28);
	static_assert(offsetof(gentity_s, box) == 192);
	static_assert(offsetof(gentity_s, absBox) == 220);
	static_assert(offsetof(gentity_s, client) == 280);
	static_assert(offsetof(gentity_s, script_classname) == 336);
	static_assert(offsetof(gentity_s, flags) == 364);
	static_assert(offsetof(gentity_s, s) == 140);

	struct pathnode_yaworient_t
	{
		float fLocalAngle;
		float localForward[2];
	};

	union $3936EE84564F75EDA6DCBAC77A545FC8
	{
		pathnode_yaworient_t yaw_orient;
		float angles[3];
	};

	union PathNodeParentUnion
	{
		scr_string_t name;
		unsigned short index;
	};

	enum nodeType
	{
		NODE_ERROR = 0x0,
		NODE_PATHNODE = 0x1,
		NODE_NEGOTIATION_BEGIN = 0x13,
		NODE_NEGOTIATION_END = 0x14
	};

	enum PathNodeErrorCode : std::int32_t
	{
		PNERR_NONE = 0x0,
		PNERR_INSOLID = 0x1,
		PNERR_FLOATING = 0x2,
		PNERR_NOLINK = 0x3,
		PNERR_DUPLICATE = 0x4,
		PNERR_NOSTANCE = 0x5,
		PNERR_INVALIDDOOR = 0x6,
		PNERR_NOANGLES = 0x7,
		PNERR_BADPLACEMENT = 0x8,
		NUM_PATH_NODE_ERRORS = 0x9,
	};

	union $5F11B9753862CE791E23553F99FA1738
	{
		float minUseDistSq;
		PathNodeErrorCode error;
	};

	struct pathlink_s
	{
		float fDist;
		unsigned short nodeNum;
		unsigned char disconnectCount;
		unsigned char negotiationLink;
		unsigned char flags;
		unsigned char ubBadPlaceCount[3];
	};

	struct pathnode_constant_t
	{
		unsigned short type;
		unsigned int spawnflags;
		scr_string_t targetname;
		scr_string_t script_linkName;
		scr_string_t script_noteworthy;
		scr_string_t target;
		scr_string_t animscript;
		int animscriptfunc;
		float vLocalOrigin[3];
		$3936EE84564F75EDA6DCBAC77A545FC8 ___u9;
		PathNodeParentUnion parent;
		$5F11B9753862CE791E23553F99FA1738 ___u11;
		short wOverlapNode[2];
		char __pad0[4];
		unsigned short totalLinkCount;
		pathlink_s* Links;
		scr_string_t unk;
		char __pad1[4];
	};

	struct SentientHandle
	{
		unsigned short number;
		unsigned short infoIndex;
	};

	struct pathnode_dynamic_t
	{
		SentientHandle pOwner;
		int iFreeTime;
		int iValidTime[3];
		short wLinkCount;
		short wOverlapCount;
		short turretEntNumber;
		unsigned char userCount;
		unsigned char hasBadPlaceLink;
		int spreadUsedTime[2];
		short flags;
		short dangerousCount;
		int recentUseProxTime;
	};

	union $73F238679C0419BE2C31C6559E8604FC
	{
		float nodeCost;
		int linkIndex;
	};

	struct pathnode_t;
	struct pathnode_transient_t
	{
		int iSearchFrame;
		pathnode_t* pNextOpen;
		pathnode_t* pPrevOpen;
		pathnode_t* pParent;
		float fCost;
		float fHeuristic;
		$73F238679C0419BE2C31C6559E8604FC ___u6;
	};

	struct pathnode_t
	{
		pathnode_constant_t constant;
		pathnode_dynamic_t dynamic;
		pathnode_transient_t transient;
	};

	struct pathnode_tree_nodes_t
	{
		int nodeCount;
		unsigned short* nodes;
	};

	struct pathnode_tree_t;
	union pathnode_tree_info_t
	{
		pathnode_tree_t* child[2];
		pathnode_tree_nodes_t s;
	};

	struct pathnode_tree_t
	{
		int axis;
		float dist;
		pathnode_tree_info_t u;
	};

	struct PathDynamicNodeGroup
	{
		unsigned short parentIndex;
		int nodeTreeCount;
		pathnode_tree_t* nodeTree;
	};

	struct PathData
	{
		const char* name;
		unsigned int nodeCount;
		pathnode_t* nodes;
		bool parentIndexResolved;
		unsigned short version;
		int visBytes;
		unsigned char* pathVis;
		int nodeTreeCount;
		pathnode_tree_t* nodeTree;
		int dynamicNodeGroupCount;
		PathDynamicNodeGroup* dynamicNodeGroups;
		int exposureBytes;
		unsigned char* pathExposure;
		int noPeekVisBytes;
		unsigned char* pathNoPeekVis;
		int zoneCount;
		int zonesBytes;
		unsigned char* pathZones;
		int dynStatesBytes;
		unsigned char* pathDynStates;
	};

	struct GfxImage;

	union MaterialTextureDefInfo
	{
		GfxImage* image;
		void* water;
	};

	struct MaterialTextureDef
	{
		unsigned int nameHash;
		char nameStart;
		char nameEnd;
		char samplerState;
		char semantic;
		MaterialTextureDefInfo u;
	};

	struct MaterialPass
	{
		void* vertexShader;
		void* vertexDecl;
		void* hullShader;
		void* domainShader;
		void* pixelShader;
		char pixelOutputMask;
		char perPrimArgCount;
		char perObjArgCount;
		char stableArgCount;
		unsigned __int16 perPrimArgSize;
		unsigned __int16 perObjArgSize;
		unsigned __int16 stableArgSize;
		char zone;
		char perPrimConstantBuffer;
		char perObjConstantBuffer;
		char stableConstantBuffer;
		unsigned int customBufferFlags;
		char customSamplerFlags;
		char precompiledIndex;
		char stageConfig;
		void* args;
	};

	struct MaterialTechnique
	{
		const char* name;
		unsigned __int16 flags;
		unsigned __int16 passCount;
		MaterialPass passArray[1];
	};

	struct MaterialTechniqueSet
	{
		const char* name;
		unsigned __int16 flags;
		char worldVertFormat;
		char preDisplacementOnlyCount;
		MaterialTechnique* techniques[309];
	};

	struct GfxStateBits
	{
		unsigned int loadBits[3];
		char zone;
		char depthStencilState[11];
		char blendState;
		char rasterizerState;
	};

	struct Material
	{
		const char* name;
		char __pad0[0x124];
		char textureCount;
		char __pad1[0xB];
		MaterialTechniqueSet* techniqueSet;
		MaterialTextureDef* textureTable;
		void* constantTable;
		GfxStateBits* stateBitsTable;
		char __pad2[0x118];
	};

	static_assert(sizeof(Material) == 0x270);

	struct point
	{
		float x;
		float y;
		float f2;
		float f3;
	};

	struct rectangle
	{
		point p0;
		point p1;
		point p2;
		point p3;
	};

	struct rgba
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};

	struct Glyph
	{
		unsigned short letter;
		char x0;
		char y0;
		char dx;
		char pixelWidth;
		char pixelHeight;
		float s0;
		float t0;
		float s1;
		float t1;
	};

	struct Font_s
	{
		const char* fontName;
		int pixelHeight;
		int glyphCount;
		Material* material;
		Material* glowMaterial;
		Glyph* glyphs;
	};

	enum keyNum_t
	{
		K_NONE = 0x0,
		K_FIRSTGAMEPADBUTTON_RANGE_1 = 0x1,
		K_BUTTON_A = 0x1,
		K_BUTTON_B = 0x2,
		K_BUTTON_X = 0x3,
		K_BUTTON_Y = 0x4,
		K_BUTTON_LSHLDR = 0x5,
		K_BUTTON_RSHLDR = 0x6,
		K_LASTGAMEPADBUTTON_RANGE_1 = 0x6,
		K_BS = 0x8,
		K_TAB = 0x9,
		K_ENTER = 0xD,
		K_FIRSTGAMEPADBUTTON_RANGE_2 = 0xE,
		K_BUTTON_START = 0xE,
		K_BUTTON_BACK = 0xF,
		K_BUTTON_LSTICK = 0x10,
		K_BUTTON_RSTICK = 0x11,
		K_BUTTON_LTRIG = 0x12,
		K_BUTTON_RTRIG = 0x13,
		K_DPAD_UP = 0x14,
		K_FIRSTDPAD = 0x14,
		K_DPAD_DOWN = 0x15,
		K_DPAD_LEFT = 0x16,
		K_DPAD_RIGHT = 0x17,
		K_BUTTON_LSTICK_ALTIMAGE2 = 0x10,
		K_BUTTON_RSTICK_ALTIMAGE2 = 0x11,
		K_BUTTON_LSTICK_ALTIMAGE = 0xBC,
		K_BUTTON_RSTICK_ALTIMAGE = 0xBD,
		K_LASTDPAD = 0x17,
		K_LASTGAMEPADBUTTON_RANGE_2 = 0x17,
		K_ESCAPE = 0x1B,
		K_FIRSTGAMEPADBUTTON_RANGE_3 = 0x1C,
		K_APAD_UP = 0x1C,
		K_FIRSTAPAD = 0x1C,
		K_APAD_DOWN = 0x1D,
		K_APAD_LEFT = 0x1E,
		K_APAD_RIGHT = 0x1F,
		K_LASTAPAD = 0x1F,
		K_LASTGAMEPADBUTTON_RANGE_3 = 0x1F,
		K_SPACE = 0x20,
		K_GRAVE = 0x60,
		K_TILDE = 0x7E,
		K_BACKSPACE = 0x7F,
		K_ASCII_FIRST = 0x80,
		K_ASCII_181 = 0x80,
		K_ASCII_191 = 0x81,
		K_ASCII_223 = 0x82,
		K_ASCII_224 = 0x83,
		K_ASCII_225 = 0x84,
		K_ASCII_228 = 0x85,
		K_ASCII_229 = 0x86,
		K_ASCII_230 = 0x87,
		K_ASCII_231 = 0x88,
		K_ASCII_232 = 0x89,
		K_ASCII_233 = 0x8A,
		K_ASCII_236 = 0x8B,
		K_ASCII_241 = 0x8C,
		K_ASCII_242 = 0x8D,
		K_ASCII_243 = 0x8E,
		K_ASCII_246 = 0x8F,
		K_ASCII_248 = 0x90,
		K_ASCII_249 = 0x91,
		K_ASCII_250 = 0x92,
		K_ASCII_252 = 0x93,
		K_END_ASCII_CHARS = 0x94,
		K_COMMAND = 0x96,
		K_CAPSLOCK = 0x97,
		K_POWER = 0x98,
		K_PAUSE = 0x99,
		K_UPARROW = 0x9A,
		K_DOWNARROW = 0x9B,
		K_LEFTARROW = 0x9C,
		K_RIGHTARROW = 0x9D,
		K_ALT = 0x9E,
		K_CTRL = 0x9F,
		K_SHIFT = 0xA0,
		K_INS = 0xA1,
		K_DEL = 0xA2,
		K_PGDN = 0xA3,
		K_PGUP = 0xA4,
		K_HOME = 0xA5,
		K_END = 0xA6,
		K_F1 = 0xA7,
		K_F2 = 0xA8,
		K_F3 = 0xA9,
		K_F4 = 0xAA,
		K_F5 = 0xAB,
		K_F6 = 0xAC,
		K_F7 = 0xAD,
		K_F8 = 0xAE,
		K_F9 = 0xAF,
		K_F10 = 0xB0,
		K_F11 = 0xB1,
		K_F12 = 0xB2,
		K_F13 = 0xB3,
		K_F14 = 0xB4,
		K_F15 = 0xB5,
		K_KP_HOME = 0xB6,
		K_KP_UPARROW = 0xB7,
		K_KP_PGUP = 0xB8,
		K_KP_LEFTARROW = 0xB9,
		K_KP_5 = 0xBA,
		K_KP_RIGHTARROW = 0xBB,
		K_KP_END = 0xBC,
		K_KP_DOWNARROW = 0xBD,
		K_KP_PGDN = 0xBE,
		K_KP_ENTER = 0xBF,
		K_KP_INS = 0xC0,
		K_KP_DEL = 0xC1,
		K_KP_SLASH = 0xC2,
		K_KP_MINUS = 0xC3,
		K_KP_PLUS = 0xC4,
		K_KP_NUMLOCK = 0xC5,
		K_KP_STAR = 0xC6,
		K_KP_EQUALS = 0xC7,
		K_MOUSE1 = 0xC8,
		K_MOUSE2 = 0xC9,
		K_MOUSE3 = 0xCA,
		K_MOUSE4 = 0xCB,
		K_MOUSE5 = 0xCC,
		K_MWHEELDOWN = 0xCD,
		K_MWHEELUP = 0xCE,
		K_AUX1 = 0xCF,
		K_AUX2 = 0xD0,
		K_AUX3 = 0xD1,
		K_AUX4 = 0xD2,
		K_AUX5 = 0xD3,
		K_AUX6 = 0xD4,
		K_AUX7 = 0xD5,
		K_AUX8 = 0xD6,
		K_AUX9 = 0xD7,
		K_AUX10 = 0xD8,
		K_AUX11 = 0xD9,
		K_AUX12 = 0xDA,
		K_AUX13 = 0xDB,
		K_AUX14 = 0xDC,
		K_AUX15 = 0xDD,
		K_AUX16 = 0xDE,
		K_LAST_KEY = 0xDF
	};

	enum errorParm
	{
		ERR_FATAL = 0,
		ERR_DROP = 1,
		ERR_SERVERDISCONNECT = 2,
		ERR_DISCONNECT = 3,
		ERR_SCRIPT = 4,
		ERR_SCRIPT_DROP = 5,
		ERR_LOCALIZATION = 6,
		ERR_MAPLOADERRORSUMMARY = 7,
	};

	enum threadType
	{
		THREAD_CONTEXT_MAIN = 0x0,
		THREAD_CONTEXT_BACKEND = 0x1,
		THREAD_CONTEXT_WORKER0 = 0x2,
		THREAD_CONTEXT_WORKER1 = 0x3,
		THREAD_CONTEXT_WORKER2 = 0x4,
		THREAD_CONTEXT_WORKER3 = 0x5,
		THREAD_CONTEXT_WORKER4 = 0x6,
		THREAD_CONTEXT_WORKER5 = 0x7,
		THREAD_CONTEXT_WORKER6 = 0x8,
		THREAD_CONTEXT_WORKER7 = 0x9,
		THREAD_CONTEXT_SERVER = 0xA,
		THREAD_CONTEXT_TRACE_COUNT = 0xB,
		THREAD_CONTEXT_TRACE_LAST = 0xA,
		THREAD_CONTEXT_CINEMATIC = 0xB,
		THREAD_CONTEXT_WINDOW = 0xC,
		THREAD_CONTEXT_INPUT = 0xD,
		THREAD_CONTEXT_DATABASE = 0xE,
		THREAD_CONTEXT_STREAM = 0xF,
		THREAD_CONTEXT_UNK_16 = 0x10,
		THREAD_CONTEXT_UNK_17 = 0x11,
		THREAD_CONTEXT_UNK_18 = 0x12,
		THREAD_COUNT,
	};

	struct KeyState
	{
		int down;
		int repeats;
		int binding;
	};

	struct PlayerKeyState
	{
		int overstrikeMode;
		int anyKeyDown;
		KeyState keys[256];
	};

	enum DvarSetSource : std::uint32_t
	{
		DVAR_SOURCE_INTERNAL = 0x0,
		DVAR_SOURCE_EXTERNAL = 0x1,
		DVAR_SOURCE_SCRIPT = 0x2,
		DVAR_SOURCE_UISCRIPT = 0x3,
		DVAR_SOURCE_SERVERCMD = 0x4,
		DVAR_SOURCE_NUM = 0x5,
	};

	enum DvarFlags : std::uint32_t
	{
		DVAR_FLAG_NONE = 0,
		DVAR_FLAG_SAVED = 0x1,
		DVAR_FLAG_LATCHED = 0x2,
		DVAR_FLAG_CHEAT = 0x4,
		DVAR_FLAG_REPLICATED = 0x8,
		DVAR_FLAG_WRITE = 0x800,
		DVAR_FLAG_READ = 0x2000,
	};

	enum dvar_type : std::int8_t
	{
		boolean = 0,
		value = 1,
		vec2 = 2,
		vec3 = 3,
		vec4 = 4,
		integer = 5,
		enumeration = 6,
		string = 7,
		color = 8,
		rgb = 9 // Color without alpha
	};

	union dvar_value
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		float value;
		float vector[4];
		const char* string;
		char color[4];
	};

	struct $A37BA207B3DDD6345C554D4661813EDD
	{
		int stringCount;
		const char* const* strings;
	};

	struct $9CA192F9DB66A3CB7E01DE78A0DEA53D
	{
		int min;
		int max;
	};

	struct $251C2428A496074035CACA7AAF3D55BD
	{
		float min;
		float max;
	};

	union dvar_limits
	{
		$A37BA207B3DDD6345C554D4661813EDD enumeration;
		$9CA192F9DB66A3CB7E01DE78A0DEA53D integer;
		$251C2428A496074035CACA7AAF3D55BD value;
		$251C2428A496074035CACA7AAF3D55BD vector;
	};

	struct dvar_t
	{
		union
		{
			int name;
			int hash;
		};
		unsigned int flags; //08
		dvar_type type; //0C
		bool modified; //0D
		dvar_value current; //10
		dvar_value latched;
		dvar_value reset;
		dvar_limits domain;
		char __pad0[0xC];
	};

	struct ScreenPlacement
	{
		vec2_t scaleVirtualToReal;
		vec2_t scaleVirtualToFull;
		vec2_t scaleRealToVirtual;
		vec2_t realViewportPosition;
		vec2_t realViewportSize;
		vec2_t virtualViewableMin;
		vec2_t virtualViewableMax;
		vec2_t realViewableMin;
		vec2_t realViewableMax;
		vec2_t virtualAdjustableMin;
		vec2_t virtualAdjustableMax;
		vec2_t realAdjustableMin;
		vec2_t realAdjustableMax;
		vec2_t subScreenLeft;
	};

	struct refdef_t
	{
		char __pad0[0x10];
		float fovX;
		float fovY;
		char __pad1[0x8];
		float org[3];
		float axis[3][3];
	};

	struct CmdArgs
	{
		int nesting;
		int localClientNum[8];
		int controllerIndex[8];
		int argc[8];
		const char** argv[8];
	};

	struct cmd_function_s
	{
		cmd_function_s* next;
		const char* name;
		void(__cdecl* function)();
	};

	enum XAssetType
	{
		ASSET_TYPE_PHYSPRESET,
		ASSET_TYPE_PHYS_COLLMAP,
		ASSET_TYPE_PHYSWATERPRESET,
		ASSET_TYPE_PHYS_WORLDMAP,
		ASSET_TYPE_PHYSCONSTRAINT,
		ASSET_TYPE_XANIM,
		ASSET_TYPE_XMODELSURFS,
		ASSET_TYPE_XMODEL,
		ASSET_TYPE_MATERIAL,
		ASSET_TYPE_COMPUTESHADER,
		ASSET_TYPE_VERTEXSHADER,
		ASSET_TYPE_HULLSHADER,
		ASSET_TYPE_DOMAINSHADER,
		ASSET_TYPE_PIXELSHADER,
		ASSET_TYPE_VERTEXDECL,
		ASSET_TYPE_TECHSET,
		ASSET_TYPE_IMAGE,
		ASSET_TYPE_SOUND,
		ASSET_TYPE_SOUNDSUBMIX,
		ASSET_TYPE_SNDCURVE,
		ASSET_TYPE_LPFCURVE,
		ASSET_TYPE_REVERBSENDCURVE,
		ASSET_TYPE_SNDCONTEXT,
		ASSET_TYPE_LOADED_SOUND,
		ASSET_TYPE_COL_MAP_SP,
		ASSET_TYPE_COM_MAP,
		ASSET_TYPE_GLASS_MAP,
		ASSET_TYPE_AIPATHS,
		ASSET_TYPE_VEHICLE_TRACK,
		ASSET_TYPE_MAP_ENTS,
		ASSET_TYPE_FX_MAP,
		ASSET_TYPE_GFX_MAP,
		ASSET_TYPE_LIGHTDEF,
		ASSET_TYPE_UI_MAP,
		ASSET_TYPE_MENUFILE,
		ASSET_TYPE_MENU,
		ASSET_TYPE_ANIMCLASS,
		ASSET_TYPE_LOCALIZE,
		ASSET_TYPE_ATTACHMENT,
		ASSET_TYPE_WEAPON,
		ASSET_TYPE_SNDDRIVERGLOBALS,
		ASSET_TYPE_FX,
		ASSET_TYPE_IMPACTFX,
		ASSET_TYPE_SURFACEFX,
		ASSET_TYPE_AITYPE,
		ASSET_TYPE_MPTYPE,
		ASSET_TYPE_CHARACTER,
		ASSET_TYPE_XMODELALIAS,
		ASSET_TYPE_RAWFILE,
		ASSET_TYPE_SCRIPTFILE,
		ASSET_TYPE_STRINGTABLE,
		ASSET_TYPE_LEADERBOARDDEF,
		ASSET_TYPE_VIRTUALLEADERBOARDDEF,
		ASSET_TYPE_STRUCTUREDDATADEF,
		ASSET_TYPE_DDL,
		ASSET_TYPE_PROTO,
		ASSET_TYPE_TRACER,
		ASSET_TYPE_VEHICLE,
		ASSET_TYPE_ADDON_MAP_ENTS,
		ASSET_TYPE_NETCONSTSTRINGS,
		ASSET_TYPE_REVERBPRESET,
		ASSET_TYPE_LUAFILE,
		ASSET_TYPE_SCRIPTABLE,
		ASSET_TYPE_EQUIPSNDTABLE,
		ASSET_TYPE_VECTORFIELD,
		ASSET_TYPE_DOPPLERPRESET,
		ASSET_TYPE_PARTICLESIMANIMATION,
		ASSET_TYPE_LASER,
		ASSET_TYPE_SKELETONSCRIPT,
		ASSET_TYPE_CLUT,
		ASSET_TYPE_TTF,
		ASSET_TYPE_COUNT,
	};

	struct StreamFileNameRaw
	{
		const char* dir;
		const char* name;
	};

	struct StreamFileNamePacked
	{
		unsigned __int64 offset;
		unsigned __int64 length;
	};

	union StreamFileInfo
	{
		StreamFileNameRaw raw;
		StreamFileNamePacked packed;
	};

	struct SpeakerLevels
	{
		char speaker;
		char numLevels;
		float levels[2];
	};

	struct ChannelMap
	{
		int speakerCount;
		SpeakerLevels speakers[6];
	};

	struct SpeakerMap
	{
		bool isDefault;
		const char* name;
		int a;
		ChannelMap channelMaps[2][2];
	}; //static_assert(sizeof(SpeakerMap) == 0x148);

	struct StreamFileName
	{
		bool isLocalized;
		bool isStreamed;
		unsigned __int16 fileIndex;
		StreamFileInfo info;
	};

	struct StreamedSound
	{
		StreamFileName filename;
		unsigned int totalMsec;
	};

	struct StreamFile
	{
		void* handle;
		__int64 length;
		__int64 startOffset;
		bool isPacked;
	};

	struct LoadedSoundInfo
	{
		char* data;
		unsigned int sampleRate;
		unsigned int dataByteCount;
		unsigned int numSamples;
		char channels;
		char numBits;
		char blockAlign;
		short format;
		int loadedSize;
	}; static_assert(sizeof(LoadedSoundInfo) == 0x20);

	struct LoadedSound
	{
		const char* name;
		StreamFileName filename;
		LoadedSoundInfo info;
	}; static_assert(sizeof(LoadedSound) == 0x40);

	union SoundFileRef
	{
		LoadedSound* loadSnd;
		StreamedSound streamSnd;
	};

	enum snd_alias_type_t : std::int8_t
	{
		SAT_UNKNOWN = 0x0,
		SAT_LOADED = 0x1,
		SAT_STREAMED = 0x2,
		SAT_PRIMED = 0x3,
		SAT_COUNT = 0x4,
	};

	struct SoundFile
	{
		snd_alias_type_t type;
		char exists;
		SoundFileRef u;
	};

	struct SndContext
	{
		const char* name;
		char __pad0[8];
	};

	struct SndCurve
	{
		bool isDefault;
		union
		{
			const char* filename;
			const char* name;
		};
		unsigned short knotCount;
		float knots[16][2];
	}; static_assert(sizeof(SndCurve) == 0x98);

	struct DopplerPreset
	{
		const char* name;
		float speedOfSound;
		float playerVelocityScale;
		float minPitch;
		float maxPitch;
		float smoothing;
	}; static_assert(sizeof(DopplerPreset) == 0x20);

	struct snd_alias_t
	{
		const char* aliasName;
		const char* subtitle;
		const char* secondaryAliasName;
		const char* chainAliasName;
		SoundFile* soundFile;
		const char* mixerGroup;
		char __pad0[8];
		int sequence;
		int u4;
		int u5;
		float volMin;
		float volMax;
		int volModIndex;
		float pitchMin;
		float pitchMax;
		float distMin;
		float distMax;
		float velocityMin;
		int flags;
		char masterPriority;
		float masterPercentage;
		float slavePercentage;
		char u18;
		float probability;
		char u20; // value: 0-4
		SndContext* sndContext;
		char __pad1[12];
		int startDelay;
		SndCurve* sndCurve;
		char __pad2[8];
		SndCurve* lpfCurve;
		SndCurve* hpfCurve;
		SndCurve* reverbSendCurve;
		SpeakerMap* speakerMap;
		char __pad3[47];
		float u34;
	};

	static_assert(sizeof(snd_alias_t) == 256);

	struct snd_alias_list_t
	{
		const char* aliasName;
		snd_alias_t* head;
		short* unk;
		unsigned char count;
		unsigned char unkCount;
		char __pad0[6];
	};

	struct RawFile
	{
		const char* name;
		int compressedLen;
		int len;
		const char* buffer;
	};

	struct ScriptFile
	{
		const char* name;
		int compressedLen;
		int len;
		int bytecodeLen;
		char* buffer;
		char* bytecode;
	};

	struct StringTableCell
	{
		const char* string;
		int hash;
	};

	struct StringTable
	{
		const char* name;
		int columnCount;
		int rowCount;
		StringTableCell* values;
	};

	struct LuaFile
	{
		const char* name;
		int len;
		char strippingType;
		const char* buffer;
	};

	struct TTF
	{
		const char* name;
		int len;
		const char* buffer;
		int fontFace;
	};

	struct MapEnts
	{
		const char* name;
		char* entityString;
		int numEntityChars;
	};

	struct TriggerModel
	{
		int contents;
		unsigned __int16 hullCount;
		unsigned __int16 firstHull;
	};

	struct TriggerHull
	{
		Bounds bounds;
		int contents;
		unsigned __int16 slabCount;
		unsigned __int16 firstSlab;
	};

	struct TriggerSlab
	{
		float dir[3];
		float midPoint;
		float halfSize;
	};

	struct MapTriggers
	{
		unsigned int modelCount;
		TriggerModel* models;
		unsigned int hullCount;
		TriggerHull* hulls;
		unsigned int slabCount;
		TriggerSlab* slabs;
	};

	struct AddonMapEnts
	{
		const char* name;
		char* entityString;
		int numEntityChars;
		MapTriggers trigger;
		void* info;
		unsigned int numSubModels;
		void* cmodels;
		void* models;
	};

	struct LocalizeEntry
	{
		const char* value;
		const char* name;
	};

	struct PhysWorld;
	struct clipMap_t;

	union XAssetHeader
	{
		void* data;
		clipMap_t* col_map;
		PhysWorld* phys_map;
		Material* material;
		Font_s* font;
		RawFile* rawfile;
		ScriptFile* scriptfile;
		StringTable* stringTable;
		LuaFile* luaFile;
		TTF* ttf;
		MapEnts* mapents;
		AddonMapEnts* addon_mapents;
		LocalizeEntry* localize;
		snd_alias_list_t* sound;
		DopplerPreset* doppler_preset;
		SndContext* snd_context;
		SndCurve* snd_curve;
		LoadedSound* loaded_sound;
	};

	struct XAsset
	{
		XAssetType type;
		XAssetHeader header;
	};

	struct XAssetEntry
	{
		XAsset asset;
		char zoneIndex;
		volatile char inuseMask;
		unsigned int nextHash;
		unsigned int nextOverride;
		unsigned int nextPoolEntry;
	};

	enum DBSyncMode
	{
		DB_LOAD_ASYNC = 0x0,
		DB_LOAD_SYNC = 0x1,
		DB_LOAD_ASYNC_WAIT_ALLOC = 0x2,
		DB_LOAD_ASYNC_FORCE_FREE = 0x3,
		DB_LOAD_ASYNC_NO_SYNC_THREADS = 0x4,
		DB_LOAD_SYNC_SKIP_ALWAYS_LOADED = 0x5,
	};

	enum DBAllocFlags : std::int32_t
	{
		DB_ZONE_NONE = 0x0,
		DB_ZONE_COMMON = 0x1,
		DB_ZONE_UI = 0x2,
		DB_ZONE_GAME = 0x4,
		DB_ZONE_LOAD = 0x8,
		DB_ZONE_DEV = 0x10,
		DB_ZONE_BASEMAP = 0x20,
		DB_ZONE_TRANSIENT_POOL = 0x40,
		DB_ZONE_TRANSIENT_MASK = 0x40,
		DB_ZONE_CUSTOM = 0x1000 // added for custom zone loading
	};

	struct XZone
	{
		char __pad0[24];
		char name[64];
		char __pad1[128];
	};

	static_assert(sizeof(XZone) == 216);

	struct XZoneInfo
	{
		const char* name;
		int allocFlags;
		int freeFlags;
	};

	struct LevelLoad
	{
		XZoneInfo info[24];
		unsigned __int64 sizeEstimate[24];
		char names[24][64];
		unsigned int numZones;
		unsigned int loadPhaseCount[3];
		unsigned int numPhases;
	};

	struct scr_entref_t
	{
		unsigned short entnum;
		unsigned short classnum;
	};

	typedef void(*BuiltinMethod)(scr_entref_t);
	typedef void(*BuiltinFunction)();

	enum
	{
		VAR_UNDEFINED = 0x0,
		VAR_BEGIN_REF = 0x1,
		VAR_POINTER = 0x1,
		VAR_STRING = 0x2,
		VAR_ISTRING = 0x3,
		VAR_VECTOR = 0x4,
		VAR_END_REF = 0x5,
		VAR_FLOAT = 0x5,
		VAR_INTEGER = 0x6,
		VAR_CODEPOS = 0x7,
		VAR_PRECODEPOS = 0x8,
		VAR_FUNCTION = 0x9,
		VAR_BUILTIN_FUNCTION = 0xA,
		VAR_BUILTIN_METHOD = 0xB,
		VAR_STACK = 0xC,
		VAR_ANIMATION = 0xD,
		VAR_PRE_ANIMATION = 0xE,
		VAR_THREAD = 0xF,
		VAR_NOTIFY_THREAD = 0x10,
		VAR_TIME_THREAD = 0x11,
		VAR_CHILD_THREAD = 0x12,
		VAR_OBJECT = 0x13,
		VAR_DEAD_ENTITY = 0x14,
		VAR_ENTITY = 0x15,
		VAR_ARRAY = 0x16,
		VAR_DEAD_THREAD = 0x17,
		VAR_COUNT = 0x18,
		VAR_FREE = 0x18,
		VAR_THREAD_LIST = 0x19,
		VAR_ENDON_LIST = 0x1A,
		VAR_TOTAL_COUNT = 0x1B,
	};

	enum scriptType_e
	{
		SCRIPT_NONE = 0,
		SCRIPT_OBJECT = 1,
		SCRIPT_STRING = 2,
		SCRIPT_ISTRING = 3,
		SCRIPT_VECTOR = 4,
		SCRIPT_FLOAT = 5,
		SCRIPT_INTEGER = 6,
		SCRIPT_END = 8,
		SCRIPT_FUNCTION = 9,
		SCRIPT_STRUCT = 19,
		SCRIPT_ENTITY = 21,
		SCRIPT_ARRAY = 22,
	};

	struct VariableStackBuffer
	{
		const char* pos;
		unsigned __int16 size;
		unsigned __int16 bufLen;
		unsigned __int16 localId;
		char time;
		char buf[1];
	};

	union VariableUnion
	{
		int intValue;
		unsigned int uintValue;
		float floatValue;
		unsigned int stringValue;
		const float* vectorValue;
		const char* codePosValue;
		unsigned int pointerValue;
		VariableStackBuffer* stackValue;
		unsigned int entityOffset;
		uint64_t value;
	};

	struct VariableValue
	{
		VariableUnion u;
		int type;
	};

	struct function_stack_t
	{
		const char* pos;
		unsigned int localId;
		unsigned int localVarCount;
		VariableValue* top;
		VariableValue* startTop;
	};

	struct function_frame_t
	{
		function_stack_t fs;
		int topType;
	};

	struct scrVmPub_t
	{
		unsigned int* localVars;
		VariableValue* maxstack;
		int function_count;
		function_frame_t* function_frame;
		VariableValue* top;
		unsigned int inparamcount;
		unsigned int outparamcount;
		function_frame_t function_frame_start[32];
		VariableValue stack[2048];
	};

	struct scr_classStruct_t
	{
		unsigned __int16 id;
		unsigned __int16 entArrayId;
		char charId;
		const char* name;
	};

	struct ObjectVariableChildren
	{
		unsigned __int16 firstChild;
		unsigned __int16 lastChild;
	};

	struct ObjectVariableValue_u_f
	{
		unsigned __int16 prev;
		unsigned __int16 next;
	};

	union ObjectVariableValue_u_o_u
	{
		unsigned __int16 size;
		unsigned __int16 entnum;
		unsigned __int16 nextEntId;
		unsigned __int16 self;
	};

	struct	ObjectVariableValue_u_o
	{
		unsigned __int16 refCount;
		ObjectVariableValue_u_o_u u;
	};

	union ObjectVariableValue_w
	{
		unsigned int type;
		unsigned int classnum;
		unsigned int notifyName;
		unsigned int waitTime;
		unsigned int parentLocalId;
	};

	struct ChildVariableValue_u_f
	{
		unsigned __int16 prev;
		unsigned __int16 next;
	};

	union ChildVariableValue_u
	{
		ChildVariableValue_u_f f;
		VariableUnion u;
	};

	struct ChildBucketMatchKeys_keys
	{
		unsigned __int16 name_hi;
		unsigned __int16 parentId;
	};

	union ChildBucketMatchKeys
	{
		ChildBucketMatchKeys_keys keys;
		unsigned int match;
	};

	struct	ChildVariableValue
	{
		ChildVariableValue_u u;
		unsigned __int16 next;
		char type;
		char name_lo;
		ChildBucketMatchKeys k;
		unsigned __int16 nextSibling;
		unsigned __int16 prevSibling;
	};

	union ObjectVariableValue_u
	{
		ObjectVariableValue_u_f f;
		ObjectVariableValue_u_o o;
	};

	struct ObjectVariableValue
	{
		ObjectVariableValue_u u;
		ObjectVariableValue_w w;
	};

	struct scrVarGlob_t
	{
		ObjectVariableValue objectVariableValue[56320];
		ObjectVariableChildren objectVariableChildren[56320];
		unsigned __int16 childVariableBucket[65536];
		ChildVariableValue childVariableValue[384000];
	};

	enum GfxDrawSceneMethod
	{
		GFX_DRAW_SCENE_STANDARD = 0x0,
	};

	enum MaterialTechniqueType
	{
		TECHNIQUE_UNLIT = 8,
		TECHNIQUE_EMISSIVE = 9,
		TECHNIQUE_LIT = 13,
		TECHNIQUE_WIREFRAME = 53,
	};

	struct GfxDrawMethod_s
	{
		int drawScene;
		int baseTechType;
		int emissiveTechType;
		int forceTechType;
	};

	struct GfxImageLoadDef
	{
		char levelCount;
		char numElements;
		char pad[2];
		int flags;
		int format;
		int resourceSize;
		char data[1];
	};

	struct GfxTexture
	{
		union
		{
			ID3D11Texture1D* linemap;
			ID3D11Texture2D* map;
			ID3D11Texture3D* volmap;
			ID3D11Texture2D* cubemap;
			GfxImageLoadDef* loadDef;
		};
		ID3D11ShaderResourceView* shaderView;
		ID3D11ShaderResourceView* shaderViewAlternate;
	};

	struct Picmip
	{
		unsigned char platform[2];
	};

	struct GfxImageStreamData
	{
		unsigned short width;
		unsigned short height;
		union
		{
			unsigned char bytes[4];
			unsigned int pixelSize;
		};
	};

	enum MapType : std::uint8_t
	{
		MAPTYPE_NONE = 0x0,
		MAPTYPE_INVALID1 = 0x1,
		MAPTYPE_1D = 0x2,
		MAPTYPE_2D = 0x3,
		MAPTYPE_3D = 0x4,
		MAPTYPE_CUBE = 0x5,
		MAPTYPE_ARRAY = 0x6,
		MAPTYPE_COUNT = 0x7,
	};

#pragma warning(push)
#pragma warning(disable: 4201)
	struct GfxImage
	{
		GfxTexture texture;
		DXGI_FORMAT imageFormat;
		MapType mapType;
		unsigned char semantic;
		unsigned char category;
		unsigned char flags;
		union
		{
			struct
			{
				Picmip picmip;
				char __pad0[2];
			};
			unsigned int resourceSize;
		};
		unsigned int dataLen1;
		unsigned int dataLen2;
		unsigned short width;
		unsigned short height;
		unsigned short depth;
		unsigned short numElements;
		unsigned char levelCount;
		unsigned char streamed;
		char __pad1[2];
		unsigned char* pixelData;
		GfxImageStreamData streams[4];
		const char* name;
	};
#pragma warning(pop)

	static_assert(offsetof(GfxImage, resourceSize) == 32);
	static_assert(offsetof(GfxImage, dataLen1) == 36);
	static_assert(offsetof(GfxImage, flags) == 31);

	struct GfxSky
	{
		int skySurfCount;
		int* skyStartSurfs;
		GfxImage* skyImage;
		unsigned char skySamplerState;
		Bounds bounds;
	};

	struct GfxWorldDpvsPlanes
	{
		int cellCount;
		void* planes;
		unsigned int* nodes;
		unsigned int* sceneEntCellBits;
	};

	struct GfxCellTreeCount
	{
		int aabbTreeCount;
	};

	struct GfxAabbTree
	{
		Bounds bounds;
		int childrenOffset;
		unsigned short childCount;
		unsigned short smodelIndexCount;
		unsigned short* smodelIndexes;
		int startSurfIndex;
		unsigned short surfaceCount;
		unsigned short pad;
	};

	struct GfxCellTree
	{
		GfxAabbTree* aabbTree;
	};

	struct GfxPortal;
	struct GfxPortalWritable
	{
		bool isQueued;
		bool isAncestor;
		unsigned char recursionDepth;
		unsigned char hullPointCount;
		float(*hullPoints)[2];
		GfxPortal* queuedParent;
	};

	struct DpvsPlane
	{
		float coeffs[4];
	};

	struct GfxPortal
	{
		GfxPortalWritable writable;
		DpvsPlane plane;
		float(*vertices)[3];
		unsigned short cellIndex;
		unsigned short closeDistance;
		unsigned char vertexCount;
		float hullAxis[2][3];
	};

	struct GfxCell
	{
		Bounds bounds;
		short portalCount;
		unsigned char reflectionProbeCount;
		unsigned char reflectionProbeReferenceCount;
		GfxPortal* portals;
		unsigned char* reflectionProbes;
		unsigned char* reflectionProbeReferences;
	};

	struct GfxPortalGroupInfo
	{
		char __pad0[4];
	};

	struct GfxPortalGroup
	{
		const char* group;
		GfxPortalGroupInfo* info;
		char __pad0[8];
		int infoCount;
	};

	struct GfxReflectionProbeVolume
	{
		unsigned short* data;
		unsigned int count;
	};

	struct GfxReflectionProbe
	{
		float origin[3];
		GfxReflectionProbeVolume* probeVolumes;
		unsigned int probeVolumeCount;
	};

	struct GfxReflectionProbeReferenceOrigin
	{
		float origin[3];
	};

	struct GfxReflectionProbeReference
	{
		unsigned char index;
	};

	struct GfxRawTexture
	{
		char __pad0[32];
	};

	struct GfxLightmapArray
	{
		GfxImage* primary;
		GfxImage* secondary;
	};

	struct GfxWorldVertex
	{
		float xyz[3];
		float binormalSign;
		unsigned int color;
		float texCoord[2];
		float lmapCoord[2];
		unsigned int normal;
		unsigned int tangent;
	};

	union GfxWorldVertex0Union
	{
		GfxWorldVertex* vertices;
	};

	struct GfxWorldVertexData
	{
		GfxWorldVertex* vertices;
		ID3D11Buffer* worldVb;
		ID3D11ShaderResourceView* worldVbView;
	};

	struct GfxWorldVertexLayerData
	{
		unsigned char* data;
		ID3D11Buffer* layerVb;
		ID3D11ShaderResourceView* layerVbView;
	};

	struct GfxDisplacementParms
	{
		char __pad0[16];
	};

	struct GfxWorldDraw
	{
		unsigned int reflectionProbeCount;
		GfxImage** reflectionProbes;
		GfxReflectionProbe* reflectionProbeOrigins;
		GfxRawTexture* reflectionProbeTextures;
		unsigned int reflectionProbeReferenceCount;
		GfxReflectionProbeReferenceOrigin* reflectionProbeReferenceOrigins;
		GfxReflectionProbeReference* reflectionProbeReferences;
		int lightmapCount;
		GfxLightmapArray* lightmaps;
		GfxRawTexture* lightmapPrimaryTextures;
		GfxRawTexture* lightmapSecondaryTextures;
		GfxImage* lightmapOverridePrimary;
		GfxImage* lightmapOverrideSecondary;
		int u1[2];
		int u2[2];
		int u3;
		unsigned int trisType;
		unsigned int vertexCount;
		GfxWorldVertexData vd;
		unsigned int vertexLayerDataSize;
		GfxWorldVertexLayerData vld;
		unsigned int indexCount;
		unsigned short* indices;
		ID3D11Buffer* indexBuffer;
		ID3D11ShaderResourceView* indexBufferView;
		int displacementParmsCount;
		GfxDisplacementParms* displacementParms;
		ID3D11Buffer* displacementParmsBuffer;
		ID3D11ShaderResourceView* displacementParmsBufferView;
	};

	struct GfxLightGridEntry
	{
		unsigned int colorsIndex;
		unsigned short primaryLightEnvIndex;
		unsigned char unused;
		unsigned char needsTrace;
	};

	struct GfxLightGridColors
	{
		unsigned char colorVec6[56][6];
	};

	struct GfxLightGridColorsHDR
	{
		unsigned char colorVec6[56][6];
	};

	struct GfxLightGridTree
	{
		unsigned char maxDepth;
		int nodeCount;
		int leafCount;
		int coordMinGridSpace[3];
		int coordMaxGridSpace[3];
		int coordHalfSizeGridSpace[3];
		int defaultColorIndexBitCount;
		int defaultLightIndexBitCount;
		unsigned int* p_nodeTable;
		int leafTableSize;
		unsigned char* p_leafTable;
	};

	struct GfxLightGrid
	{
		bool hasLightRegions;
		bool useSkyForLowZ;
		unsigned int lastSunPrimaryLightIndex;
		unsigned short mins[3];
		unsigned short maxs[3];
		unsigned int rowAxis;
		unsigned int colAxis;
		unsigned short* rowDataStart;
		unsigned int rawRowDataSize;
		unsigned char* rawRowData;
		unsigned int entryCount;
		GfxLightGridEntry* entries;
		unsigned int colorCount;
		GfxLightGridColors* colors;
		char __pad0[24];
		int tableVersion;
		int paletteVersion;
		char rangeExponent8BitsEncoding;
		char rangeExponent12BitsEncoding;
		char rangeExponent16BitsEncoding;
		unsigned char stageCount;
		float* stageLightingContrastGain;
		unsigned int paletteEntryCount;
		int* paletteEntryAddress;
		unsigned int paletteBitstreamSize;
		unsigned char* paletteBitstream;
		GfxLightGridColorsHDR skyLightGridColors;
		GfxLightGridColorsHDR defaultLightGridColors;
		GfxLightGridTree tree[3];
	};

	struct GfxBrushModelWritable
	{
		Bounds bounds;
		vec3_t origin;
		vec4_t quat;
		int mdaoVolumeProcessed;
	};

	struct GfxBrushModel
	{
		GfxBrushModelWritable writable;
		Bounds bounds;
		float radius;
		unsigned int startSurfIndex;
		unsigned short surfaceCount;
		int mdaoVolumeIndex;
	};

	struct MaterialMemory
	{
		Material* material;
		int memory;
	};

	struct sunflare_t
	{
		bool hasValidData;
		Material* spriteMaterial;
		Material* flareMaterial;
		float spriteSize;
		float flareMinSize;
		float flareMinDot;
		float flareMaxSize;
		float flareMaxDot;
		float flareMaxAlpha;
		int flareFadeInTime;
		int flareFadeOutTime;
		float blindMinDot;
		float blindMaxDot;
		float blindMaxDarken;
		int blindFadeInTime;
		int blindFadeOutTime;
		float glareMinDot;
		float glareMaxDot;
		float glareMaxLighten;
		int glareFadeInTime;
		int glareFadeOutTime;
		float sunFxPosition[3];
	};

	struct XModelDrawInfo
	{
		unsigned char hasGfxEntIndex;
		unsigned char lod;
		unsigned short surfId;
	};

	struct GfxSceneDynModel
	{
		XModelDrawInfo info;
		unsigned short dynEntId;
	};

	struct BModelDrawInfo
	{
		unsigned short surfId;
	};

	struct GfxSceneDynBrush
	{
		BModelDrawInfo info;
		unsigned short dynEntId;
	};

	struct GfxShadowGeometry
	{
		unsigned short surfaceCount;
		unsigned short smodelCount;
		unsigned int* sortedSurfIndex;
		unsigned short* smodelIndex;
	};

	struct GfxLightRegionAxis
	{
		float dir[3];
		float midPoint;
		float halfSize;
	};

	struct GfxLightRegionHull
	{
		float kdopMidPoint[9];
		float kdopHalfSize[9];
		unsigned int axisCount;
		GfxLightRegionAxis* axis;
	};

	struct GfxLightRegion
	{
		unsigned int hullCount;
		GfxLightRegionHull* hulls;
	};

	struct GfxStaticModelInst
	{
		float mins[3];
		float maxs[3];
		float lightingOrigin[3];
	};

	struct srfTriangles_t
	{
		unsigned int vertexLayerData;
		unsigned int firstVertex;
		float maxEdgeLength;
		int unk;
		unsigned short vertexCount;
		unsigned short triCount;
		unsigned int baseIndex;
	};

	struct GfxSurfaceLightingAndFlagsFields
	{
		unsigned char lightmapIndex;
		unsigned char reflectionProbeIndex;
		unsigned short primaryLightEnvIndex;
		unsigned char flags;
		unsigned char unused[3];
	};

	union GfxSurfaceLightingAndFlags
	{
		GfxSurfaceLightingAndFlagsFields fields;
		unsigned __int64 packed;
	};

	struct GfxSurface
	{
		srfTriangles_t tris;
		Material* material;
		GfxSurfaceLightingAndFlags laf;
	};

	struct GfxSurfaceBounds
	{
		Bounds bounds;
		char __pad0[11];
		char flags;
	};

	struct GfxPackedPlacement
	{
		float origin[3];
		float axis[3][3];
		float scale;
	};

	enum StaticModelFlag : std::int16_t
	{
		STATIC_MODEL_FLAG_NO_CAST_SHADOW = 0x10,
		STATIC_MODEL_FLAG_GROUND_LIGHTING = 0x20,
		STATIC_MODEL_FLAG_LIGHTGRID_LIGHTING = 0x40,
		STATIC_MODEL_FLAG_VERTEXLIT_LIGHTING = 0x80,
		STATIC_MODEL_FLAG_LIGHTMAP_LIGHTING = 0x100,
		STATIC_MODEL_FLAG_ALLOW_FXMARK = 0x200,
		STATIC_MODEL_FLAG_REACTIVEMOTION = 0x400,
		STATIC_MODEL_FLAG_ANIMATED_VERTS = 0x800,
	};

	struct GfxStaticModelDrawInst
	{
		void* model;
		GfxPackedPlacement placement;
		unsigned short cullDist;
		unsigned short flags;
		unsigned short lightingHandle;
		unsigned short staticModelId;
		short pad;
		unsigned short primaryLightEnvIndex;
		char unk;
		unsigned char reflectionProbeIndex;
		unsigned char firstMtlSkinIndex;
		unsigned char sunShadowFlags;
	};

	struct GfxStaticModelVertexLighting
	{
		unsigned char visibility[4];
		unsigned short ambientColorFloat16[4];
		unsigned short highlightColorFloat16[4];
	};

	struct GfxStaticModelVertexLightingInfo
	{
		GfxStaticModelVertexLighting* lightingValues;
		ID3D11Buffer* lightingValuesVb;
		int numLightingValues;
	};

	struct GfxStaticModelLightmapInfo
	{
		unsigned short smodelCacheIndex[4];
		unsigned short unk1;
		unsigned short unk2;
		float unk3;
		int unk4;
		int unk5;
		/*
		unsigned short V0[4];
		unsigned short V1[4];
		unsigned short V2[4];
		*/
	};

	struct GfxStaticModelLighting
	{
		union
		{
			GfxStaticModelVertexLightingInfo info;
			GfxStaticModelLightmapInfo info2;
		};
	};

	struct GfxSubdivCache
	{
		unsigned int size;
		ID3D11Buffer* subdivCacheBuffer;
		ID3D11ShaderResourceView* subdivCacheView;
	};

	struct GfxSubdivVertexLightingInfo
	{
		int vertexLightingIndex;
		ID3D11Buffer* vb;
		GfxSubdivCache cache;
	};

	struct GfxDepthAndSurf
	{
		char __pad0[8];
	};

	typedef char* GfxWorldDpvsVoid;

	struct GfxWorldDpvsUnk
	{
		char __pad0[8];
		GfxStaticModelVertexLightingInfo info;
		char __pad1[24];
	};

	struct GfxWorldDpvsStatic
	{
		unsigned int smodelCount; // 0
		unsigned int subdivVertexLightingInfoCount; // 4
		unsigned int staticSurfaceCount; // 8
		unsigned int litOpaqueSurfsBegin; // 12
		unsigned int litOpaqueSurfsEnd; // 16
		unsigned int unkSurfsBegin;
		unsigned int unkSurfsEnd;
		unsigned int litDecalSurfsBegin; // 28
		unsigned int litDecalSurfsEnd; // 32
		unsigned int litTransSurfsBegin; // 36
		unsigned int litTransSurfsEnd; // 40
		unsigned int shadowCasterSurfsBegin; // 44
		unsigned int shadowCasterSurfsEnd; // 48
		unsigned int emissiveSurfsBegin; // 52
		unsigned int emissiveSurfsEnd; // 56
		unsigned int smodelVisDataCount; // 60
		unsigned int surfaceVisDataCount; // 64
		unsigned int unkCount1; // 68
		unsigned int* smodelVisData[4]; // 72 80 88 96
		unsigned int* smodelUnknownVisData[27];
		unsigned int* surfaceVisData[4]; // 320 328 336 344
		unsigned int* surfaceUnknownVisData[27];
		unsigned int* smodelUmbraVisData[4]; // 568 576 584 592
		unsigned int* surfaceUmbraVisData[4]; // 600 608 616 624
		unsigned int unkCount2; // 632
		unsigned int* lodData; // 640
		unsigned int* tessellationCutoffVisData; // 648
		unsigned int* sortedSurfIndex; // 656
		GfxStaticModelInst* smodelInsts; // 664
		GfxSurface* surfaces; // 672
		GfxSurfaceBounds* surfacesBounds; // 680
		GfxStaticModelDrawInst* smodelDrawInsts; // 688
		unsigned int* unknownSModelVisData1; // 696
		unsigned int* unknownSModelVisData2; // 704
		GfxStaticModelLighting* smodelLighting; // 712 (array)
		GfxSubdivVertexLightingInfo* subdivVertexLighting; // 720 (array)
		void* surfaceMaterials; // 728
		unsigned int* surfaceCastsSunShadow; // 736
		unsigned int sunShadowOptCount; // 744
		unsigned int sunSurfVisDataCount; // 748
		unsigned int* surfaceCastsSunShadowOpt; // 752
		GfxDepthAndSurf* surfaceDeptAndSurf; // 760
		GfxWorldDpvsVoid* constantBuffersLit; // 768
		GfxWorldDpvsVoid* constantBuffersAmbient; // 776
		GfxWorldDpvsUnk* gfx_unk; // 784
		int usageCount; // 792
	};
	static_assert(sizeof(GfxWorldDpvsStatic) == 800);

	struct GfxWorldDpvsDynamic
	{
		unsigned int dynEntClientWordCount[2]; // 0 4
		unsigned int dynEntClientCount[2]; // 8 12
		unsigned int* dynEntCellBits[2]; // 16 24
		unsigned char* dynEntVisData[2][4]; // 32 40 48 56 64 72 80 88
	};

	struct GfxHeroOnlyLight
	{
		unsigned char type;
		unsigned char unused[3];
		float color[3];
		float dir[3];
		float up[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		int exponent;
	};

	typedef void* umbraTomePtr_t;

	struct GfxBuildInfo
	{
		const char* args0;
		const char* args1;
		const char* buildStartTime;
		const char* buildEndTime;
	};

	struct GfxWorld
	{
		const char* name; // 0
		const char* baseName; // 8
		unsigned int bspVersion; // 16
		int planeCount; // 20
		int nodeCount; // 24
		unsigned int surfaceCount; // 28
		int skyCount; // 32
		GfxSky* skies; // 40
		unsigned int portalGroupCount; // 48
		char __pad0[16];
		unsigned int lastSunPrimaryLightIndex; // 68
		unsigned int primaryLightCount; // 72
		unsigned int primaryLightEnvCount; // 76
		unsigned int sortKeyLitDecal; // 80
		unsigned int sortKeyEffectDecal; // 84
		unsigned int sortKeyTopDecal; // 88
		unsigned int sortKeyEffectAuto; // 92
		unsigned int sortKeyDistortion; // 96
		unsigned int sortKeyUnknown; // 100
		unsigned int sortKeyUnknown2; // 104
		char __pad1[4]; // 108
		GfxWorldDpvsPlanes dpvsPlanes; // 112
		GfxCellTreeCount* aabbTreeCounts; // 144
		GfxCellTree* aabbTrees; // 152
		GfxCell* cells; // 160
		GfxPortalGroup* portalGroup; // 168
		int unk_vec4_count_0; // 176
		char __pad2[4]; // 180
		vec4_t* unk_vec4_0; // 184
		GfxWorldDraw draw; // 192
		GfxLightGrid lightGrid; // 448
		int modelCount; // 1528
		GfxBrushModel* models; // 1536
		vec3_t mins1;
		vec3_t maxs1;
		vec3_t mins2;
		vec3_t maxs2;
		unsigned int checksum; // 1592
		int materialMemoryCount; // 1596
		MaterialMemory* materialMemory; // 1600
		sunflare_t sun; // 1608
		float outdoorLookupMatrix[4][4]; // 1720
		GfxImage* outdoorImage; // 1784
		unsigned int* cellCasterBits; // 1792
		unsigned int* cellHasSunLitSurfsBits; // 1800
		GfxSceneDynModel* sceneDynModel; // 1808
		GfxSceneDynBrush* sceneDynBrush; // 1816
		unsigned int* primaryLightEntityShadowVis; // 1824
		unsigned int* primaryLightDynEntShadowVis[2]; // 1832 1840
		unsigned short* nonSunPrimaryLightForModelDynEnt; // 1848
		GfxShadowGeometry* shadowGeom; // 1856
		GfxShadowGeometry* shadowGeomOptimized; // 1864
		GfxLightRegion* lightRegion; // 1872
		GfxWorldDpvsStatic dpvs; // 1880
		GfxWorldDpvsDynamic dpvsDyn; // 2680
		unsigned int mapVtxChecksum; // 2776
		unsigned int heroOnlyLightCount; // 2780
		GfxHeroOnlyLight* heroOnlyLights; // 2784
		unsigned char fogTypesAllowed; // 2792
		unsigned int umbraTomeSize; // 2796
		char* umbraTomeData; // 2800
		umbraTomePtr_t umbraTomePtr; // 2808
		unsigned int mdaoVolumesCount; // 2816
		void* mdaoVolumes; // 2824
		char __pad3[32];
		GfxBuildInfo buildInfo; // 2864
	};

	static_assert(sizeof(GfxWorld) == 0xB50);

	struct PhysPreset
	{
		const char* name;
		char __pad0[32];
		const char* sndAliasPrefix;
		char __pad1[48];
	}; static_assert(sizeof(PhysPreset) == 0x60);

	struct dmMeshNode_array_unk
	{
		unsigned int unk : 6;
		unsigned int offset : 26;
	};

	union dmMeshNode_array_unk_packed
	{
		dmMeshNode_array_unk fields;
		unsigned int packed;
	};

	static_assert(sizeof(dmMeshNode_array_unk_packed) == 4);

	struct dmMeshNode_array_t
	{
		short values[6];
		dmMeshNode_array_unk_packed unk;
	};

	static_assert(sizeof(dmMeshNode_array_t) == 16);

	struct dmMeshTriangle
	{
		int indices[3];
		int unk_indices[3];
		int unk;
		int flags;
	};

	typedef vec4_t dm_vec4;

	struct dmMeshData
	{
		dmMeshNode_array_t* meshNodes;
		dm_vec4* meshVertices;
		dmMeshTriangle* meshTriangles;
		Bounds bounds;
		float unk_vec2[3];
		unsigned int m_nodeCount;
		unsigned int m_vertexCount;
		unsigned int m_triangleCount;
		int unk;
		int contents;
	}; static_assert(sizeof(dmMeshData) == 0x50);

	struct dmSubEdge
	{
		int value;
	};

	struct dmPolytopeData
	{
		vec4_t* vec4_array0;
		vec4_t* vec4_array1;
		unsigned short* uint16_array0;
		unsigned short* uint16_array1;
		dmSubEdge* edges;
		unsigned char* uint8_array0;
		char __pad0[12];
		unsigned int count0;
		unsigned int count1;
		unsigned int count2;
		char __pad1[40];
	}; static_assert(sizeof(dmPolytopeData) == 0x70);

	struct PhysGeomInfo
	{
		dmPolytopeData* data;
	};

	struct PhysMass
	{
		float centerOfMass[3];
		float momentsOfInertia[3];
		float productsOfInertia[3];
	};

	struct PhysCollmap
	{
		const char* name;
		unsigned int count;
		PhysGeomInfo* geoms;
		PhysMass mass;
		Bounds bounds;
	}; static_assert(sizeof(PhysCollmap) == 0x58);

	struct PhysWaterPreset
	{
		const char* name;
		char __pad0[64];
		void* fx0;
		void* fx1;
		void* fx2;
		void* fx3;
		void* fx4;
		void* fx5;
		void* fx6;
	}; static_assert(sizeof(PhysWaterPreset) == 0x80);

	struct PhysWaterVolumeDef
	{
		PhysWaterPreset* physWaterPreset;
		char __pad0[12];
		scr_string_t string;
		char __pad1[8];
	}; static_assert(sizeof(PhysWaterVolumeDef) == 0x20);
	static_assert(offsetof(PhysWaterVolumeDef, string) == 20);

	struct PhysBrushModel
	{
		char __pad0[8];
	};

	struct PhysWorld
	{
		const char* name;
		PhysBrushModel* models;
		dmPolytopeData* polytopeDatas;
		dmMeshData* meshDatas;
		PhysWaterVolumeDef* waterVolumes;
		unsigned int modelsCount;
		unsigned int polytopeDatasCount;
		unsigned int meshDatasCount;
		unsigned int waterVolumesCount;
	};

	union GfxColor
	{
		unsigned char color[4];
		unsigned int packed;
	};

	union PackedUnitVec
	{
		unsigned int packed;
	};

	struct GfxVertex
	{
		float xyzw[4];
		GfxColor color;
		float texCoord[2];
		PackedUnitVec normal;
	};

	struct cplane_s
	{
		float normal[3];
		float dist;
		unsigned char type;
		//unsigned char pad[3];
	}; 

	enum CSurfaceFlags : std::uint32_t
	{
		SURF_FLAG_DEFAULT = 0x00000000,
		SURF_FLAG_BARK = 0x00100000,
		SURF_FLAG_BRICK = 0x00200000,
		SURF_FLAG_CARPET = 0x00300000,
		SURF_FLAG_CLOTH = 0x00400000,
		SURF_FLAG_CONCRETE = 0x00500000,
		SURF_FLAG_DIRT = 0x00600000,
		SURF_FLAG_FLESH = 0x00700000,
		SURF_FLAG_FOLIAGE_DEBRIS = 0x00800000,
		SURF_FLAG_GLASS = 0x00900000,
		SURF_FLAG_GRASS = 0x00A00000,
		SURF_FLAG_GRAVEL = 0x00B00000,
		SURF_FLAG_ICE = 0x00C00000,
		SURF_FLAG_METAL_SOLID = 0x00D00000,
		SURF_FLAG_METAL_GRATE = 0x00E00000,
		SURF_FLAG_MUD = 0x00F00000,
		SURF_FLAG_PAPER = 0x01000000,
		SURF_FLAG_PLASTER = 0x01100000,
		SURF_FLAG_ROCK = 0x01200000,
		SURF_FLAG_SAND = 0x01300000,
		SURF_FLAG_SNOW = 0x01400000,
		SURF_FLAG_WATER_WAIST = 0x01500000,
		SURF_FLAG_WOOD_SOLID = 0x01600000,
		SURF_FLAG_ASPHALT = 0x01700000,
		SURF_FLAG_CERAMIC = 0x01800000,
		SURF_FLAG_PLASTIC_SOLID = 0x01900000,
		SURF_FLAG_RUBBER = 0x01A00000,
		SURF_FLAG_FRUIT = 0x01B00000,
		SURF_FLAG_PAINTEDMETAL = 0x01C00000,
		SURF_FLAG_RIOTSHIELD = 0x01D00000,
		SURF_FLAG_SLUSH = 0x01E00000,
		SURF_FLAG_ASPHALT_WET = 0x01F00000,
		SURF_FLAG_ASPHALT_DEBRIS = 0x02000000,
		SURF_FLAG_CONCRETE_WET = 0x02100000,
		SURF_FLAG_CONCRETE_DEBRIS = 0x02200000,
		SURF_FLAG_FOLIAGE_VEGETATION = 0x02300000,
		SURF_FLAG_FOLIAGE_LEAVES = 0x02400000,
		SURF_FLAG_GRASS_TALL = 0x02500000,
		SURF_FLAG_METAL_HOLLOW = 0x02600000,
		SURF_FLAG_METAL_VEHICLE = 0x02700000,
		SURF_FLAG_METAL_THIN = 0x02800000,
		SURF_FLAG_METAL_WET = 0x02900000,
		SURF_FLAG_METAL_DEBRIS = 0x02A00000,
		SURF_FLAG_PLASTIC_HOLLOW = 0x02B00000,
		SURF_FLAG_PLASTIC_TARP = 0x02C00000,
		SURF_FLAG_ROCK_WET = 0x02D00000,
		SURF_FLAG_ROCK_DEBRIS = 0x02E00000,
		SURF_FLAG_WATER_ANKLE = 0x02F00000,
		SURF_FLAG_WATER_KNEE = 0x03000000,
		SURF_FLAG_WATER_HOLLOW = 0x03100000,
		SURF_FLAG_WOOD_HOLLOW = 0x03200000,
		SURF_FLAG_WOOD_DEBRIS = 0x03300000,
		SURF_FLAG_CUSHION = 0x03400000,
		SURF_FLAG_CLIPMISSILE = 0x00000000,
		SURF_FLAG_AI_NOSIGHT = 0x00000000,
		SURF_FLAG_CLIPSHOT = 0x00000000,
		SURF_FLAG_PLAYERCLIP = 0x00000000,
		SURF_FLAG_MONSTERCLIP = 0x00000000,
		SURF_FLAG_AICLIPALLOWDEATH = 0x00000000,
		SURF_FLAG_VEHICLECLIP = 0x00000000,
		SURF_FLAG_ITEMCLIP = 0x00000000,
		SURF_FLAG_NODROP = 0x00000000,
		SURF_FLAG_NONSOLID = 0x00004000,
		SURF_FLAG_NOGRAPPLE = 0x00008000,
		SURF_FLAG_DETAIL = 0x00000000,
		SURF_FLAG_STRUCTURAL = 0x00000000,
		SURF_FLAG_PORTAL = 0x80000000,
		SURF_FLAG_CANSHOOTCLIP = 0x00000000,
		SURF_FLAG_ORIGIN = 0x00000000,
		SURF_FLAG_SKY = 0x00000004,
		SURF_FLAG_NOCASTSHADOW = 0x00040000,
		SURF_FLAG_PHYSICSGEOM = 0x00000000,
		SURF_FLAG_LIGHTPORTAL = 0x00000000,
		SURF_FLAG_OUTDOORBOUNDS = 0x00000000,
		SURF_FLAG_SLICK = 0x00000002,
		SURF_FLAG_NOIMPACT = 0x00000010,
		SURF_FLAG_NOMARKS = 0x00000020,
		SURF_FLAG_NOPENETRATE = 0x00000100,
		SURF_FLAG_LADDER = 0x00000008,
		SURF_FLAG_NODAMAGE = 0x00000001,
		SURF_FLAG_MANTLEON = 0x04000000,
		SURF_FLAG_MANTLEOVER = 0x08000000,
		SURF_FLAG_STAIRS = 0x00000200,
		SURF_FLAG_SOFT = 0x00001000,
		SURF_FLAG_NOSTEPS = 0x00002000,
		SURF_FLAG_NODRAW = 0x00000080,
		SURF_FLAG_NOLIGHTMAP = 0x00000400,
		SURF_FLAG_NODLIGHT = 0x00020000,
		SURF_FLAG_TRANSSORT = 0x00080000,
	};

	struct ClipMaterial
	{
		const char* name;
		int surfaceFlags;
		int contents;
	}; 
	struct cLeafBrushNodeLeaf_t
	{
		unsigned short* brushes;
	};

	struct cLeafBrushNodeChildren_t
	{
		float dist;
		float range;
		unsigned short childOffset[2];
	};

	union cLeafBrushNodeData_t
	{
		cLeafBrushNodeLeaf_t leaf;
		cLeafBrushNodeChildren_t children;
	};

	struct cLeafBrushNode_s
	{
		unsigned char axis;
		short leafBrushCount;
		int contents;
		cLeafBrushNodeData_t data;
	};

	typedef unsigned short LeafBrush;

	struct BrushesCollisionTree
	{
		unsigned int leafbrushNodesCount;
		cLeafBrushNode_s* leafbrushNodes;
		unsigned int numLeafBrushes;
		LeafBrush* leafbrushes;
	};

	union CollisionAabbTreeIndex
	{
		int firstChildIndex;
		int partitionIndex;
	};

	struct CollisionAabbTree
	{
		float midPoint[3];
		unsigned short materialIndex;
		unsigned short childCount;
		float halfSize[3];
		CollisionAabbTreeIndex u;
	}; 

	struct PatchesCollisionTree
	{
		int aabbTreeCount;
		CollisionAabbTree* aabbTrees;
	};

	struct SModelAabbNode
	{
		Bounds bounds;
		unsigned short firstChild;
		unsigned short childCount;
	};

	struct SModelsCollisionTree
	{
		unsigned short numStaticModels;
		unsigned short smodelNodeCount;
		SModelAabbNode* smodelNodes;
	};

	struct cbrushside_t
	{
		unsigned int planeIndex;
		unsigned short materialNum;
		unsigned char firstAdjacentSideOffset;
		unsigned char edgeCount;
	};

	typedef unsigned char cbrushedge_t;

	struct cbrush_t
	{
		unsigned short numsides;
		unsigned short glassPieceIndex;
		cbrushside_t* sides;
		cbrushedge_t* baseAdjacentSide;
		short axialMaterialNum[2][3];
		unsigned char firstAdjacentSideOffsets[2][3];
		unsigned char edgeCount[2][3];
	};;

	struct BrushesCollisionData
	{
		unsigned int numBrushSides;
		cbrushside_t* brushSides;
		unsigned int numBrushEdges;
		cbrushedge_t* brushEdges;
		unsigned int numBrushes;
		cbrush_t* brushes;
		Bounds* brushBounds;
		int* brushContents;
	};

	static_assert(offsetof(BrushesCollisionData, brushEdges) == 24);

	static_assert(sizeof(BrushesCollisionData) == 0x40);

	struct CollisionBorder
	{
		float distEq[3];
		float zBase;
		float zSlope;
		float start;
		float length;
	};

	struct CollisionPartition
	{
		unsigned char triCount;
		unsigned char borderCount;
		unsigned char firstVertSegment;
		int firstTri;
		CollisionBorder* borders;
	};

	struct PatchesCollisionData
	{
		unsigned int vertCount;
		vec3_t* verts;
		int triCount;
		unsigned short* triIndices;
		unsigned char* triEdgeIsWalkable;
		int borderCount;
		CollisionBorder* borders;
		int partitionCount;
		CollisionPartition* partitions;
	}; 

	struct cStaticModel_s
	{
		void* xmodel;
		float origin[3];
		float invScaledAxis[3][3];
		Bounds absBounds;
		int lightingHandle;
	}; 

	struct SModelsCollisionData
	{
		unsigned int numStaticModels;
		cStaticModel_s* staticModelList;
	}; 

	struct ClipInfo
	{
		int planeCount;
		cplane_s* planes;
		unsigned int numMaterials;
		ClipMaterial* materials;
		BrushesCollisionTree bCollisionTree;
		PatchesCollisionTree pCollisionTree;
		SModelsCollisionTree sCollisionTree;
		BrushesCollisionData bCollisionData;
		PatchesCollisionData pCollisionData;
		SModelsCollisionData sCollisionData;
	};

	struct /*alignas(128)*/ clipMap_t
	{
		const char* name; // 0
		int isInUse; // 8
		ClipInfo info; // 16
		ClipInfo* pInfo; // 264
	};

	static_assert(sizeof(GfxVertex) == 32);

	struct materialCommands_t
	{
		GfxVertex verts[5450];
		unsigned __int16 indices[1048576];
		int vertDeclType;
		unsigned int vertexSize;
		unsigned int indexCount;
		unsigned int vertexCount;
		unsigned int firstVertex;
		unsigned int lastVertex;
	};

	static_assert(offsetof(materialCommands_t, indices) == 174400);
	static_assert(offsetof(materialCommands_t, indexCount) == 2271560);
	static_assert(offsetof(materialCommands_t, vertexCount) == 2271564);

	struct playerState_s
	{
		char __pad0[48];
		unsigned short gravity;
		char __pad1[34];
		int pm_flags;
		char __pad2[40];
		vec3_t origin;
		vec3_t velocity;
	};

	static_assert(offsetof(playerState_s, origin) == 128);

	struct SprintState_s
	{
		int sprintButtonUpRequired;
		int sprintDelay;
		int lastSprintStart;
		int lastSprintEnd;
		int sprintStartMaxLength;
	};

	struct usercmd_s
	{
		int serverTime;
		int buttons;
		char __pad0[20];
		char forwardmove;
		char rightmove;
		char __pad1[2];
		float unk_float;
		char __pad2[28];
	};

	struct pmove_t
	{
		playerState_s* ps;
		usercmd_s cmd;
		usercmd_s oldcmd;
		int tracemask;
		int numtouch;
		int touchents[32];
		Bounds bounds;
		char __pad0[28];
		char handler;
		char __pad1[0x180];
	};

	//static_assert(sizeof(pmove_t) == 328);
	static_assert(offsetof(pmove_t, handler) == 324);
	static_assert(offsetof(pmove_t, bounds) == 272);
	static_assert(offsetof(pmove_t, tracemask) == 136);

	struct trace_t
	{
		float fraction;
		float normal[3];
		int surfaceFlags;
		int contents;
		int hitType;
		unsigned short hitId;
		char __pad1[11];
		bool allsolid;
		bool startsolid;
		bool walkable;
		char __pad0[8];
	};

	struct pml_t
	{
		float forward[3];
		float right[3];
		float up[3];
		float frametime;
		int msec;
		int walking;
		int groundPlane;
		trace_t groundTrace;
		float previous_origin[3];
		float previous_velocity[3];
		float wishdir[3];
		float platformUp[3];
		float impactSpeed;
		int flinch;
	};

	static_assert(offsetof(pml_t, frametime) == 36);
	static_assert(offsetof(pml_t, groundTrace) == 52);
	static_assert(offsetof(pml_t, flinch) == 156);

	enum Sys_Folder : std::int32_t
	{
		SF_ZONE = 0x0,
		SF_ZONE_LOC = 0x1,
		SF_VIDEO = 0x2,
		SF_VIDEO_LOC = 0x3,
		SF_PAKFILE = 0x4,
		SF_PAKFILE_LOC = 0x5,
		SF_ZONE_REGION = 0x6,
		SF_COUNT = 0x7,
	};

	enum FileSysResult : std::int32_t
	{
		FILESYSRESULT_SUCCESS = 0x0,
		FILESYSRESULT_EOF = 0x1,
		FILESYSRESULT_ERROR = 0x2,
	};

	struct DB_IFileSysFile
	{
		void* file;
		uint64_t last_read;
		uint64_t bytes_read;
	};

	static_assert(sizeof(DB_IFileSysFile) == 24);

	struct DB_FileSysInterface;

	// this is a best guess, interface doesn't match up exactly w/other games (IW8, T9)
	struct DB_FileSysInterface_vtbl
	{
		DB_IFileSysFile* (__fastcall* OpenFile)(DB_FileSysInterface* _this, Sys_Folder folder, const char* filename);
		FileSysResult (__fastcall* Read)(DB_FileSysInterface* _this, DB_IFileSysFile* handle, unsigned __int64 offset, unsigned __int64 size, void* dest);
		FileSysResult (__fastcall* Tell)(DB_FileSysInterface* _this, DB_IFileSysFile* handle, unsigned __int64* bytesRead);
		__int64 (__fastcall* Size)(DB_FileSysInterface* _this, DB_IFileSysFile* handle);
		void (__fastcall* Close)(DB_FileSysInterface* _this, DB_IFileSysFile* handle);
		bool (__fastcall* Exists)(DB_FileSysInterface* _this, Sys_Folder folder, const char* filename);
	};

	struct DB_FileSysInterface
	{
		DB_FileSysInterface_vtbl* vftbl;
	};

	struct map_t
	{
		const char* name;
		int id;
		int unk;
	};

	static_assert(sizeof(map_t) == 0x10);

	__declspec(align(8)) struct DiskFile
	{
		DWORD status;
		HANDLE handle;
		_OVERLAPPED overlapped;
	};

	struct language_values
	{
		const char* name;
		const char* shortname;
		const char* prefix1;
		const char* prefix2;
		const char* prefix3;
		char is_supported;
		char __pad0[0x7];
	};

	enum language_t
	{
		LANGUAGE_ENGLISH = 0,
		LANGUAGE_FRENCH = 1,
		LANGUAGE_GERMAN = 2,
		LANGUAGE_ITALIAN = 3,
		LANGUAGE_SPANISH = 4,
		LANGUAGE_RUSSIAN = 5,
		LANGUAGE_POLISH = 6,
		LANGUAGE_PORTUGUESE = 7,
		LANGUAGE_JAPANESE_FULL = 8,
		LANGUAGE_JAPANESE_PARTIAL = 9,
		LANGUAGE_TRADITIONAL_CHINESE = 10,
		LANGUAGE_SIMPLIFIED_CHINESE = 11,
		LANGUAGE_ARABIC = 12,
		LANGUAGE_CZECH = 13,
		LANGUAGE_SPANISHNA = 14,
		LANGUAGE_KOREAN = 15,
		LANGUAGE_ENGLISH_SAFE = 16,
		LANGUAGE_RUSSIAN_PARTIAL = 17,
		LANGUAGE_COUNT
	};

	struct rectDef_s
	{
		float x;
		float y;
		float w;
		float h;
		int horzAlign;
		int vertAlign;
	};

	enum PMem_Direction
	{
		PHYS_ALLOC_LOW = 0x0,
		PHYS_ALLOC_HIGH = 0x1,
		PHYS_ALLOC_COUNT = 0x2,
	};

	enum PMem_Source
	{
		PMEM_SOURCE_EXTERNAL = 0x0,
		PMEM_SOURCE_DATABASE = 0x1,
		PMEM_SOURCE_DEFAULT_LOW = 0x2,
		PMEM_SOURCE_DEFAULT_HIGH = 0x3,
		PMEM_SOURCE_MOVIE = 0x4,
		PMEM_SOURCE_SCRIPT = 0x5,
		PMEM_SOURCE_UNK5 = 0x5,
		PMEM_SOURCE_UNK6 = 0x6,
		PMEM_SOURCE_UNK7 = 0x7,
		PMEM_SOURCE_UNK8 = 0x8,
		PMEM_SOURCE_CUSTOMIZATION = 0x9,
	};

	struct PhysicalMemoryAllocation
	{
		const char* name;
		char __pad0[16];
		unsigned __int64 pos;
		char __pad1[8];
	}; static_assert(sizeof(PhysicalMemoryAllocation) == 40);

	struct PhysicalMemoryPrim
	{
		const char* name;
		unsigned int allocListCount;
		char __pad0[4];
		unsigned char* buf;
		char __pad1[8];
		int unk1;
		char __pad2[4];
		unsigned __int64 pos;
		PhysicalMemoryAllocation allocList[32];
	}; static_assert(sizeof(PhysicalMemoryPrim) == 1328);

	struct PhysicalMemory
	{
		PhysicalMemoryPrim prim[2];
	}; static_assert(sizeof(PhysicalMemory) == 0xA60);

	union GamerProfileDataUnion
	{
		unsigned __int8 byteVal;
		bool boolVal;
		__int16 shortVal;
		int intVal;
		float floatVal;
		const char* stringVal;
	};

	enum level_number
	{
		LEVEL_TRAINER,
		LEVEL_ROADKILL,
		LEVEL_CLIFFHANGER,
		LEVEL_AIRPORT,
		LEVEL_FAVELA,

		LEVEL_INVASION,
		LEVEL_FAVELA_ESCAPE,
		LEVEL_ARCADIA,
		LEVEL_OILRIG,
		LEVEL_GULAG,
		LEVEL_DCBURNING,

		LEVEL_CONTINGENCY,
		LEVEL_DCEMP,
		LEVEL_DC_WHITEHOUSE,
		LEVEL_ESTATE,
		LEVEL_BONEYARD,
		LEVEL_AF_CAVES,
		LEVEL_AF_CHASE,

		LEVEL_ENDING,

		LEVEL_COUNT,
	};

	enum GamerProfileDataType : __int32
	{
		TYPE_INVALID = 0x0,
		TYPE_BYTE = 0x1,
		TYPE_BOOL = 0x2,
		TYPE_SHORT = 0x3,
		TYPE_INT = 0x4,
		TYPE_FLOAT = 0x5,
		TYPE_STRING = 0x6,
		TYPE_BUFFER = 0x7,
		TYPE_FLAG = 0x8,
	};

	struct GamerProfileData
	{
		GamerProfileDataType type;
		GamerProfileDataUnion u;
	};

	namespace hks
	{
		struct lua_State;
		struct HashTable;
		struct cclosure;

		struct GenericChunkHeader
		{
			unsigned __int64 m_flags;
		};

		struct ChunkHeader : GenericChunkHeader
		{
			ChunkHeader* m_next;
		};

		struct UserData : ChunkHeader
		{
			unsigned __int64 m_envAndSizeOffsetHighBits;
			unsigned __int64 m_metaAndSizeOffsetLowBits;
			char m_data[8];
		};

		struct InternString
		{
			unsigned __int64 m_flags;
			unsigned __int64 m_lengthbits;
			unsigned int m_hash;
			char m_data[30];
		};

		union HksValue
		{
			cclosure* cClosure;
			void* closure;
			UserData* userData;
			HashTable* table;
			void* tstruct;
			InternString* str;
			void* thread;
			void* ptr;
			float number;
			long long i64;
			unsigned long long ui64;
			unsigned int native;
			bool boolean;
		};

		enum HksObjectType
		{
			TANY = 0xFFFFFFFE,
			TNONE = 0xFFFFFFFF,
			TNIL = 0x0,
			TBOOLEAN = 0x1,
			TLIGHTUSERDATA = 0x2,
			TNUMBER = 0x3,
			TSTRING = 0x4,
			TTABLE = 0x5,
			TFUNCTION = 0x6,  // idk
			TUSERDATA = 0x7,
			TTHREAD = 0x8,
			TIFUNCTION = 0x9, // Lua function
			TCFUNCTION = 0xA, // C function
			TUI64 = 0xB,
			TSTRUCT = 0xC,
			NUM_TYPE_OBJECTS = 0xE,
		};

		struct HksObject
		{
			HksObjectType t;
			HksValue v;
		};

		const struct hksInstruction
		{
			unsigned int code;
		};

		struct ActivationRecord
		{
			HksObject* m_base;
			const hksInstruction* m_returnAddress;
			__int16 m_tailCallDepth;
			__int16 m_numVarargs;
			int m_numExpectedReturns;
		};

		struct CallStack
		{
			ActivationRecord* m_records;
			ActivationRecord* m_lastrecord;
			ActivationRecord* m_current;
			const hksInstruction* m_current_lua_pc;
			const hksInstruction* m_hook_return_addr;
			int m_hook_level;
		};

		struct ApiStack
		{
			HksObject* top;
			HksObject* base;
			HksObject* alloc_top;
			HksObject* bottom;
		};

		struct UpValue : ChunkHeader
		{
			HksObject m_storage;
			HksObject* loc;
			UpValue* m_next;
		};

		struct CallSite
		{
			_SETJMP_FLOAT128 m_jumpBuffer[16];
			CallSite* m_prev;
		};

		enum Status
		{
			NEW = 0x1,
			RUNNING = 0x2,
			YIELDED = 0x3,
			DEAD_ERROR = 0x4,
		};

		enum HksError
		{
			HKS_NO_ERROR = 0x0,
			HKS_ERRSYNTAX = 0xFFFFFFFC,
			HKS_ERRFILE = 0xFFFFFFFB,
			HKS_ERRRUN = 0xFFFFFF9C,
			HKS_ERRMEM = 0xFFFFFF38,
			HKS_ERRERR = 0xFFFFFED4,
			HKS_THROWING_ERROR = 0xFFFFFE0C,
			HKS_GC_YIELD = 0x1,
		};

		struct lua_Debug
		{
			int event;
			const char* name;
			const char* namewhat;
			const char* what;
			const char* source;
			int currentline;
			int nups;
			int nparams;
			int ishksfunc;
			int linedefined;
			int lastlinedefined;
			char short_src[512];
			int callstack_level;
			int is_tail_call;
		};

		using lua_function = int(__fastcall*)(lua_State*);

		struct luaL_Reg
		{
			const char* name;
			lua_function function;
		};

		struct Node
		{
			HksObject m_key;
			HksObject m_value;
		};

		struct Metatable
		{
		};

		struct HashTable : ChunkHeader
		{
			Metatable* m_meta;
			unsigned int m_version;
			unsigned int m_mask;
			Node* m_hashPart;
			HksObject* m_arrayPart;
			unsigned int m_arraySize;
			Node* m_freeNode;
		};

		struct cclosure : ChunkHeader
		{
			lua_function m_function;
			HashTable* m_env;
			__int16 m_numUpvalues;
			__int16 m_flags;
			InternString* m_name;
			HksObject m_upvalues[1];
		};

		enum HksCompilerSettings_BytecodeSharingFormat
		{
			BYTECODE_DEFAULT = 0x0,
			BYTECODE_INPLACE = 0x1,
			BYTECODE_REFERENCED = 0x2,
		};

		enum HksCompilerSettings_IntLiteralOptions
		{
			INT_LITERALS_NONE = 0x0,
			INT_LITERALS_LUD = 0x1,
			INT_LITERALS_32BIT = 0x1,
			INT_LITERALS_UI64 = 0x2,
			INT_LITERALS_64BIT = 0x2,
			INT_LITERALS_ALL = 0x3,
		};

		struct HksCompilerSettings
		{
			int m_emitStructCode;
			const char** m_stripNames;
			int m_emitGlobalMemoization;
			int _m_isHksGlobalMemoTestingMode;
			HksCompilerSettings_BytecodeSharingFormat m_bytecodeSharingFormat;
			HksCompilerSettings_IntLiteralOptions m_enableIntLiterals;
			int(__fastcall* m_debugMap)(const char*, int);
		};

		enum HksBytecodeSharingMode
		{
			HKS_BYTECODE_SHARING_OFF = 0x0,
			HKS_BYTECODE_SHARING_ON = 0x1,
			HKS_BYTECODE_SHARING_SECURE = 0x2,
		};

		struct HksGcWeights
		{
			int m_removeString;
			int m_finalizeUserdataNoMM;
			int m_finalizeUserdataGcMM;
			int m_cleanCoroutine;
			int m_removeWeak;
			int m_markObject;
			int m_traverseString;
			int m_traverseUserdata;
			int m_traverseCoroutine;
			int m_traverseWeakTable;
			int m_freeChunk;
			int m_sweepTraverse;
		};

		struct GarbageCollector_Stack
		{
			void* m_storage;
			unsigned int m_numEntries;
			unsigned int m_numAllocated;
		};

		struct ProtoList
		{
			void** m_protoList;
			unsigned __int16 m_protoSize;
			unsigned __int16 m_protoAllocSize;
		};

		struct GarbageCollector
		{
			int m_target;
			int m_stepsLeft;
			int m_stepLimit;
			HksGcWeights m_costs;
			int m_unit;
			_SETJMP_FLOAT128(*m_jumpPoint)[16];
			lua_State* m_mainState;
			lua_State* m_finalizerState;
			void* m_memory;
			int m_phase;
			GarbageCollector_Stack m_resumeStack;
			GarbageCollector_Stack m_greyStack;
			GarbageCollector_Stack m_remarkStack;
			GarbageCollector_Stack m_weakStack;
			int m_finalizing;
			HksObject m_safeTableValue;
			lua_State* m_startOfStateStackList;
			lua_State* m_endOfStateStackList;
			lua_State* m_currentState;
			HksObject m_safeValue;
			void* m_compiler;
			void* m_bytecodeReader;
			void* m_bytecodeWriter;
			int m_pauseMultiplier;
			int m_stepMultiplier;
			bool m_stopped;
			int(__fastcall* m_gcPolicy)(lua_State*);
			unsigned __int64 m_pauseTriggerMemoryUsage;
			int m_stepTriggerCountdown;
			unsigned int m_stringTableIndex;
			unsigned int m_stringTableSize;
			UserData* m_lastBlackUD;
			UserData* m_activeUD;
		};

		enum MemoryManager_ChunkColor
		{
			RED = 0x0,
			BLACK = 0x1,
		};

		struct ChunkList
		{
			ChunkHeader m_prevToStart;
		};

		enum Hks_DeleteCheckingMode
		{
			HKS_DELETE_CHECKING_OFF = 0x0,
			HKS_DELETE_CHECKING_ACCURATE = 0x1,
			HKS_DELETE_CHECKING_SAFE = 0x2,
		};

		struct MemoryManager
		{
			void* (__fastcall* m_allocator)(void*, void*, unsigned __int64, unsigned __int64);
			void* m_allocatorUd;
			MemoryManager_ChunkColor m_chunkColor;
			unsigned __int64 m_used;
			unsigned __int64 m_highwatermark;
			ChunkList m_allocationList;
			ChunkList m_sweepList;
			ChunkHeader* m_lastKeptChunk;
			lua_State* m_state;
			ChunkList m_deletedList;
			int m_deleteMode;
			Hks_DeleteCheckingMode m_deleteCheckingMode;
		};

		struct StaticStringCache
		{
			HksObject m_objects[41];
		};

		enum HksBytecodeEndianness
		{
			HKS_BYTECODE_DEFAULT_ENDIAN = 0x0,
			HKS_BYTECODE_BIG_ENDIAN = 0x1,
			HKS_BYTECODE_LITTLE_ENDIAN = 0x2,
		};

		struct RuntimeProfileData_Stats
		{
			unsigned __int64 hksTime;
			unsigned __int64 callbackTime;
			unsigned __int64 gcTime;
			unsigned __int64 cFinalizerTime;
			unsigned __int64 compilerTime;
			unsigned int hkssTimeSamples;
			unsigned int callbackTimeSamples;
			unsigned int gcTimeSamples;
			unsigned int compilerTimeSamples;
			unsigned int num_newuserdata;
			unsigned int num_tablerehash;
			unsigned int num_pushstring;
			unsigned int num_pushcfunction;
			unsigned int num_newtables;
		};

		struct RuntimeProfileData
		{
			__int64 stackDepth;
			__int64 callbackDepth;
			unsigned __int64 lastTimer;
			RuntimeProfileData_Stats frameStats;
			unsigned __int64 gcStartTime;
			unsigned __int64 finalizerStartTime;
			unsigned __int64 compilerStartTime;
			unsigned __int64 compilerStartGCTime;
			unsigned __int64 compilerStartGCFinalizerTime;
			unsigned __int64 compilerCallbackStartTime;
			__int64 compilerDepth;
			void* outFile;
			lua_State* rootState;
		};

		struct HksGlobal
		{
			MemoryManager m_memory;
			GarbageCollector m_collector;
			StringTable m_stringTable;
			HksBytecodeSharingMode m_bytecodeSharingMode;
			unsigned int m_tableVersionInitializer;
			HksObject m_registry;
			ProtoList m_protoList;
			HashTable* m_structProtoByName;
			ChunkList m_userDataList;
			lua_State* m_root;
			StaticStringCache m_staticStringCache;
			void* m_debugger;
			void* m_profiler;
			RuntimeProfileData m_runProfilerData;
			HksCompilerSettings m_compilerSettings;
			int(__fastcall* m_panicFunction)(lua_State*);
			void* m_luaplusObjectList;
			int m_heapAssertionFrequency;
			int m_heapAssertionCount;
			void (*m_logFunction)(lua_State*, const char*, ...);
			HksBytecodeEndianness m_bytecodeDumpEndianness;
		};

		struct lua_State : ChunkHeader
		{
			HksGlobal* m_global;
			CallStack m_callStack;
			ApiStack m_apistack;
			UpValue* pending;
			HksObject globals;
			HksObject m_cEnv;
			CallSite* m_callsites;
			int m_numberOfCCalls;
			void* m_context;
			InternString* m_name;
			lua_State* m_nextState;
			lua_State* m_nextStateStack;
			Status m_status;
			HksError m_error;
		};
	}
}
