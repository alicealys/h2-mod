#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "command.hpp"
#include "gui.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

namespace asset_list
{
	namespace
	{
		bool shown_assets[game::XAssetType::ASSET_TYPE_COUNT];
		std::string asset_type_filter;
		std::string assets_name_filter;

		void enum_assets(const game::XAssetType type, const std::function<void(game::XAssetHeader)>& callback, const bool includeOverride)
		{
			game::DB_EnumXAssets_Internal(type, static_cast<void(*)(game::XAssetHeader, void*)>([](game::XAssetHeader header, void* data)
			{
				const auto& cb = *static_cast<const std::function<void(game::XAssetHeader)>*>(data);
				cb(header);
			}), &callback, includeOverride);
		}

		void on_frame()
		{
			if (!gui::enabled_menus["asset_list"])
			{
				return;
			}

			{
				ImGui::Begin("Asset list", &gui::enabled_menus["asset_list"]);

				ImGui::InputText("asset type", &asset_type_filter);
				ImGui::BeginChild("asset type list");

				for (auto i = 0; i < game::XAssetType::ASSET_TYPE_COUNT; i++)
				{
					const auto name = game::g_assetNames[i];
					const auto type = static_cast<game::XAssetType>(i);

					if (utils::string::find_lower(name, asset_type_filter))
					{
						ImGui::Checkbox(name, &shown_assets[type]);
					}
				}

				ImGui::EndChild();
				ImGui::End();
			}

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

				ImGui::InputText("asset name", &assets_name_filter);
				ImGui::BeginChild("assets list");

				enum_assets(type, [type](const game::XAssetHeader header)
				{
					const auto asset = game::XAsset{type, header};
					const auto* const asset_name = game::DB_GetXAssetName(&asset);

					if (utils::string::find_lower(asset_name, assets_name_filter) && ImGui::Button(asset_name))
					{
						gui::copy_to_clipboard(asset_name);
					}
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
			gui::on_frame(on_frame);
		}
	};
}

REGISTER_COMPONENT(asset_list::component)
