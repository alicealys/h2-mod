#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "component/scheduler.hpp"
#include "component/command.hpp"
#include "component/fastfiles.hpp"
#include "gui.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

namespace gui::asset_list
{
	namespace
	{
		bool shown_assets[game::XAssetType::ASSET_TYPE_COUNT]{};
		std::string asset_type_filter;
		std::string assets_name_filter[game::XAssetType::ASSET_TYPE_COUNT];

		void render_window()
		{
			static auto* enabled = &gui::enabled_menus["asset_list"];
			ImGui::Begin("Asset list", enabled);

			ImGui::InputText("asset type", &asset_type_filter);
			ImGui::BeginChild("asset type list");

			for (auto i = 0; i < game::XAssetType::ASSET_TYPE_COUNT; i++)
			{
				const auto name = game::g_assetNames[i];
				const auto type = static_cast<game::XAssetType>(i);

				if (utils::string::strstr_lower(name, asset_type_filter.data()))
				{
					ImGui::Checkbox(name, &shown_assets[type]);
				}
			}

			ImGui::EndChild();
			ImGui::End();

			for (auto i = 0; i < game::XAssetType::ASSET_TYPE_COUNT; i++)
			{
				const auto name = game::g_assetNames[i];
				const auto type = static_cast<game::XAssetType>(i);

				if (!shown_assets[type])
				{
					continue;
				}

				ImGui::SetNextWindowSizeConstraints(ImVec2(500, 500), ImVec2(1000, 1000));
				ImGui::Begin(name, &shown_assets[type]);

				ImGui::InputText("asset name", &assets_name_filter[type]);
				ImGui::BeginChild("assets list");

				size_t asset_num{};
				fastfiles::enum_assets(type, [type, &asset_num](const game::XAssetHeader header)
				{
					const auto asset = game::XAsset{type, header};
					auto asset_name = game::DB_GetXAssetName(&asset);
					if (asset_name[0] == '\0')
					{
						asset_name = utils::string::va("__%i", asset_num);
					}

					if (utils::string::strstr_lower(asset_name, assets_name_filter[type].data()) && ImGui::Button(asset_name))
					{
						gui::copy_to_clipboard(asset_name);
					}

					asset_num++;
				}, true);

				ImGui::EndChild();
				ImGui::End();
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			gui::register_menu("asset_list", "Asset List", render_window);
		}
	};
}

REGISTER_COMPONENT(gui::asset_list::component)
