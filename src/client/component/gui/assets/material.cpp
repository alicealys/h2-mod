#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "component/scheduler.hpp"
#include "component/command.hpp"
#include "component/fastfiles.hpp"
#include "../gui.hpp"
#include "../asset_list.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

namespace gui::asset_list::material
{
	namespace
	{
		std::unordered_map<unsigned char, std::string> image_type_names =
		{
			{0x0, "2D"},
			{0x1, "FUNCTION"},
			{0x2, "COLOR_MAP"},
			{0x3, "DETAIL_MAP"},
			{0x4, "UNUSED_2"},
			{0x5, "NORMAL_MAP"},
			{0x6, "UNUSED_3"},
			{0x7, "UNUSED_4"},
			{0x8, "SPECULAR_MAP"},
			{0x9, "UNUSED_5"},
			{0xA, "OCEANFLOW_DISPLACEMENT_MAP"},
			{0xB, "WATER_MAP"},
			{0xC, "OCEAN_DISPLACEMENT_MAP"},
			{0xD, "DISPLACEMENT_MAP"},
			{0xE, "PARALLAX_MAP"},
		};

		std::string get_image_type_name(unsigned char type)
		{
			if (!image_type_names.contains(type))
			{
				return "UNKNOWN";
			}

			return image_type_names[type];
		}

		bool draw_material_window(game::Material* asset)
		{
			ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
			if (ImGui::TreeNode("textures"))
			{
				for (auto i = 0; i < asset->textureCount; i++)
				{
					if (asset->textureTable && asset->textureTable->u.image && asset->textureTable->u.image->texture.shaderView)
					{
						const auto type_name = get_image_type_name(asset->textureTable[i].semantic);

						ImGui::Text("%s", type_name.data());
						ImGui::SameLine();
						if (ImGui::Button(asset->textureTable[i].u.image->name))
						{
							gui::copy_to_clipboard(asset->textureTable->u.image->name);
						}

						const auto width = asset->textureTable[i].u.image->width;
						const auto height = asset->textureTable[i].u.image->height;
						const auto ratio = static_cast<float>(width) / static_cast<float>(height);
						constexpr auto size = 200.f;

						ImGui::Image(asset->textureTable[i].u.image->texture.shaderView,
							ImVec2(size * ratio, size)
						);
					}
				}

				ImGui::TreePop();
			}

#define DRAW_ASSET_PROPERTY(__name__, __fmt__) \
				ImGui::Text(#__name__ ": " __fmt__, asset->__name__); \

#define DRAW_ASSET_PROPERTY_COPY(__name__) \
				ImGui::Text(#__name__ ": "); \
				ImGui::SameLine(); \
				if (ImGui::Button(asset->__name__)) \
				{ \
					gui::copy_to_clipboard(asset->__name__); \
				} \

			DRAW_ASSET_PROPERTY_COPY(name);
			DRAW_ASSET_PROPERTY_COPY(techniqueSet->name);
			DRAW_ASSET_PROPERTY(textureCount, "%i");
			DRAW_ASSET_PROPERTY(constantCount, "%i");
			DRAW_ASSET_PROPERTY(stateBitsCount, "%i");
			DRAW_ASSET_PROPERTY(stateFlags, "%i");
			DRAW_ASSET_PROPERTY(cameraRegion, "%i");
			DRAW_ASSET_PROPERTY(materialType, "%i");
			DRAW_ASSET_PROPERTY(layerCount, "%i");
			DRAW_ASSET_PROPERTY(assetFlags, "%X");

			return true;
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			gui::asset_list::add_asset_view<game::Material>(game::ASSET_TYPE_MATERIAL, draw_material_window);
		}
	};
}

REGISTER_COMPONENT(gui::asset_list::material::component)
