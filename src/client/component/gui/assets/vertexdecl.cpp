#include <std_include.hpp>

#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "component/fastfiles.hpp"
#include "../gui.hpp"
#include "../asset_list.hpp"

#include <utils/string.hpp>
#include <utils/compression.hpp>

namespace gui::asset_list::vertexdecl
{
	namespace
	{
		bool draw_asset(game::MaterialVertexDeclaration* asset)
		{
			ImGui::Text("name: %s", asset->name);
			ImGui::Text("streamCount: %i", asset->streamCount);
			ImGui::Text("hasOptionalSource: %i", asset->hasOptionalSource);

			if (ImGui::TreeNode("routing"))
			{
				for (auto i = 0; i < asset->streamCount; i++)
				{
					const auto data = &asset->routing.data[i];
					ImGui::PushID(i);
					if (ImGui::TreeNode("stream", "stream %i", i))
					{
						gui::input_u8("source", &data->source);
						gui::input_u8("dest", &data->dest);
						gui::input_u8("mask", &data->mask);
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
			gui::asset_list::add_asset_view<game::MaterialVertexDeclaration>(game::ASSET_TYPE_VERTEXDECL, draw_asset);
		}
	};
}

REGISTER_COMPONENT(gui::asset_list::vertexdecl::component)
