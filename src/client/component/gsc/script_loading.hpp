#pragma once

#include "game/scripting/functions.hpp"
#include <xsk/gsc/engine/h2.hpp>

namespace gsc
{
	struct col_line_t
	{
		std::uint16_t line;
		std::uint16_t column;
	};

	extern std::unique_ptr<xsk::gsc::h2::context> gsc_ctx;
	extern game::dvar_t* developer_script;

	game::ScriptFile* find_script(game::XAssetType type, const char* name, int allow_create_default);

	std::optional<std::map<std::uint32_t, col_line_t>*> get_script_devmap(const std::string& name);
}
