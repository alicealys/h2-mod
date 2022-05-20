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
		char __pad0[0x8C];
		vec3_t velocity;
		char __pad1[0xE870];
		char flags;
	};

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

	struct EntityState
	{
		char entityNum;
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
		EntityState s;
		char __pad0[0x1B];
		vec3_t origin;
		char __pad1[0x98];
		Bounds box;
		char __pad2[0x4];
		Bounds absBox;
		char __pad3[0x24];
		gclient_s* client;
		char __pad4[0x30];
		scr_string_t script_classname;
		char __pad5[0x18];
		char flags;
		char __pad6[0x188];
	}; // size = 760

	//auto a = sizeof(gentity_s);

	static_assert(sizeof(gentity_s) == 760);

	struct pathlink_s
	{
		char __pad0[0x4];
		unsigned __int16 nodeNum;
		char __pad[0x6];
	};

	static_assert(sizeof(pathlink_s) == 12);

	struct pathnode_t
	{
		unsigned __int16 type;
		unsigned int spawnflags;
		unsigned int targetname;
		unsigned int script_linkName;
		unsigned int script_noteworthy;
		unsigned int target;
		unsigned int animscript;
		int animscriptfunc;
		float vLocalOrigin[0x3];
		char __pad0[0x1C];
		unsigned __int16 totalLinkCount;
		char __pad1[0x2];
		pathlink_s* Links;
		char __pad2[0x68];
	}; // size = 192

	static_assert(sizeof(pathnode_t) == 192);

	struct PathData
	{
		const char* name;
		unsigned int nodeCount;
		pathnode_t* nodes;
		// ... 
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
		THREAD_CONTEXT_DATABASE = 0xC,
		THREAD_CONTEXT_STREAM = 0xD,
		THREAD_CONTEXT_SNDSTREAMPACKETCALLBACK = 0xE,
		THREAD_CONTEXT_STATS_WRITE = 0xF,
		THREAD_CONTEXT_COUNT = 0x10,
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
		int name; //00
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

	struct StreamFileName
	{
		unsigned __int16 isLocalized;
		unsigned __int16 fileIndex;
		StreamFileInfo info;
	};

	struct StreamedSound
	{
		StreamFileName filename;
		unsigned int totalMsec;
	};

	union SoundFileRef
	{
		StreamedSound streamSnd;
	};

	struct SoundFile
	{
		char type;
		char exists;
		SoundFileRef u;
	};

	struct snd_alias_t
	{
		const char* aliasName;
		char __pad0[24];
		SoundFile* soundFile;
		char __pad1[198];
		// not gonna map this out...
	};

	struct snd_alias_list_t
	{
		const char* aliasName;
		snd_alias_t* head;
		void* unk;
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
		const char* buffer;
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

	union XAssetHeader
	{
		void* data;
		Material* material;
		Font_s* font;
		RawFile* rawfile;
		ScriptFile* scriptfile;
		StringTable* stringTable;
		LuaFile* luaFile;
		TTF* ttf;
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

	struct XZoneInfo
	{
		const char* name;
		int allocFlags;
		int freeFlags;
	};

	struct scr_entref_t
	{
		unsigned short entnum;
		unsigned short classnum;
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

	union $3FA29451CE6F1FA138A5ABAB84BE9676
	{
		ID3D11Texture1D* linemap;
		ID3D11Texture2D* map;
		ID3D11Texture3D* volmap;
		ID3D11Texture2D* cubemap;
		GfxImageLoadDef* loadDef;
	};

	struct GfxTexture
	{
		$3FA29451CE6F1FA138A5ABAB84BE9676 ___u0;
		ID3D11ShaderResourceView* shaderView;
		ID3D11ShaderResourceView* shaderViewAlternate;
	};

	struct Picmip
	{
		char platform[2];
	};

	struct CardMemory
	{
		int platform[2];
	};

	struct GfxImage
	{
		GfxTexture textures;
		int flags;
		int imageFormat;
		int resourceSize;
		char mapType;
		char semantic;
		char category;
		char flags2;
		Picmip picmip;
		char track;
		//CardMemory cardMemory;
		unsigned short width;
		unsigned short height;
		unsigned short depth;
		unsigned short numElements;
		char pad3[4];
		void* pixelData;
		//GfxImageLoadDef *loadDef;
		uint64_t streams[4];
		const char* name;
	};

	enum PlayerHandIndex
	{
		WEAPON_HAND_RIGHT = 0x0,
		WEAPON_HAND_LEFT = 0x1,
		NUM_WEAPON_HANDS = 0x2,
		WEAPON_HAND_DEFAULT = 0x0,
	};

	struct pmove_t
	{
	};

	struct trace_t
	{
		char __pad0[0x29];
		bool allsolid; // Confirmed in CM_PositionTestCapsuleInTriangle
		bool startsolid; // Confirmed in PM_JitterPoint
	};

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

	struct DB_IFileSysFile;

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
