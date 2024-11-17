#pragma once
#include <d3d11.h>

#define assert_sizeof(__ASSET__, __SIZE__) static_assert(sizeof(__ASSET__) == __SIZE__)
#define assert_offsetof(__ASSET__, __VARIABLE__, __OFFSET__) static_assert(offsetof(__ASSET__, __VARIABLE__) == __OFFSET__)

#pragma warning(disable: 4201)

namespace game
{
	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

	template <std::size_t N>
	struct VecInternal
	{
		float data[N];
	};

	struct dummy
	{
	};

	enum scr_string_t : std::int32_t
	{
	};

	enum XAssetType
	{
		ASSET_TYPE_PHYSPRESET,
		ASSET_TYPE_PHYSCOLLMAP,
		ASSET_TYPE_PHYSWATERPRESET,
		ASSET_TYPE_PHYSWORLDMAP,
		ASSET_TYPE_PHYSCONSTRAINT,
		ASSET_TYPE_XANIM,
		ASSET_TYPE_XMODEL_SURFS,
		ASSET_TYPE_XMODEL,
		ASSET_TYPE_MATERIAL,
		ASSET_TYPE_COMPUTESHADER,
		ASSET_TYPE_VERTEXSHADER,
		ASSET_TYPE_HULLSHADER,
		ASSET_TYPE_DOMAINSHADER,
		ASSET_TYPE_PIXELSHADER,
		ASSET_TYPE_VERTEXDECL,
		ASSET_TYPE_TECHNIQUE_SET,
		ASSET_TYPE_IMAGE,
		ASSET_TYPE_SOUND,
		ASSET_TYPE_SOUNDSUBMIX,
		ASSET_TYPE_SOUND_CURVE,
		ASSET_TYPE_LPF_CURVE,
		ASSET_TYPE_REVERB_CURVE,
		ASSET_TYPE_SOUND_CONTEXT,
		ASSET_TYPE_LOADED_SOUND,
		ASSET_TYPE_COL_MAP_SP,
		ASSET_TYPE_COM_MAP,
		ASSET_TYPE_GLASS_MAP,
		ASSET_TYPE_AIPATHS,
		ASSET_TYPE_VEHICLE_TRACK,
		ASSET_TYPE_MAP_ENTS,
		ASSET_TYPE_FX_MAP,
		ASSET_TYPE_GFX_MAP,
		ASSET_TYPE_LIGHT_DEF,
		ASSET_TYPE_UI_MAP,
		ASSET_TYPE_MENULIST,
		ASSET_TYPE_MENU,
		ASSET_TYPE_ANIMCLASS,
		ASSET_TYPE_LOCALIZE_ENTRY,
		ASSET_TYPE_ATTACHMENT,
		ASSET_TYPE_WEAPON,
		ASSET_TYPE_SNDDRIVERGLOBALS,
		ASSET_TYPE_FX,
		ASSET_TYPE_IMPACT_FX,
		ASSET_TYPE_SURFACE_FX,
		ASSET_TYPE_AITYPE,
		ASSET_TYPE_MPTYPE,
		ASSET_TYPE_CHARACTER,
		ASSET_TYPE_XMODELALIAS,
		ASSET_TYPE_RAWFILE,
		ASSET_TYPE_SCRIPTFILE,
		ASSET_TYPE_STRINGTABLE,
		ASSET_TYPE_LEADERBOARDDEF,
		ASSET_TYPE_VIRTUAL_LEADERBOARD,
		ASSET_TYPE_STRUCTUREDDATADEF,
		ASSET_TYPE_DDL,
		ASSET_TYPE_PROTO,
		ASSET_TYPE_TRACER,
		ASSET_TYPE_VEHICLE,
		ASSET_TYPE_ADDON_MAP_ENTS,
		ASSET_TYPE_NET_CONST_STRINGS,
		ASSET_TYPE_REVERB_PRESET,
		ASSET_TYPE_LUA_FILE,
		ASSET_TYPE_SCRIPTABLE,
		ASSET_TYPE_EQUIPMENT_SND_TABLE,
		ASSET_TYPE_VECTORFIELD,
		ASSET_TYPE_DOPPLER_PRESET,
		ASSET_TYPE_PARTICLE_SIM_ANIMATION,
		ASSET_TYPE_LASER,
		ASSET_TYPE_SKELETONSCRIPT,
		ASSET_TYPE_CLUT,
		ASSET_TYPE_TTF,
		ASSET_TYPE_COUNT
	};

	struct FxEffectDef;

	struct Bounds
	{
		vec3_t midPoint;
		vec3_t halfSize;
	};

	struct PhysPreset
	{
		const char* name;
		char __pad0[32];
		const char* sndAliasPrefix;
		char __pad1[48];
	}; static_assert(sizeof(PhysPreset) == 0x60);

	typedef std::int8_t dm_int8;
	typedef std::uint8_t dm_uint8;
	typedef std::int16_t dm_int16;
	typedef std::uint16_t dm_uint16;
	typedef std::int32_t dm_int32;
	typedef std::uint32_t dm_uint32;
	typedef std::int64_t dm_int64;
	typedef std::uint64_t dm_uint64;
	typedef float dm_float32;

	struct dmFloat3
	{
		dm_float32 x;
		dm_float32 y;
		dm_float32 z;
	};

	struct dmFloat4
	{
		dm_float32 x;
		dm_float32 y;
		dm_float32 z;
		dm_float32 w;
	};

	struct dmMeshNode_anon_fields
	{
		unsigned int axis : 2;
		unsigned int triangleCount : 4;
		unsigned int index : 26;
	};

	union dmMeshNode_anon
	{
		dmMeshNode_anon_fields fields;
		unsigned int packed;
	};

	struct dmMeshNode
	{
		dm_int16 lowerX;
		dm_int16 lowerY;
		dm_int16 lowerZ;
		dm_int16 upperX;
		dm_int16 upperY;
		dm_int16 upperZ;
		dmMeshNode_anon anon;
	}; assert_sizeof(dmMeshNode, 16);

	struct dmMeshTriangle
	{
		dm_int32 i1;
		dm_int32 i2;
		dm_int32 i3;
		dm_int32 w1;
		dm_int32 w2;
		dm_int32 w3;
		dm_int32 materialIndex;
		dm_int32 collisionFlags;
	}; assert_sizeof(dmMeshTriangle, 32);

	struct dmMeshData
	{
		dmMeshNode* m_pRoot;
		dmFloat4* m_aVertices;
		dmMeshTriangle* m_aTriangles;
		dmFloat3 m_center;
		dmFloat3 m_extents;
		dmFloat3 m_unquantize;
		dm_int32 m_nodeCount;
		dm_int32 m_vertexCount;
		dm_int32 m_triangleCount;
		dm_int32 m_height;
		dm_int32 contents;
	}; assert_sizeof(dmMeshData, 0x50);

	struct dmSubEdge
	{
		dm_int8 twinOffset;
		dm_uint8 tail;
		dm_uint8 left;
		dm_uint8 next;
	}; assert_sizeof(dmSubEdge, 4);

