#pragma once

#define WEAK __declspec(selectany)

namespace game
{
	// Functions
	
	WEAK symbol<void(int type, VariableUnion u)> AddRefToValue{0x5C0EB0};
	WEAK symbol<void(unsigned int id)> AddRefToObject{0x5C0EA0};
	WEAK symbol<unsigned int(unsigned int id)> AllocThread{0x5C1200};
	WEAK symbol<void(int type, VariableUnion u)> RemoveRefToValue{0x5C29B0};
	WEAK symbol<void(unsigned int id)> RemoveRefToObject{0x5C28A0};

	WEAK symbol<void(int localClientNum, const char* text)> Cbuf_AddText{0x59A050};

	WEAK symbol<void(int localClientNum, const char* message)> CG_GameMessage{0x37F450};
	WEAK symbol<void(int localClientNum, const char* message)> CG_GameMessageBold{0x37F1B0};

	WEAK symbol<void(const char* cmdName, void(), cmd_function_s* allocedCmd)> Cmd_AddCommandInternal{0x59A5F0};
	WEAK symbol<void(int localClientNum, int controllerIndex, const char* text)> Cmd_ExecuteSingleCommand{0x59ABA0};

	WEAK symbol<void(errorParm code, const char* message, ...)> Com_Error{0x5A2D80};
	WEAK symbol<void()> Com_Quit_f{0x5A50D0};

	WEAK symbol<void(XAssetType type, void(__cdecl* func)(game::XAssetHeader, void*), const void* inData, bool includeOverride)>
		DB_EnumXAssets_Internal{0x4129F0};
	WEAK symbol<const char*(const XAsset* asset)> DB_GetXAssetName{0x3E4090};
	WEAK symbol<void(XZoneInfo* zoneInfo, unsigned int zoneCount, DBSyncMode syncMode)> DB_LoadXAssets{0x414FF0};
	WEAK symbol<XAssetHeader(XAssetType type, const char* name, int allowCreateDefault)> DB_FindXAssetHeader{0x412F60};
	WEAK symbol<int(const RawFile* rawfile)> DB_GetRawFileLen{0x413D80};
	WEAK symbol<int(const RawFile* rawfile, char* buf, int size)> DB_GetRawBuffer{0x413C40};

	WEAK symbol<dvar_t*(const char* name)> Dvar_FindVar{0x618F90};
	WEAK symbol<void(char* buffer, int index)> Dvar_GetCombinedString{0x5A75D0};
	WEAK symbol<dvar_t*(int hash, const char* name, bool value, unsigned int flags)> Dvar_RegisterBool{0x617BB0};
	WEAK symbol<dvar_t*(int hash, const char* name, int value, int min, int max, unsigned int flags)> Dvar_RegisterInt{0x618090};
	WEAK symbol<dvar_t* (int hash, const char* dvarName, float value, float min, float max, unsigned int flags)>
		Dvar_RegisterFloat{0x617F80};
	WEAK symbol<dvar_t* (int hash, const char* dvarName, const char* value, unsigned int flags)>
		Dvar_RegisterString{0x618170};
	WEAK symbol<dvar_t*(int dvarName, const char* a2, float x, float y, float z, float w, float min, float max,
		unsigned int flags)> Dvar_RegisterVec4{0x6185F0};
	WEAK symbol<const char* (dvar_t* dvar, void* a2, void* value)> Dvar_ValueToString{0x61B8F0};
	WEAK symbol<void(int hash, const char* name, const char* buffer)> Dvar_SetCommand{0x61A5C0};
	WEAK symbol<void(const char* dvarName, const char* string, DvarSetSource source)> Dvar_SetFromStringFromSource{0x61A910};

	WEAK symbol<int(const char* fname)> generateHashValue{0x343D20};

	WEAK symbol<bool()> CL_IsCgameInitialized{0x3CA0C0};

	WEAK symbol<unsigned int (unsigned int parentId, unsigned int name)> FindVariable{0x5C1D50};
	WEAK symbol<unsigned int(int entnum, unsigned int classnum)> FindEntityId{0x5C1C50};
	WEAK symbol<void(VariableValue* result, unsigned int classnum, int entnum, int offset)> GetEntityFieldValue{0x5C6100};

	WEAK symbol<unsigned int(const char* name)> G_GetWeaponForName{0x51B260};
	WEAK symbol<int(void* ps, unsigned int weapon, int a3, int a4, __int64 a5, int a6)> 
		G_GivePlayerWeapon{0x51B660};
	WEAK symbol<void(void* ps, const unsigned int weapon, int hadWeapon)> G_InitializeAmmo{0x4C4110};
	WEAK symbol<void(int clientNum, const unsigned int weapon)> G_SelectWeapon{0x51C0D0};

	WEAK symbol<char*(char* string)> I_CleanStr{0x620660};

	WEAK symbol<char* (GfxImage* image, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipCount, 
		uint32_t imageFlags, DXGI_FORMAT imageFormat, int a8, const char* name, const void* initData)> Image_Setup{0x74B2A0};

