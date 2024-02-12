#pragma once

#include "game/game.hpp"
#include <utils/memory.hpp>

namespace fastfiles
{
	void enum_assets(const game::XAssetType type, const std::function<void(game::XAssetHeader)>& callback, const bool includeOverride);
	void enum_asset_entries(const game::XAssetType type, const std::function<void(game::XAssetEntry*)>& callback, bool include_override);

	std::string get_current_fastfile();

	bool exists(const std::string& zone);
	bool try_load_zone(const std::string& name, bool localized, bool game = false);
}