	struct dmPolytopeData
	{
		dmFloat4* vec4_array0; // count: m_vertexCount, m_aVertices?
		dmFloat4* vec4_array1; // count: m_faceCount, m_aPlanes?
		dm_uint16* uint16_array0; // count: m_faceCount, m_vertexMaterials? surfaceType? // ALWAYS 0
		dm_uint16* uint16_array1; // count: m_vertexCount, m_vertexMaterials? // ALWAYS 0
		dmSubEdge* m_aSubEdges; // count: m_subEdgeCount
		dm_uint8* m_aFaceSubEdges; // count: m_faceCount
		dmFloat3 m_centroid;
		dm_int32 m_vertexCount;
		dm_int32 m_faceCount;
		dm_int32 m_subEdgeCount;
		float pad1[8];
		int contents;
		int pad2;
	}; assert_sizeof(dmPolytopeData, 0x70);

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
	}; assert_sizeof(PhysCollmap, 0x58);

	struct PhysWaterPreset
	{
		const char* name;
		char __pad0[64];
		FxEffectDef* fx0;
		FxEffectDef* fx1;
		FxEffectDef* fx2;
		FxEffectDef* fx3;
		FxEffectDef* fx4;
		FxEffectDef* fx5;
		FxEffectDef* fx6;
	}; assert_sizeof(PhysWaterPreset, 0x80);

	struct PhysWaterVolumeDef
	{
		PhysWaterPreset* physWaterPreset;
		char __pad0[12];
		scr_string_t string;
		short brushModelIndex;
		char __pad1[6];
	}; assert_sizeof(PhysWaterVolumeDef, 0x20);
	assert_offsetof(PhysWaterVolumeDef, string, 20);

	struct PhysBrushModelPacked
	{
		std::uint64_t p;
	};

	struct PhysBrushModelFields
	{
		short polytopeIndex;
		short unk;
		short worldIndex;
		short meshIndex;
	};

	union PhysBrushModel
	{
		PhysBrushModelPacked packed;
		PhysBrushModelFields fields;
	}; assert_sizeof(PhysBrushModel, 8);

	struct PhysWorld // PhysicsWorld
	{
		const char* name;
		PhysBrushModel* brushModels;
		dmPolytopeData* polytopeDatas;
		dmMeshData* meshDatas;
		PhysWaterVolumeDef* waterVolumeDefs;
		unsigned int brushModelCount;
		unsigned int polytopeCount;
		unsigned int meshDataCount;
		unsigned int waterVolumeDefCount;
	}; assert_sizeof(PhysWorld, 0x38);


	struct PhysConstraint
	{
		const char* name;
		char __pad0[32];
	}; static_assert(sizeof(PhysConstraint) == 0x28);

	struct Packed128
	{
		std::uint64_t p0;
		std::uint64_t p1;
	};

	union GfxDrawSurf
	{
		//GfxDrawSurfFields fields;
		Packed128 packed;
	};

	enum MaterialTechniqueType : std::int32_t
	{
		TECHNIQUE_UNLIT = 8,
		TECHNIQUE_EMISSIVE = 9,
		TECHNIQUE_LIT = 13,
		TECHNIQUE_WIREFRAME = 53,
	};

	struct GfxComputeShaderLoadDef
	{
		unsigned char* program;
		unsigned int programSize;
		char __pad[4];
	};

	struct ComputeShaderProgram
	{
		ID3D11ComputeShader* cs;
		GfxComputeShaderLoadDef loadDef;
	};

	struct ComputeShader
	{
		const char* name;
		ComputeShaderProgram prog;
	}; static_assert(sizeof(ComputeShader) == 0x20);

	struct GfxVertexShaderLoadDef
	{
		unsigned char* program;
		unsigned int programSize;
		unsigned int microCodeCrc;
	};

	struct MaterialVertexShaderProgram
	{
		ID3D11VertexShader* vs;
		GfxVertexShaderLoadDef loadDef;
	};

	struct MaterialVertexShader
	{
		const char* name;
		MaterialVertexShaderProgram prog;
	}; static_assert(sizeof(MaterialVertexShader) == 0x20);

	struct GfxPixelShaderLoadDef
	{
		unsigned char* program;
		unsigned int programSize;
		unsigned int microCodeCrc;
	};

	struct MaterialPixelShaderProgram
	{
		ID3D11PixelShader* ps;
		GfxPixelShaderLoadDef loadDef;
	};

	struct MaterialPixelShader
	{
		const char* name;
		MaterialPixelShaderProgram prog;
	}; static_assert(sizeof(MaterialPixelShader) == 0x20);

	struct GfxHullShaderLoadDef
	{
		unsigned char* program;
		unsigned int programSize;
		char __pad[4];
	};

	struct MaterialHullShaderProgram
	{
		ID3D11HullShader* hs;
		GfxHullShaderLoadDef loadDef;
	};

	struct MaterialHullShader
	{
		const char* name;
		MaterialHullShaderProgram prog;
	}; static_assert(sizeof(MaterialHullShader) == 0x20);

	struct GfxDomainShaderLoadDef
	{
		unsigned char* program;
		unsigned int programSize;
		char __pad[4];
	};

	struct MaterialDomainShaderProgram
	{
		ID3D11DomainShader* ds;
		GfxDomainShaderLoadDef loadDef;
	};

	struct MaterialDomainShader
	{
		const char* name;
		MaterialDomainShaderProgram prog;
	}; static_assert(sizeof(MaterialDomainShader) == 0x20);

	enum MaterialConstSource : std::int32_t
	{
		CONST_SRC_CODE_LIGHT_POSITION,
		CONST_SRC_CODE_LIGHT_DIFFUSE,
		CONST_SRC_CODE_LIGHT_PHYSICALSIZE,
		CONST_SRC_CODE_LIGHT_FADEOFFSET,
		CONST_SRC_CODE_LIGHT_SPECULAR,
		CONST_SRC_CODE_LIGHT_SPOTDIR,
		CONST_SRC_CODE_LIGHT_SPOTFACTORS,
		CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT,
		CONST_SRC_CODE_LIGHT_CUCOLORIS_ANIM,
		CONST_SRC_CODE_LIGHT_POSITION1,
		CONST_SRC_CODE_LIGHT_DIFFUSE1,
		CONST_SRC_CODE_LIGHT_PHYSICALSIZE1,
		CONST_SRC_CODE_LIGHT_FADEOFFSET1,
		CONST_SRC_CODE_LIGHT_SPECULAR1,
		CONST_SRC_CODE_LIGHT_SPOTDIR1,
		CONST_SRC_CODE_LIGHT_SPOTFACTORS1,
		CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT1,
		CONST_SRC_CODE_LIGHT_CUCOLORIS_ANIM1,
		CONST_SRC_CODE_LIGHT_POSITION2,
		CONST_SRC_CODE_LIGHT_DIFFUSE2,
		CONST_SRC_CODE_LIGHT_PHYSICALSIZE2,
		CONST_SRC_CODE_LIGHT_FADEOFFSET2,
		CONST_SRC_CODE_LIGHT_SPECULAR2,
		CONST_SRC_CODE_LIGHT_SPOTDIR2,
		CONST_SRC_CODE_LIGHT_SPOTFACTORS2,
		CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT2,
		CONST_SRC_CODE_LIGHT_CUCOLORIS_ANIM2,
		CONST_SRC_CODE_LIGHT_POSITION3,
		CONST_SRC_CODE_LIGHT_DIFFUSE3,
		CONST_SRC_CODE_LIGHT_PHYSICALSIZE3,
		CONST_SRC_CODE_LIGHT_FADEOFFSET3,
		CONST_SRC_CODE_LIGHT_SPECULAR3,
		CONST_SRC_CODE_LIGHT_SPOTDIR3,
		CONST_SRC_CODE_LIGHT_SPOTFACTORS3,
		CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT3,
		CONST_SRC_CODE_LIGHT_CUCOLORIS_ANIM3,
		CONST_SRC_CODE_LIGHT_POSITION_DB_ARRAY_0,
		CONST_SRC_CODE_LIGHT_POSITION_DB_ARRAY_1,
		CONST_SRC_CODE_LIGHT_POSITION_DB_ARRAY_2,
		CONST_SRC_CODE_LIGHT_POSITION_DB_ARRAY_3,
		CONST_SRC_CODE_LIGHT_DIFFUSE_DB_ARRAY_0,
		CONST_SRC_CODE_LIGHT_DIFFUSE_DB_ARRAY_1,
		CONST_SRC_CODE_LIGHT_DIFFUSE_DB_ARRAY_2,
		CONST_SRC_CODE_LIGHT_DIFFUSE_DB_ARRAY_3,
		CONST_SRC_CODE_LIGHT_PHYSICALSIZE_DB_ARRAY_0,
		CONST_SRC_CODE_LIGHT_PHYSICALSIZE_DB_ARRAY_1,
		CONST_SRC_CODE_LIGHT_PHYSICALSIZE_DB_ARRAY_2,
		CONST_SRC_CODE_LIGHT_PHYSICALSIZE_DB_ARRAY_3,
		CONST_SRC_CODE_LIGHT_FADEOFFSET_DB_ARRAY_0,
		CONST_SRC_CODE_LIGHT_FADEOFFSET_DB_ARRAY_1,
		CONST_SRC_CODE_LIGHT_FADEOFFSET_DB_ARRAY_2,
		CONST_SRC_CODE_LIGHT_FADEOFFSET_DB_ARRAY_3,
		CONST_SRC_CODE_LIGHT_SPECULAR_DB_ARRAY_0,
		CONST_SRC_CODE_LIGHT_SPECULAR_DB_ARRAY_1,
		CONST_SRC_CODE_LIGHT_SPECULAR_DB_ARRAY_2,
		CONST_SRC_CODE_LIGHT_SPECULAR_DB_ARRAY_3,
		CONST_SRC_CODE_LIGHT_SPOTDIR_DB_ARRAY_0,
		CONST_SRC_CODE_LIGHT_SPOTDIR_DB_ARRAY_1,
		CONST_SRC_CODE_LIGHT_SPOTDIR_DB_ARRAY_2,
		CONST_SRC_CODE_LIGHT_SPOTDIR_DB_ARRAY_3,
		CONST_SRC_CODE_LIGHT_SPOTFACTORS_DB_ARRAY_0,
		CONST_SRC_CODE_LIGHT_SPOTFACTORS_DB_ARRAY_1,
		CONST_SRC_CODE_LIGHT_SPOTFACTORS_DB_ARRAY_2,
		CONST_SRC_CODE_LIGHT_SPOTFACTORS_DB_ARRAY_3,
		CONST_SRC_CODE_LIGHT_CUCOLORIS_ANIM_DB_ARRAY_0,
		CONST_SRC_CODE_LIGHT_CUCOLORIS_ANIM_DB_ARRAY_1,
		CONST_SRC_CODE_LIGHT_CUCOLORIS_ANIM_DB_ARRAY_2,
		CONST_SRC_CODE_LIGHT_CUCOLORIS_ANIM_DB_ARRAY_3,
		CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT_DB_ARRAY_0,
		CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT_DB_ARRAY_1,
		CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT_DB_ARRAY_2,
		CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT_DB_ARRAY_3,
		CONST_SRC_CODE_LIGHT_DYN_COUNT,
		CONST_SRC_CODE_LIGHT_DYN_TYPES,
		CONST_SRC_CODE_LIGHT_DYN_SHADOW_TYPES,
		CONST_SRC_CODE_PARTICLE_CLOUD_COLOR,
		CONST_SRC_CODE_GAMETIME,
		CONST_SRC_CODE_SYSTEMTIME,
		CONST_SRC_CODE_GENERIC_MATERIAL_DATA,
		CONST_SRC_CODE_EYEOFFSET,
		CONST_SRC_CODE_RATIO_MASK,
		CONST_SRC_CODE_WORLD_MATRIX_EYE_OFFSET,
		CONST_SRC_CODE_HUD_OUTLINE_PARMS,
		CONST_SRC_CODE_MODEL_VELOCITY_PARMS,
		CONST_SRC_CODE_MODEL_VELOCITY_SKINNED_PARMS,
		CONST_SRC_CODE_POSTFX_CONTROL0,
		CONST_SRC_CODE_POSTFX_CONTROL1,
		CONST_SRC_CODE_POSTFX_CONTROL2,
		CONST_SRC_CODE_POSTFX_CONTROL3,
		CONST_SRC_CODE_POSTFX_CONTROL4,
		CONST_SRC_CODE_POSTFX_CONTROL5,
		CONST_SRC_CODE_POSTFX_CONTROL6,
		CONST_SRC_CODE_POSTFX_CONTROL7,
		CONST_SRC_CODE_POSTFX_CONTROL8,
		CONST_SRC_CODE_POSTFX_CONTROL9,
		CONST_SRC_CODE_POSTFX_CONTROLA,
		CONST_SRC_CODE_POSTFX_CONTROLB,
		CONST_SRC_CODE_POSTFX_CONTROLC,
		CONST_SRC_CODE_POSTFX_CONTROLD,
		CONST_SRC_CODE_POSTFX_CONTROLE,
		CONST_SRC_CODE_POSTFX_CONTROLF,
		CONST_SRC_CODE_SCRIPT_PARMS,
		CONST_SRC_CODE_EFFECT_MODEL_COLOR,
		CONST_SRC_CODE_EFFECT_MODEL_COLOR_EMISSIVE,
		CONST_SRC_CODE_FILTER_TAP_0,
		CONST_SRC_CODE_FILTER_TAP_1,
		CONST_SRC_CODE_FILTER_TAP_2,
		CONST_SRC_CODE_FILTER_TAP_3,
		CONST_SRC_CODE_FILTER_TAP_4,
		CONST_SRC_CODE_FILTER_TAP_5,
		CONST_SRC_CODE_FILTER_TAP_6,
		CONST_SRC_CODE_FILTER_TAP_7,
		CONST_SRC_CODE_FILTER_TAP_8,
		CONST_SRC_CODE_FILTER_TAP_9,
		CONST_SRC_CODE_FILTER_TAP_10,
		CONST_SRC_CODE_FILTER_TAP_11,
		CONST_SRC_CODE_COLOR_MATRIX_R,
		CONST_SRC_CODE_COLOR_MATRIX_G,
		CONST_SRC_CODE_COLOR_MATRIX_B,
		CONST_SRC_CODE_RENDER_TARGET_SIZE,
		CONST_SRC_CODE_RENDER_SOURCE_SIZE,
		CONST_SRC_CODE_VEIL_PARAMS,
		CONST_SRC_CODE_FXAA_RCPFRAMEOPT,
		CONST_SRC_CODE_VOLUME_LIGHT_SCATTER_1,
		CONST_SRC_CODE_VOLUME_LIGHT_SCATTER_2,
		CONST_SRC_CODE_VOLUME_LIGHT_SCATTER_3,
		CONST_SRC_CODE_MODEL_LIGHTMAP_PARAMS,
		CONST_SRC_CODE_NEARPLANE_ORG,
		CONST_SRC_CODE_NEARPLANE_DX,
		CONST_SRC_CODE_NEARPLANE_DY,
		CONST_SRC_CODE_SSAO_POWER_BB_SHARPNESS_STEP,
		CONST_SRC_CODE_SSAO_COLOR_COEFF,
		CONST_SRC_CODE_SSAO_FALLOFF_DEPTH_SCALE,
		CONST_SRC_CODE_SSAO_UV_TO_ROT_SCALE_AND_FADE,
		CONST_SRC_CODE_SSAO_SAMPLE_MAT_SCALE,
		CONST_SRC_CODE_SSAO_DEPTH_OFFSET_REJECT,
		CONST_SRC_CODE_LIT2D_LIGHTDIRX2,
		CONST_SRC_CODE_LIT2D_SPECHALFANGLEDIRX2,
		CONST_SRC_CODE_LIT2D_AMBIENTCOLOR,
		CONST_SRC_CODE_LIT2D_DIFFUSECOLOR,
		CONST_SRC_CODE_LIT2D_SPECCOLOR_SPECEXPONENT,
		CONST_SRC_CODE_LIT2D_ADDITIVECOLOR,
		CONST_SRC_CODE_LIT2D_BLOODSPATTER_AMBIENTCOLOR,
		CONST_SRC_CODE_LIT2D_BLOODSPATTER_DIFFUSECOLOR,
		CONST_SRC_CODE_LIT2D_BLOODSPATTER_SPECCOLOR_SPECEXPONENT,
		CONST_SRC_CODE_LIT2D_BLOODSPATTER_ADDITIVECOLOR,
		CONST_SRC_CODE_LIT2D_HUDBLOOD_COLOR,
		CONST_SRC_CODE_LIT2D_HUDBLOOD_COLOR_THIN,
		CONST_SRC_CODE_DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR,
		CONST_SRC_CODE_DOF_EQUATION_SCENE,
		CONST_SRC_CODE_DOF_UNK,
		CONST_SRC_CODE_DOF_LERP_SCALE,
		CONST_SRC_CODE_DOF_LERP_BIAS,
		CONST_SRC_CODE_DOF_ROW_DELTA,
		CONST_SRC_CODE_DOF_EQUATION,
		CONST_SRC_CODE_DOF_PARAMS,
		CONST_SRC_CODE_DOF_TILE_PARAMS,
		CONST_SRC_CODE_FRAME_COUNT_PARAMS,
		CONST_SRC_CODE_MOTION_MATRIX_X,
		CONST_SRC_CODE_MOTION_MATRIX_Y,
		CONST_SRC_CODE_MOTION_MATRIX_W,
		CONST_SRC_CODE_SSR_PREV_FRAME_VIEWPROJECTION_MATRIX_R0,
		CONST_SRC_CODE_SSR_PREV_FRAME_VIEWPROJECTION_MATRIX_R1,
		CONST_SRC_CODE_SSR_PREV_FRAME_VIEWPROJECTION_MATRIX_R2,
		CONST_SRC_CODE_SSR_PREV_FRAME_VIEWPROJECTION_MATRIX_R3,
		CONST_SRC_CODE_PREV_EYEPOSITION_TRANSFORM,
		CONST_SRC_CODE_SCREEN_SPACE_REFLECTION_PARAMETERS,
		CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE_AND_OFFSET,
		CONST_SRC_CODE_SSR_CLIP_TO_FADE_SCALE_OFFSET_PS,
		CONST_SRC_CODE_SSS_MRT_OFF_SHADER_OFF,
		CONST_SRC_CODE_MDAO_VIEWTOOCCLUDER_MATRIX_R0,
		CONST_SRC_CODE_MDAO_VIEWTOOCCLUDER_MATRIX_R1,
		CONST_SRC_CODE_MDAO_VIEWTOOCCLUDER_MATRIX_R2,
		CONST_SRC_CODE_MDAO_VIEWTOOCCLUDER_MATRIX_R3,
		CONST_SRC_CODE_MDAO_WORLDTOOCCLUDERFRAME_MATRIX_R0,
		CONST_SRC_CODE_MDAO_WORLDTOOCCLUDERFRAME_MATRIX_R1,
		CONST_SRC_CODE_MDAO_WORLDTOOCCLUDERFRAME_MATRIX_R2,
		CONST_SRC_CODE_MDAO_WORLDTOOCCLUDERFRAME_MATRIX_R3,
		CONST_SRC_CODE_MDAO_SCREEN_PARAMS,
		CONST_SRC_CODE_MDAO_CAMERA_PARAMS,
		CONST_SRC_CODE_MDAO_LOOKUP_PARAMS,
		CONST_SRC_CODE_MDAO_VOLUME_PARAMS,
		CONST_SRC_CODE_DISTORTION_SAMPLE_LIMITS_PS,
		CONST_SRC_CODE_SCOPE_SAMPLE_LIMITS_PS,
		CONST_SRC_CODE_UV_TO_PREV_SCALED_CLIP_MAT_C0_PS,
		CONST_SRC_CODE_UV_TO_PREV_SCALED_CLIP_MAT_C1_PS,
		CONST_SRC_CODE_UV_TO_PREV_SCALED_CLIP_MAT_C3_PS,
		CONST_SRC_CODE_PREV_FRAME_VIEWPROJECTION_MATRIX_R0,
		CONST_SRC_CODE_PREV_FRAME_VIEWPROJECTION_MATRIX_R1,
		CONST_SRC_CODE_PREV_FRAME_VIEWPROJECTION_MATRIX_R2,
		CONST_SRC_CODE_PREV_FRAME_VIEWPROJECTION_MATRIX_R3,
		CONST_SRC_CODE_PREV_FRAME_WORLD_MATRIX_EYE_OFFSET,
		CONST_SRC_CODE_STATIC_VELOCITY_PARMS,
		CONST_SRC_CODE_MOTION_BLUR_HQ_PARAMS,
		CONST_SRC_CODE_MOTION_BLUR_HQ_TILE_MAX_PARAMS,
		CONST_SRC_CODE_MB_UV_TO_PREV_SCALED_CLIP_MAT_C0_PS,
		CONST_SRC_CODE_MB_UV_TO_PREV_SCALED_CLIP_MAT_C1_PS,
		CONST_SRC_CODE_MB_UV_TO_PREV_SCALED_CLIP_MAT_C3_PS,
		CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION_ARRAY_0,
		CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION_ARRAY_1,
		CONST_SRC_CODE_SHADOWMAP_DISTANCE_BIAS,
		CONST_SRC_CODE_SHADOWMAP_SCALE,
		CONST_SRC_CODE_SHADOWMAP_PARTITION_UV_OFFSET,
		CONST_SRC_CODE_SHADOWMAP_CASCADE_MASK,
		CONST_SRC_CODE_SHADOWMAP_DISTANCE_BLEND,
		CONST_SRC_CODE_ZNEAR,
		CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE,
		CONST_SRC_CODE_INV_SCENE_PROJECTION,
		CONST_SRC_CODE_RIM_LIGHT_0_DIR,
		CONST_SRC_CODE_RIM_LIGHT_0_COLOR,
		CONST_SRC_CODE_RIM_LIGHT_1_DIR,
		CONST_SRC_CODE_RIM_LIGHT_1_COLOR,
		CONST_SRC_CODE_RIM_LIGHT_TECHNIQUE,
		CONST_SRC_CODE_DEBUG_BUMPMAP,
		CONST_SRC_CODE_DEBUG_TEXDENSITY,
		CONST_SRC_CODE_MATERIAL_COLOR,
		CONST_SRC_CODE_FOG,
		CONST_SRC_CODE_FOG_COLOR_LINEAR,
		CONST_SRC_CODE_FOG_COLOR_GAMMA,
		CONST_SRC_CODE_FOG_SUN_CONSTS,
		CONST_SRC_CODE_FOG_SUN_COLOR_LINEAR,
		CONST_SRC_CODE_FOG_SUN_COLOR_GAMMA,
		CONST_SRC_CODE_FOG_SUN_DIR,
		CONST_SRC_CODE_FOG_SKY_FOG,
		CONST_SRC_CODE_FOG_HEIGHT_FOG,
		CONST_SRC_CODE_ATMOS_FOG_PARMS_0,
		CONST_SRC_CODE_ATMOS_FOG_PARMS_1,
		CONST_SRC_CODE_ATMOS_FOG_PARMS_2,
		CONST_SRC_CODE_ATMOS_FOG_PARMS_3,
		CONST_SRC_CODE_ATMOS_FOG_PARMS_4,
		CONST_SRC_CODE_ATMOS_FOG_PARMS_5,
		CONST_SRC_CODE_ATMOS_FOG_PARMS_6,
		CONST_SRC_CODE_ATMOS_FOG_PARMS_7,
		CONST_SRC_CODE_FOG_SKY_DIR,
		CONST_SRC_CODE_FOG_DEPTHHACK,
		CONST_SRC_CODE_GLOW_SETUP,
		CONST_SRC_CODE_GLOW_APPLY,
		CONST_SRC_CODE_GLOW_SETUP_ALT_COLOR_SCALE,
		CONST_SRC_CODE_COLOR_BIAS,
		CONST_SRC_CODE_COLOR_TINT_BASE,
		CONST_SRC_CODE_COLOR_TINT_DELTA,
		CONST_SRC_CODE_COLOR_TINT_QUADRATIC_DELTA,
		CONST_SRC_CODE_COLOR_TINT_HDR_CROSSOVER,
		CONST_SRC_CODE_COLOR_TINT_HDR_DARK,
		CONST_SRC_CODE_COLOR_TINT_HDR_DARK_TANGENT,
		CONST_SRC_CODE_COLOR_TINT_HDR_MEDIUM,
		CONST_SRC_CODE_COLOR_TINT_HDR_MEDIUM_TANGENT,
		CONST_SRC_CODE_COLOR_TINT_HDR_LIGHT,
		CONST_SRC_CODE_COLOR_TINT_HDR_LIGHT_TANGENT,
		CONST_SRC_CODE_COLOR_TINT_HDR_SPECULAR,
		CONST_SRC_CODE_COLOR_TINT_HDR_SPECULAR_TANGENT,
		CONST_SRC_CODE_PERCEPTUAL_PARMS,
		CONST_SRC_CODE_FILMIC_PARMS,
		CONST_SRC_CODE_FILMIC_SHOULDER_PARMS,
		CONST_SRC_CODE_FILMIC_TOE_PARMS,
		CONST_SRC_CODE_TONEMAP_PARMS,
		CONST_SRC_CODE_HDR_STAGE_PARMS,
		CONST_SRC_CODE_UI3D_UV_SETUP_0,
		CONST_SRC_CODE_UI3D_UV_SETUP_1,
		CONST_SRC_CODE_UI3D_UV_SETUP_2,
		CONST_SRC_CODE_UI3D_UV_SETUP_3,
		CONST_SRC_CODE_UI3D_UV_SETUP_4,
		CONST_SRC_CODE_UI3D_UV_SETUP_5,
		CONST_SRC_CODE_HUDFX_PARMS,
		CONST_SRC_CODE_HUDFX_PARMS2,
		CONST_SRC_CODE_SSSS_PARAMS,
		CONST_SRC_CODE_SSSS_SCALE,
		CONST_SRC_CODE_SSSS_KERNEL_0,
		CONST_SRC_CODE_SSSS_KERNEL_1,
		CONST_SRC_CODE_SSSS_KERNEL_2,
		CONST_SRC_CODE_SSSS_KERNEL_3,
		CONST_SRC_CODE_SSSS_KERNEL_4,
		CONST_SRC_CODE_SSSS_KERNEL_5,
		CONST_SRC_CODE_SSSS_KERNEL_6,
		CONST_SRC_CODE_SSSS_KERNEL_7,
		CONST_SRC_CODE_SSSS_KERNEL_8,
		CONST_SRC_CODE_SSSS_KERNEL_9,
		CONST_SRC_CODE_SSSS_KERNEL_10,
		CONST_SRC_CODE_SSSS_KERNEL_11,
		CONST_SRC_CODE_SSSS_KERNEL_12,
		CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS,
		CONST_SRC_CODE_ENVMAP_PARMS,
		CONST_SRC_CODE_SUN_SHADOWMAP_PIXEL_ADJUST_ARRAY_0,
		CONST_SRC_CODE_SUN_SHADOWMAP_PIXEL_ADJUST_ARRAY_1,
		CONST_SRC_CODE_SUN_SHADOWMAP_PIXEL_ADJUST_ARRAY_2,
		CONST_SRC_CODE_SUN_SHADOWMAP_CASCADE_V_CLAMP,
		CONST_SRC_CODE_SUN_SHADOWMAP_NEAR_FAR_PLANE,
		CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST,
		CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST_1,
		CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST_2,
		CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST_3,
		CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST_ARRAY_0,
		CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST_ARRAY_1,
		CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST_ARRAY_2,
		CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST_ARRAY_3,
		CONST_SRC_CODE_SPOT_SHADOWMAP_ZTRANSFORM,
		CONST_SRC_CODE_SPOT_SHADOWMAP_ZTRANSFORM_1,
		CONST_SRC_CODE_SPOT_SHADOWMAP_ZTRANSFORM_2,
		CONST_SRC_CODE_SPOT_SHADOWMAP_ZTRANSFORM_3,
		CONST_SRC_CODE_SPOT_SHADOWMAP_ZTRANSFORM_ARRAY_0,
		CONST_SRC_CODE_SPOT_SHADOWMAP_ZTRANSFORM_ARRAY_1,
		CONST_SRC_CODE_SPOT_SHADOWMAP_ZTRANSFORM_ARRAY_2,
		CONST_SRC_CODE_SPOT_SHADOWMAP_ZTRANSFORM_ARRAY_3,
		CONST_SRC_CODE_COMPOSITE_FX_DISTORTION,
		CONST_SRC_CODE_POSTFX_FADE_EFFECT,
		CONST_SRC_CODE_SCENE_UV_MULT,
		CONST_SRC_CODE_SCENE_VIEW_MULT,
		CONST_SRC_CODE_TEXCOORD_CLAMP_0,
		CONST_SRC_CODE_TEXCOORD_CLAMP_1,
		CONST_SRC_CODE_VIEWPORT_DIMENSIONS,
		CONST_SRC_CODE_FRAMEBUFFER_READ,
		CONST_SRC_CODE_THERMAL_COLOR_OFFSET,
		CONST_SRC_CODE_THERMAL_FADE_CONTROL,
		CONST_SRC_CODE_THERMAL_FADE_COLOR,
		CONST_SRC_CODE_PLAYLIST_POPULATION_PARAMS,
		CONST_SRC_CODE_TESSELLATION_PARMS,
		CONST_SRC_CODE_TESSELLATION_PARMS2,
		CONST_SRC_CODE_EYE_PARAMETER_DVAR,
		CONST_SRC_CODE_MP_RIM_PARAMETER_DVAR1,
		CONST_SRC_CODE_MP_RIM_PARAMETER_DVAR2,
		CONST_SRC_CODE_HEAD_ROT_MAT_R0,
		CONST_SRC_CODE_HEAD_ROT_MAT_R1,
		CONST_SRC_CODE_HEAD_ROT_MAT_R2,
		CONST_SRC_CODE_GUN_SIGHT_COLOR,
		CONST_SRC_CODE_GUN_RETICLE_COLOR,
		CONST_SRC_CODE_ADS_OVERLAY_RECT,
		CONST_SRC_CODE_BASE_LIGHTING_COORDS,
		CONST_SRC_CODE_LIGHT_PROBE_AMBIENT,
		CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE,
		CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET,
		CONST_SRC_CODE_PARTICLE_CLOUD_TEXTURE_ATLAS_SETTINGS,
		CONST_SRC_CODE_PARTICLE_CLOUD_VEL_WORLD,
		CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX0,
		CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX1,
		CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX2,
		CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR0,
		CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR1,
		CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR2,
		CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM0,
		CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM1,
		CONST_SRC_CODE_LIT2D_BLOODSPATTER_FADESHARPNESS,
		CONST_SRC_CODE_BLUR_SCENE_PIXEL_POS_TO_UV,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_0,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_1,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_2,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_3,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_4,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_5,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_6,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_7,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_8,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_9,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_10,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_11,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_12,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_13,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_14,
		CONST_SRC_CODE_REACTIVEMOTION_CENTERS_15,
		CONST_SRC_CODE_DEPTH_FROM_CLIP,
		CONST_SRC_CODE_CODE_MESH_ARG_0,
		CONST_SRC_CODE_CODE_MESH_ARG_1,
		CONST_SRC_CODE_CODE_MESH_ARG_3,
		CONST_SRC_CODE_CODE_MESH_ARG_4,
		CONST_SRC_CODE_SCRIPT_CONST0,
		CONST_SRC_CODE_VIEW_MATRIX,
		CONST_SRC_CODE_INVERSE_VIEW_MATRIX,
		CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX,
		CONST_SRC_CODE_PROJECTION_MATRIX,
		CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX,
		CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX,
		CONST_SRC_CODE_VIEW_PROJECTION_MATRIX,
		CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX,
		CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX,
		CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX,
		CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX,
		CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX,
		CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX1,
		CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX1,
		CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX1,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX1,
		CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX2,
		CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX2,
		CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX2,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX2,
		CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX3,
		CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX3,
		CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX3,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX3,
		CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX,
		CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX,
		CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX,
		CONST_SRC_CODE_PREV_FRAME_WORLD_MATRIX,
		CONST_SRC_CODE_PREV_FRAME_INVERSE_WORLD_MATRIX,
		CONST_SRC_CODE_PREV_FRAME_TRANSPOSE_WORLD_MATRIX,
		CONST_SRC_CODE_PREV_FRAME_INVERSE_TRANSPOSE_WORLD_MATRIX,
		CONST_SRC_CODE_WORLD_MATRIX0,
		CONST_SRC_CODE_INVERSE_WORLD_MATRIX0,
		CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX0,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX0,
		CONST_SRC_CODE_WORLD_VIEW_MATRIX0,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX0,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX0,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0,
		CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX0,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX0,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0,
		CONST_SRC_CODE_WORLD_MATRIX1,
		CONST_SRC_CODE_INVERSE_WORLD_MATRIX1,
		CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX1,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX1,
		CONST_SRC_CODE_WORLD_VIEW_MATRIX1,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX1,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX1,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX1,
		CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX1,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX1,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1,
		CONST_SRC_CODE_WORLD_MATRIX2,
		CONST_SRC_CODE_INVERSE_WORLD_MATRIX2,
		CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX2,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX2,
		CONST_SRC_CODE_WORLD_VIEW_MATRIX2,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX2,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX2,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX2,
		CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX2,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX2,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2,
		CONST_SRC_TOTAL_COUNT,
		CONST_SRC_NONE,
	};

	struct MaterialArgumentCodeConst
	{
		unsigned short index;
		unsigned char firstRow;
		unsigned char rowCount;
	};

	union MaterialArgumentDef
	{
		const float* literalConst;
		MaterialArgumentCodeConst codeConst;
		unsigned int codeSampler;
		unsigned int nameHash;
	};

	struct MaterialShaderArgument
	{
		unsigned char type;
		unsigned char shader;
		unsigned short dest;
		MaterialArgumentDef u;
	}; static_assert(sizeof(MaterialShaderArgument) == 0x10);

	struct MaterialStreamRouting
	{
		unsigned char source;
		unsigned char dest;
		unsigned char mask;
	};

	struct MaterialVertexStreamRouting
	{
		MaterialStreamRouting data[32];
		ID3D11InputLayout* decl[250];
	};

	struct MaterialVertexDeclaration
	{
		const char* name;
		unsigned char streamCount;
		bool hasOptionalSource;
		MaterialVertexStreamRouting routing;
	}; static_assert(sizeof(MaterialVertexDeclaration) == 0x840);

	struct MaterialPass
	{
		MaterialVertexShader* vertexShader;
		MaterialVertexDeclaration* vertexDecl;
		MaterialHullShader* hullShader;
		MaterialDomainShader* domainShader;
		MaterialPixelShader* pixelShader;
		unsigned char pixelOutputMask;
		unsigned char perPrimArgCount;
		unsigned char perObjArgCount;
		unsigned char stableArgCount;
		unsigned short perPrimArgSize;
		unsigned short perObjArgSize;
		unsigned short stableArgSize;
		unsigned short zone;
		unsigned char perPrimConstantBuffer;
		unsigned char perObjConstantBuffer;
		unsigned char stableConstantBuffer;
		unsigned int customBufferFlags;
		unsigned char customSamplerFlags;
		unsigned char precompiledIndex;
		unsigned char stageConfig;
		MaterialShaderArgument* args;
	};

	static_assert(offsetof(MaterialPass, zone) == 50);

	struct MaterialTechniqueHeader
	{
		const char* name;
		unsigned short flags;
		unsigned short passCount;
	};

	struct MaterialTechnique
	{
		//const char* name;
		//unsigned short flags;
		//unsigned short passCount;
		MaterialTechniqueHeader hdr;
		MaterialPass passArray[1];
	};

	struct MaterialTechniqueSet
	{
		const char* name;
		unsigned short flags;
		unsigned char worldVertFormat;
		unsigned char preDisplacementOnlyCount;
		MaterialTechnique* techniques[252];
	}; static_assert(sizeof(MaterialTechniqueSet) == 0x7F0);

	static_assert(offsetof(MaterialTechniqueSet, techniques) == 16);

	struct GfxImage;

	struct WaterWritable
	{
		float floatTime;
	};

	struct water_t
	{
		WaterWritable writable;
		float* H0X;
		float* H0Y;
		float* wTerm;
		int M;
		int N;
		float Lx;
		float Lz;
		float gravity;
		float windvel;
		float winddir[2];
		float amplitude;
		GfxImage* image;
		GfxImage* stagingImage;
	};

	union MaterialTextureDefInfo
	{
		GfxImage* image;
		water_t* water;
	};

	struct MaterialTextureDef
	{
		unsigned int nameHash;
		char nameStart;
		char nameEnd;
		unsigned char samplerState;
		unsigned char semantic;
		MaterialTextureDefInfo u;
	}; static_assert(sizeof(MaterialTextureDef) == 0x10);

	struct MaterialConstantDef
	{
		unsigned int nameHash;
		char name[12];
		float literal[4];
	}; static_assert(sizeof(MaterialConstantDef) == 0x20);

	struct GfxStateBits
	{
		unsigned int loadBits[6]; // loadbits[3], blendstatebits[3]
		unsigned short zone;
		unsigned char depthStencilState[10];
		unsigned char blendState;
		unsigned char rasterizerState;
	}; static_assert(sizeof(GfxStateBits) == 0x28);

	struct MaterialConstantBufferDef
	{
		unsigned int vsDataSize;
		unsigned int hsDataSize;
		unsigned int dsDataSize;
		unsigned int psDataSize;
		unsigned int vsOffsetDataSize;
		unsigned int hsOffsetDataSize;
		unsigned int dsOffsetDataSize;
		unsigned int psOffsetDataSize;
		unsigned char* vsData;
		unsigned char* hsData;
		unsigned char* dsData;
		unsigned char* psData;
		unsigned short* vsOffsetData;
		unsigned short* hsOffsetData;
		unsigned short* dsOffsetData;
		unsigned short* psOffsetData;
		ID3D11Buffer* vsConstantBuffer;
		ID3D11Buffer* hsConstantBuffer;
		ID3D11Buffer* dsConstantBuffer;
		ID3D11Buffer* psConstantBuffer;
	};

	enum SurfaceTypeBits : std::uint64_t
	{
		SURFTYPE_BITS_NONE = 0x0,
		SURFTYPE_BITS_DEFAULT = 0x1,
		SURFTYPE_BITS_BARK = 0x2,
		SURFTYPE_BITS_CARPET = 0x4,
		SURFTYPE_BITS_CLOTH = 0x8,
		SURFTYPE_BITS_CONCRETE = 0x10,
		SURFTYPE_BITS_DIRT = 0x20,
		SURFTYPE_BITS_FLESH = 0x40,
		SURFTYPE_BITS_FOLIAGE_DEBRIS = 0x80,
		SURFTYPE_BITS_GLASS = 0x100,
		SURFTYPE_BITS_GRASS = 0x200,
		SURFTYPE_BITS_GRAVEL = 0x400,
		SURFTYPE_BITS_ICE = 0x800,
		SURFTYPE_BITS_METAL_SOLID = 0x1000,
		SURFTYPE_BITS_METAL_GRATE = 0x2000,
		SURFTYPE_BITS_MUD = 0x4000,
		SURFTYPE_BITS_PAPER = 0x8000,
		SURFTYPE_BITS_PLASTER = 0x10000,
		SURFTYPE_BITS_ROCK = 0x20000,
		SURFTYPE_BITS_SAND = 0x40000,
		SURFTYPE_BITS_SNOW = 0x80000,
		SURFTYPE_BITS_WATER_WAIST = 0x100000,
		SURFTYPE_BITS_WOOD_SOLID = 0x200000,
		SURFTYPE_BITS_ASPHALT = 0x400000,
		SURFTYPE_BITS_CERAMIC = 0x800000,
		SURFTYPE_BITS_PLASTIC_SOLID = 0x1000000,
		SURFTYPE_BITS_RUBBER = 0x2000000,
		SURFTYPE_BITS_FRUIT = 0x4000000,
		SURFTYPE_BITS_PAINTEDMETAL = 0x8000000,
		SURFTYPE_BITS_RIOTSHIELD = 0x10000000,
		SURFTYPE_BITS_SLUSH = 0x20000000,
		SURFTYPE_BITS_ASPHALT_WET = 0x40000000,
		SURFTYPE_BITS_ASPHALT_DEBRIS = 0x80000000,
		SURFTYPE_BITS_CONCRETE_WET = 0x100000000,
		SURFTYPE_BITS_CONCRETE_DEBRIS = 0x200000000,
		SURFTYPE_BITS_FOLIAGE_VEGETATION = 0x400000000,
		SURFTYPE_BITS_FOLIAGE_LEAVES = 0x800000000,
		SURFTYPE_BITS_GRASS_TALL = 0x1000000000,
		SURFTYPE_BITS_METAL_HOLLOW = 0x2000000000,
		SURFTYPE_BITS_METAL_VEHICLE = 0x4000000000,
		SURFTYPE_BITS_METAL_THIN = 0x8000000000,
		SURFTYPE_BITS_METAL_WET = 0x10000000000,
		SURFTYPE_BITS_METAL_DEBRIS = 0x20000000000,
		SURFTYPE_BITS_PLASTIC_HOLLOW = 0x40000000000,
		SURFTYPE_BITS_PLASTIC_TARP = 0x80000000000,
		SURFTYPE_BITS_ROCK_WET = 0x100000000000,
		SURFTYPE_BITS_ROCK_DEBRIS = 0x200000000000,
		SURFTYPE_BITS_WATER_ANKLE = 0x400000000000,
		SURFTYPE_BITS_WATER_KNEE = 0x800000000000,
		SURFTYPE_BITS_WOOD_HOLLOW = 0x1000000000000,
		SURFTYPE_BITS_WOOD_WET = 0x2000000000000,
		SURFTYPE_BITS_WOOD_DEBRIS = 0x4000000000000,
		SURFTYPE_BITS_CUSHION = 0x8000000000000,
	};

	struct MaterialInfo
	{
		const char* name;
		unsigned char gameFlags;
		unsigned char sortKey;
		unsigned char textureAtlasRowCount;
		unsigned char textureAtlasColumnCount;
		unsigned char textureAtlasFrameBlend;
		unsigned char textureAtlasAsArray;
		unsigned char renderFlags;
		GfxDrawSurf drawSurf;
		SurfaceTypeBits surfaceTypeBits;
		unsigned int hashIndex;
	}; static_assert(sizeof(MaterialInfo) == 48);

	struct Material
	{
		union
		{
			const char* name;
			MaterialInfo info;
		};
		unsigned char stateBitsEntry[252];
		unsigned char textureCount;
		unsigned char constantCount;
		unsigned char stateBitsCount;
		unsigned char stateFlags;
		unsigned char cameraRegion;
		unsigned char materialType;
		unsigned char layerCount;
		unsigned char assetFlags;
		MaterialTechniqueSet* techniqueSet;
		MaterialTextureDef* textureTable;
		MaterialConstantDef* constantTable;
		GfxStateBits* stateBitsTable; 
		unsigned char constantBufferIndex[252];
		char __pad0[4];
		MaterialConstantBufferDef* constantBufferTable;
		unsigned char constantBufferCount;
		const char** subMaterials;
	}; static_assert(sizeof(Material) == 0x270);

	static_assert(offsetof(Material, textureCount) == 0x12C);
	static_assert(offsetof(Material, techniqueSet) == 0x138);
	static_assert(offsetof(Material, textureTable) == 0x140);
	static_assert(offsetof(Material, constantTable) == 0x148);
	static_assert(offsetof(Material, stateBitsTable) == 0x150);
	static_assert(offsetof(Material, constantBufferTable) == 0x258);
	static_assert(offsetof(Material, constantBufferCount) == 0x260);
	static_assert(offsetof(Material, subMaterials) == 0x268);

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
	}; static_assert(sizeof(GfxImage) == 0x68);

	struct GfxLightImage
	{
		GfxImage* image;
		unsigned char samplerState;
	};

	struct GfxLightDef
	{
		const char* name;
		GfxLightImage attenuation;
		GfxLightImage cucoloris;
		int lmapLookupStart;
	}; static_assert(sizeof(GfxLightDef) == 0x30);

	struct GfxColorFloat
	{
		float array[4];
	};

	enum snd_alias_type_t : std::int8_t
	{
		SAT_UNKNOWN = 0x0,
		SAT_LOADED = 0x1,
		SAT_STREAMED = 0x2,
		SAT_PRIMED = 0x3,
		SAT_COUNT = 0x4,
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
		bool isLocalized;
		bool isStreamed;
		unsigned short fileIndex;
		StreamFileInfo info;
	};

	struct StreamedSound
	{
		StreamFileName filename;
		unsigned int totalMsec;
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

	struct PrimedSound
	{
		LoadedSound* loadedPart;
		StreamFileName streamedPart;
		int dataOffset; // not sure
		int totalSize; // not sure
	}; static_assert(sizeof(PrimedSound) == 0x28);

	union SoundFileRef
	{
		LoadedSound* loadSnd;
		StreamedSound streamSnd;
		PrimedSound primedSnd;
	};

	struct SoundFile
	{
		snd_alias_type_t type;
		bool exists;
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
		int unknown;
		ChannelMap channelMaps[2][2];
	}; static_assert(sizeof(SpeakerMap) == 0x148);

	struct DopplerPreset
	{
		const char* name;
		float speedOfSound;
		float playerVelocityScale;
		float minPitch;
		float maxPitch;
		float smoothing;
	}; static_assert(sizeof(DopplerPreset) == 0x20);

	union SoundAliasFlags
	{
		struct _
		{
			unsigned int looping : 1;
			unsigned int isMaster : 1;
			unsigned int isSlave : 1;
			unsigned int fullDryLevel : 1;
			unsigned int noWetLevel : 1;
			unsigned int is3d : 1;
			unsigned int type : 2;
		};
		unsigned int intValue;
	};

	struct snd_alias_t
	{
		const char* aliasName;
		const char* subtitle;
		const char* secondaryAliasName;
		const char* chainAliasName;
		SoundFile* soundFile;
		const char* mixerGroup;
		short poly;
		short polyGlobal;
		char polyEntityType;
		char polyGlobalType;
		char dspBusIndex;
		char priority;
		char __pad0[12]; // unknown
		float volMin;
		float volMax;
		short volModIndex;
		float pitchMin;
		float pitchMax;
		float distMin;
		float distMax;
		float velocityMin;
		int flags;
		char masterPriority;
		float masterPercentage;
		float slavePercentage;
		char playbackPercentage;
		float probability;
		char u1; // value: 0-4
		SndContext* sndContext;
		int sequence;
		float lfePercentage;
		float centerPercentage;
		int startDelay;
		SndCurve* sndCurve;
		float envelopMin;
		float envelopMax;
		SndCurve* lpfCurve;
		SndCurve* hpfCurve;
		SndCurve* reverbSendCurve;
		SpeakerMap* speakerMap;
		float reverbWetMixOverride;
		float reverbMultiplier;
		float smartPanDistance2d;
		float smartPanDistance3d;
		float smartPanAttenuation3d;
		float envelopPercentage;
		short stereo3dAngle;
		//char __padding4[2]; // padding
		float stereo3dStart;
		float stereo3dEnd;
		unsigned char allowDoppler;
		//char __padding5[3]; // padding
		DopplerPreset* dopplerPreset;
		float u2;
		//char __padding6[4]; // padding
	};

	static_assert(sizeof(snd_alias_t) == 256);

	struct snd_alias_context_list
	{
		short unk;
	};

	struct snd_alias_list_t
	{
		union
		{
			const char* aliasName;
			const char* name;
		};
		snd_alias_t* head;
		snd_alias_context_list* contextList;
		unsigned char count;
		unsigned char contextListCount;
		char __pad0[6];
	};

	struct LocalizeEntry
	{
		const char* value;
		const char* name;
	}; static_assert(sizeof(LocalizeEntry) == 0x10);

	struct TriggerModel
	{
		int contents;
		unsigned short hullCount;
		unsigned short firstHull;
	};

	struct TriggerHull
	{
		Bounds bounds;
		int contents;
		unsigned short slabCount;
		unsigned short firstSlab;
	};

	struct TriggerSlab
	{
		float dir[3];
		float midPoint;
		float halfSize;
	};

	struct MapTriggers
	{
		unsigned int count;
		TriggerModel* models;
		unsigned int hullCount;
		TriggerHull* hulls;
		unsigned int slabCount;
		TriggerSlab* slabs;
	};

	struct ClientTriggerAabbNode
	{
		Bounds bounds;
		unsigned short firstChild;
		unsigned short childCount;
	};

	struct ClientTriggers
	{
		MapTriggers trigger;
		unsigned short numClientTriggerNodes;
		ClientTriggerAabbNode* clientTriggerAabbTree;
		unsigned int triggerStringLength;
		char* triggerString;
		short* visionSetTriggers;
		short* blendLookup;
		short* unk1;
		short* triggerType;
		vec3_t* origins;
		float* scriptDelay;
		short* audioTriggers;
		short* unk2;
		short* unk3;
		short* unk4;
		short* unk5;
		short* unk6;
	}; static_assert(sizeof(ClientTriggers) == 0xB0);

	struct ClientTriggerBlendNode
	{
		float pointA[3];
		float pointB[3];
		unsigned short triggerA;
		unsigned short triggerB;
	};

	struct ClientTriggerBlend
	{
		unsigned short numClientTriggerBlendNodes;
		ClientTriggerBlendNode* blendNodes;
	};

	struct SpawnPointEntityRecord
	{
		unsigned short index;
		scr_string_t name;
		scr_string_t target;
		scr_string_t script_noteworthy;
		scr_string_t unknown;
		float origin[3];
		float angles[3];
	};

	struct SpawnPointRecordList
	{
		unsigned short spawnsCount;
		SpawnPointEntityRecord* spawns;
	};

	struct SplinePointEntityRecord
	{
		int splineId;
		int splineNodeId;
		char* splineNodeLabel;
		float splineNodeTension;
		float origin[3];
		float corridorDims[2];
		float tangent[3];
		float distToNextNode;
		vec3_t* positionCubic;
		vec3_t* tangentQuadratic;
	};

	struct SplinePointRecordList
	{
		unsigned short splinePointCount;
		float splineLength;
		SplinePointEntityRecord* splinePoints;
	};

	struct SplineRecordList
	{
		unsigned short splineCount;
		SplinePointRecordList* splines;
	};

	struct MapEnts
	{
		const char* name;
		char* entityString;
		int numEntityChars;
		MapTriggers trigger;
		ClientTriggers clientTrigger;
		ClientTriggerBlend clientTriggerBlend;
		SpawnPointRecordList spawnList;
		SplineRecordList splineList;
	}; static_assert(sizeof(MapEnts) == 0x128);

	struct AddonMapEntsUnk1
	{
		char __pad0[8];
	};

	struct AddonMapEntsUnk2_unk1
	{
		char __pad0[16];
	};

	struct AddonMapEntsUnk2_unk2
	{
		char __pad0[16];
	};

	struct AddonMapEntsUnk2
	{
		AddonMapEntsUnk2_unk1* unk1; // 0 [unkCount1]
		AddonMapEntsUnk2_unk2* unk2; // 8 [unkCount2]
		unsigned short* unk3; // 16 [unkCount2]
		unsigned short* unk4; // 24 [unkCount1]
		unsigned int* unk5; // 32 [unkCount3]
		unsigned char* unk6; // 40 [unkCount2]
		char __pad0[12];
		unsigned int unkCount1;
		unsigned int unkCount2;
		unsigned int unkCount3;
		char __pad1[40];
	};

	assert_sizeof(AddonMapEntsUnk2, 112);
	assert_offsetof(AddonMapEntsUnk2, unk1, 0);
	assert_offsetof(AddonMapEntsUnk2, unk2, 8);
	assert_offsetof(AddonMapEntsUnk2, unk3, 16);
	assert_offsetof(AddonMapEntsUnk2, unk4, 24);
	assert_offsetof(AddonMapEntsUnk2, unk5, 32);
	assert_offsetof(AddonMapEntsUnk2, unk6, 40);
	assert_offsetof(AddonMapEntsUnk2, unkCount1, 60);
	assert_offsetof(AddonMapEntsUnk2, unkCount2, 64);
	assert_offsetof(AddonMapEntsUnk2, unkCount3, 68);

	struct AddonMapEntsUnk3
	{
		char __pad0[80];
	};

	struct cmodel_t;
	struct ClipInfo;
	struct GfxBrushModel;

	struct AddonMapEnts
	{
		const char* name;
		char* entityString;
		int numEntityChars;
		MapTriggers trigger;
		ClipInfo* info;
		unsigned int numSubModels;
		cmodel_t* cmodels;
		GfxBrushModel* models;
		AddonMapEntsUnk1* unk1; // [numSubmodels]
		AddonMapEntsUnk2* unk2; // [unkCount1]
		AddonMapEntsUnk3* unk3; // [unkCount2]
		unsigned int unkCount1;
		unsigned int unkCount2;
	};

	static_assert(offsetof(AddonMapEnts, info) == 72);
	static_assert(offsetof(AddonMapEnts, cmodels) == 88);
	static_assert(offsetof(AddonMapEnts, numSubModels) == 80);
	assert_offsetof(AddonMapEnts, models, 96);
	assert_offsetof(AddonMapEnts, unk1, 104);
	assert_offsetof(AddonMapEnts, unk2, 112);
	assert_offsetof(AddonMapEnts, unk3, 120);
	assert_offsetof(AddonMapEnts, unkCount1, 128);
	assert_offsetof(AddonMapEnts, unkCount2, 132);

	static_assert(sizeof(AddonMapEnts) == 136);

	struct Clut
	{
		int count0;
		int count1;
		int count2;
		int pad;
		char* unk;
		const char* name;
	}; assert_sizeof(Clut, 0x20);

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

	struct pathnode_t;

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

	assert_sizeof(pathnode_constant_t, 0x60);
	assert_offsetof(pathnode_constant_t, parent.index, 56);

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
	}; assert_sizeof(PathData, 0x88);

	assert_offsetof(PathData, parentIndexResolved, 24);

	struct RawFile
	{
		const char* name;
		int compressedLen;
		int len;
		const char* buffer;
	}; static_assert(sizeof(RawFile) == 0x18);

	struct ScriptFile
	{
		const char* name;
		int compressedLen;
		int len;
		int bytecodeLen;
		const char* buffer;
		char* bytecode;
	}; static_assert(sizeof(ScriptFile) == 0x28);

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
	}; static_assert(sizeof(StringTable) == 0x18);

	struct StructuredDataEnumEntry
	{
		scr_string_t string;
		unsigned short index;
	};

	struct StructuredDataEnum
	{
		int entryCount;
		int reservedEntryCount;
		StructuredDataEnumEntry* entries;
	};

	enum StructuredDataTypeCategory
	{
		DATA_INT = 0x0,
		DATA_BYTE = 0x1,
		DATA_BOOL = 0x2,
		DATA_STRING = 0x3,
		DATA_ENUM = 0x4,
		DATA_STRUCT = 0x5,
		DATA_INDEXED_ARRAY = 0x6,
		DATA_ENUM_ARRAY = 0x7,
		DATA_FLOAT = 0x8,
		DATA_SHORT = 0x9,
		DATA_COUNT = 0xA,
	};

	union StructuredDataTypeUnion
	{
		unsigned int stringDataLength;
		int enumIndex;
		int structIndex;
		int indexedArrayIndex;
		int enumedArrayIndex;
		int index;
	};

	struct StructuredDataType
	{
		StructuredDataTypeCategory type;
		StructuredDataTypeUnion u;
	};

	enum StructuredDataValidationType
	{
		VALIDATION_NONE = 0x0,
	};

	struct StructuredDataStructProperty
	{
		scr_string_t name;
		StructuredDataType type;
		unsigned int offset;
		StructuredDataValidationType validation;
	};

	struct StructuredDataStruct
	{
		int propertyCount;
		StructuredDataStructProperty* properties;
		int size;
		unsigned int bitOffset;
	};

	struct StructuredDataIndexedArray
	{
		int arraySize;
		StructuredDataType elementType;
		unsigned int elementSize;
	};

	struct StructuredDataEnumedArray
	{
		int enumIndex;
		StructuredDataType elementType;
		unsigned int elementSize;
	};

	struct StructuredDataDef
	{
		int version;
		unsigned int formatChecksum;
		int enumCount;
		StructuredDataEnum* enums;
		int structCount;
		StructuredDataStruct* structs;
		int indexedArrayCount;
		StructuredDataIndexedArray* indexedArrays;
		int enumedArrayCount;
		StructuredDataEnumedArray* enumedArrays;
		StructuredDataType rootType;
		unsigned int size;
	}; static_assert(sizeof(StructuredDataDef) == 0x58);

	struct StructuredDataDefSet
	{
		const char* name;
		unsigned int defCount;
		StructuredDataDef* defs;
	}; static_assert(sizeof(StructuredDataDefSet) == 0x18);

	enum NetConstStringType
	{
	};

	enum NetConstStringSource
	{
	};

	struct NetConstStrings
	{
		const char* name;
		NetConstStringType stringType;
		NetConstStringSource sourceType;
		unsigned int entryCount;
		const char** stringList;
	}; static_assert(sizeof(NetConstStrings) == 0x20);

	struct LuaFile
	{
		const char* name;
		int len;
		char strippingType;
		const char* buffer;
	}; static_assert(sizeof(LuaFile) == 0x18);

	struct TTFDef
	{
		const char* name;
		int fileLen;
		const char* file;
		void* ftFace;
	}; static_assert(sizeof(TTFDef) == 0x20);

	struct FxParticleSimAnimationHeader
	{
		float playbackRate;
		float duration;
		unsigned int frameCount;
		float minX;
		float minY;
		float minZ;
		float boundsXDelta;
		float boundsYDelta;
		float boundsZDelta;
		float maxWidth;
		float maxHeight;
		unsigned int colorTableSize;
		unsigned int particleDataCount;
		bool evalVisStatePerParticle;
		bool sortParticlesAtRuntime;
	};

	struct FxParticleSimAnimationParticleData
	{
		unsigned short xNormalizedPos;
		unsigned short yNormalizedPos;
		unsigned short zNormalizedPos;
		unsigned short xNormalizedWidth;
		unsigned short yNormalizedHeight;
		unsigned short orientation;
		unsigned short lifetime;
		unsigned short particleID;
		unsigned short xNormalizedPosNextFrame;
		unsigned short yNormalizedPosNextFrame;
		unsigned short zNormalizedPosNextFrame;
		unsigned short xNormalizedWidthNextFrame;
		unsigned short yNormalizedHeightNextFrame;
		short orientationDelta;
		unsigned short colorTableIndex;
		unsigned short nextColorTableIndex;
	};

	struct FxParticleSimAnimationFrame
	{
		unsigned int particleDataOffset;
		unsigned int numActiveParticles;
	};

	struct FxParticleSimAnimation
	{
		const char* name;
		Material* material;
		FxParticleSimAnimationHeader header;
		FxParticleSimAnimationParticleData* particleData;
		FxParticleSimAnimationFrame* frames;
		GfxColorFloat* colorTable;
	};

	enum FxElemType : std::uint8_t
	{
		FX_ELEM_TYPE_SPRITE_BILLBOARD = 0,
		FX_ELEM_TYPE_SPRITE_ORIENTED = 1,
		FX_ELEM_TYPE_SPRITE_ROTATED = 2,
		FX_ELEM_TYPE_TAIL = 3,
		FX_ELEM_TYPE_LINE = 4,
		FX_ELEM_TYPE_TRAIL = 5,
		FX_ELEM_TYPE_FLARE = 6,
		FX_ELEM_TYPE_PARTICLE_SIM_ANIMATION = 7,
		FX_ELEM_TYPE_CLOUD = 8,
		FX_ELEM_TYPE_SPARK_CLOUD = 9,
		FX_ELEM_TYPE_SPARK_FOUNTAIN = 10,
		FX_ELEM_TYPE_MODEL = 11,
		FX_ELEM_TYPE_OMNI_LIGHT = 12,
		FX_ELEM_TYPE_SPOT_LIGHT = 13,
		FX_ELEM_TYPE_SOUND = 14,
		FX_ELEM_TYPE_DECAL = 15,
		FX_ELEM_TYPE_RUNNER = 16,
		FX_ELEM_TYPE_VECTORFIELD = 17,
	};

	struct FxFloatRange
	{
		float base;
		float amplitude;
	};

	struct FxSpawnDefLooping
	{
		int intervalMsec;
		int count;
	};

	struct FxIntRange
	{
		int base;
		int amplitude;
	};

	struct FxSpawnDefOneShot
	{
		FxIntRange count;
	};

	union FxSpawnDef
	{
		FxSpawnDefLooping looping;
		FxSpawnDefOneShot oneShot;
	};

	struct FxElemAtlas
	{
		unsigned char behavior;
		unsigned char index;
		unsigned char fps;
		unsigned char loopCount;
		unsigned char colIndexBits;
		unsigned char rowIndexBits;
		short entryCount;
	};

	struct FxEffectDef;
	union FxEffectDefRef
	{
		FxEffectDef* handle;
		const char* name;
	};

	struct FxElemVec3Range
	{
		float base[3];
		float amplitude[3];
	};

	struct FxElemVelStateInFrame
	{
		FxElemVec3Range velocity;
		FxElemVec3Range totalDelta;
		FxElemVec3Range unk_range;
	};

	struct FxElemVelStateSample
	{
		FxElemVelStateInFrame local;
		FxElemVelStateInFrame world;
	}; static_assert(sizeof(FxElemVelStateSample) == 144);

	struct FxElemVisualState
	{
		float color[4];
		float pad1[3];
		float rotationDelta;
		float rotationTotal;
		float size[2];
		float scale;
		float pad2[2];
	};

	static_assert(sizeof(FxElemVisualState) == 56);

	struct FxElemVisStateSample
	{
		FxElemVisualState base;
		FxElemVisualState amplitude;
	}; static_assert(sizeof(FxElemVisStateSample) == 112);

	struct FxElemMarkVisuals
	{
		Material* materials[3];
	};

	struct XModel;
	union FxElemVisuals
	{
		const void* anonymous;
		Material* material;
		XModel* model;
		FxEffectDefRef effectDef;
		const char* soundName;
		const char* vectorFieldName;
		GfxLightDef* lightDef;
		FxParticleSimAnimation* particleSimAnimation;
	};

	union FxElemDefVisuals
	{
		FxElemMarkVisuals* markArray;
		FxElemVisuals* array;
		FxElemVisuals instance;
	};

	struct FxTrailVertex
	{
		float pos[2];
		float normal[2];
		float texCoord[2];
		char __pad0[8];
	}; static_assert(sizeof(FxTrailVertex) == 32);

	struct FxTrailDef
	{
		int scrollTimeMsec;
		int repeatDist;
		float invSplitDist;
		float invSplitArcDist;
		float invSplitTime;
		char __pad0[12];
		int vertCount;
		FxTrailVertex* verts;
		int indCount;
		unsigned short* inds;
	}; static_assert(sizeof(FxTrailDef) == 0x40);
	static_assert(offsetof(FxTrailDef, verts) == 40);
	static_assert(offsetof(FxTrailDef, inds) == 56);
	static_assert(offsetof(FxTrailDef, vertCount) == 32);
	static_assert(offsetof(FxTrailDef, indCount) == 48);

	struct FxSparkFountainDef
	{
		float gravity;
		float bounceFrac;
		float bounceRand;
		float sparkSpacing;
		float sparkLength;
		int sparkCount;
		float loopTime;
		float velMin;
		float velMax;
		float velConeFrac;
		float restSpeed;
		float boostTime;
		float boostFactor;
	}; static_assert(sizeof(FxSparkFountainDef) == 0x34);

	struct FxSpotLightDef
	{
		float fovInnerFraction;
		float startRadius;
		float endRadius;
		float brightness;
		float maxLength;
		int exponent;
		char __pad0[24];
	}; static_assert(sizeof(FxSpotLightDef) == 0x30);

	struct FxOmniLightDef
	{
		char __pad0[16];
	}; static_assert(sizeof(FxOmniLightDef) == 0x10);

	struct FxFlareDef
	{
		float position;
		int angularRotCount;
		int flags;
		FxFloatRange depthScaleRange;
		FxFloatRange depthScaleValue;
		FxFloatRange radialRot;
		FxFloatRange radialScaleX;
		FxFloatRange radialScaleY;
		float dir[3];
		int intensityXIntervalCount;
		int intensityYIntervalCount;
		int srcCosIntensityIntervalCount;
		int srcCosScaleIntervalCount;
		float* intensityX;
		float* intensityY;
		float* srcCosIntensity;
		float* srcCosScale;
	}; static_assert(sizeof(FxFlareDef) == 0x70);

	struct FxDecalDef
	{
		char __pad0[3];
	};

	union FxElemExtendedDefPtr
	{
		char* unknownDef;
		FxTrailDef* trailDef;
		FxSparkFountainDef* sparkFountainDef;
		FxSpotLightDef* spotLightDef;
		FxOmniLightDef* omniLightDef;
		FxFlareDef* flareDef;
		FxDecalDef* decalDef;
	};

	struct FxElemDef
	{
		int flags;
		int flags2;
		FxSpawnDef spawn;
		FxFloatRange spawnRange;
		FxFloatRange fadeInRange;
		FxFloatRange fadeOutRange;
		float spawnFrustumCullRadius;
		FxIntRange spawnDelayMsec;
		FxIntRange lifeSpanMsec;
		FxFloatRange spawnOrigin[3];
		char __pad0[4];
		FxFloatRange spawnOffsetRadius;
		FxFloatRange spawnOffsetHeight;
		char __pad1[8];
		FxFloatRange spawnAngles[3];
		FxFloatRange angularVelocity[3];
		FxFloatRange initialRotation;
		FxFloatRange gravity;
		FxFloatRange reflectionFactor;
		FxElemAtlas atlas;
		FxElemType elemType;
		unsigned char elemLitType;
		unsigned char visualCount;
		unsigned char velIntervalCount;
		unsigned char visStateIntervalCount;
		FxElemVelStateSample* velSamples;
		FxElemVisStateSample* visSamples;
		FxElemDefVisuals visuals;
		Bounds collBounds;
		FxEffectDefRef effectOnImpact;
		FxEffectDefRef effectOnDeath;
		FxEffectDefRef effectEmitted;
		FxFloatRange emitDist;
		FxFloatRange emitDistVariance;
		FxElemExtendedDefPtr extended;
		unsigned char sortOrder;
		unsigned char lightingFrac;
		unsigned char useItemClip;
		unsigned char fadeInfo;
		int randomSeed;
		float unk_floats[4];
	}; static_assert(sizeof(FxElemDef) == 0x140);

	static_assert(offsetof(FxElemDef, spawnOrigin) == 60);
	static_assert(offsetof(FxElemDef, spawnOffsetRadius) == 88);
	static_assert(offsetof(FxElemDef, spawnOffsetHeight) == 96);
	static_assert(offsetof(FxElemDef, spawnAngles) == 112);
	static_assert(offsetof(FxElemDef, spawnDelayMsec) == 44);
	static_assert(offsetof(FxElemDef, lifeSpanMsec) == 52);
	static_assert(offsetof(FxElemDef, spawn) == 8);
	static_assert(offsetof(FxElemDef, visualCount) == 194);
	static_assert(offsetof(FxElemDef, effectOnDeath) == 256);
	static_assert(offsetof(FxElemDef, spawnAngles) == 112);
	static_assert(offsetof(FxElemDef, elemType) == 192);
	static_assert(offsetof(FxElemDef, visSamples) == 0xD0);
	static_assert(offsetof(FxElemDef, visuals) == 0xD8);
	static_assert(offsetof(FxElemDef, effectOnImpact) == 0xF8);
	static_assert(offsetof(FxElemDef, effectOnDeath) == 0x100);
	static_assert(offsetof(FxElemDef, effectEmitted) == 0x108);
	static_assert(offsetof(FxElemDef, extended) == 0x120);
	static_assert(offsetof(FxElemDef, effectEmitted) == 0x108);
	static_assert(offsetof(FxElemDef, visStateIntervalCount) == 0xC4);
	static_assert(offsetof(FxElemDef, visualCount) == 194);
	static_assert(offsetof(FxElemDef, velIntervalCount) == 0xC3);

	struct FxEffectDef
	{
		const char* name;
		int flags;
		int totalSize;
		int msecLoopingLife;
		int elemDefCountLooping;
		int elemDefCountEmission;
		int elemDefCountOneShot;
		float elemMaxRadius;
		float occlusionQueryDepthBias;
		int occlusionQueryFadeIn;
		int occlusionQueryFadeOut;
		FxFloatRange occlusionQueryScaleRange;
		float xU_01;
		FxElemDef* elemDefs;
	}; static_assert(sizeof(FxEffectDef) == 0x48);

	static_assert(offsetof(FxEffectDef, elemDefs) == 0x40);
	static_assert(offsetof(FxEffectDef, elemDefCountLooping) == 0x14);
	//static_assert(offsetof(FxEffectDef, elemDefCountOneShot) == 0x18);
	//static_assert(offsetof(FxEffectDef, elemDefCountEmission) == 0x1C);

	struct XModelIKData
	{
		unsigned char charDataLen;
		unsigned char floatDataLen;
		unsigned char int32DataLen;
		unsigned char stringsCount;
		char* charData;
		float* floatData;
		int* int32Data;
		scr_string_t* strings;
	}; static_assert(sizeof(XModelIKData) == 0x28);

	struct SkeletonScriptCode
	{
		char __pad0[4];
	};

	struct SkeletonScript
	{
		const char* name;
		XModelIKData ikData;
		unsigned short codeLen;
		SkeletonScriptCode* code;
	}; static_assert(sizeof(SkeletonScript) == 0x40);

	static_assert(offsetof(SkeletonScript, ikData) == 8);
	static_assert(offsetof(XModelIKData, strings) == 32);
	static_assert(offsetof(XModelIKData, stringsCount) == 3);
	static_assert(offsetof(XModelIKData, int32Data) == 24);
	static_assert(offsetof(XModelIKData, int32DataLen) == 2);
	static_assert(offsetof(XModelIKData, floatData) == 16);
	static_assert(offsetof(XModelIKData, floatDataLen) == 1);

	union XAnimDynamicFrames
	{
		unsigned char(*_1)[3];
		unsigned short(*_2)[3];
	};

	union XAnimDynamicIndices
	{
		unsigned char _1[1];
		unsigned short _2[1];
	};

	struct XAnimPartTransFrames
	{
		float mins[3];
		float size[3];
		XAnimDynamicFrames frames;
		XAnimDynamicIndices indices;
	};

	union XAnimPartTransData
	{
		XAnimPartTransFrames frames;
		float frame0[3];
	};

	struct XAnimPartTrans
	{
		unsigned short size;
		unsigned char smallTrans;
		XAnimPartTransData u;
	};

	struct XAnimDeltaPartQuatDataFrames2
	{
		short(*frames)[2];
		XAnimDynamicIndices indices;
	};

	union XAnimDeltaPartQuatData2
	{
		XAnimDeltaPartQuatDataFrames2 frames;
		short frame0[2];
	};

	struct XAnimDeltaPartQuat2
	{
		unsigned short size;
		XAnimDeltaPartQuatData2 u;
	};

	struct XAnimDeltaPartQuatDataFrames
	{
		short(*frames)[4];
		XAnimDynamicIndices indices;
	};

	union XAnimDeltaPartQuatData
	{
		XAnimDeltaPartQuatDataFrames frames;
		short frame0[4];
	};

	struct XAnimDeltaPartQuat
	{
		unsigned short size;
		XAnimDeltaPartQuatData u;
	};

	struct XAnimDeltaPart
	{
		XAnimPartTrans* trans;
		XAnimDeltaPartQuat2* quat2;
		XAnimDeltaPartQuat* quat;
	};

	union XAnimIndices
	{
		unsigned char* _1;
		unsigned short* _2;
		void* data;
	};

	struct XAnimNotifyInfo
	{
		scr_string_t name;
		float time;
	};

	enum XAnimPartsFlags : std::uint8_t
	{
		ANIM_NONE = 0x0,
		ANIM_LOOP = 0x1,
		ANIM_DELTA = 0x2,
		ANIM_DELTA_3D = 0x4,
		ANIM_DEFAULT = 0x8,
	};

	typedef float BlendShapeWeight;
	typedef char XAnimScriptedViewmodelAnimData;

	struct XAnimParts
	{
		const char* name; // 0
		unsigned short dataByteCount; // 8
		unsigned short dataShortCount; // 10
		unsigned short dataIntCount; // 12
		unsigned short numframes; // 14
		unsigned char flags; // 15
		unsigned char boneCount[10]; // 16
		char u1; // unused?
		char u2; // unused?
		unsigned char notifyCount; // 29
		unsigned char assetType; // 30
		unsigned char ikType; // 31
		unsigned int randomDataByteCount; // 32
		unsigned int randomDataShortCount; // 36
		unsigned int randomDataIntCount; // 40
		unsigned int indexCount; // 44
		float framerate;  // 48
		float frequency; // 56
		scr_string_t* names; // 56
		char* dataByte; // 64
		short* dataShort; // 72
		int* dataInt; // 80
		short* randomDataShort; // 88
		unsigned char* randomDataByte; // 96
		int* randomDataInt; // 104
		XAnimIndices indices; // 112
		XAnimNotifyInfo* notify; // 120
		XAnimDeltaPart* deltaPart; // 128
		const char* secondaryName; // 136
		short u3; // unknown
		unsigned short blendShapeWeightCount; // 146
		short u4; // unused? padding?
		scr_string_t* blendShapeWeightNames; // 152
		char(*blendShapeWeightUnknown1)[3]; // 160
		unsigned short* blendShapeWeightUnknown2; // 168
		unsigned short* blendShapeWeightUnknown3; // 176
		unsigned short* blendShapeWeightUnknown4; // 184
		BlendShapeWeight* blendShapeWeights; // 192
		std::uint64_t u5; // unused?
		XAnimScriptedViewmodelAnimData* scriptedViewmodelAnimData; // 208 // count = 8
	}; static_assert(sizeof(XAnimParts) == 0xD8);

	union PackedUnitVec
	{
		unsigned int packed;
	};

	union PackedTexCoords
	{
		unsigned int packed;
	};

	union GfxColor
	{
		unsigned char array[4];
		unsigned int packed;
	};

	struct GfxPackedVertex
	{
		float xyz[3];
		float binormalSign;
		GfxColor color;
		PackedTexCoords texCoord;
		PackedUnitVec normal;
		PackedUnitVec tangent;
	};

	struct GfxPackedMotionVertex
	{
		float xyz[3];
		float binormalSignAndHeight;
		GfxColor pieceIndex;
		PackedTexCoords texCoord;
		PackedUnitVec normal;
		PackedUnitVec tangent;
	};

	union GfxVertexUnion0
	{
		GfxPackedVertex* packedVerts0;
		GfxPackedMotionVertex* packedMotionVerts0;
		void* verts0;
	};

	struct Face
	{
		unsigned short v1;
		unsigned short v2;
		unsigned short v3;
	};

	struct XSurfaceCollisionAabb
	{
		unsigned short mins[3];
		unsigned short maxs[3];
	};

	struct XSurfaceCollisionNode
	{
		XSurfaceCollisionAabb aabb;
		unsigned short childBeginIndex;
		unsigned short childCount;
	};

	struct XSurfaceCollisionLeaf
	{
		unsigned short triangleBeginIndex;
	};

	struct XSurfaceCollisionTree
	{
		float trans[3];
		float scale[3];
		unsigned int nodeCount;
		XSurfaceCollisionNode* nodes;
		unsigned int leafCount;
		XSurfaceCollisionLeaf* leafs;
	};

	struct XRigidVertList
	{
		unsigned short boneOffset;
		unsigned short vertCount;
		unsigned short triOffset;
		unsigned short triCount;
		XSurfaceCollisionTree* collisionTree;
	};

	struct UnknownXSurface0
	{
		float xyz[3];
		PackedUnitVec normal;
	};

	struct BlendVertsUnknown
	{
		unsigned short b[15];
		unsigned short blendVertCountIndex; // 30
	}; static_assert(sizeof(BlendVertsUnknown) == 32);

	struct XSubdivRigidVertList
	{
		unsigned int firstFace;
		unsigned int faceCount;
		unsigned int firstRegularPatch;
		unsigned int regularPatchCount;
	};

	struct XSurfaceSubdivLevel
	{
		XSubdivRigidVertList* rigidVertLists;
		unsigned int faceCount;
		unsigned int regularPatchCount;
		unsigned int regularPatchOffset;
		unsigned int facePointCount;
		unsigned int facePointValence4Count;
		unsigned int facePointBufferSize;
		unsigned int edgePointCount;
		unsigned int edgePointSmoothEnd;
		unsigned int edgePointUVBorderEnd;
		unsigned int vertexPointCount;
		unsigned int vertexPointValence4Count;
		unsigned int vertexPointBufferSize;
		unsigned int normalCount;
		unsigned int normalBufferSize;
		unsigned int transitionPointCount;
		unsigned int vertCount;
		unsigned int vertOffset;
		unsigned short* faceIndices;
		unsigned short* regularPatchIndices;
		unsigned int* regularPatchFlags;
		unsigned int* facePoints;
		unsigned int* edgePoints;
		unsigned int* vertexPoints;
		unsigned int* normals;
		unsigned int* transitionPoints;
		float* regularPatchCones;
		ID3D11Buffer* regularPatchIndexBuffer;
		ID3D11Buffer* faceIndexBuffer;
		ID3D11ShaderResourceView* regularPatchIndexBufferView;
		ID3D11ShaderResourceView* regularPatchFlagsView;
		ID3D11ShaderResourceView* facePointsView;
		ID3D11ShaderResourceView* edgePointsView;
		ID3D11ShaderResourceView* vertexPointsView;
		ID3D11ShaderResourceView* normalsView;
		ID3D11ShaderResourceView* transitionPointsView;
		ID3D11ShaderResourceView* regularPatchConesView;
	}; static_assert(sizeof(XSurfaceSubdivLevel) == 0xE8);

	struct GfxSubdivCache
	{
		unsigned int size;
		ID3D11Buffer* subdivCacheBuffer;
		ID3D11ShaderResourceView* subdivCacheView;
	}; static_assert(sizeof(GfxSubdivCache) == 0x18);

	struct XSurfaceSubdivInfo
	{
		XSurfaceSubdivLevel* levels;
		char __pad0[24];
		GfxSubdivCache cache;
	}; static_assert(sizeof(XSurfaceSubdivInfo) == 0x38);

	struct BlendShapeVert
	{
		char __pad0[32];
	};

	struct BlendShape
	{
		unsigned int vertCount;
		BlendShapeVert* verts;
		ID3D11Buffer* blendShapeVertsBuffer;
		ID3D11ShaderResourceView* blendShapeVertsView;
	};

	typedef char alignCompBufByte_t;
	typedef unsigned short alignVertBufFloat16Vec2_t[2];
	typedef unsigned short alignCompBufUShort_t;
	typedef float alignCompBufFloat_t;
	typedef unsigned short XBlendInfo;

	enum XSurfaceFlags : std::uint16_t
	{
		SURF_FLAG_VERTCOL_GREY = 0x1,
		SURF_FLAG_VERTCOL_NONE = 0x2,
		SURF_FLAG_SKINNED = 0x4,
		SURF_FLAG_REACTIVE_MOTION = 0x8,
		SURF_FLAG_LIGHTMAP_COORDS = 0x10,
		SURF_FLAG_TENSION = 0x20,
	};

	struct XSurface
	{
		unsigned short flags;
		unsigned short vertCount;
		unsigned short triCount;
		unsigned char rigidVertListCount;
		unsigned char subdivLevelCount;
		short blendVertCounts[8];
		GfxVertexUnion0 verts0;
		Face* triIndices;
		Face* triIndices2;
		ID3D11Buffer* vb0;
		ID3D11ShaderResourceView* vb0View;
		ID3D11Buffer* indexBuffer;
		XRigidVertList* rigidVertLists;
		UnknownXSurface0* unknown0;
		XBlendInfo* blendVerts;
		BlendVertsUnknown* blendVertsTable;
		ID3D11Buffer* blendVertsBuffer;
		ID3D11ShaderResourceView* blendVertsView;
		alignVertBufFloat16Vec2_t* lmapUnwrap;
		ID3D11Buffer* vblmapBuffer;
		ID3D11ShaderResourceView* vblmapView;
		XSurfaceSubdivInfo* subdiv;
		alignCompBufFloat_t* tensionData;
		alignCompBufUShort_t* tensionAccumTable;
		ID3D11Buffer* tensionAccumTableBuffer;
		ID3D11ShaderResourceView* tensionAccumTableView;
		ID3D11Buffer* tensionDataBuffer;
		ID3D11ShaderResourceView* tensionDataView;
		ID3D11ShaderResourceView* indexBufferView;
		BlendShape* blendShapes;
		unsigned int blendShapesCount;
		unsigned int vertexLightingIndex;
		char __pad0[4];
		int partBits[8];
		char __pad1[4];
	}; static_assert(sizeof(XSurface) == 0x108);

	struct XModelSurfs
	{
		const char* name;
		XSurface* surfs;
		unsigned short numsurfs;
		int partBits[8];
	}; static_assert(sizeof(XModelSurfs) == 0x38);

	struct ReactiveMotionModelPart
	{
		char __pad0[32];
	}; static_assert(sizeof(ReactiveMotionModelPart) == 32);

	struct XModelLodInfo
	{
		float dist; // 0
		unsigned short numsurfs; // 4
		unsigned short surfIndex; // 6
		XModelSurfs* modelSurfs; // 8
		int partBits[8]; // 16 20 24 28 32 36 40 44
		XSurface* surfs; // 48
		ReactiveMotionModelPart* reactiveMotionParts; // 56 (sizeof struct = 32) (reactiveMotionParts?)
		char __pad0[5]; // 64 (something must be here)
		char numReactiveMotionParts; // 69 (numReactiveMotionParts?)
		char __pad1[2]; // 70 (padding?)
	}; static_assert(sizeof(XModelLodInfo) == 72);

	struct XModelAngle
	{
		short x;
		short y;
		short z;
		short base;
	};

	struct XModelTagPos
	{
		float x;
		float y;
		float z;
	};

	struct DObjAnimMat
	{
		float quat[4];
		float trans[3];
		float transWeight;
	};

	struct ReactiveMotionModelTweaks
	{
		float scale[4];
	};

	struct XModelCollSurf_s
	{
		Bounds bounds;
		int boneIdx;
		int contents;
		int surfFlags;
	};

	struct XBoneInfo
	{
		Bounds bounds;
		union
		{
			float radiusSquared;
			unsigned int radiusSquaredAsInt;
		};
	};

	struct BlendShapeWeightMap
	{
		unsigned short weightIndex;
		unsigned short targetIndex;
		float fullWeight;
	};

	struct ExtentBounds
	{
		vec3_t mins;
		vec3_t maxs;
	};

	struct MdaoVolume
	{
		ExtentBounds bounds;
		unsigned __int16 cellCount[3];
		unsigned __int16 parentBoneIndex;
		GfxImage* volumeData;
	}; static_assert(sizeof(MdaoVolume) == 0x28);

	struct XPhysBoneInfo
	{
		PhysPreset* physPreset;
		PhysConstraint* physContraint;
		PhysCollmap* physCollmap;
		char __pad0[8];
	}; static_assert(sizeof(XPhysBoneInfo) == 0x20);

	enum XModelFlags : std::uint16_t
	{
		XMODEL_FLAG_COMPOSITE = 0x400,
	};

	struct XModel
	{
		const char* name; // 0
		unsigned char numBones; // 8
		unsigned char numRootBones; // 9
		unsigned char numsurfs; // 10
		unsigned char lodRampType; // 11
		unsigned char numBonePhysics; // 12
		unsigned char numCompositeModels; // 13
		char __pad0[2]; // 14-16
		float unk_float; // 16
		float scale;
		unsigned int noScalePartBits[8]; // 20
		scr_string_t* boneNames; // 56
		unsigned char* parentList; // 64
		XModelAngle* tagAngles; // 72
		XModelTagPos* tagPositions; // 80
		unsigned char* partClassification; // 88
		DObjAnimMat* baseMat; // 96
		ReactiveMotionModelTweaks* reactiveMotionTweaks; // 104
		Material** materialHandles; // 112
		XModelLodInfo lodInfo[6]; // 120
		char numLods; // 552
		char collLod; // 553
		char __pad1[2]; // 554-556
		short flags; // 556
		short numCollSurfs; // 558
		XModelCollSurf_s* collSurfs; // 560
		XBoneInfo* boneInfo; // 568
		int contents;
		float radius;
		Bounds bounds;
		unsigned short* invHighMipRadius; // 608
		MdaoVolume* mdaoVolumes; // 616
		unsigned short mdaoVolumeCount; // 624
		unsigned short targetCount; // 626
		unsigned short numberOfWeights; // 628
		unsigned short numberOfWeightMaps; // 630
		scr_string_t* weightNames; // 632
		BlendShapeWeightMap* blendShapeWeightMap; // 640
		PhysPreset* physPreset; // 648
		PhysCollmap* physCollmap; // 656
		float quantization;
		int memUsage;
		SkeletonScript* skeletonScript; // 672
		XModel** compositeModels; // 680
		XPhysBoneInfo* bonePhysics; // 688
	};  static_assert(sizeof(XModel) == 0x2B8);

	assert_offsetof(XModel, noScalePartBits, 24);
	assert_offsetof(XModel, boneNames, 56);
	assert_offsetof(XModel, bonePhysics, 688);
	assert_offsetof(XModel, lodInfo, 120);

	enum activeReticleType_t : std::int32_t
	{
		VEH_ACTIVE_RETICLE_NONE = 0x0,
		VEH_ACTIVE_RETICLE_PIP_ON_A_STICK = 0x1,
		VEH_ACTIVE_RETICLE_BOUNCING_DIAMOND = 0x2,
		VEH_ACTIVE_RETICLE_COUNT = 0x3,
	};

	enum playerAnimType_t : std::uint8_t
	{
		PLAYERANIMTYPE_NONE = 0x0,
		PLAYERANIMTYPE_OTHER = 0x1,
		PLAYERANIMTYPE_PISTOL = 0x2,
		PLAYERANIMTYPE_SMG = 0x3,
		PLAYERANIMTYPE_AUTORIFLE = 0x4,
		PLAYERANIMTYPE_SNIPER = 0x5,
		PLAYERANIMTYPE_ROCKET_LAUNCHER = 0x6,
		PLAYERANIMTYPE_GRENADE = 0x7,
		PLAYERANIMTYPE_M203 = 0x8,
		PLAYERANIMTYPE_HOLD = 0x9,
		PLAYERANIMTYPE_BRIEFCASE = 0xA,
		PLAYERANIMTYPE_RIOTSHIELD = 0xB,
		PLAYERANIMTYPE_LAPTOP = 0xC,
		PLAYERANIMTYPE_THROWINGKNIFE = 0xD,
		PLAYERANIMTYPE_MINIGUN = 0xE,
		PLAYERANIMTYPE_SMG_BULLPUP = 0x1F,
		PLAYERANIMTYPE_AUTOFILE_BULLPUP = 0x10,
		PLAYERANIMTYPE_SNIPER_BULLPUP = 0x11,
		PLAYERANIMTYPE_KILLSTREAKTRIGGER = 0x12,
		PLAYERANIMTYPE_TROPHYSYSTEM = 0x13,
		PLAYERANIMTYPE_COMBATKNIFE = 0x14,
		PLAYERANIMTYPE_NUM = 0x15,
	};

	enum weapType_t : std::int32_t
	{
		WEAPCLASS_NONE = 0x0,
		WEAPCLASS_BULLET = 0x1,
		WEAPCLASS_GRENADE = 0x2,
		WEAPCLASS_PROJECTILE = 0x3,
		WEAPCLASS_RIOTSHIELD = 0x4,
		WEAPCLASS_ENERGY = 0x5,
		WEAPCLASS_NUM = 0x6,
	};

	enum weapClass_t : std::int32_t
	{
		WEAPTYPE_RIFLE = 0x1,
		WEAPTYPE_SNIPER = 0x2,
		WEAPTYPE_MG = 0x3,
		WEAPTYPE_SMG = 0x4,
		WEAPTYPE_SPREAD = 0x5,
		WEAPTYPE_PISTOL = 0x6,
		WEAPTYPE_ROCKETLAUNCHER = 0x7,
		WEAPTYPE_TURRET = 0x8,
		WEAPTYPE_THROWINGKNIFE = 0x9,
		WEAPTYPE_NON_PLAYER = 0xA,
		WEAPTYPE_ITEM = 0xB,
		WEAPTYPE_CONE = 0xC,
		WEAPTYPE_BEAM = 0xD,
		WEAPTYPE_SHIELD = 0xE,
		WEAPTYPE_HOVER = 0xF,
		WEAPTYPE_CLOAK = 0x10,
		WEAPTYPE_PING = 0x11,
		WEAPTYPE_REPULSOR = 0x12,
		WEAPTYPE_ADRENALINE = 0x13,
		WEAPTYPE_HEALTH = 0x14,
		WEAPTYPE_MUTE = 0x15,
		WEAPTYPE_UNDERWATER = 0x16,
		WEAPTYPE_BALL = 0x17,
		WEAPTYPE_NUM = 0x18,
	};

	enum weapInventoryType_t : std::int32_t
	{
		WEAPINVENTORY_PRIMARY = 0x0,
		WEAPINVENTORY_OFFHAND = 0x1,
		WEAPINVENTORY_ITEM = 0x2,
		WEAPINVENTORY_ALTMODE = 0x3,
		WEAPINVENTORY_EXCLUSIVE = 0x4,
		WEAPINVENTORY_SCAVENGER = 0x5,
		WEAPINVENTORYCOUNT = 0x6,
	};

	enum weapFireType_t : std::int32_t
	{
		WEAPON_FIRETYPE_FULLAUTO = 0x0,
		WEAPON_FIRETYPE_SINGLESHOT = 0x1,
		WEAPON_FIRETYPE_BURSTFIRE2 = 0x2,
		WEAPON_FIRETYPE_BURSTFIRE3 = 0x3,
		WEAPON_FIRETYPE_BURSTFIRE4 = 0x4,
		WEAPON_FIRETYPE_DOUBLEBARREL = 0x5,
		WEAPON_FIRETYPECOUNT = 0x6,
		WEAPON_FIRETYPE_BURSTFIRE_FIRST = 0x2,
		WEAPON_FIRETYPE_BURSTFIRE_LAST = 0x4,
	};

	enum PenetrateType : std::uint32_t
	{
		PENETRATE_TYPE_NONE = 0x0,
		PENETRATE_TYPE_SMALL = 0x1,
		PENETRATE_TYPE_MEDIUM = 0x2,
		PENETRATE_TYPE_LARGE = 0x3,
		PENETRATE_TYPE_COUNT = 0x4,
	};

	enum ImpactType : std::int32_t
	{

	};

	enum weapStance_t : std::int32_t
	{
		WEAPSTANCE_STAND = 0x0,
		WEAPSTANCE_DUCK = 0x1,
		WEAPSTANCE_PRONE = 0x2,
		WEAPSTANCE_NUM = 0x3,
	};

	enum OffhandClass : std::uint32_t
	{
		OFFHAND_CLASS_NONE = 0x0,
		OFFHAND_CLASS_FRAG_GRENADE = 0x1,
		OFFHAND_CLASS_SMOKE_GRENADE = 0x2,
		OFFHAND_CLASS_FLASH_GRENADE = 0x3,
		OFFHAND_CLASS_OTHER = 0x9,
		OFFHAND_CLASS_COUNT = 0xA,
	};

	enum weapProjExposion_t : std::int32_t
	{
		WEAPPROJEXP_GRENADE = 0x0,
		WEAPPROJEXP_ROCKET = 0x1,
		WEAPPROJEXP_FLASHBANG = 0x2,
		WEAPPROJEXP_NONE = 0x3,
		WEAPPROJEXP_DUD = 0x4,
		WEAPPROJEXP_SMOKE = 0x5,
		WEAPPROJEXP_HEAVY = 0x6,
		WEAPPROJEXP_NUM = 0x7,
	};

	enum weapAdsFireMode_t : std::int32_t
	{
		WEAPADSFIREMODE_NUM = 0x3,
	};

	enum weapGreebleType_t : std::int32_t
	{
		WEAPGREEBLE_NONE = 0x0,
		WEAPGREEBLE_ACCURACYLEVEL1 = 0x1,
		WEAPGREEBLE_ACCURACYLEVEL2 = 0x2,
		WEAPGREEBLE_FIRERATELEVEL1 = 0x3,
		WEAPGREEBLE_FIRERATELEVEL2 = 0x4,
		WEAPGREEBLE_NUM = 0x5,
	};

	enum weapAutoReloadType_t : std::int32_t
	{

	};

	enum weapFireBarrels_t : std::int32_t
	{

	};

	enum weapFireTimeInterpolation_t : std::int32_t
	{

	};

	enum WeaponSlotRestriction : std::int32_t
	{
		SLOT_RESTRICTION_NONE = 0x0,
		SLOT_RESTRICTION_OPEN = 0x1,
		SLOT_RESTRICTION_PRESET = 0x2,
		SLOT_RESTRICTION_COUNT = 0x3,
	};

	enum AttachmentType : std::int32_t
	{
		ATTACHMENT_SCOPE = 0x0,
		ATTACHMENT_UNDERBARREL = 0x1,
		ATTACHMENT_OTHER = 0x2,
		ATTACHMENT_COUNT = 0x3,
	};

	struct AttChargeInfo
	{
		char __pad0[28];
	}; static_assert(sizeof(AttChargeInfo) == 28);

	struct AttHybridSettings
	{
		char __pad0[72];
	}; static_assert(sizeof(AttHybridSettings) == 72);

	union WAFieldParm
	{
		char p_char;
		bool p_bool;
		int p_int;
		float p_float;
		const char* string;
	};

	enum WAFieldType : std::uint8_t
	{
		WAFIELD_TYPE_STRING = 0,
		WAFIELD_TYPE_INT = 4,
		WAFIELD_TYPE_BOOL = 6,
		WAFIELD_TYPE_FLOAT = 7,
		WAFIELD_TYPE_FLOAT32 = 9,
		WAFIELD_TYPE_FX = 10,
		WAFIELD_TYPE_MODEL = 11,
		WAFIELD_TYPE_ANIM = 12,
		WAFIELD_TYPE_MATERIAL = 13,
		WAFIELD_TYPE_SOUND = 15,
		WAFIELD_TYPE_TRACER = 16,
	};

	enum WAFieldCode : std::uint8_t
	{
		FIELD_OP_STRING_SET = 0,
		FIELD_OP_STRING_REPLACE = 1,
		FIELD_OP_STRING_APPEND = 2,
		FIELD_OP_NUMBER_BEGIN = 3,
		FIELD_OP_NUMBER_SET = 3,
		FIELD_OP_NUMBER_END = 6,
		FIELD_OP_COUNT = 7,
	};

	struct WAField
	{
		unsigned char index;
		unsigned char fieldType; //WAFieldType fieldType;
		unsigned char code; // WAFieldCode code;
		WAFieldParm parm;
	}; static_assert(sizeof(WAField) == 16);

	struct WeaponAttachment
	{
		union
		{
			const char* szInternalName;
			const char* name;
		};
		const char* szDisplayName; // 8
		AttachmentType type; // 16
		weapType_t weaponType; // 20
		weapClass_t weapClass; // 24
		XModel** worldModels; // 32 (2 xmodels)
		XModel** viewModels; // 40 (2 xmodels)
		XModel** reticleViewModels; // 48 (64 xmodels)
		snd_alias_list_t** bounceSounds; // 56 (53 sounds)
		snd_alias_list_t** rollingSounds; // 64 (53 sounds)
		AttChargeInfo* chargeInfo; // 72
		AttHybridSettings* hybridSettings; // 80
		scr_string_t* stringArray1; // 88 (4 strings) (hideTags?)
		scr_string_t* stringArray2; // 96 (4 strings) (showTags?)
		unsigned short* waFieldOffsets; // 104
		WAField* waFields; // 112
		unsigned int waFieldsCount; // 120 (MAX_ATTACH_FIELDS_PER_WEAPON = 256)
		char __pad0[14];
		bool riotShield; // 138
		char __pad1[5];
		// size: 144
	}; static_assert(sizeof(WeaponAttachment) == 0x90);
	static_assert(offsetof(WeaponAttachment, riotShield) == 138);

	struct AnimOverrideEntry
	{
		unsigned short attachment1;
		unsigned short attachment2;
		XAnimParts* overrideAnim;
		XAnimParts* altmodeAnim;
		//unsigned int animTreeType;
		int animTime;
		int altTime;
	}; static_assert(sizeof(AnimOverrideEntry) == 32);

	struct SoundOverrideEntry
	{
		unsigned short attachment1;
		unsigned short attachment2;
		snd_alias_list_t* overrideSound;
		snd_alias_list_t* altmodeSound;
		//unsigned int soundType;
	}; static_assert(sizeof(SoundOverrideEntry) == 24);

	struct FXOverrideEntry
	{
		unsigned short attachment1;
		unsigned short attachment2;
		FxEffectDef* overrideFX;
		FxEffectDef* altmodeFX;
		//unsigned int fxType;
	}; static_assert(sizeof(FXOverrideEntry) == 24);

	struct ReloadStateTimerEntry
	{
		int attachment;
		int reloadAddTime;
		int reloadEmptyAddTime;
		int reloadStartAddTime;
	}; static_assert(sizeof(ReloadStateTimerEntry) == 16);

	struct NoteTrackToSoundEntry
	{
		int attachment;
		scr_string_t* notetrackSoundMapKeys;
		scr_string_t* notetrackSoundMapValues;
	}; static_assert(sizeof(NoteTrackToSoundEntry) == 24);

	struct TracerDef
	{
		const char* name;
		Material* material;
		FxEffectDef* effect;
		unsigned int drawInterval;
		float speed;
		float beamLength;
		float beamWidth;
		float screwRadius;
		float screwDist;
		float colors[5][4];
	}; static_assert(sizeof(TracerDef) == 0x80);

	struct LaserDef
	{
		const char* name;
		Material* laserMaterial;
		Material* laserLightMaterial;
		FxEffectDef* effect;
		LaserDef* altLaser;
		scr_string_t value;
		float float_values[17];
		char char_values[4];
	}; static_assert(sizeof(LaserDef) == 120);

	struct TurretHydraulicSettings
	{
		float minVelocity;
		float maxVelocity; // unused or padding
		snd_alias_list_t* verticalSound;
		snd_alias_list_t* verticalStopSound;
		snd_alias_list_t* horizontalSound;
		snd_alias_list_t* horizontalStopSound;
	}; static_assert(sizeof(TurretHydraulicSettings) == 40);

	enum weapOverlayReticle_t : std::int32_t
	{
		WEAPOVERLAYRETICLE_NONE = 0x0,
		WEAPOVERLAYRETICLE_CROSSHAIR = 0x1,
		WEAPOVERLAYRETICLE_NUM = 0x2,
	};

	struct ADSOverlay
	{
		Material* shader;
		Material* shaderLowRes;
		Material* shaderEMP;
		Material* shaderEMPLowRes;
		weapOverlayReticle_t reticle;
		float xU_01;
		float xU_02;
		float width;
		float height;
		float widthSplitscreen;
		float heightSplitscreen;
	}; static_assert(sizeof(ADSOverlay) == 0x40);

	enum weaponIconRatioType_t : std::int32_t
	{
		WEAPON_ICON_RATIO_1TO1 = 0x0,
		WEAPON_ICON_RATIO_2TO1 = 0x1,
		WEAPON_ICON_RATIO_4TO1 = 0x2,
		WEAPON_ICON_RATIO_COUNT = 0x3,
	};

	enum WeapStickinessType : std::int32_t
	{
		WEAPSTICKINESS_NONE = 0x0,
		WEAPSTICKINESS_ALL = 0x1,
		WEAPSTICKINESS_ALL_ORIENT = 0x2,
		WEAPSTICKINESS_GROUND = 0x3,
		WEAPSTICKINESS_GROUND_WITH_YAW = 0x4,
		WEAPSTICKINESS_KNIFE = 0x5,
		WEAPSTICKINESS_COUNT = 0x6,
	};

	enum guidedMissileType_t : std::int32_t
	{
		MISSILE_GUIDANCE_NONE = 0x0,
		MISSILE_GUIDANCE_SIDEWINDER = 0x1,
		MISSILE_GUIDANCE_HELLFIRE = 0x2,
		MISSILE_GUIDANCE_JAVELIN = 0x3,
		MISSILE_GUIDANCE_UNKNOWN0 = 0x4,
		MISSILE_GUIDANCE_UNKNOWN1 = 0x5,
		MISSILE_GUIDANCE_UNKNOWN2 = 0x6,
		MISSILE_GUIDANCE_COUNT = 0x7,
	};

	enum WeapOverlayInteface_t : std::int32_t
	{
		WEAPOVERLAYINTERFACE_NONE = 0x0,
		WEAPOVERLAYINTERFACE_JAVELIN = 0x1,
		WEAPOVERLAYINTERFACE_TURRETSCOPE = 0x2,
		WEAPOVERLAYINTERFACECOUNT = 0x3,
	};

	enum weapAnimFiles_t : std::int32_t
	{
		WEAP_ANIM_INVALID = -1,
		NUM_WEAP_ANIMS = 202,
	};

	struct StateTimers
	{
		int fireDelay; // 1640 * x
		int meleeDelay; // 1644 * x
		int meleeChargeDelay; // 1648 * x
		int detonateDelay; // 1652 * x
		int fireTime; // 1656 * x
		int rechamberTime; // 1660 * x
		int rechamberTimeOneHanded; // 1664 * x
		int rechamberBoltTime; // 1668 * x
		int holdFireTime; // 1672 * x
		int grenadePrimeReadyToThrowTime; // 1676 * x
		int detonateTime; // 1680 * x
		int meleeTime; // 1684 * x
		int meleeChargeTime; // 1688 * x
		int reloadTime; // 1692 * x
		int reloadShowRocketTime; // 1696 * x
		int reloadEmptyTime; // 1700 * x
		int reloadAddTime; // 1704 * x
		int reloadEmptyAddTime; // 1708 * x
		int reloadStartTime; // 1712 * x
		int reloadStartAddTime; // 1716 * x
		int reloadEndTime; // 1720 * x
		int reloadTimeDualWield; // 1724 * x
		int reloadAddTimeDualWield; // 1728 * x
		int reloadEmptyDualMag; // 1732 * x
		int reloadEmptyAddTimeDualMag; // 1736 * x
		int u25; // 1740 * x // (unused)
		int u26; // 1744 * x // (unused)
		int dropTime; // 1748 * x
		int raiseTime; // 1752 * x
		int altDropTime; // 1756 * x
		int altRaiseTime; // 1760 * x
		int quickDropTime; // 1764 * x
		int quickRaiseTime; // 1768 * x
		int firstRaiseTime; // 1772 * x
		int breachRaiseTime; // 1776 * x
		int emptyRaiseTime; // 1780 * x
		int emptyDropTime; // 1784 * x
		int sprintInTime; // 1788 * x
		int sprintLoopTime; // 1792 * x
		int sprintOutTime; // 1796 * x
		int stunnedTimeBegin; // 1800 * x
		int stunnedTimeLoop; // 1804 * x
		int stunnedTimeEnd; // 1808 * x
		int nightVisionWearTime; // 1812 * x
		int nightVisionWearTimeFadeOutEnd; // 1816 * x
		int nightVisionWearTimePowerUp; // 1820 * x
		int nightVisionRemoveTime; // 1824 * x
		int nightVisionRemoveTimePowerDown; // 1828 * x
		int nightVisionRemoveTimeFadeInStart; // 1832 * x
		int aiFuseTime; // 1836 * x
		int fuseTime; // 1840 * x
		int missileTime; // 1844 * x
		int primeTime; // 1848 * x
		bool bHoldFullPrime; // 1852 * x
		int blastFrontTime; // 1856 * x
		int blastRightTime; // 1860 * x
		int blastBackTime; // 1864 * x
		int blastLeftTime; // 1868 * x
		int u58; // 1872 * x (unused)
		int u59; // 1876 * x (unused)
		int u60; // 1880 * x (unused)
		int u61; // 1884 * x (unused)
		int u62; // 1888 * x (unused)
		int u63; // 1892 * x (unused)
		int u64; // 1896 * x (unused)
		int u65; // 1900 * x (unused)
		int u66; // 1904 * x (unused)
		int u67; // 1908 * x (unused)
		int u68; // 1912 * x (unused)
		int offhandSwitchTime; // 1916 * x
		int u70; // 1920 * x (unknown)
		int u71; // 1924 * x (unknown)
		int u72; // 1928 * x (unknown)
		int u73; // 1932 * x (unknown)
		int u74; // 1936 * x (unknown)
		int u75; // 1936 * x (unknown)
		int u76; // 1936 * x (unknown)
		int u77; // 1936 * x (unknown)
		int u78; // 1936 * x (unknown)
	}; static_assert(sizeof(StateTimers) == 316);

	struct WeaponDef
	{
		union
		{
			const char* szInternalName;
			const char* name;
		};
		const char* szDisplayName; // 8
		const char* szOverlayName; // 16
		const char* szAttachmentName;
		const char* szUnknownName;
		XModel** gunModel; // 24 (2 xmodels)
		XModel* handModel; // 32
		XModel* unknownModel; // 40
		XModel** reticleViewModels; // 48 (64 xmodels)
		const char* szModeName; // 56
		XAnimParts** szXAnimsRightHanded; // 64 (190 xanims)
		XAnimParts** szXAnimsLeftHanded; // 72 (190 xanims)
		scr_string_t* hideTags; // 80 (32 xstrings)
		WeaponAttachment** attachments; // 88 (weaponDef + 1332 attachments)
		XAnimParts** szXAnims; // 96 (190 xanims)
		AnimOverrideEntry* animOverrides; // 104 (weaponDef + 1333 overrides)
		SoundOverrideEntry* soundOverrides; // 112 (weaponDef + 1334 overrides)
		FXOverrideEntry* fxOverrides; // 120 (weaponDef + 1335 overrides)
		ReloadStateTimerEntry* reloadOverrides; // 128 (weaponDef + 1336 overrides)
		NoteTrackToSoundEntry* notetrackOverrides; // 136 (weaponDef + 1337 overrides)
		scr_string_t* notetrackSoundMapKeys; // 144 (36 xstrings)
		scr_string_t* notetrackSoundMapValues; // 152 (36 xstrings)
		scr_string_t* notetrackRumbleMapKeys; // 160 (16 xstrings)
		scr_string_t* notetrackRumbleMapValues; // 168 (16 xstrings)
		scr_string_t* notetrackFXMapKeys; // 176 (16 xstrings)
		FxEffectDef** notetrackFXMapValues; // 184 (16 effects)
		scr_string_t* notetrackFXMapTagValues; // 192 (16 xstrings)
		scr_string_t* notetrackUnknownKeys; // 200 (16 xstrings)
		char* notetrackUnknown; // 208 (16 chars)
		scr_string_t* notetrackUnknownValues; // 216 (16 xstrings)
		const char* szAltWeaponName; // 224
		FxEffectDef* viewFlashEffect; // 232
		FxEffectDef* viewBodyFlashEffect; // 240
		FxEffectDef* worldFlashEffect; // 248
		FxEffectDef* viewFlashADSEffect; // 256
		FxEffectDef* viewBodyFlashADSEffect; // 264
		FxEffectDef* effect06; // 272
		FxEffectDef* effect07; // 280
		FxEffectDef* effect08; // 288
		FxEffectDef* effect09; // 296
		FxEffectDef* effect10; // 304
		FxEffectDef* effect11; // 312
		FxEffectDef* effect12; // 320
		snd_alias_list_t* pickupSound; // 1
		snd_alias_list_t* pickupSoundPlayer; // 2
		snd_alias_list_t* ammoPickupSound; // 3
		snd_alias_list_t* ammoPickupSoundPlayer; // 4
		snd_alias_list_t* projectileSound; // 5
		snd_alias_list_t* pullbackSound; // 6
		snd_alias_list_t* pullbackSoundPlayer; // 7
		snd_alias_list_t* pullbackSoundQuick; // 8
		snd_alias_list_t* pullbackSoundQuickPlayer; // 9
		snd_alias_list_t* fireSound;  // 10
		snd_alias_list_t* fireSoundPlayer;  // 11
		snd_alias_list_t* fireSoundPlayerLeft;  // 12
		snd_alias_list_t* fireSoundPlayerRight;  // 13
		snd_alias_list_t* sound14;  // 14
		snd_alias_list_t* sound15;  // 15
		snd_alias_list_t* sound16;  // 16
		snd_alias_list_t* sound17;  // 17
		snd_alias_list_t* sound18;  // 18
		snd_alias_list_t* fireLoopSound;  // 19
		snd_alias_list_t* fireLoopSoundPlayer;  // 20
		snd_alias_list_t* sound21;  // 21
		snd_alias_list_t* sound22;  // 22
		snd_alias_list_t* sound23;  // 23
		snd_alias_list_t* sound24;  // 24
		snd_alias_list_t* sound25;  // 25
		snd_alias_list_t* sound26;  // 26
		snd_alias_list_t* fireStopSound;  // 27
		snd_alias_list_t* fireStopSoundPlayer;  // 28
		snd_alias_list_t* sound29;  // 29
		snd_alias_list_t* sound30;  // 30
		snd_alias_list_t* sound31;  // 31
		snd_alias_list_t* sound32;  // 32
		snd_alias_list_t* sound33;  // 33
		snd_alias_list_t* fireFirstSound;  // 34
		snd_alias_list_t* fireFirstSoundPlayer;  // 35
		snd_alias_list_t* fireSound2;  // 36
		snd_alias_list_t* fireSoundPlayer2;  // 37
		snd_alias_list_t* fireSpecialSound;  // 38
		snd_alias_list_t* fireSpecialSoundPlayer;  // 39
		snd_alias_list_t* emptyFireSound;  // 40
		snd_alias_list_t* emptyFireSoundPlayer;  // 41
		snd_alias_list_t* emptyFireSoundPlayerLeft;  // 42
		snd_alias_list_t* emptyFireSoundPlayerRight;  // 43
		snd_alias_list_t* emptyFireSoundReleasePlayer;  // 44
		snd_alias_list_t* emptyFireSoundReleasePlayerLeft;  // 45
		snd_alias_list_t* emptyFireSoundReleasePlayerRight;  // 46
		snd_alias_list_t* sound47;  // 47
		snd_alias_list_t* meleeSwipeSound;  // 48
		snd_alias_list_t* meleeSwipeSoundPlayer;  // 49
		snd_alias_list_t* meleeHitSound;  // 50
		snd_alias_list_t* meleeHitSoundPlayer;  // 51
		snd_alias_list_t* meleeMissSound;  // 52
		snd_alias_list_t* meleeMissSoundPlayer;  // 53
		snd_alias_list_t* sound54;  // 54
		snd_alias_list_t* sound55;  // 55
		snd_alias_list_t* sound56;  // 56
		snd_alias_list_t* sound57;  // 57
		snd_alias_list_t* sound58;  // 58
		snd_alias_list_t* sound59;  // 59
		snd_alias_list_t* sound60;  // 60
		snd_alias_list_t* sound61;  // 61
		snd_alias_list_t* sound62;  // 62
		snd_alias_list_t* sound63;  // 63
		snd_alias_list_t* sound64;  // 64
		snd_alias_list_t* sound65;  // 65
		snd_alias_list_t* nightVisionWearSound;  // 66
		snd_alias_list_t* nightVisionWearSoundPlayer;  // 67
		snd_alias_list_t* nightVisionRemoveSound;  // 68
		snd_alias_list_t* nightVisionRemoveSoundPlayer;  // 69
		snd_alias_list_t* raiseSound;  // 70
		snd_alias_list_t* raiseSoundPlayer;  // 71
		snd_alias_list_t* raiseSoundPlayerLeft;  // 72
		snd_alias_list_t* raiseSoundPlayerRight;  // 73
		snd_alias_list_t* sound74;  // 74
		snd_alias_list_t* quickRaiseSoundPlayer;  // 75
		snd_alias_list_t* quickRaiseSoundPlayerLeft;  // 76
		snd_alias_list_t* quickRaiseSoundPlayerRight;  // 77
		snd_alias_list_t* raiseSound2;  // 78
		snd_alias_list_t* sound79;  // 79
		snd_alias_list_t* sound80;  // 80
		snd_alias_list_t* sound81;  // 81
		snd_alias_list_t* putawaySound;  // 82
		snd_alias_list_t* putawaySoundPlayer;  // 83
		snd_alias_list_t* putawaySoundPlayerLeft;  // 84
		snd_alias_list_t* putawaySoundPlayerRight;  // 85
		snd_alias_list_t* sound86;  // 86
		snd_alias_list_t* sound87;  // 87
		snd_alias_list_t* adsEnterSoundPlayer;  // 88
		snd_alias_list_t* adsLeaveSoundPlayer;  // 89
		snd_alias_list_t* adsCrosshairEnemySound;  // 90
		snd_alias_list_t** bounceSound; // 920 (53 sounds)
		snd_alias_list_t** rollingSound; // 928 (53 sounds)
		FxEffectDef* viewShellEjectEffect; // 936
		FxEffectDef* worldShellEjectEffect; // 944
		FxEffectDef* viewLastShotEjectEffect; // 952
		FxEffectDef* worldLastShotEjectEffect; // 960
		FxEffectDef* viewMagEjectEffect; // 968
		Material* reticleCenter; // 976
		Material* reticleSide; // 984
		XModel** worldModel; // 992 (2 xmodels)
		XModel* worldClipModel; // 1000
		XModel* rocketModel; // 1008
		XModel* knifeModel; // 1016
		XModel* worldKnifeModel; // 1024
		Material* hudIcon; // 1032
		Material* pickupIcon; // 1040
		Material* unknownIcon2; // 1048
		Material* unknownIcon3; // 1056
		Material* unknownIcon4; // 1064
		Material* ammoCounterIcon; // 1072
		const char* szAmmoName; // 1080
		const char* szClipName; // 1088
		const char* szSharedAmmoCapName; // 1096
		PhysCollmap* physCollmap; // 1104
		PhysPreset* physPreset; // 1112
		const char* szUseHintString; // 1120
		const char* dropHintString; // 1128
		float* locationDamageMultipliers; // 1136 (22 floats)
		const char* fireRumble; // 1144
		const char* fireMedRumble; // 1152
		const char* fireHighRumble; // 1160
		const char* meleeImpactRumble; // 1168
		TracerDef* tracer1; // 1176
		TracerDef* tracer2; // 1184
		LaserDef* laser; // 1192
		snd_alias_list_t* turretOverheatSound; // 1200
		FxEffectDef* turretOverheatEffect; // 1208
		const char* turretBarrelSpinRumble; // 1216
		snd_alias_list_t* turretBarrelSpinMaxSnd; // 1224
		snd_alias_list_t* turretBarrelSpinUpSnd[4]; // 1232
		snd_alias_list_t* turretBarrelSpinDownSnd[4]; // 1264
		snd_alias_list_t* missileConeSoundAlias; // 1296
		snd_alias_list_t* missileConeSoundAliasAtBase; // 1304
		XModel* stowOffsetModel; // 1312
		TurretHydraulicSettings* turretHydraulicSettings; // 1320
		int altWeapon; // 1328
		unsigned char numWeaponAttachments; // 1332
		unsigned char numAnimOverrides; // 1333
		unsigned char numSoundOverrides; // 1334
		unsigned char numFXOverrides; // 1335
		unsigned char numReloadStateTimerOverrides; // 1336
		unsigned char numNotetrackOverrides; // 1337
		playerAnimType_t playerAnimType; // 1338
		char __pad000[1];
		weapType_t weapType; // 1340
		weapClass_t weapClass; // 1344
		PenetrateType penetrateType; // 1348
		float penetrateDepth; // 1352
		ImpactType impactType; // 1356
		weapInventoryType_t inventoryType; // 1360
		weapFireType_t fireType; // 1364
		weapFireBarrels_t fireBarrels; // 1368
		weapAdsFireMode_t adsFireMode; // 1372
		float burstFireCooldown; // 1376
		weapGreebleType_t greebleType; // 1380
		weapAutoReloadType_t autoReloadType; // 1384
		WeaponSlotRestriction slotRestriction; // 1388
		OffhandClass offhandClass; // 1392
		weapStance_t stance; // 1396
		int reticleCenterSize; // 1400
		int reticleSideSize; // 1404
		int reticleMinOfs; // 1408
		activeReticleType_t activeReticleType; // 1412
		float standMove[3]; // 1416 1420 1424
		float standRot[3]; // 1428 1432 1436
		float strafeMove[3]; // 1440 1444 1448
		float strafeRot[3]; // 1452 1456 1460
		float duckedOfs[3]; // 1464 1468 1472
		float duckedMove[3]; // 1476 1480 1484
		float duckedRot[3]; // 1488 1492 1496
		float proneOfs[3]; // 1500 1504 1508
		float proneMove[3]; // 1512 1516 1520
		float proneRot[3]; // 1524 1528 1532
		float unkVec1[3];
		float unkVec2[3];
		float posMoveRate; // 1536
		float posProneMoveRate; // 1540
		float standMoveMinSpeed; // 1544
		float duckedMoveMinSpeed; // 1548
		float proneMoveMinSpeed; // 1552
		float posRotRate; // 1556
		float posProneRotRate; // 1560
		weaponIconRatioType_t hudIconRatio; // 1564
		weaponIconRatioType_t pickupIconRatio; // 1568
		weaponIconRatioType_t ammoCounterIconRatio; // 1572
		int ammoCounterClip; // 1576
		int startAmmo; // 1580
		int ammoIndex; // 1752
		char ammoIndexUnknown; // 1588 (runtime variable)
		char __pad002[3]; // padding?
		int clipIndex; // 1592 (runtime variable)
		char clipIndexUnknown; // 1596 (runtime variable)
		char __pad003[3]; // padding?
		int maxAmmo; // 1600
		int minAmmoReq; // 1604
		int clipSize; // 1608
		int shotCount; // 1612
		int sharedAmmoCapIndex; // 1616
		int sharedAmmoCap; // 1620
		int damage; // 1624
		int playerDamage; // 1628
		int meleeDamage; // 1632
		int damageType; // 1636
		StateTimers stateTimers; // 1640
		StateTimers akimboStateTimers; // 1940
		float autoAimRange; // 2240
		float aimAssistRange; // 2244
		float aimAssistRangeAds; // 2248
		float aimPadding; // 2252
		float enemyCrosshairRange; // 2256 
		float moveSpeedScale; // 2260
		float adsMoveSpeedScale; // 2264
		float sprintDurationScale; // 2268
		float adsZoomFov; // 2272
		float adsZoomInFrac; // 2276
		float adsZoomOutFrac; // 2280
		float adsSceneBlur; // 2284 (1401FC630) : float
		float fU_007; // 2288 (1400CF870) : float (related to scene blur)
		float xU_008; // 2292 X
		ADSOverlay overlay; // 2296
		WeapOverlayInteface_t overlayInterface; // 2352
		float adsBobFactor; // 2356
		float adsViewBobMult; // 2360
		float hipSpreadStandMin; // 2364
		float hipSpreadDuckedMin; // 2368
		float hipSpreadProneMin; // 2372
		float hipSpreadStandMax; // 2376
		float xU_009; // 2380 X
		float xU_010; // 2384 X
		float hipSpreadDuckedMax; // 2388
		float hipSpreadProneMax; // 2392
		float hipSpreadDecayRate; // 2396
		float hipSpreadFireAdd; // 2400
		float hipSpreadTurnAdd; // 2404
		float hipSpreadMoveAdd; // 2408
		float hipSpreadDuckedDecay; // 2412
		float hipSpreadProneDecay; // 2416
		float hipReticleSidePos; // 2420
		float adsIdleAmount; // 2424
		float hipIdleAmount; // 2428
		float adsIdleSpeed; // 2432
		float hipIdleSpeed; // 2436f
		float idleCrouchFactor; // 2440
		float idleProneFactor; // 2444
		float gunMaxPitch; // 2448
		float gunMaxYaw; // 2452
		float adsIdleLerpStartTime; // 2456
		float adsIdleLerpTime; // 2460
		int adsTransInTime; // 2464
		int xU_011; // 2468 X
		int adsTransOutTime; // 2472
		float xU_012; // 2476 X
		float swayMaxAngle; // 2480
		float swayLerpSpeed; // 2484
		float swayPitchScale; // 2488
		float swayYawScale; // 2492
		float swayVertScale; // 2496
		float swayHorizScale; // 2500
		float swayShellShockScale; // 2504
		float adsSwayMaxAngle; // 2508
		float adsSwayLerpSpeed; // 2512
		float adsSwayPitchScale; // 2516
		float adsSwayYawScale; // 2520
		float adsSwayHorizScale; // 2524
		float adsSwayVertScale; // 2528
		float adsViewErrorMin; // 2532
		float adsViewErrorMax; // 2536
		float adsFireAnimFrac; // 2540
		float dualWieldViewModelOffset; // 2544
		float scopeDriftDelay; // 2548
		float scopeDriftLerpInTime; // 2552
		float scopeDriftSteadyTime; // 2556
		float scopeDriftLerpOutTime; // 2560
		float scopeDriftSteadyFactor; // 2564
		float scopeDriftUnsteadyFactor; // 2568
		float bobVerticalFactor; // 2572
		float bobHorizontalFactor; // 2576
		float bobViewVerticalFactor; // 2580
		float bobViewHorizontalFactor; // 2584
		float stationaryZoomFov; // 2588
		float stationaryZoomDelay; // 2592
		float stationaryZoomLerpInTime; // 2596
		float stationaryZoomLerpOutTime; // 2600
		float adsDofStart; // 2604
		float adsDofEnd; // 2608
		float xU_020; // 2612 X
		Material* killIcon; // 2616
		Material* dpadIcon; // 2624
		Material* hudProximityWarningIcon; // 2632
		weaponIconRatioType_t killIconRatio; // 2640
		weaponIconRatioType_t dpadIconRatio; // 2644
		int fireAnimLength; // 2648
		int fireAnimLengthAkimbo; // 2652
		int inspectAnimTime; // 2656
		int reloadAmmoAdd; // 2660
		int reloadStartAdd; // 2664
		int ammoDropStockMin; // 2668
		int ammoDropStockMax; // 2672
		int ammoDropClipPercentMin; // 2676
		int ammoDropClipPercentMax; // 2680
		int explosionRadius; // 2684
		int explosionRadiusMin; // 2688
		int explosionInnerDamage; // 2692
		int explosionOuterDamage; // 2696
		float damageConeAngle; // 2700
		float bulletExplDmgMult; // 2704
		float bulletExplRadiusMult; // 2708
		int projectileSpeed; // 2712
		int projectileSpeedUp; // 2716
		int projectileSpeedForward; // 2720
		int projectileActivateDist; // 2724
		float projLifetime; // 2728
		float timeToAccelerate; // 2732
		float projectileCurvature; // 2736
		float xU_021; // 2740 X
		const char* projectileName; // 2744
		XModel* projectileModel; // 2752
		FxEffectDef* projExplosionEffect; // 2760
		FxEffectDef* projDudEffect; // 2768
		snd_alias_list_t* projExplosionSound; // 2776
		snd_alias_list_t* projDudSound; // 2784
		weapProjExposion_t projExplosion; // 2792
		WeapStickinessType stickiness; // 2796
		float lowAmmoWarningThreshold; // 2800
		float ricochetChance; // 2804
		int riotShieldHealth; // 2808
		float riotShieldDamageMult; // 2812
		float* parallelBounce; // 2816 (53 floats)
		float* perpendicularBounce; // 2824 (53 floats)
		FxEffectDef* projTrailEffect; // 2832
		FxEffectDef* projBeaconEffect; // 2840
		float projectileColor[3]; // 2848 2852 2856
		guidedMissileType_t guidedMissileType; // 2860
		float maxSteeringAccel; // 2864
		int projIgnitionDelay; // 2868
		FxEffectDef* projIgnitionEffect; // 2872
		snd_alias_list_t* projIgnitionSound; // 2880
		float adsAimPitch; // 2888
		float adsCrosshairInFrac; // 2892
		float adsCrosshairOutFrac; // 2896
		int adsGunKickReducedKickBullets; // 2900
		float adsGunKickReducedKickPercent; // 2904
		float adsGunKickPitchMin; // 2908
		float adsGunKickPitchMax; // 2912
		float adsGunKickYawMin; // 2916
		float adsGunKickYawMax; // 2920
		float adsGunKickMagMin; // 2924
		float adsGunKickAccel; // 2928
		float adsGunKickSpeedMax; // 2932
		float adsGunKickSpeedDecay; // 2936
		float adsGunKickStaticDecay; // 2940
		float adsViewKickPitchMin; // 2944
		float adsViewKickPitchMax; // 2948
		float adsViewKickYawMin; // 2952
		float adsViewKickYawMax; // 2956
		float adsViewKickMagMin; // 2960
		float adsViewKickCenterSpeed; // 2964
		float adsViewScatterMin; // 2968 X
		float adsViewScatterMax; // 2972 X
		float adsSpread; // 2976
		int hipGunKickReducedKickBullets; // 2980
		float hipGunKickReducedKickPercent; // 2984
		float hipGunKickPitchMin; // 2988
		float hipGunKickPitchMax; // 2992
		float hipGunKickYawMin; // 2996
		float hipGunKickYawMax; // 3000
		float hipGunKickMagMin; // 3004
		float hipGunKickAccel; // 3008
		float hipGunKickSpeedMax; // 3012
		float hipGunKickSpeedDecay; // 3016
		float hipGunKickStaticDecay; // 3020
		float hipViewKickPitchMin; // 3024
		float hipViewKickPitchMax; // 3028
		float hipViewKickYawMin; // 3032
		float hipViewKickYawMax; // 3036
		float hipViewKickMagMin; // 3040
		float hipViewKickCenterSpeed; // 3044
		float hipViewScatterMin; // 3048 //*
		float hipViewScatterMax; // 3052 //*
		float xU_043; // 3056 //*
		int adsReloadTransTime; // 3060
		float fightDist; // 3064
		float maxDist; // 3068
		const char* accuracyGraphName[2]; // 3072
		vec2_t* accuracyGraphKnots[2]; // 3088
		vec2_t* originalAccuracyGraphKnots[2]; // 3104
		short accuracyGraphKnotCount[2]; // 3120
		int positionReloadTransTime; // 3124 X
		float leftArc; // 3128
		float rightArc; // 3132
		float topArc; // 3136
		float bottomArc; // 3140
		float accuracy; // 3144
		float aiSpread; // 3148
		float playerSpread; // 3152
		float minTurnSpeed[2]; //3156
		float maxTurnSpeed[2]; // 3164
		float pitchConvergenceTime; // 3172
		float yawConvergenceTime; // 3176
		float suppressTime; // 3180
		float maxRange; // 3184
		float animHorRotateInc; // 3188
		float playerPositionDist; // 3192
		unsigned int useHintStringIndex; // 3196 (runtime variable)
		unsigned int dropHintStringIndex; // 3200 (runtime variable)
		float horizViewJitter; // 3204
		float vertViewJitter; // 3208
		float scanSpeed; // 3212
		float scanAccel; // 3216
		int scanPauseTime; // 3220
		char __pad[16];
		const char* szScript; // 3224
		int minDamage; // 3232
		int midDamage; // 3236
		int minPlayerDamage; // 3240
		int midPlayerDamage; // 3244
		float maxDamageRange; // 3248
		float minDamageRange; // 3252
		int iU_045; // 3256 X
		int iU_046; // 3260 X
		int iU_047; // 3264 X
		int iU_048; // 3268 X
		float fU_049; // 3272 X
		float fU_050; // 3276 X
		float destabilizationRateTime; // 3280
		float destabilizationCurvatureMax; // 3284
		int destabilizeDistance; // 3288
		float turretADSTime; // 3292
		float turretFov; // 3296 X
		float turretFovADS; // 3300 X
		float turretScopeZoomRate; // 3304 X
		float turretScopeZoomMin; // 3308 X
		float turretScopeZoomMax; // 3312 X
		float xU_056; // 3316 X
		float xU_057; // 3320 X
		float xU_058; // 3324 X
		float xU_059; // 3328 X
		float turretBarrelSpinSpeed; // 3332
		float turretBarrelSpinUpTime; // 3336
		float turretBarrelSpinDownTime; // 3340 X
		float missileConeSoundRadiusAtTop; // 3344 X
		float missileConeSoundRadiusAtBase; // 3348 X
		float missileConeSoundHeight; // 3352 X
		float missileConeSoundOriginOffset; // 3356 X
		float missileConeSoundVolumescaleAtCore; // 3360 X
		float missileConeSoundVolumescaleAtEdge; // 3364 X
		float missileConeSoundVolumescaleCoreSize; // 3368 X
		float missileConeSoundPitchAtTop; // 3372 X
		float missileConeSoundPitchAtBottom; // 3376 X
		float missileConeSoundPitchTopSize; // 3380 X
		float missileConeSoundPitchBottomSize; // 3384 X
		float missileConeSoundCrossfadeTopSize; // 3388 X
		float missileConeSoundCrossfadeBottomSize; // 3392 X
		float aim_automelee_lerp; // 3396
		float aim_automelee_range; // 3400
		float aim_automelee_region_height; // 3404
		float aim_automelee_region_width; // 3408
		float player_meleeHeight; // 3412
		float player_meleeRange; // 3416
		float player_meleeWidth; // 3420
		float signatureFireTime; // 3424
		int signatureNumBullets; // 3428
		weapFireTimeInterpolation_t fireTimeInterpolationType; // 3432
		int xU_075; // 3436 X
		int ammoUsedPerShot; // 3440
		int xU_076; // 3444 X
		int xU_077; // 3448 X
		int xU_078; // 3452 X
		int iU_079; // 3456 // int numBulletTags (BG_ShowHideTagsBasedOnAltMode)
		int iU_080; // 3460 // int tagForAmmo (1400C77D0)
		scr_string_t stowTag; // 3464
		bool bU_081; // 3468 X
		bool unknownReticleBooleanValue1; // 3469 (CG_DrawCrosshair)
		bool unknownReticleBooleanValue2; // 3470 (CG_DrawCrosshair)
		bool turretADSEnabled; // 3471 X
		bool knifeAttachTagLeft; // 3472
		bool knifeAlwaysAttached; // 3473
		bool meleeOverrideValues; // 3474
		bool bU_083; // 3475 X
		bool bU_084; // 3476 X
		bool sharedAmmo; // 3477
		bool lockonSupported; // 3478
		bool requireLockonToFire; // 3479
		bool isAirburstWeapon; // 3480 X
		bool bigExplosion; // 3481 X
		bool noAdsWhenMagEmpty; // 3482
		bool avoidDropCleanup; // 3483
		bool inheritsPerks; // 3484
		bool crosshairColorChange; // 3485
		bool rifleBullet; // 3486
		bool armorPiercing; // 3487
		bool boltAction; // 3488
		bool aimDownSight; // 3489
		bool canHoldBreath; // 3490
		bool meleeOnly; // 3491
		bool bU_085; // 3492 bool isMeleeAnimDelayed;? (0x14009FDC0)(1401F2BC0)
		bool bU_086; // 3493 X bool oldWeaponBot;?
		bool canVariableZoom; // 3494
		bool rechamberWhileAds; // 3495
		bool bulletExplosiveDamage; // 3496
		bool cookOffHold; // 3497
		bool reticleSpin45; // 3498 X
		bool reticleSideEnabled; // 3499
		bool clipOnly; // 3500
		bool noAmmoPickup; // 3501
		bool disableSwitchToWhenEmpty; // 3502
		bool bU_088; // 3503 (14017E520) bool hiddenAmmo;?
		bool hasMotionTracker; // 3504
		bool bU_089; // 3505 X
		bool noDualWield; // 3506
		bool flipKillIcon; // 3507
		bool actionSlotShowAmmo; // 3508
		bool noPartialReload; // 3509
		bool segmentedReload; // 3510
		bool multipleReload; // 3511
		bool blocksProne; // 3512
		bool silenced; // 3513
		bool isRollingGrenade; // 3514
		bool projExplosionEffectForceNormalUp; // 3515
		bool projExplosionEffectInheritParentDirection; // 3516
		bool projImpactExplode; // 3517
		bool projTrajectoryEvents; // 3518 X
		bool projWhizByEnabled; // 3519 X
		bool stickToPlayers; // 3520 X
		bool stickToVehicles; // 3521 X
		bool stickToTurrets; // 3522 X
		bool thrownSideways; // 3523 X
		bool hasDetonatorEmptyThrow; // 3524
		bool hasDetonatorDoubleTap; // 3525
		bool disableFiring; // 3526
		bool timedDetonation; // 3527
		bool bU_090; // 3528 (G_FireGrenade)(CheckCrumpleMissile) bool usesGrenadeTimer?
		bool bU_091; // 3529 (G_FireRocket) bool usesRocketTimer?
		bool rotate; // 3530
		bool holdButtonToThrow; // 3531 X
		bool freezeMovementWhenFiring; // 3532
		bool thermalScope; // 3533
		bool thermalToggle; // 3534
		bool outlineEnemies; // 3535
		bool altModeSameWeapon; // 3536
		bool turretBarrelSpinEnabled; // 3537
		bool missileConeSoundEnabled; // 3538 X
		bool missileConeSoundPitchshiftEnabled; // 3539 X
		bool missileConeSoundCrossfadeEnabled; // 3540 X
		bool offhandHoldIsCancelable; // 3541
		bool doNotAllowAttachmentsToOverrideSpread; // 3542
		bool useFastReloadAnims; // 3543 (140202800)
		bool dualMagReloadSupported; // 3544
		bool reloadStopsAlt; // 3545 X
		bool bU_092; // 3546 X
		bool alwaysShatterGlassOnImpact; // 
		bool bU_093; //
		bool oldWeapon; // 3773 !!
		bool bU_094; // 
		bool xU_095; // 
		bool hasCounterSilencer; // 
		bool xU_097; // 
		bool xU_098; // 
		bool disableVariableAutosimRate; // 
		bool bU_100; // 
		bool bU_101; // 
		bool bU_102; // 
		bool bU_103; // 
		bool bU_104; // 
		bool bU_105; // 
		bool cloakedWeapon; //
		bool adsHideWeapon; //
		bool adsHideHands; // 
		bool bU_108; // 
		bool adsBlurSceneEnabled; // 
		bool usesSniperScope; // 
		bool bU_112; // 
		bool bU_113; // 
		bool bU_114; // 
		bool bU_115; // 
		float xU_03_1;
		float adsDofPhysicalFStop; // 
		float adsDofPhysicalFocusDistance; // 
		float autosimSpeedScalar; //
		float explosionReactiveMotionParts[5]; // 
		char __pad_unknown[12]; //
	}; static_assert(sizeof(WeaponDef) == 0xF08);
	
	static_assert(offsetof(WeaponDef, viewFlashEffect) == 248);
	static_assert(offsetof(WeaponDef, usesSniperScope) == 3791);
	static_assert(offsetof(WeaponDef, adsHideWeapon) == 3787);
	static_assert(offsetof(WeaponDef, adsHideHands) == 3788);
	static_assert(offsetof(WeaponDef, oldWeapon) == 3773);
	static_assert(offsetof(WeaponDef, gunModel) == 0x28);
	static_assert(offsetof(WeaponDef, handModel) == 0x30);
	static_assert(offsetof(WeaponDef, szModeName) == 0x48);
	static_assert(offsetof(WeaponDef, szXAnimsLeftHanded) == 88);
	static_assert(offsetof(WeaponDef, hideTags) == 0x60);
	static_assert(offsetof(WeaponDef, attachments) == 104);
	static_assert(offsetof(WeaponDef, numReloadStateTimerOverrides) == 1480);
	static_assert(offsetof(WeaponDef, viewFlashADSEffect) == 272);
	static_assert(offsetof(WeaponDef, viewBodyFlashADSEffect) == 280);
	static_assert(offsetof(WeaponDef, reticleCenter) == 1120);
	static_assert(offsetof(WeaponDef, reticleSide) == 1128);
	static_assert(offsetof(WeaponDef, projTrailEffect) == 3040);
	static_assert(offsetof(WeaponDef, projBeaconEffect) == 3048);
	static_assert(offsetof(WeaponDef, projIgnitionEffect) == 3080);
	static_assert(offsetof(WeaponDef, projIgnitionSound) == 3088);
	static_assert(offsetof(WeaponDef, accuracyGraphName) == 3280);
	static_assert(offsetof(WeaponDef, accuracyGraphKnots) == 3296);
	static_assert(offsetof(WeaponDef, originalAccuracyGraphKnots) == 3312);
	static_assert(offsetof(WeaponDef, accuracyGraphName[1]) == 3288);
	static_assert(offsetof(WeaponDef, accuracyGraphKnots[1]) == 3304);
	static_assert(offsetof(WeaponDef, originalAccuracyGraphKnots[1]) == 3320);
	static_assert(offsetof(WeaponDef, szScript) == 3448);
	static_assert(offsetof(WeaponDef, stowTag) == 3688);

	static_assert(offsetof(WeaponDef, ammoIndex) == 1752);
	static_assert(offsetof(WeaponDef, standMoveMinSpeed) == 1712);
	static_assert(offsetof(WeaponDef, weapType) == 1484);
	static_assert(offsetof(WeaponDef, altWeapon) == 1472);
	static_assert(offsetof(WeaponDef, clipIndex) == 1760);
	static_assert(offsetof(WeaponDef, burstFireCooldown) == 1520);
	static_assert(offsetof(WeaponDef, greebleType) == 1524);

	struct cplane_s
	{
		float normal[3];
		float dist;
		unsigned char type;
		//unsigned char pad[3];
	}; assert_sizeof(cplane_s, 20);

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
	}; assert_sizeof(ClipMaterial, 16);

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
	}; assert_sizeof(cLeafBrushNode_s, 24);

	typedef unsigned short LeafBrush;

	struct BrushesCollisionTree
	{
		unsigned int leafbrushNodesCount;
		cLeafBrushNode_s* leafbrushNodes;
		unsigned int numLeafBrushes;
		LeafBrush* leafbrushes;
	}; assert_sizeof(BrushesCollisionTree, 32);

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
	}; assert_sizeof(CollisionAabbTree, 32);

	struct PatchesCollisionTree
	{
		int aabbTreeCount;
		CollisionAabbTree* aabbTrees;
	}; assert_sizeof(PatchesCollisionTree, 16);

	struct SModelAabbNode
	{
		Bounds bounds;
		unsigned short firstChild;
		unsigned short childCount;
	}; assert_sizeof(SModelAabbNode, 28);

	struct SModelsCollisionTree
	{
		unsigned short numStaticModels;
		unsigned short smodelNodeCount;
		SModelAabbNode* smodelNodes;
	}; assert_sizeof(SModelsCollisionTree, 16);

	struct cbrushside_t
	{
		unsigned int planeIndex;
		unsigned short materialNum;
		unsigned char firstAdjacentSideOffset;
		unsigned char edgeCount;
	}; assert_sizeof(cbrushside_t, 8);

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
	}; assert_sizeof(cbrush_t, 48);
	assert_offsetof(cbrush_t, sides, 8);

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

	static_assert(sizeof(BrushesCollisionData) == 0x40);

	struct CollisionBorder
	{
		float distEq[3];
		float zBase;
		float zSlope;
		float start;
		float length;
	}; assert_sizeof(CollisionBorder, 28);

	struct CollisionPartition
	{
		unsigned char triCount;
		unsigned char borderCount;
		unsigned char firstVertSegment;
		int firstTri;
		CollisionBorder* borders;
	}; assert_sizeof(CollisionPartition, 16);

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
	}; assert_sizeof(PatchesCollisionData, 72);

	struct cStaticModel_s
	{
		XModel* xmodel;
		float origin[3];
		float invScaledAxis[3][3];
		Bounds absBounds;
		int lightingHandle;
	}; assert_sizeof(cStaticModel_s, 88);

	struct SModelsCollisionData
	{
		unsigned int numStaticModels;
		cStaticModel_s* staticModelList;
	}; assert_sizeof(SModelsCollisionData, 16);

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
	}; assert_sizeof(ClipInfo, 0xF8);
	assert_offsetof(ClipInfo, planes, 8);
	assert_offsetof(ClipInfo, bCollisionTree, 32);
	assert_offsetof(ClipInfo, pCollisionTree, 64);
	assert_offsetof(ClipInfo, sCollisionTree, 80);
	assert_offsetof(ClipInfo, bCollisionData, 96);
	assert_offsetof(ClipInfo, pCollisionData, 160);
	assert_offsetof(ClipInfo, sCollisionData, 232);
	assert_offsetof(ClipInfo, bCollisionData.brushes, 136);

	struct cNode_t
	{
		cplane_s* plane;
		short children[2];
	}; assert_sizeof(cNode_t, 16);

	struct cLeaf_t
	{
		unsigned int firstCollAabbIndex;
		unsigned int collAabbCount;
		int brushContents;
		int terrainContents;
		Bounds bounds;
		int leafBrushNode;
	}; assert_sizeof(cLeaf_t, 44);

	struct cmodel_t
	{
		Bounds bounds;
		float radius;
		ClipInfo* info;
		cLeaf_t leaf;
	}; assert_sizeof(cmodel_t, 88);

	assert_offsetof(cmodel_t, info, 32);

	struct Stage
	{
		const char* name;
		float origin[3];
		unsigned short triggerIndex;
		unsigned char sunPrimaryLightIndex;
		unsigned int unk;
	}; assert_sizeof(Stage, 32);

	enum DynEntityType : std::int32_t
	{
		DYNENT_TYPE_INVALID = 0x0,
		DYNENT_TYPE_CLUTTER = 0x1,
		DYNENT_TYPE_DESTRUCT = 0x2,
		DYNENT_TYPE_HINGE = 0x3,
		DYNENT_TYPE_SCRIPTABLEINST = 0x4,
		DYNENT_TYPE_SCRIPTABLEPHYSICS = 0x5,
		DYNENT_TYPE_LINKED = 0x6,
		DYNENT_TYPE_LINKED_NOSHADOW = 0x7,
		DYNENT_TYPE_COUNT = 0x8,
	};

	struct GfxPlacement
	{
		float quat[4];
		float origin[3];
	};

	struct DynEntityHingeDef
	{
		float axisOrigin[3];
		float axisDir[3];
		bool isLimited;
		float angleMin;
		float angleMax;
		float momentOfInertia;
		float friction;
	}; assert_sizeof(DynEntityHingeDef, 44);

	struct DynEntityLinkToDef
	{
		int anchorIndex;
		float originOffset[3];
		float angleOffset[3];
	}; assert_sizeof(DynEntityLinkToDef, 28);

	struct DynEntityDef
	{
		DynEntityType type;
		GfxPlacement pose;
		XModel* baseModel;
		unsigned short brushModel;
		unsigned short physicsBrushModel;
		unsigned short scriptableIndex;
		unsigned short health;
		FxEffectDef* destroyFx;
		snd_alias_list_t* sound;
		PhysPreset* physPreset;
		DynEntityHingeDef* hinge;
		DynEntityLinkToDef* linkTo;
		PhysMass mass;
		int contents;
		char __pad0[8];
	}; assert_sizeof(DynEntityDef, 136);

	struct DynEntityPose
	{
		GfxPlacement pose;
		float radius;
		char __pad0[4];
	}; assert_sizeof(DynEntityPose, 36);

	struct Hinge
	{
		float angle;
		float quat[4];
		float angularVel;
		float torqueAccum;
		bool active;
		float autoDisableTimeLeft;
		DynEntityHingeDef* def;
		PhysPreset* physPreset;
		float centerOfMassRelToAxisOriginAtAngleZero[3];
	};

	struct DynEntityClient
	{
		__int64 physObjId;
		unsigned short flags;
		unsigned short lightingHandle;
		unsigned short health;
		Hinge* hinge;
		XModel* activeModel;
		int contents;
	}; assert_sizeof(DynEntityClient, 40);

	struct DynEntityColl
	{
		unsigned short sector;
		unsigned short nextEntInSector;
		float linkMins[2];
		float linkMaxs[2];
	}; assert_sizeof(DynEntityColl, 20);

	enum ScriptableEventType : std::int32_t
	{
		SCRIPTABLE_EVENT_MODEL = 0x0,
		SCRIPTABLE_EVENT_FX = 0x1,
		SCRIPTABLE_EVENT_STOP_FX = 0x2,
		SCRIPTABLE_EVENT_SOUND = 0x3,
		SCRIPTABLE_EVENT_ANIMATION = 0x4,
		SCRIPTABLE_EVENT_EXPLODE = 0x5,
		SCRIPTABLE_EVENT_HEALTHDRAIN = 0x6,
		SCRIPTABLE_EVENT_PHYSICSLAUNCH = 0x7,
		SCRIPTABLE_EVENT_LIGHTSETTINGS = 0x8,
		SCRIPTABLE_EVENT_SUNLIGHTSETTINGS = 0x9,
		SCRIPTABLE_EVENT_SHAKE = 0xA,
		SCRIPTABLE_EVENT_TRANSLATE = 0xB,
		SCRIPTABLE_EVENT_ROTATE = 0xC,
		SCRIPTABLE_EVENT_STATECHANGE = 0xD,
		SCRIPTABLE_EVENT_COUNT = 0xE,
	};

	struct ScriptableEventModelDef
	{
		XModel* model;
	}; assert_sizeof(ScriptableEventModelDef, 8);

	struct ScriptableEventFxDef
	{
		FxEffectDef* handle;
		scr_string_t tagName;
		unsigned short loopTime;
		unsigned char loopTimeStreamIndex;
		bool tagUseAngles;
	}; assert_sizeof(ScriptableEventFxDef, 16);

	struct ScriptableEventStopFxDef
	{
		FxEffectDef* handle;
		scr_string_t tagName;
		unsigned short loopTime;
		unsigned char loopTimeStreamIndex;
		bool tagUseAngles;
	}; assert_sizeof(ScriptableEventFxDef, 16);

	struct ScriptableEventSoundDef
	{
		snd_alias_list_t* alias;
		bool looping;
	}; assert_sizeof(ScriptableEventSoundDef, 16);

	struct ScriptableEventAnimationDef
	{
		const char* animName;
		bool override;
		bool stateful;
		unsigned char animEntryIndex;
		unsigned char animPlaybackStreamIndex;
		unsigned short timeOffsetMin;
		unsigned short timeOffsetMax;
		unsigned short playbackRateMin;
		unsigned short playbackRateMax;
		unsigned short blendTime;
	}; assert_sizeof(ScriptableEventAnimationDef, 24);

	struct ScriptableEventExplodeDef
	{
		unsigned short forceMin;
		unsigned short forceMax;
		unsigned short radius;
		unsigned short damageMin;
		unsigned short damageMax;
		bool aiAvoid;
	}; assert_sizeof(ScriptableEventExplodeDef, 12);

	struct ScriptableEventHealthDef
	{
		unsigned short amount;
		unsigned short interval;
		unsigned short badPlaceRadius;
		unsigned char streamIndex;
	}; assert_sizeof(ScriptableEventHealthDef, 8);

	struct ScriptableEventPhysicsDef
	{
		XModel* model;
		unsigned char launchDirX;
		unsigned char launchDirY;
		unsigned char launchDirZ;
		unsigned short explForceScale;
		unsigned short bulletForceScale;
	}; assert_sizeof(ScriptableEventPhysicsDef, 16);

	struct ScriptableEventLightSettingsDef
	{
		unsigned char color[4];
		unsigned char lightIndexConstIndex;
		unsigned char transStateStreamIndex;
		unsigned char useColor;
		unsigned char useStateTransitionTime;
		unsigned short intensityScaleMin;
		unsigned short intensityScaleMax;
		unsigned short radiusScaleMin;
		unsigned short radiusScaleMax;
		unsigned short transitionTimeMin;
		unsigned short transitionTimeMax;
		const char* noteworthy;
	}; assert_sizeof(ScriptableEventLightSettingsDef, 32);
	assert_offsetof(ScriptableEventLightSettingsDef, noteworthy, 24);

	struct ScriptableEventSunlightSettingsDef
	{
		unsigned char color[4];
		unsigned char transStateStreamIndex;
		unsigned char flags;
		unsigned short intensityScaleMin;
		unsigned short intensityScaleMax;
		unsigned short pitchMin;
		unsigned short pitchMax;
		unsigned short headingMin;
		unsigned short headingMax;
		unsigned short transitionTimeMin;
		unsigned short transitionTimeMax;
		char __pad0[6];
	}; assert_sizeof(ScriptableEventSunlightSettingsDef, 28);

	struct ScriptableEventShakeDef
	{
		const char* rumbleName;
		unsigned short duration;
		unsigned short durationFadeUp;
		unsigned short durationFadeDown;
		unsigned short radius;
		unsigned short exponent;
		unsigned short scaleEarthquake;
		unsigned char scalePitch;
		unsigned char scaleYaw;
		unsigned char scaleRoll;
		unsigned char frequencyPitch;
		unsigned char frequencyYaw;
		unsigned char frequencyRoll;
		unsigned char flags;
	}; assert_sizeof(ScriptableEventShakeDef, 32);

	struct ScriptableEventTranslateDef
	{
		char __pad0[24];
		const char* str;
	}; assert_sizeof(ScriptableEventTranslateDef, 32);

	struct ScriptableEventRotateDef
	{
		char __pad0[24];
		const char* str;
	}; assert_sizeof(ScriptableEventTranslateDef, 32);

	struct ScriptableEventStateChangeDef
	{
		unsigned char targetIndex;
		unsigned char delayStreamIndex;
		unsigned short delayMin;
		unsigned short delayMax;
	}; assert_sizeof(ScriptableEventStateChangeDef, 6);

	union ScriptableEventDataUnion
	{
		ScriptableEventModelDef setModel;
		ScriptableEventFxDef playFx;
		ScriptableEventStopFxDef stopFx;
		ScriptableEventSoundDef playSound;
		ScriptableEventAnimationDef playAnim;
		ScriptableEventExplodeDef doExplosion;
		ScriptableEventHealthDef healthDrain;
		ScriptableEventPhysicsDef physicsLaunch;
		ScriptableEventLightSettingsDef lightSettings;
		ScriptableEventSunlightSettingsDef sunlightSettings;
		ScriptableEventShakeDef shake;
		ScriptableEventTranslateDef translate;
		ScriptableEventRotateDef rotate;
		ScriptableEventStateChangeDef stateChange;
	};

	struct ScriptableEventDef
	{
		ScriptableEventType type;
		ScriptableEventDataUnion data;
	};

	struct ScriptableStateDef
	{
		scr_string_t name;
		scr_string_t tagName;
		ScriptableEventDef* onEnterEvents;
		unsigned char onEnterEventCount;
		unsigned char damageFlags;
		unsigned char damageParentTransferRate;
		unsigned char damageParentReceiveRate;
		unsigned short maxHealth;
	}; assert_sizeof(ScriptableStateDef, 24);

	struct ScriptablePartDef
	{
		ScriptableStateDef* states;
		scr_string_t name;
		unsigned char stateCount;
		unsigned char flags;
		unsigned char eventStreamTimeRemainIndex;
		unsigned char eventStreamNextChangeTimeIndex;
	}; assert_sizeof(ScriptablePartDef, 16);

	enum ScriptableNotetrackType : std::int32_t
	{
		SCRIPTABLE_NT_FX = 0x0,
		SCRIPTABLE_NT_SOUND = 0x1,
		SCRIPTABLE_NT_COUNT = 0x2,
	};

	struct ScriptableNotetrackFxDef
	{
		FxEffectDef* handle;
		scr_string_t tagName;
		bool useAngles;
	};

	struct ScriptableNotetrackSoundDef
	{
		snd_alias_list_t* alias;
	};

	union ScriptableNotetrackDataUnion
	{
		ScriptableNotetrackFxDef playFx;
		ScriptableNotetrackSoundDef playSound;
	};

	struct ScriptableNotetrackDef
	{
		scr_string_t name;
		ScriptableNotetrackType type;
		ScriptableNotetrackDataUnion data;
	};

	enum ScriptableType : std::int32_t
	{
		SCRIPTABLE_TYPE_GENERAL = 0x0,
		SCRIPTABLE_TYPE_CHARACTER = 0x1,
		SCRIPTABLE_TYPE_COUNT = 0x2,
	};

	struct ScriptableDef
	{
		const char* name;
		XModel* baseModel;
		const char* baseCollisionBrush;
		const char* destroyedCollisionBrush;
		ScriptablePartDef* parts;
		ScriptableNotetrackDef* notetracks;
		ScriptableType type;
		unsigned char flags;
		unsigned char partCount;
		unsigned char serverInstancePartCount;
		unsigned char serverControlledPartCount;
		unsigned char notetrackCount;
		unsigned char eventStreamSize;
		unsigned char eventConstantsSize;
	}; assert_sizeof(ScriptableDef, 0x40);
	assert_offsetof(ScriptableDef, partCount, 53);
	assert_offsetof(ScriptableDef, eventConstantsSize, 58);

	struct ScriptableInstanceTargetData
	{
		char __pad0[68];
	}; assert_sizeof(ScriptableInstanceTargetData, 68);

	struct ScriptableInstancePartState
	{
		unsigned short curHealth;
		unsigned char lastExecutedStateIndex;
		unsigned char stateIndex;
	}; assert_sizeof(ScriptableInstancePartState, 4);

	struct ScriptableInstance
	{
		ScriptableDef* def;
		unsigned char* eventConstantsBuf;
		ScriptableInstanceTargetData* targetData;
		float origin[3];
		float angles[3];
		char __pad0[24];
		scr_string_t targetname;
		unsigned short preBrushModel;
		unsigned short postBrushModel;
		unsigned char flags;
		unsigned char targetDataCount;
		char __pad1[6];
		XModel* currentModel;
		ScriptableInstancePartState* partStates;
		unsigned char* eventStreamBuf;
		unsigned int currentPartBits[8];
		unsigned int damageOwnerEntHandle;
		unsigned short updateNextInstance;
		unsigned short linkedObject;
	}; assert_sizeof(ScriptableInstance, 152);
	assert_offsetof(ScriptableInstance, targetData, 16);
	assert_offsetof(ScriptableInstance, targetname, 72);
	assert_offsetof(ScriptableInstance, currentModel, 88);
	assert_offsetof(ScriptableInstance, partStates, 96);
	assert_offsetof(ScriptableInstance, eventStreamBuf, 104);

	struct ScriptableAnimationEntry
	{
		const char* animName;
		unsigned __int64 runtimeBuf;
	}; assert_sizeof(ScriptableAnimationEntry, 16);

	struct ScriptableMapEnts
	{
		unsigned int instanceStateSize;
		unsigned int instanceCount;
		unsigned int reservedInstanceCount;
		ScriptableInstance* instances;
		unsigned int animEntryCount;
		ScriptableAnimationEntry* animEntries;
		unsigned int replicatedInstanceCount;
	}; assert_sizeof(ScriptableMapEnts, 48);
	assert_offsetof(ScriptableMapEnts, instances, 16);
	assert_offsetof(ScriptableMapEnts, animEntries, 32);

	struct sphere_tree_t
	{
		char __pad0[8];
		int unk_count;
		char __pad1[4];
		unsigned int* unk;
		char __pad2[8];
	}; assert_sizeof(sphere_tree_t, 32);
	assert_offsetof(sphere_tree_t, unk_count, 8);
	assert_offsetof(sphere_tree_t, unk, 16);

	struct sphere_tree_obj_t
	{
		char __pad0[20];
	}; assert_sizeof(sphere_tree_obj_t, 20);

	struct sphere_tree_data_t
	{
		int sphereTreeCount;
		sphere_tree_t* sphereTree;
		int sphereTreeObjCount;
		sphere_tree_obj_t* sphereTreeObj;
	}; assert_sizeof(sphere_tree_data_t, 32);

	struct grapple_magnet_t
	{
		char __pad0[40];
	}; assert_sizeof(grapple_magnet_t, 40);

	struct grapple_data_t
	{
		sphere_tree_data_t sphereTreeData;
		grapple_magnet_t* magnet;
		unsigned int magnetCount;
		char __pad0[4];
	}; assert_sizeof(grapple_data_t, 48);

	struct /*alignas(128)*/ clipMap_t
	{
		const char* name; // 0
		int isInUse; // 8
		ClipInfo info; // 16
		ClipInfo* pInfo; // 264
		unsigned int numNodes; // 272
		cNode_t* nodes; // 280
		unsigned int numLeafs; // 288
		cLeaf_t* leafs; // 296
		unsigned int numSubModels; // 304
		cmodel_t* cmodels; // 312
		MapEnts* mapEnts; // 320
		Stage* stages; // 328
		unsigned char stageCount; // 336
		MapTriggers stageTrigger; // 344
		unsigned short dynEntCount[2];
		DynEntityDef* dynEntDefList[2];
		DynEntityPose* dynEntPoseList[2];
		DynEntityClient* dynEntClientList[2];
		DynEntityColl* dynEntCollList[2];
		unsigned int dynEntAnchorCount; // 464
		scr_string_t* dynEntAnchorNames; // 472
		ScriptableMapEnts scriptableMapEnts; // 480
		grapple_data_t grappleData; // 528
		unsigned int checksum;
		char __pad0[60]; // alignment padding
	}; assert_sizeof(clipMap_t, 0x280);
	assert_offsetof(clipMap_t, info, 16);
	assert_offsetof(clipMap_t, pInfo, 264);
	assert_offsetof(clipMap_t, nodes, 280);
	assert_offsetof(clipMap_t, leafs, 296);
	assert_offsetof(clipMap_t, cmodels, 312);
	assert_offsetof(clipMap_t, stageTrigger, 344);
	assert_offsetof(clipMap_t, scriptableMapEnts, 480);
	assert_offsetof(clipMap_t, grappleData, 528);


	enum GfxLightType : std::uint8_t
	{
		GFX_LIGHT_TYPE_NONE = 0x0,
		GFX_LIGHT_TYPE_DIR = 0x1,
		GFX_LIGHT_TYPE_SPOT = 0x2,
		GFX_LIGHT_TYPE_OMNI = 0x3,
		GFX_LIGHT_TYPE_COUNT = 0x4,
		GFX_LIGHT_TYPE_DIR_SHADOWMAP = 0x4,
		GFX_LIGHT_TYPE_SPOT_SHADOWMAP = 0x5,
		GFX_LIGHT_TYPE_OMNI_SHADOWMAP = 0x6,
		GFX_LIGHT_TYPE_COUNT_WITH_SHADOWMAP_VERSIONS = 0x7,
		GFX_LIGHT_TYPE_SPOT_SHADOWMAP_CUCOLORIS = 0x7,
		GFX_LIGHT_TYPE_COUNT_WITH_ALL_VERSIONS = 0x8,
	};

	struct ComPrimaryLight
	{
		GfxLightType type; // 0
		unsigned char canUseShadowMap; // 1
		unsigned char needsDynamicShadows; // 2
		unsigned char exponent; // 3
		unsigned char isVolumetric; // 4
		char __pad0[3];
		float color[3]; // 8 12 16
		float color2[3]; // 8 12 16
		float dir[3]; // 20 24 28
		float up[3]; // 32 36 40
		float origin[3]; // 44 48 52
		float fadeOffset[2];
		float bulbRadius;
		float bulbLength[3];
		float radius; // 80
		float cosHalfFovOuter; // 84
		float cosHalfFovInner; // 88
		float cosHalfFovExpanded; // 92
		float rotationLimit; // 96
		float translationLimit; // 100
		float cucRotationOffsetRad; // 104
		float cucRotationSpeedRad; // 108
		float cucScrollVector[2]; // 112 116
		float cucScaleVector[2]; // 120 124
		float cucTransVector[2]; // 128 132
		const char* defName; // 136
	}; 
	
	assert_sizeof(ComPrimaryLight, 160);
	assert_offsetof(ComPrimaryLight, defName, 152);
	assert_offsetof(ComPrimaryLight, origin, 56);
	assert_offsetof(ComPrimaryLight, dir, 32);
	assert_offsetof(ComPrimaryLight, cosHalfFovOuter, 96);
	assert_offsetof(ComPrimaryLight, rotationLimit, 108);

	struct ComPrimaryLightEnv
	{
		unsigned short primaryLightIndices[4];
		unsigned char numIndices;
	}; assert_sizeof(ComPrimaryLightEnv, 10);

	struct ComWorld
	{
		const char* name;
		int isInUse;
		unsigned int primaryLightCount;
		ComPrimaryLight* primaryLights;
		unsigned int primaryLightEnvCount;
		ComPrimaryLightEnv* primaryLightEnvs;
	}; assert_sizeof(ComWorld, 0x28);

	struct G_GlassPiece
	{
		unsigned short damageTaken;
		unsigned short collapseTime;
		int lastStateChangeTime;
		unsigned char impactDir;
		unsigned char impactPos[2];
	}; assert_sizeof(G_GlassPiece, 12);

	struct G_GlassName
	{
		char* nameStr;
		scr_string_t name; // set during runtime from nameStr, G_InitGlass
		unsigned short pieceCount;
		unsigned short* pieceIndices;
	}; assert_sizeof(G_GlassName, 24);

	struct G_GlassData
	{
		G_GlassPiece* glassPieces;
		unsigned int pieceCount;
		unsigned short damageToWeaken;
		unsigned short damageToDestroy;
		unsigned int glassNameCount;
		G_GlassName* glassNames;
		unsigned char pad[108];
	}; assert_sizeof(G_GlassData, 0x90);

	struct GlassWorld
	{
		const char* name;
		G_GlassData* g_glassData;
	}; assert_sizeof(GlassWorld, 0x10);

	struct FxGlassDef
	{
		float halfThickness;
		float texVecs[2][2];
		GfxColor color;
		Material* material;
		Material* materialShattered;
		PhysPreset* physPreset;
		FxEffectDef* pieceBreakEffect;
		FxEffectDef* shatterEffect;
		FxEffectDef* shatterSmallEffect;
		FxEffectDef* crackDecalEffect;
		snd_alias_list_t* damagedSound;
		snd_alias_list_t* destroyedSound;
		snd_alias_list_t* destroyedQuietSound;
		char __pad[8];
		int numCrackRings;
		bool isOpaque;
	}; assert_sizeof(FxGlassDef, 120);

	struct FxSpatialFrame
	{
		float quat[4];
		float origin[3];
	};

	struct $03A8A7B39FA20F64B5AB79125E07CD62
	{
		FxSpatialFrame frame;
		float radius;
		float unk;
	};

	union FxGlassPiecePlace
	{
		$03A8A7B39FA20F64B5AB79125E07CD62 __s0;
		unsigned int nextFree;
	}; assert_sizeof(FxGlassPiecePlace, 36);

	struct FxGlassPieceState
	{
		float texCoordOrigin[2];
		unsigned int supportMask;
		unsigned short initIndex;
		unsigned short geoDataStart;
		unsigned short lightingIndex;
		unsigned char defIndex;
		unsigned char pad[3];
		unsigned char vertCount;
		unsigned char holeDataCount;
		unsigned char crackDataCount;
		unsigned char fanDataCount;
		unsigned short flags;
		float areaX2;
	}; assert_sizeof(FxGlassPieceState, 32);

	struct FxGlassPieceDynamics
	{
		int fallTime;
		__int64 physObjId;
		__int64 physJointId;
		float vel[3];
		float avel[3];
	}; assert_sizeof(FxGlassPieceDynamics, 48);

	struct FxGlassVertex
	{
		short x;
		short y;
	};

	struct FxGlassHoleHeader
	{
		unsigned short uniqueVertCount;
		unsigned char touchVert;
		unsigned char pad[1];
	};

	struct FxGlassCrackHeader
	{
		unsigned short uniqueVertCount;
		unsigned char beginVertIndex;
		unsigned char endVertIndex;
	};

	union FxGlassGeometryData
	{
		FxGlassVertex vert;
		FxGlassHoleHeader hole;
		FxGlassCrackHeader crack;
		unsigned char asBytes[4];
		short anonymous[2];
	}; assert_sizeof(FxGlassGeometryData, 4);

	struct FxGlassInitPieceState
	{
		FxSpatialFrame frame;
		float radius;
		float texCoordOrigin[2];
		unsigned int supportMask;
		unsigned int unkMask;
		float areaX2;
		unsigned short lightingIndex;
		unsigned char defIndex;
		unsigned char vertCount;
		unsigned char fanDataCount;
		unsigned char pad[1];
	}; assert_sizeof(FxGlassInitPieceState, 60);

	struct FxGlassSystem
	{
		int time;
		int prevTime;
		unsigned int defCount;
		unsigned int pieceLimit;
		unsigned int pieceWordCount;
		unsigned int cellCount;
		unsigned int activePieceCount; //
		unsigned int firstFreePiece; //
		unsigned int geoDataLimit;
		unsigned int geoDataCount;
		unsigned int initGeoDataCount;
		//
		FxGlassDef* defs;
		FxGlassPiecePlace* piecePlaces;
		FxGlassPieceState* pieceStates;
		FxGlassPieceDynamics* pieceDynamics;
		FxGlassGeometryData* geoData;
		unsigned int* isInUse;
		unsigned int* cellBits;
		unsigned char* visData;
		float(*linkOrg)[3];
		float* halfThickness;
		unsigned short* lightingHandles;
		FxGlassGeometryData* initGeoData;
		bool needToCompactData; //
		unsigned char initCount; //
		float effectChanceAccum; //
		int lastPieceDeletionTime; //
		unsigned int initPieceCount;
		FxGlassInitPieceState* initPieceStates;
	}; assert_sizeof(FxGlassSystem, 168);
	assert_offsetof(FxGlassSystem, initPieceStates, 160);

	struct FxWorld
	{
		const char* name;
		FxGlassSystem glassSys;
	}; assert_sizeof(FxWorld, 0xB0);

	struct GfxSky
	{
		int skySurfCount;
		int* skyStartSurfs;
		GfxImage* skyImage;
		unsigned char skySamplerState;
		Bounds bounds;
	}; assert_sizeof(GfxSky, 56);

	struct mnode_t
	{
		unsigned short unk0;
		unsigned short unk1;
	};

	struct GfxWorldDpvsPlanes
	{
		int cellCount;
		cplane_s* planes;
		mnode_t* nodes;
		unsigned int* sceneEntCellBits;
	}; assert_sizeof(GfxWorldDpvsPlanes, 32);

	assert_offsetof(GfxWorldDpvsPlanes, planes, 8);
	assert_offsetof(GfxWorldDpvsPlanes, nodes, 16);
	assert_offsetof(GfxWorldDpvsPlanes, sceneEntCellBits, 24);

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
	}; assert_sizeof(GfxAabbTree, 48);
	assert_offsetof(GfxAabbTree, childrenOffset, 24);
	assert_offsetof(GfxAabbTree, childCount, 28);
	assert_offsetof(GfxAabbTree, smodelIndexCount, 30);
	assert_offsetof(GfxAabbTree, smodelIndexes, 32);
	assert_offsetof(GfxAabbTree, startSurfIndex, 40);
	assert_offsetof(GfxAabbTree, surfaceCount, 44);

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
	}; assert_sizeof(GfxPortal, 80);
	assert_offsetof(GfxPortal, vertices, 40);
	assert_offsetof(GfxPortal, vertexCount, 52);

	struct GfxCell
	{
		Bounds bounds;
		short portalCount;
		unsigned char reflectionProbeCount;
		unsigned char reflectionProbeReferenceCount;
		GfxPortal* portals;
		unsigned char* reflectionProbes;
		unsigned char* reflectionProbeReferences;
	}; assert_sizeof(GfxCell, 56);
	assert_offsetof(GfxCell, portalCount, 24);
	assert_offsetof(GfxCell, reflectionProbeCount, 26);
	assert_offsetof(GfxCell, reflectionProbeReferenceCount, 27);
	assert_offsetof(GfxCell, portals, 32);
	assert_offsetof(GfxCell, reflectionProbes, 40);
	assert_offsetof(GfxCell, reflectionProbeReferences, 48);

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
	
	assert_sizeof(GfxPortalGroup, 32);
	assert_offsetof(GfxPortalGroup, infoCount, 24);

	struct GfxReflectionProbeVolume
	{
		unsigned short* data;
		unsigned int count;
	}; assert_sizeof(GfxReflectionProbeVolume, 16);

	struct GfxReflectionProbe
	{
		float origin[3];
		GfxReflectionProbeVolume* probeVolumes;
		unsigned int probeVolumeCount;
	}; assert_sizeof(GfxReflectionProbe, 32);
	assert_offsetof(GfxReflectionProbe, probeVolumeCount, 24);

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
	}; assert_sizeof(GfxRawTexture, 32);

	struct GfxLightmapArray
	{
		GfxImage* primary;
		GfxImage* secondary;
	};

	struct GfxWorldVertex
	{
		float xyz[3];
		float binormalSign;
		GfxColor color;
		float texCoord[2];
		float lmapCoord[2];
		PackedUnitVec normal;
		PackedUnitVec tangent;
	}; assert_sizeof(GfxWorldVertex, 44);

	union GfxWorldVertex0Union
	{
		GfxWorldVertex* vertices;
	};

	struct GfxWorldVertexData
	{
		GfxWorldVertex* vertices;
		ID3D11Buffer* worldVb;
		ID3D11ShaderResourceView* worldVbView;
	}; assert_sizeof(GfxWorldVertexData, 24);

	struct GfxWorldVertexLayerData
	{
		unsigned char* data;
		ID3D11Buffer* layerVb;
		ID3D11ShaderResourceView* layerVbView;
	}; assert_sizeof(GfxWorldVertexLayerData, 24);

	struct GfxDisplacementParms
	{
		char __pad0[16];
	}; assert_sizeof(GfxDisplacementParms, 16);

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
	}; assert_sizeof(GfxWorldDraw, 256);
	assert_offsetof(GfxWorldDraw, reflectionProbeTextures, 24);
	assert_offsetof(GfxWorldDraw, lightmaps, 64);
	assert_offsetof(GfxWorldDraw, lightmapOverrideSecondary, 96);
	assert_offsetof(GfxWorldDraw, vertexCount, 128);
	assert_offsetof(GfxWorldDraw, vd, 136);
	assert_offsetof(GfxWorldDraw, vld, 168);
	assert_offsetof(GfxWorldDraw, indexCount, 192);
	assert_offsetof(GfxWorldDraw, indices, 200);
	assert_offsetof(GfxWorldDraw, displacementParmsCount, 224);
	assert_offsetof(GfxWorldDraw, displacementParms, 232);

	struct GfxLightGridEntry
	{
		unsigned int colorsIndex;
		unsigned short primaryLightEnvIndex;
		unsigned char unused;
		unsigned char needsTrace;
	}; assert_sizeof(GfxLightGridEntry, 8);

	struct GfxLightGridColors
	{
		unsigned char rgb[56][6];
	}; assert_sizeof(GfxLightGridColors, 336);

	struct GfxLightGridColorsHDR
	{
		unsigned char rgb[56][6];
	}; assert_sizeof(GfxLightGridColorsHDR, 336);

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
	}; assert_sizeof(GfxLightGridTree, 80);

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
	}; assert_sizeof(GfxLightGrid, 1080);
	assert_offsetof(GfxLightGrid, rowDataStart, 32);
	assert_offsetof(GfxLightGrid, rawRowData, 48);
	assert_offsetof(GfxLightGrid, entries, 64);
	assert_offsetof(GfxLightGrid, colors, 80);
	assert_offsetof(GfxLightGrid, paletteVersion, 116);
	assert_offsetof(GfxLightGrid, rangeExponent8BitsEncoding, 120);
	assert_offsetof(GfxLightGrid, stageCount, 123);
	assert_offsetof(GfxLightGrid, stageLightingContrastGain, 128);
	assert_offsetof(GfxLightGrid, paletteEntryCount, 136);
	assert_offsetof(GfxLightGrid, paletteEntryAddress, 144);
	assert_offsetof(GfxLightGrid, paletteBitstreamSize, 152);
	assert_offsetof(GfxLightGrid, paletteBitstream, 160);
	assert_offsetof(GfxLightGrid, tree, 840);

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
	}; assert_sizeof(GfxBrushModel, 96);

	struct MaterialMemory
	{
		Material* material;
		int memory;
	}; assert_sizeof(MaterialMemory, 16);

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
	}; assert_sizeof(sunflare_t, 112);

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
	}; assert_sizeof(GfxSceneDynModel, 6);

	struct BModelDrawInfo
	{
		unsigned short surfId;
	};

	struct GfxSceneDynBrush
	{
		BModelDrawInfo info;
		unsigned short dynEntId;
	}; assert_sizeof(GfxSceneDynBrush, 4);

	struct GfxShadowGeometry
	{
		unsigned short surfaceCount;
		unsigned short smodelCount;
		unsigned int* sortedSurfIndex;
		unsigned short* smodelIndex;
	}; assert_sizeof(GfxShadowGeometry, 24);

	struct GfxLightRegionAxis
	{
		float dir[3];
		float midPoint;
		float halfSize;
	}; assert_sizeof(GfxLightRegionAxis, 20);

	struct GfxLightRegionHull
	{
		float kdopMidPoint[9];
		float kdopHalfSize[9];
		unsigned int axisCount;
		GfxLightRegionAxis* axis;
	}; assert_sizeof(GfxLightRegionHull, 88);

	struct GfxLightRegion
	{
		unsigned int hullCount;
		GfxLightRegionHull* hulls;
	}; assert_sizeof(GfxLightRegion, 16);

	struct GfxStaticModelInst
	{
		float mins[3];
		float maxs[3];
		float lightingOrigin[3];
	}; assert_sizeof(GfxStaticModelInst, 36);

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
	}; assert_sizeof(GfxSurface, 40);
	assert_offsetof(GfxSurface, material, 24);

	struct GfxSurfaceBounds
	{
		Bounds bounds;
		char __pad0[11];
		char flags;
	}; assert_sizeof(GfxSurfaceBounds, 36);

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
		XModel* model;
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
	assert_sizeof(GfxStaticModelDrawInst, 80);
	assert_offsetof(GfxStaticModelDrawInst, model, 0);
	assert_offsetof(GfxStaticModelDrawInst, cullDist, 60);
	assert_offsetof(GfxStaticModelDrawInst, flags, 62);
	assert_offsetof(GfxStaticModelDrawInst, lightingHandle, 64);
	assert_offsetof(GfxStaticModelDrawInst, primaryLightEnvIndex, 70);
	assert_offsetof(GfxStaticModelDrawInst, reflectionProbeIndex, 73); // maybe wrong
	assert_offsetof(GfxStaticModelDrawInst, firstMtlSkinIndex, 74);

	struct GfxStaticModelVertexLighting
	{
		unsigned char visibility[4];
		unsigned short ambientColorFloat16[4];
		unsigned short highlightColorFloat16[4];
	}; assert_sizeof(GfxStaticModelVertexLighting, 20);

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
	}; assert_sizeof(GfxStaticModelLighting, 24);

	struct GfxSubdivVertexLightingInfo
	{
		int vertexLightingIndex;
		ID3D11Buffer* vb;
		GfxSubdivCache cache;
	}; assert_sizeof(GfxSubdivVertexLightingInfo, 40);

	struct GfxDepthAndSurf
	{
		char __pad0[8];
	}; assert_sizeof(GfxDepthAndSurf, 8);

	typedef char* GfxWorldDpvsVoid;

	struct GfxWorldDpvsUnk
	{
		char __pad0[8];
		GfxStaticModelVertexLightingInfo info;
		char __pad1[24];
	};

	assert_sizeof(GfxWorldDpvsUnk, 56);

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
		GfxDrawSurf* surfaceMaterials; // 728
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
	assert_offsetof(GfxWorldDpvsStatic, smodelDrawInsts, 688);
	assert_offsetof(GfxWorldDpvsStatic, surfaces, 672);
	assert_offsetof(GfxWorldDpvsStatic, lodData, 640);
	assert_offsetof(GfxWorldDpvsStatic, sortedSurfIndex, 656);
	assert_offsetof(GfxWorldDpvsStatic, tessellationCutoffVisData, 648);
	assert_offsetof(GfxWorldDpvsStatic, gfx_unk, 784);
	assert_offsetof(GfxWorldDpvsStatic, sunShadowOptCount, 744);
	assert_offsetof(GfxWorldDpvsStatic, sunSurfVisDataCount, 748);
	assert_offsetof(GfxWorldDpvsStatic, surfaceCastsSunShadowOpt, 752);
	assert_offsetof(GfxWorldDpvsStatic, surfaceDeptAndSurf, 760);
	assert_offsetof(GfxWorldDpvsStatic, subdivVertexLighting, 720);
	assert_offsetof(GfxWorldDpvsStatic, constantBuffersLit, 768);
	assert_offsetof(GfxWorldDpvsStatic, constantBuffersAmbient, 776);
	assert_offsetof(GfxWorldDpvsStatic, usageCount, 792);
	assert_offsetof(GfxWorldDpvsStatic, unknownSModelVisData1, 696);
	assert_offsetof(GfxWorldDpvsStatic, unkCount1, 68);
	assert_offsetof(GfxWorldDpvsStatic, unkCount2, 632);
	assert_offsetof(GfxWorldDpvsStatic, surfaceUmbraVisData, 600);
	assert_offsetof(GfxWorldDpvsStatic, smodelInsts, 664);

	struct GfxWorldDpvsDynamic
	{
		unsigned int dynEntClientWordCount[2]; // 0 4
		unsigned int dynEntClientCount[2]; // 8 12
		unsigned int* dynEntCellBits[2]; // 16 24
		unsigned char* dynEntVisData[2][4]; // 32 40 48 56 64 72 80 88
	}; assert_sizeof(GfxWorldDpvsDynamic, 96);

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
	}; assert_sizeof(GfxHeroOnlyLight, 68);

	typedef void* umbraTomePtr_t;

	struct GfxBuildInfo
	{
		const char* args0;
		const char* args1;
		const char* buildStartTime;
		const char* buildEndTime;
	}; assert_sizeof(GfxBuildInfo, 32);

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
		unsigned int sortKeyHair; // 100
		unsigned int sortKeyEffectBlend; // 104
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
		Bounds unkBounds;
		Bounds shadowBounds;
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
		MdaoVolume* mdaoVolumes; // 2824
		char __pad3[32];
		GfxBuildInfo buildInfo; // 2864
	};

	assert_offsetof(GfxWorld, skies, 40);
	assert_offsetof(GfxWorld, sortKeyEffectBlend, 104);
	assert_offsetof(GfxWorld, aabbTreeCounts, 144);
	assert_offsetof(GfxWorld, portalGroup, 168);
	assert_offsetof(GfxWorld, lightGrid, 448);
	assert_offsetof(GfxWorld, modelCount, 1528);
	assert_offsetof(GfxWorld, models, 1536);
	assert_offsetof(GfxWorld, checksum, 1592);
	assert_offsetof(GfxWorld, materialMemoryCount, 1596);
	assert_offsetof(GfxWorld, sun, 1608);
	assert_offsetof(GfxWorld, outdoorLookupMatrix, 1720);
	assert_offsetof(GfxWorld, dpvsPlanes.nodes, 112 + 16);
	assert_offsetof(GfxWorld, dpvs, 1880);
	assert_offsetof(GfxWorld, dpvsPlanes, 112);
	assert_offsetof(GfxWorld, primaryLightCount, 72);
	assert_offsetof(GfxWorld, lastSunPrimaryLightIndex, 68);
	assert_offsetof(GfxWorld, aabbTreeCounts, 144);
	assert_offsetof(GfxWorld, portalGroupCount, 48);
	assert_offsetof(GfxWorld, portalGroupCount, 48);
	assert_offsetof(GfxWorld, mdaoVolumesCount, 2816);
	assert_offsetof(GfxWorld, mdaoVolumes, 2824);
	assert_offsetof(GfxWorld, umbraTomeData, 2800);
	assert_offsetof(GfxWorld, umbraTomeSize, 2796);
	assert_offsetof(GfxWorld, sceneDynBrush, 1816);
	assert_offsetof(GfxWorld, models, 1536);
	assert_offsetof(GfxWorld, umbraTomeSize, 2796);
	assert_offsetof(GfxWorld, fogTypesAllowed, 2792);
	assert_offsetof(GfxWorld, umbraTomeData, 2800);
	assert_offsetof(GfxWorld, dpvsDyn, 2680);
	assert_offsetof(GfxWorld, buildInfo, 2864);
	assert_offsetof(GfxWorld, mdaoVolumesCount, 2816);
	assert_offsetof(GfxWorld, mdaoVolumes, 2824);

	static_assert(sizeof(GfxWorld) == 0xB50);

	enum VehicleType
	{
		VEH_WHEELS_4 = 0x0,
		VEH_TANK = 0x1,
		VEH_PLANE = 0x2,
		VEH_BOAT = 0x3,
		VEH_ARTILLERY = 0x4,
		VEH_HELICOPTER = 0x5,
		VEH_SNOWMOBILE = 0x6,
		VEH_SUBMARINE = 0x7,
		VEH_UGV = 0x8,
		VEH_TYPE_COUNT = 0x9,
	};

	enum VehicleAxleType
	{
		VEH_AXLE_FRONT = 0x0,
		VEH_AXLE_REAR = 0x1,
		VEH_AXLE_ALL = 0x2,
		VEH_AXLE_COUNT = 0x3,
	};

	struct VehicleUnknown
	{
		char __pad[36];
	};

	static_assert(sizeof(VehicleUnknown) == 36);

	struct VehiclePhysDef
	{
		int physicsEnabled;
		const char* physPresetName;
		PhysPreset* physPreset;
		const char* unkString01;
		VehicleUnknown* vehicleUnknown01;
		const char* accelGraphName;
		VehicleAxleType steeringAxle;
		VehicleAxleType powerAxle;
		VehicleAxleType brakingAxle;
		float floatValues01[53];
	};

	static_assert(offsetof(VehiclePhysDef, physPreset) == 16);
	static_assert(offsetof(VehiclePhysDef, unkString01) == 24);
	static_assert(offsetof(VehiclePhysDef, physPresetName) == 8);
	static_assert(offsetof(VehiclePhysDef, vehicleUnknown01) == 32);
	static_assert(offsetof(VehiclePhysDef, accelGraphName) == 40);
	static_assert(sizeof(VehiclePhysDef) == 0x110);

	enum VehicleTurretFireType
	{
		VEH_TURRET_SINGLE_FIRE = 0x0,
		VEH_TURRET_DUAL_FIRE = 0x1,
		VEH_TURRET_ALT_FIRE = 0x2,
		VEH_TURRET_FIRE_TYPE_COUNT = 0x3,
	};

	struct VehicleDef
	{
		const char* internalName;
		VehicleType vehicleType;
		const char* useHintString;
		int health;
		float floatValues01[37]; // 184
		int intValues01[8];
		VehiclePhysDef vehPhysDef;
		float floatValues02[12]; // 48
		const char* unkString01;
		FxEffectDef* effect01; // 536
		FxEffectDef* effect02;
		FxEffectDef* effect03;
		FxEffectDef* effect04;
		FxEffectDef* effect05;
		float floatValues03[7]; // 168
		int intValues02[1];
		float floatValues04[32]; // 168
		int vehHelicopterSoftCollisions;
		int vehHelicopterUseGroundFX;
		FxEffectDef* vehHelicopterGroundFx;
		FxEffectDef* vehHelicopterGroundWaterFx;
		float floatValues05[78];
		const char* unkString02;
		float floatValues06[82];
		const char* turretWeaponName;
		WeaponDef* turretWeapon;
		float turretHorizSpanLeft;
		float turretHorizSpanRight;
		float turretVertSpanUp;
		float turretVertSpanDown;
		float turretHorizResistLeft;
		float turretHorizResistRight;
		float turretVertResistUp;
		float turretVertResistDown;
		float turretRotRate;
		VehicleTurretFireType turretFireType;
		snd_alias_list_t* turretSpinSnd;
		snd_alias_list_t* turretStopSnd;
		int trophyEnabled;
		float trophyRadius;
		float trophyInactiveRadius;
		int trophyAmmoCount;
		float trophyReloadTime;
		scr_string_t trophyTags[4];
		FxEffectDef* trophyExplodeFx;
		FxEffectDef* trophyFlashFx;
		Material* compassFriendlyIcon;
		Material* compassEnemyIcon;
		Material* compassFriendlyAltIcon;
		Material* compassEnemyAltIcon;
		int compassIconWidth;
		int compassIconHeight;
		snd_alias_list_t* idleLowSnd;
		snd_alias_list_t* idleHighSnd;
		snd_alias_list_t* engineLowSnd;
		snd_alias_list_t* engineHighSnd;
		snd_alias_list_t* sound01; // rename
		float engineSndSpeed;
		scr_string_t audioOriginTag;
		snd_alias_list_t* idleLowSndAlt;
		snd_alias_list_t* idleHighSndAlt;
		snd_alias_list_t* engineLowSndAlt;
		snd_alias_list_t* engineHighSndAlt;
		float engineSndSpeedAlt;
		scr_string_t audioOriginTagAlt;
		snd_alias_list_t* turretSpinSndAlt;
		snd_alias_list_t* turretStopSndAlt;
		snd_alias_list_t* engineStartUpSnd;
		int engineStartUpLength;
		snd_alias_list_t* engineShutdownSnd;
		snd_alias_list_t* engineIdleSnd;
		snd_alias_list_t* engineSustainSnd;
		snd_alias_list_t* engineRampUpSnd;
		int engineRampUpLength;
		snd_alias_list_t* engineRampDownSnd;
		int engineRampDownLength;
		snd_alias_list_t* suspensionSoftSnd;
		float suspensionSoftCompression;
		snd_alias_list_t* suspensionHardSnd;
		float suspensionHardCompression;
		snd_alias_list_t* collisionSnd;
		float collisionBlendSpeed;
		snd_alias_list_t* speedSnd;
		float speedSndBlendSpeed;
		const char* surfaceSndPrefix;
		snd_alias_list_t* surfaceSnds[53];
		float surfaceSndBlendSpeed;
		float slideVolume;
		float slideBlendSpeed;
		float inAirPitch;
		const char* soundTriggerOverrideZone;
		bool soundTriggerOverrideReverb;
		bool soundTriggerOverrideMix;
		bool soundTriggerOverrideFilter;
		bool soundTriggerOverrideOcclusion;
		bool soundTriggerOverrideAmbient;
		bool soundTriggerOverrideAmbientEvents;
		bool soundTriggerOverrideADSR;
	};

	static_assert(offsetof(VehicleDef, unkString01) == 528);
	static_assert(offsetof(VehicleDef, unkString02) == 1072);
	static_assert(offsetof(VehicleDef, turretWeaponName) == 1408);
	static_assert(offsetof(VehicleDef, turretSpinSnd) == 1464);
	static_assert(offsetof(VehicleDef, turretStopSnd) == 1472);
	static_assert(offsetof(VehicleDef, effect01) == 536);
	static_assert(offsetof(VehicleDef, compassFriendlyIcon) == 1536);
	static_assert(offsetof(VehicleDef, compassEnemyAltIcon) == 1560);
	//static_assert(offsetof(VehicleDef, effect07) == 752);
	static_assert(offsetof(VehicleDef, trophyTags) == 1500);
	static_assert(offsetof(VehicleDef, trophyExplodeFx) == 1520);
	static_assert(offsetof(VehicleDef, idleLowSnd) == 1576);
	static_assert(offsetof(VehicleDef, sound01) == 1608);
	static_assert(offsetof(VehicleDef, audioOriginTag) == 1620);
	static_assert(offsetof(VehicleDef, idleLowSndAlt) == 1624);
	static_assert(offsetof(VehicleDef, engineHighSndAlt) == 1648);
	static_assert(offsetof(VehicleDef, audioOriginTagAlt) == 1660);
	static_assert(offsetof(VehicleDef, turretSpinSndAlt) == 1664);
	static_assert(offsetof(VehicleDef, engineStartUpSnd) == 1680);
	static_assert(offsetof(VehicleDef, engineShutdownSnd) == 1696);
	static_assert(offsetof(VehicleDef, engineIdleSnd) == 1704);
	static_assert(offsetof(VehicleDef, engineSustainSnd) == 1712);
	static_assert(offsetof(VehicleDef, engineRampUpSnd) == 1720);
	static_assert(offsetof(VehicleDef, engineRampDownSnd) == 1736);
	static_assert(offsetof(VehicleDef, suspensionSoftSnd) == 1752);
	static_assert(offsetof(VehicleDef, suspensionHardSnd) == 1768);
	static_assert(offsetof(VehicleDef, collisionSnd) == 1784);
	static_assert(offsetof(VehicleDef, speedSnd) == 1800);
	static_assert(offsetof(VehicleDef, surfaceSndPrefix) == 1816);
	static_assert(offsetof(VehicleDef, surfaceSnds) == 1824);

