#pragma once

#include "game/structs.hpp"
#include "gui.hpp"

namespace gui::asset_list
{
	void add_asset_view_callback(game::XAssetType, const std::function<void(const std::string&)>& callback);
	void add_view_button(int id, game::XAssetType type, const char* name);

	template <typename T>
	void add_asset_view(game::XAssetType type, const std::function<void(T*)>& draw_callback)
	{
		static std::unordered_set<std::string> opened_assets;
		add_asset_view_callback(type, [](const std::string& name)
		{
			opened_assets.insert(name);
		});

		gui::register_callback([=]()
		{
			for (auto i = opened_assets.begin(); i != opened_assets.end(); )
			{
				const auto& name = *i;
				const auto header = reinterpret_cast<T*>(game::DB_FindXAssetHeader(type, name.data(), 0).data);
				if (header == nullptr)
				{
					i = opened_assets.erase(i);
					continue;
				}

				auto is_open = true;
				if (ImGui::Begin(name.data(), &is_open))
				{
					draw_callback(header);
				}
				ImGui::End();

				if (is_open)
				{
					++i;
				}
				else
				{
					i = opened_assets.erase(i);
				}
			}
		}, false);
	}
}
