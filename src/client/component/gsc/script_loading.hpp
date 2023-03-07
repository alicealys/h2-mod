#pragma once

#include "game/scripting/functions.hpp"
#include <xsk/gsc/engine/h2.hpp>

namespace gsc
{
	extern std::unique_ptr<xsk::gsc::h2::context> gsc_ctx;
	extern game::dvar_t* developer_script;

	game::ScriptFile* find_script(game::XAssetType type, const char* name, int allow_create_default);
}