#define sizeofmember(__struct__, __member__) sizeof(reinterpret_cast<__struct__*>(0)->__member__)

	static_assert(sizeofmember(VehicleDef, surfaceSnds) == 0x1A8);
	static_assert(offsetof(VehicleDef, soundTriggerOverrideZone) == 2264);
	static_assert(sizeof(VehicleDef) == 0x8E8);

	union XAssetHeader
	{
		void* data;
		PhysPreset* physPreset;
		PhysCollmap* physCollmap;
		PhysWaterPreset* physWaterPreset;
		PhysWorld* physWorld;
		PhysConstraint* physConstraint;
		XAnimParts* parts;
		XModelSurfs* modelSurfs;
		XModel* model;
		Material* material;
		ComputeShader* computeShader;
		MaterialVertexShader* vertexShader;
		MaterialHullShader* hullShader;
		MaterialDomainShader* domainShader;
		MaterialPixelShader* pixelShader;
		MaterialVertexDeclaration* vertexDecl;
		MaterialTechniqueSet* techniqueSet;
		GfxImage* image;
		snd_alias_list_t* sound;
		SndCurve* sndCurve;
		SndCurve* lpfCurve;
		SndCurve* reverbCurve;
		SndContext* sndContext;
		LaserDef* laser;
		LoadedSound* loadSnd;
		LocalizeEntry* localize;
		MapEnts* mapEnts;
		GfxLightDef* lightDef;
		WeaponAttachment* attachment;
		WeaponDef* weapon;
		VehicleDef* vehicle;
		FxEffectDef* fx;
		RawFile* rawfile;
		ScriptFile* scriptfile;
		StringTable* stringTable;
		StructuredDataDefSet* structuredDataDefSet;
		NetConstStrings* netConstStrings;
		TracerDef* tracerDef;
		LuaFile* luaFile;
		DopplerPreset* doppler;
		FxParticleSimAnimation* particleSimAnimation;
		SkeletonScript* skeletonScript;
		Clut* clut;
		TTFDef* ttfDef;
		PathData* aipaths;
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

	struct ScriptStringList
	{
		int count;
		const char** strings;
	};

	union GfxZoneTableEntry
	{
		char* dataPtr;
		void* voidPtr;
		ID3D11Buffer* buffer;
		ID3D11DepthStencilState* depthStencilState;
		ID3D11BlendState* blendState;
	};

	typedef std::uint32_t GfxBlendStateBits[3];

	struct XGfxGlobals
	{
		unsigned int depthStencilStateCount;
		unsigned int blendStateCount;
		std::uint64_t* depthStencilStateBits;
		GfxBlendStateBits* blendStateBits;
		GfxZoneTableEntry* depthStencilStates;
		GfxZoneTableEntry* blendStates;
		unsigned int perPrimConstantBufferCount;
		unsigned int perObjConstantBufferCount;
		unsigned int stableConstantBufferCount;
		unsigned int* perPrimConstantBufferSizes;
		unsigned int* perObjConstantBufferSizes;
		unsigned int* stableConstantBufferSizes;
		GfxZoneTableEntry* perPrimConstantBuffers;
		GfxZoneTableEntry* perObjConstantBuffers;
		GfxZoneTableEntry* stableConstantBuffers;
	};

	struct XGlobals
	{
		XGfxGlobals* gfxGlobals;
	};

	struct XAssetList
	{
		ScriptStringList stringList;
		int assetCount;
		XAsset* assets;
		XGlobals* globals;
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

	struct XZoneInfo
	{
		const char* name;
		int allocFlags;
		int freeFlags;
	};

	struct XZoneInfoInternal
	{
		char name[64];
		int flags;
		int isBaseMap;
	};
}