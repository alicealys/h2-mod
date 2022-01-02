#pragma once

#include "game/game.hpp"

namespace fastfiles
{
	void enum_assets(const game::XAssetType type, const std::function<void(game::XAssetHeader)>& callback, const bool includeOverride);
	std::string get_current_fastfile();
}
