#pragma once

#include "game/structs.hpp"

namespace gui::asset_list
{
	void add_asset_view_callback(game::XAssetType, const std::function<void(const std::string&)>& callback);
	void add_view_button(int id, game::XAssetType type, const char* name);
}
