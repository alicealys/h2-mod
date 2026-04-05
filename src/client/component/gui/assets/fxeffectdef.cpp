#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "../gui.hpp"
#include "../asset_list.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

namespace gui::asset_list::fxeffectdef
{
	namespace
	{
		constexpr const auto input_width = 100;
		constexpr const auto right_col = 300;

#define DRAW_ASSET_FLOAT_RANGE(__name__) \
			ImGui::PushID(#__name__); \
			ImGui::Text(#__name__ ": "); \
			ImGui::SameLine(); \
			ImGui::SetCursorPosX(right_col); \
			ImGui::SetNextItemWidth(input_width); \
			ImGui::DragFloat("base", &asset->__name__.base); \
			ImGui::SameLine(); \
			ImGui::SetNextItemWidth(input_width); \
			ImGui::DragFloat("amplitude", &asset->__name__.amplitude); \
			ImGui::PopID(); \

#define DRAW_ASSET_INT_RANGE(__name__) \
			ImGui::PushID(#__name__); \
			ImGui::Text(#__name__ ": "); \
			ImGui::SameLine(); \
			ImGui::SetCursorPosX(right_col); \
			ImGui::SetNextItemWidth(input_width); \
			ImGui::InputInt("base", &asset->__name__.base); \
			ImGui::SameLine(); \
			ImGui::SetNextItemWidth(input_width); \
			ImGui::InputInt("amplitude", &asset->__name__.amplitude); \
			ImGui::PopID(); \

#define DRAW_ASSET_PROPERTY_INPUT_U8(__name__) \
			ImGui::PushID(#__name__); \
			ImGui::Text(#__name__); \
			ImGui::SameLine(); \
			ImGui::SetCursorPosX(right_col); \
			ImGui::SetNextItemWidth(input_width); \
			gui::input_u8("##" #__name__, reinterpret_cast<unsigned char*>(&asset->__name__)); \
			ImGui::PopID(); \

#define DRAW_ASSET_PROPERTY_INPUT_U16(__name__) \
			ImGui::PushID(#__name__); \
			ImGui::Text(#__name__); \
			ImGui::SameLine(); \
			ImGui::SetCursorPosX(right_col); \
			ImGui::SetNextItemWidth(input_width); \
			gui::input_u16("##" #__name__, reinterpret_cast<unsigned short*>(&asset->__name__)); \
			ImGui::PopID(); \

#define DRAW_ASSET_PROPERTY_INPUT_S32(__name__) \
			ImGui::PushID(#__name__); \
			ImGui::Text(#__name__); \
			ImGui::SameLine(); \
			ImGui::SetCursorPosX(right_col); \
			ImGui::SetNextItemWidth(input_width * 2); \
			ImGui::InputInt("##" #__name__, reinterpret_cast<int*>(&asset->__name__)); \
			ImGui::PopID(); \

#define DRAW_ASSET_PROPERTY_INPUT_F32(__name__) \
			ImGui::Text(#__name__); \
			ImGui::SameLine(); \
			ImGui::SetCursorPosX(right_col); \
			ImGui::SetNextItemWidth(input_width); \
			ImGui::InputFloat("##" #__name__, &asset->__name__); \

#define DRAW_ASSET_PROPERTY(__name__, __fmt__) \
			ImGui::Text(#__name__ ": "); \
			ImGui::SameLine(); \
			ImGui::SetCursorPosX(right_col); \
			ImGui::Text(__fmt__, asset->__name__); \

#define DRAW_ASSET_PROPERTY_TABLE(__name__, __fmt__, __table__) \
			ImGui::Text(#__name__ ": "); \
			ImGui::SameLine(); \
			ImGui::SetCursorPosX(right_col); \
			ImGui::Text(__fmt__, __table__[asset->__name__]); \

#define DRAW_ASSET_PROPERTY_COPY(__name__) \
			ImGui::Text(#__name__ ": "); \
			ImGui::SameLine(); \
			ImGui::SetCursorPosX(right_col); \
			if (ImGui::Button(asset->__name__)) \
			{ \
				gui::copy_to_clipboard(asset->__name__); \
			} \

#define DRAW_SUBASSET_BUTTON(__name__, __id__) \
			ImGui::Text(#__name__ ": "); \
			ImGui::SameLine(); \
			ImGui::SetCursorPosX(right_col); \
			if (asset->__name__.handle != nullptr) \
			{ \
				ImGui::Text(asset->__name__.handle->name); \
				add_view_button(__id__, game::ASSET_TYPE_FX, asset->__name__.handle->name); \
			} \
			else \
			{ \
				ImGui::Text("(null)"); \
			} \

		void draw_fx_elem_visual(game::FxElemDef* asset, game::FxElemVisuals* visual, int index)
		{
			switch (asset->elemType)
			{
			case game::FX_ELEM_TYPE_MODEL:
				ImGui::Text(visual->model->name);
				ImGui::SameLine();
				add_view_button(index, game::ASSET_TYPE_XMODEL, visual->model->name);
				break;
			case game::FX_ELEM_TYPE_RUNNER:
				ImGui::Text(visual->effectDef.name);
				ImGui::SameLine();
				add_view_button(index, game::ASSET_TYPE_FX, visual->effectDef.name);
				break;
			case game::FX_ELEM_TYPE_SOUND:
				ImGui::Text(visual->soundName);
				ImGui::SameLine();
				add_view_button(index, game::ASSET_TYPE_SOUND, visual->soundName);
				break;
			case game::FX_ELEM_TYPE_VECTORFIELD:
				ImGui::Text(visual->vectorFieldName);
				ImGui::SameLine();
				add_view_button(index, game::ASSET_TYPE_VECTORFIELD, visual->vectorFieldName);
				break;
			case game::FX_ELEM_TYPE_PARTICLE_SIM_ANIMATION:
				ImGui::Text(visual->particleSimAnimation->name);
				ImGui::SameLine();
				add_view_button(index, game::ASSET_TYPE_PARTICLE_SIM_ANIMATION, visual->particleSimAnimation->name);
				break;
			default:
				if (asset->elemType - 12 <= 1u)
				{
					if (asset->elemType == game::FX_ELEM_TYPE_SPOT_LIGHT)
					{
						ImGui::Text(visual->lightDef->name);
						ImGui::SameLine();
						add_view_button(index, game::ASSET_TYPE_LIGHT_DEF, visual->lightDef->name);
					}
				}
				else
				{
					ImGui::Text(visual->material->name);
					ImGui::SameLine();
					add_view_button(index, game::ASSET_TYPE_MATERIAL, visual->material->name);
				}
				break;
			}
		}

		void draw_vel_state_sample(game::FxElemVelStateSample* sample)
		{
			if (sample == nullptr)
			{
				return;
			}

#define VEL_STATE_SAMPLE(__name__) \
			if (ImGui::TreeNode(#__name__)) \
			{ \
				ImGui::SetNextItemWidth(input_width * 2); \
				ImGui::DragFloat3("totalDelta.base", sample->__name__.totalDelta.base); \
				ImGui::SameLine(); \
				ImGui::SetCursorPosX(input_width * 5); \
				ImGui::SetNextItemWidth(input_width * 2); \
				ImGui::DragFloat3("totalDelta.amplitude", sample->__name__.totalDelta.amplitude); \
				ImGui::SetNextItemWidth(input_width * 2); \
				ImGui::DragFloat3("velocity.base", sample->__name__.velocity.base); \
				ImGui::SameLine(); \
				ImGui::SetCursorPosX(input_width * 5); \
				ImGui::SetNextItemWidth(input_width * 2); \
				ImGui::DragFloat3("velocity.amplitude", sample->__name__.velocity.amplitude); \
				ImGui::TreePop(); \
			} \

			VEL_STATE_SAMPLE(local);
			VEL_STATE_SAMPLE(world);
			VEL_STATE_SAMPLE(unk);
		}

		void draw_vis_state_sample(game::FxElemVisStateSample* sample)
		{
			if (sample == nullptr)
			{
				return;
			}

			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat4("color.base", sample->base.color);
			ImGui::SameLine();
			ImGui::SetCursorPosX(input_width * 5);
			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat4("color.amplitude", sample->amplitude.color);

			//

			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat3("emissiveScale.base", sample->base.emissiveScale);
			ImGui::SameLine();
			ImGui::SetCursorPosX(input_width * 5);
			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat3("emissiveScale.amplitude", sample->amplitude.emissiveScale);

			//

			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat("rotationDelta.base", &sample->base.rotationDelta);
			ImGui::SameLine();
			ImGui::SetCursorPosX(input_width * 5);
			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat("rotationDelta.amplitude", &sample->amplitude.rotationDelta);

			//

			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat("rotationTotal.base", &sample->base.rotationTotal);
			ImGui::SameLine();
			ImGui::SetCursorPosX(input_width * 5);
			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat("rotationTotal.amplitude", &sample->amplitude.rotationTotal);

			//

			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat2("size.base", sample->base.size);
			ImGui::SameLine();
			ImGui::SetCursorPosX(input_width * 5);
			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat2("size.amplitude", sample->amplitude.size);

			//

			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat("scale.base", &sample->base.scale);
			ImGui::SameLine();
			ImGui::SetCursorPosX(input_width * 5);
			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat("scale.amplitude", &sample->amplitude.scale);

			//

			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat2("pivot.base", sample->base.pivot);
			ImGui::SameLine();
			ImGui::SetCursorPosX(input_width * 5);
			ImGui::SetNextItemWidth(input_width * 2);
			ImGui::DragFloat2("pivot.amplitude", sample->amplitude.pivot);
		}

		void draw_fx_elem_def(game::FxElemDef* asset)
		{
			static const char* fx_elem_type_names[] =
			{
				"FX_ELEM_TYPE_SPRITE_BILLBOARD",
				"FX_ELEM_TYPE_SPRITE_ORIENTED",
				"FX_ELEM_TYPE_SPRITE_ROTATED",
				"FX_ELEM_TYPE_TAIL",
				"FX_ELEM_TYPE_LINE",
				"FX_ELEM_TYPE_TRAIL",
				"FX_ELEM_TYPE_FLARE",
				"FX_ELEM_TYPE_PARTICLE_SIM_ANIMATION",
				"FX_ELEM_TYPE_CLOUD",
				"FX_ELEM_TYPE_SPARK_CLOUD",
				"FX_ELEM_TYPE_SPARK_FOUNTAIN",
				"FX_ELEM_TYPE_MODEL",
				"FX_ELEM_TYPE_OMNI_LIGHT",
				"FX_ELEM_TYPE_SPOT_LIGHT",
				"FX_ELEM_TYPE_SOUND",
				"FX_ELEM_TYPE_DECAL",
				"FX_ELEM_TYPE_RUNNER",
				"FX_ELEM_TYPE_VECTORFIELD",
			};

			static std::vector<const char*> fx_elem_flags =
			{
				"",
				"FX_ELEM_SPAWN_RELATIVE_TO_EFFECT",
				"FX_ELEM_SPAWN_FRUSTUM_CULL",
				"FX_ELEM_RUNNER_USES_RAND_ROT",
				"FX_ELEM_SPAWN_OFFSET_SPHERE",
				"FX_ELEM_SPAWN_OFFSET_CYLINDER",
				"FX_ELEM_RUN_RELATIVE_TO_EFFECT",
				"FX_ELEM_RUN_RELATIVE_TO_CAMERA",
				"FX_ELEM_RUN_RELATIVE_TO_WORLD",
				"FX_ELEM_DIE_ON_TOUCH",
				"FX_ELEM_DRAW_PAST_FOG",
				"FX_ELEM_DRAW_WITH_VIEWMODEL",
				"FX_ELEM_BLOCK_SIGHT",
				"FX_ELEM_DRAW_IN_THERMAL_VIEW_ONLY",
				"FX_ELEM_TRAIL_ORIENT_BY_VELOCITY",
				"FX_ELEM_EMIT_ORIENT_BY_ELEM",
				"FX_ELEM_USE_OCCLUSION_QUERY",
				"FX_ELEM_USE_CAST_SHADOW",
				"FX_ELEM_NODRAW_IN_THERMAL_VIEW",
				"FX_ELEM_SPAWN_IMPACT_FX_WITH_SURFACE_NAME",
				"FX_ELEM_RECEIVE_DYNAMIC_LIGHT",
				"FX_ELEM_VOLUMETRIC_TRAIL",
				"FX_ELEM_USE_COLLISION",
				"FX_ELEM_USE_VECTORFIELDS",
				"FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL",
				"FX_ELEM_HAS_VELOCITY_GRAPH_WORLD",
				"FX_ELEM_HAS_GRAVITY",
				"FX_ELEM_USE_MODEL_PHYSICS",
				"FX_ELEM_NONUNIFORM_SCALE",
				"FX_ELEM_CLOUD_SHAPE_SPHERE_LARGE",
				"FX_ELEM_CLOUD_SHAPE_SPHERE_MEDIUM",
			};

			static std::vector<const char*> fx_elem_flags2 = 
			{
				"FX_ELEM2_BILLBOARD_FACING_CAMERA_PERPENDICULAR",
				"FX_ELEM2_BILLBOARD_FACING_PLAYER",
				"FX_ELEM2_EMIT_TRAILS",
				"FX_ELEM2_USE_EMISSIVE_DRAW",
				"",
				"FX_ELEM2_USE_EFFECT_MODEL_COLOR",
				"FX_ELEM2_REQUIRES_POST_RESOLVE_DRAW",
				"FX_ELEM2_DECAL_EXPONENTIAL_FADE_OUT",
				"FX_ELEM2_USE_DOPL",
				"FX_ELEM2_USE_ITEM_CLIP",
				"FX_ELEM2_RUN_RELATIVE_TO_OFFSET",
				"FX_ELEM2_HIGH_PRI_SHADOW",
				"FX_ELEM2_DISABLE_SHADOW",
				"FX_ELEM2_DRAW_DPVS_CULL",
				"FX_ELEM2_DRAW_IN_NVG_VIEW",
				"FX_ELEM2_NODRAW_IN_NVG_VIEW",
				"FX_ELEM2_LIGHTGRID_DIRECTIONAL_FETCH",
				"FX_ELEM2_HAS_PARENT_VELOCITY",
				"FX_ELEM2_THERMAL_OVERRIDES_MATERIAL",
				"FX_ELEM2_BLOCK_OCCLUSION_QUERIES",
				"FX_ELEM2_ENABLE_SPECULAR",
				"FX_ELEM2_DETECT_WATER_CLIP",
				"FX_ELEM2_ENABLE_TRAIL_DEPTH_SORT",
			};

			if (ImGui::TreeNode("flags"))
			{
				gui::input_flags(&asset->flags, fx_elem_flags);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("flags2"))
			{
				gui::input_flags(&asset->flags2, fx_elem_flags2);
				ImGui::TreePop();
			}

			DRAW_ASSET_PROPERTY_INPUT_S32(flags);
			DRAW_ASSET_PROPERTY_INPUT_S32(flags2);
			DRAW_ASSET_PROPERTY_INPUT_S32(spawn.looping.intervalMsec);
			DRAW_ASSET_PROPERTY_INPUT_S32(spawn.looping.count);
			DRAW_ASSET_FLOAT_RANGE(spawnRange);
			DRAW_ASSET_FLOAT_RANGE(fadeInRange);
			DRAW_ASSET_FLOAT_RANGE(fadeOutRange);
			DRAW_ASSET_PROPERTY_INPUT_F32(spawnFrustumCullRadius);
			DRAW_ASSET_INT_RANGE(spawnDelayMsec);
			DRAW_ASSET_INT_RANGE(lifeSpanMsec);
			DRAW_ASSET_FLOAT_RANGE(spawnOrigin[0]);
			DRAW_ASSET_FLOAT_RANGE(spawnOrigin[1]);
			DRAW_ASSET_FLOAT_RANGE(spawnOrigin[2]);
			DRAW_ASSET_FLOAT_RANGE(spawnOffsetRadius);
			DRAW_ASSET_FLOAT_RANGE(spawnOffsetHeight);
			DRAW_ASSET_FLOAT_RANGE(unkRange);
			DRAW_ASSET_FLOAT_RANGE(spawnAngles[0]);
			DRAW_ASSET_FLOAT_RANGE(spawnAngles[1]);
			DRAW_ASSET_FLOAT_RANGE(spawnAngles[2]);
			DRAW_ASSET_FLOAT_RANGE(angularVelocity[0]);
			DRAW_ASSET_FLOAT_RANGE(angularVelocity[1]);
			DRAW_ASSET_FLOAT_RANGE(angularVelocity[2]);
			DRAW_ASSET_FLOAT_RANGE(initialRotation);
			DRAW_ASSET_FLOAT_RANGE(gravity);
			DRAW_ASSET_FLOAT_RANGE(reflectionFactor);
			if (ImGui::TreeNode("atlas.behavior"))
			{
				static std::vector<const char*> atlas_flags =
				{
					"FX_ATLAS_START_RANDOM",
					"FX_ATLAS_START_INDEXED",
					"FX_ATLAS_PLAY_OVER_LIFE",
					"FX_ATLAS_LOOP_ONLY_N_TIMES",
					"FX_ATLAS_FRAME_BLEND",
					"FX_ATLAS_IS_ARRAY",
					"FLAG_UNK_6",
					"FLAG_UNK_7",
				};

				gui::input_flags8(&asset->atlas.behavior, atlas_flags);
				ImGui::TreePop();
			}
			DRAW_ASSET_PROPERTY_INPUT_U8(atlas.behavior);
			DRAW_ASSET_PROPERTY_INPUT_U8(atlas.index);
			DRAW_ASSET_PROPERTY_INPUT_U8(atlas.fps);
			DRAW_ASSET_PROPERTY_INPUT_U8(atlas.loopCount);
			DRAW_ASSET_PROPERTY_INPUT_U8(atlas.colIndexBits);
			DRAW_ASSET_PROPERTY_INPUT_U8(atlas.rowIndexBits);
			DRAW_ASSET_PROPERTY_INPUT_U16(atlas.entryCount);
			DRAW_ASSET_PROPERTY_INPUT_U8(elemType);
			ImGui::SetCursorPosX(right_col);
			ImGui::Text(fx_elem_type_names[asset->elemType]);
			DRAW_ASSET_PROPERTY_INPUT_U8(elemLitType);
			DRAW_ASSET_PROPERTY(visualCount, "%i");
			DRAW_ASSET_PROPERTY(velIntervalCount, "%i");
			DRAW_ASSET_PROPERTY(visStateIntervalCount, "%i");

			if (ImGui::TreeNode("velSamples"))
			{
				for (auto i = 0; i < asset->velIntervalCount + 1; i++)
				{
					ImGui::PushID(i);
					if (ImGui::TreeNode("velSample", "velSample %i", i))
					{
						draw_vel_state_sample(&asset->velSamples[i]);

						ImGui::TreePop();
					}

					ImGui::PopID();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("visSamples"))
			{
				for (auto i = 0; i < asset->visStateIntervalCount + 1; i++)
				{
					ImGui::PushID(i);
					if (ImGui::TreeNode("visSample", "visSample %i", i))
					{
						draw_vis_state_sample(&asset->visSamples[i]);

						ImGui::TreePop();
					}

					ImGui::PopID();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("visuals"))
			{
				if (asset->elemType == game::FX_ELEM_TYPE_DECAL)
				{
					if (asset->visuals.markArray)
					{
						for (unsigned char a = 0; a < asset->visualCount; a++)
						{
							ImGui::PushID(a);
							if (ImGui::TreeNode("visual", "visual %i", a))
							{
								if (asset->visuals.markArray[a].materials[0])
								{
									ImGui::Text("markArray[%i].materials[0]: %s", a, asset->visuals.markArray[a].materials[0]->name);
									add_view_button(1000 * a, game::ASSET_TYPE_MATERIAL, asset->visuals.markArray[a].materials[0]->name);
								}
								else
								{
									ImGui::Text("markArray[%i].materials[0]: (null)", a);
								}

								if (asset->visuals.markArray[a].materials[1])
								{
									ImGui::Text("markArray[%i].materials[0]: %s ", a, asset->visuals.markArray[a].materials[1]->name);
									add_view_button(1000 * a + 1, game::ASSET_TYPE_MATERIAL, asset->visuals.markArray[a].materials[1]->name);
								}
								else
								{
									ImGui::Text("markArray[%i].materials[0]: (null)", a);
								}

								if (asset->visuals.markArray[a].materials[2])
								{
									ImGui::Text("markArray[%i].materials[0]: %s", a, asset->visuals.markArray[a].materials[2]->name);
									add_view_button(1000 * a + 2, game::ASSET_TYPE_MATERIAL, asset->visuals.markArray[a].materials[2]->name);
								}
								else
								{
									ImGui::Text("markArray[%i].materials[2]: (null)", a);
								}

								ImGui::TreePop();
							}
							ImGui::PopID();
						}
					}
					else
					{
						ImGui::Text("(null)");
					}
				}
				else if (asset->visualCount > 1)
				{
					if (asset->visuals.array)
					{
						for (unsigned char vis = 0; vis < asset->visualCount; vis++)
						{
							draw_fx_elem_visual(asset, &asset->visuals.array[vis], vis);
						}
					}
					else
					{
						ImGui::Text("(null)");
					}
				}
				else
				{
					draw_fx_elem_visual(asset, &asset->visuals.instance, 1000);
				}

				ImGui::TreePop();
			}

			DRAW_SUBASSET_BUTTON(effectOnImpact, 1);
			DRAW_SUBASSET_BUTTON(effectOnDeath, 2);
			DRAW_SUBASSET_BUTTON(effectEmitted, 3);

			DRAW_ASSET_FLOAT_RANGE(emitDist);
			DRAW_ASSET_FLOAT_RANGE(emitDistVariance);

			if (asset->extended.unknownDef != nullptr && ImGui::TreeNode("extended"))
			{
				ImGui::TreePop();
			}

			DRAW_ASSET_PROPERTY_INPUT_U8(sortOrder);
			DRAW_ASSET_PROPERTY_INPUT_U8(lightingFrac);
			DRAW_ASSET_PROPERTY_INPUT_U8(fadeInfo);
			DRAW_ASSET_PROPERTY_INPUT_U8(fadeOutInfo);
			DRAW_ASSET_PROPERTY_INPUT_S32(randomSeed);
			DRAW_ASSET_PROPERTY_INPUT_F32(emissiveScaleScale);
			DRAW_ASSET_PROPERTY_INPUT_F32(hdrLightingFrac);
			DRAW_ASSET_PROPERTY_INPUT_F32(shadowDensityScale);
			DRAW_ASSET_PROPERTY_INPUT_F32(scatterRatio);
		}

		bool draw_asset(game::FxEffectDef* asset)
		{
			ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
			
			static std::vector<const char*> fx_flags =
			{
				"FX_EFFECT_NEEDS_LIGHT_GRID_LIGHTING_AT_SPAWN",
				"FX_EFFECT_NEEDS_CAST_SHADOW",
				"FX_EFFECT_NEEDS_LIGHT_GRID_LIGHTING_PER_FRAME",
				"FX_EFFECT_NEEDS_DYNAMIC_LIGHTING_PER_FRAME",
				"FX_EFFECT_NEEDS_EMISSIVE_DRAW",
				"FLAG_UNK_5",
				"FX_EFFECT_LINK_TO_SUN",
				"FX_EFFECT_HAS_CAMERA_RELATIVE_ELEM",
				"FLAG_UNK_8",
				"FLAG_UNK_9",
				"FLAG_UNK_10",
				"FLAG_UNK_11",
				"FLAG_UNK_12",
				"FLAG_UNK_13",
				"FLAG_UNK_14",
				"FLAG_UNK_15",
				"FLAG_UNK_16",
			};

			if (ImGui::TreeNode("flags"))
			{
				gui::input_flags(&asset->flags, fx_flags);
				ImGui::TreePop();
			}

			DRAW_ASSET_PROPERTY_INPUT_S32(flags);
			DRAW_ASSET_PROPERTY_COPY(name);
			DRAW_ASSET_PROPERTY(totalSize, "%i");
			DRAW_ASSET_PROPERTY_INPUT_S32(msecLoopingLife);
			DRAW_ASSET_PROPERTY(elemDefCountLooping, "%i");
			DRAW_ASSET_PROPERTY(elemDefCountEmission, "%i");
			DRAW_ASSET_PROPERTY(elemDefCountOneShot, "%i");
			DRAW_ASSET_PROPERTY_INPUT_F32(elemMaxRadius);
			DRAW_ASSET_PROPERTY(occlusionQueryFadeIn, "%i");
			DRAW_ASSET_PROPERTY(occlusionQueryFadeOut, "%i");
			DRAW_ASSET_PROPERTY_INPUT_F32(occlusionQueryDepthBias);
			DRAW_ASSET_FLOAT_RANGE(occlusionQueryScaleRange);
			DRAW_ASSET_PROPERTY_INPUT_F32(xU_01);

			if (asset->elemDefCountLooping && ImGui::TreeNode("looping elems"))
			{
				for (auto i = 0; i < asset->elemDefCountLooping; i++)
				{
					ImGui::PushID(i);
					if (ImGui::TreeNode("elem", "elem %i", i))
					{
						draw_fx_elem_def(&asset->elemDefs[i]);

						ImGui::TreePop();
					}
					ImGui::PopID();
				}

				ImGui::TreePop();
			}

			if (asset->elemDefCountEmission && ImGui::TreeNode("emission elems"))
			{
				for (auto i = 0; i < asset->elemDefCountEmission; i++)
				{
					ImGui::PushID(i);
					if (ImGui::TreeNode("elem", "elem %i", i))
					{
						draw_fx_elem_def(&asset->elemDefs[i + asset->elemDefCountLooping]);

						ImGui::TreePop();
					}
					ImGui::PopID();
				}

				ImGui::TreePop();
			}

			if (asset->elemDefCountOneShot && ImGui::TreeNode("one shot elems"))
			{
				for (auto i = 0; i < asset->elemDefCountOneShot; i++)
				{
					ImGui::PushID(i);
					if (ImGui::TreeNode("elem", "elem %i", i))
					{
						draw_fx_elem_def(&asset->elemDefs[i + asset->elemDefCountEmission + asset->elemDefCountLooping]);

						ImGui::TreePop();
					}
					ImGui::PopID();
				}

				ImGui::TreePop();
			}

			return true;
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			gui::asset_list::add_asset_view<game::FxEffectDef>(game::ASSET_TYPE_FX, draw_asset);

		}
	};
}

REGISTER_COMPONENT(gui::asset_list::fxeffectdef::component)