	WEAK symbol<void(int clientNum, const char* menu, int a3, int a4, unsigned int a5)> LUI_OpenMenu{0x5F0EE0};
	WEAK symbol<bool(int clientNum, const char* menu)> Menu_IsMenuOpenAndVisible{0x5EE1A0};

	WEAK symbol<Material*(const char* material)> Material_RegisterHandle{0x759BA0};

	WEAK symbol<const float* (const float* v)> Scr_AllocVector{0x5C3220};
	WEAK symbol<void()> Scr_ClearOutParams{0x5C6E50};
	WEAK symbol<scr_entref_t(unsigned int entId)> Scr_GetEntityIdRef{0x5C56C0};
	WEAK symbol<int(unsigned int classnum, int entnum, int offset)> Scr_SetObjectField{0x512190};
	WEAK symbol<void(unsigned int id, scr_string_t stringValue, unsigned int paramcount)> Scr_NotifyId{0x5C8240};

	WEAK symbol<unsigned int(unsigned int localId, const char* pos, unsigned int paramcount)> VM_Execute{0x5C8DB0};

	WEAK symbol<void(float x, float y, float width, float height, float s0, float t0, float s1, float t1,
					 float* color, Material* material)> R_AddCmdDrawStretchPic{0x3C9710};
	WEAK symbol<void(float x, float y, float width, float height, float s0, float t0, float s1, float t1,
					 float angle, float* color, Material* material)> R_AddCmdDrawStretchPicRotateXY{0x3C99B0};
	WEAK symbol<void(const char* text, int maxChars, Font_s* font, float x, float y, float xScale, float yScale, 
					 float rotation, float* color, int style)> R_AddCmdDrawText{0x76C660};
	WEAK symbol<void(rectangle* rect, float a2, float a3, float a4, float a5, float* color, Material* material)> R_DrawRectangle{0x76A280};
	WEAK symbol<void(const char* text, int maxChars, Font_s* font, int fontSize, float x, float y, float xScale, float yScale, float rotation, 
		const float* color, int style, int cursorPos, char cursor)> R_AddCmdDrawTextWithCursor{0x76CAF0};
	WEAK symbol<Font_s*(const char* font, int size)> R_RegisterFont{0x746FE0};
	WEAK symbol<int(const char* text, int maxChars, Font_s* font)> R_TextWidth{0x7472A0};
	WEAK symbol<void()> R_SyncRenderThread{0x76E7D0};
	WEAK symbol<void(const void* obj, void* pose, unsigned int entnum, unsigned int renderFxFlags, float* lightingOrigin, 
		float materialTime, __int64 a7, __int64 a8)> R_AddDObjToScene{0x775C40};

	WEAK symbol<ScreenPlacement* ()> ScrPlace_GetViewPlacement{0x3E16A0};

	WEAK symbol<const char*(scr_string_t stringValue)> SL_ConvertToString{0x5BFBB0};
	WEAK symbol<scr_string_t(const char* str, unsigned int user)> SL_GetString{0x5C0170};

	WEAK symbol<bool()> SV_Loaded{0x6B3860};

	WEAK symbol<void()> Sys_ShowConsole{0x633080};
	WEAK symbol<bool()> Sys_IsDatabaseReady2{0x5A9FE0};

	WEAK symbol<const char*(const char* string)> UI_SafeTranslateString{0x5A2930};
	WEAK symbol<int(int localClientNum, const char* sound)> UI_PlayLocalSoundAlias{0x606080};

	WEAK symbol<void*(jmp_buf* Buf, int Value)> longjmp{0x89EED0};
	WEAK symbol<int(jmp_buf* Buf)> _setjmp{0x8EC2E0};

	// Variables

	WEAK symbol<cmd_function_s*> cmd_functions{0xAD17BB8};
	WEAK symbol<CmdArgs> cmd_args{0xAD17A60};

	WEAK symbol<const char*> g_assetNames{0xBEF280};
	WEAK symbol<int> g_poolSize{0xBF2E40};

	WEAK symbol<gentity_s> g_entities{0x52DDDA0};

	WEAK symbol<DWORD> threadIds{0xB11DC80};

	WEAK symbol<GfxDrawMethod_s> gfxDrawMethod{0xEDF9E00};

	WEAK symbol<int> keyCatchers{0x203F3C0};

	WEAK symbol<PlayerKeyState> playerKeys{0x1E8767C};

	WEAK symbol<int> dvarCount{0xBFBB310};
	WEAK symbol<dvar_t*> sortedDvars{0xBFBB320};

	WEAK symbol<unsigned int> levelEntityId{0xB5E0B30};
	WEAK symbol<int> g_script_error_level{0xBA9CC24};
	WEAK symbol<jmp_buf> g_script_error{0xBA9CD40};
	WEAK symbol<scr_classStruct_t> g_classMap{0xBF95C0};

	WEAK symbol<scrVarGlob_t> scr_VarGlob{0xB617C00};
	WEAK symbol<scrVmPub_t> scr_VmPub{0xBA9EE40};
	WEAK symbol<function_stack_t> scr_function_stack{0xBAA93C0};
}