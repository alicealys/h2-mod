#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "component/scheduler.hpp"
#include "component/command.hpp"
#include "component/fastfiles.hpp"
#include "gui.hpp"
#include "asset_list.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

namespace gui::asset_list
{
	namespace
	{
		bool shown_assets[game::XAssetType::ASSET_TYPE_COUNT]{};
		std::string asset_type_filter;
		std::string assets_name_filter[game::XAssetType::ASSET_TYPE_COUNT];
		std::string zone_name_filter[game::XAssetType::ASSET_TYPE_COUNT];

		std::unordered_map<game::XAssetType, std::function<void(const std::string&)>> asset_view_callbacks;

		void render_window()
		{
			static auto* enabled = &gui::enabled_menus["asset_list"];
			ImGui::Begin("Asset list", enabled);

			static bool show_asset_zone = true;

			if (ImGui::TreeNode("loaded zones"))
			{
				for (auto i = 1u; i <= *game::g_zoneCount; i++)
				{
					if (ImGui::Button(game::g_zones[i].name))
					{
						gui::copy_to_clipboard(game::g_zones[i].name);
					}
				}

				ImGui::TreePop();
			}

			ImGui::Checkbox("show asset zone", &show_asset_zone);
			ImGui::InputText("asset type", &asset_type_filter);
			ImGui::BeginChild("asset type list");

			for (auto i = 0; i < game::XAssetType::ASSET_TYPE_COUNT; i++)
			{
				const auto name = game::g_assetNames[i];
				const auto type = static_cast<game::XAssetType>(i);

				if (asset_type_filter.size() == 0 || utils::string::strstr_lower(name, asset_type_filter.data()))
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

				static bool default_only[game::ASSET_TYPE_COUNT] = {};
				static int asset_count[game::ASSET_TYPE_COUNT] = {};
				static bool disabled_zones[game::ASSET_TYPE_COUNT][0x100] = {};

				ImGui::Text("count: %i / %i", asset_count[type], game::g_poolSize[type]);

				ImGui::InputText("asset name", &assets_name_filter[type]);

				if (ImGui::InputText("zone name", &zone_name_filter[type]))
				{
					for (auto zone = 0u; zone <= *game::g_zoneCount; zone++)
					{
						const auto zone_name = game::g_zones[zone].name;
						disabled_zones[type][zone] = !utils::string::strstr_lower(zone_name, zone_name_filter[type].data());
					}
				}

				const auto should_add_view_btn = asset_view_callbacks.contains(type);
				ImGui::Checkbox("default assets only", &default_only[type]);

				ImGui::BeginChild("assets list");

				asset_count[type] = 0;
				fastfiles::enum_asset_entries(type, [&](const game::XAssetEntry* entry)
				{
					asset_count[type]++;

					const auto asset = entry->asset;
					auto asset_name = game::DB_GetXAssetName(&asset);
					if (asset_name[0] == '\0')
					{
						return;
					}

					if (disabled_zones[type][entry->zoneIndex])
					{
						return;
					}

					const auto is_default = entry->zoneIndex == 0;
					if (default_only[type] && !is_default)
					{
						return;
					}

					if (is_default)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.43f, 0.15f, 0.15f, 1.f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.98f, 0.26f, 0.26f, 1.f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.98f, 0.06f, 0.06f, 1.f));
					}

					if (utils::string::strstr_lower(asset_name, assets_name_filter[type].data()))
					{
						if (show_asset_zone)
						{
							if (entry->zoneIndex > 0)
							{
								ImGui::Text(game::g_zones[entry->zoneIndex].name);
							}
							else
							{
								ImGui::Text("default");
							}

							ImGui::SameLine();
						}

						if (ImGui::Button(asset_name))
						{
							gui::copy_to_clipboard(asset_name);
						}

						if (should_add_view_btn)
						{
							ImGui::SameLine();
							ImGui::PushID(asset_count[type]);
							if (ImGui::Button("view"))
							{
								asset_view_callbacks.at(type)(asset_name);
							}
							ImGui::PopID();
						}
					}

					if (is_default)
					{
						ImGui::PopStyleColor(3);
					}
				}, true);

				ImGui::EndChild();
				ImGui::End();
			}
		}
	}

	void add_asset_view_callback(game::XAssetType type, const std::function<void(const std::string&)>& callback)
	{
		asset_view_callbacks.insert(std::make_pair(type, callback));
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
