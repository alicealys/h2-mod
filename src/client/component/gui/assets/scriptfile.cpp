#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "component/scheduler.hpp"
#include "component/command.hpp"
#include "component/fastfiles.hpp"
#include "component/gsc/script_loading.hpp"
#include "../gui.hpp"
#include "../asset_list.hpp"

#include "utils/mapents.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>
#include <utils/concurrency.hpp>
#include <utils/io.hpp>

namespace gui::asset_list::scriptfile
{
	namespace
	{
		std::string get_asset_display_name(const std::string& name)
		{
			const auto id = static_cast<std::uint32_t>(std::atoi(name.data()));

			if (id == 0)
			{
				return name;
			}

			return gsc::gsc_ctx->token_name(id);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			gui::asset_list::add_asset_name_override_callback(game::ASSET_TYPE_SCRIPTFILE, get_asset_display_name);
		}
	};
}

REGISTER_COMPONENT(gui::asset_list::scriptfile::component)
