#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "console.hpp"

#include <utils/hook.hpp>

namespace menus
{
	namespace
	{
		constexpr auto patch_menu_list_name = "ui/patch_code.txt";

		void ui_add_menu_list_stub(void* context, void* menu_list, int a3)
		{
			game::UI_AddMenuList(context, menu_list, a3);

			if (game::DB_XAssetExists(game::ASSET_TYPE_MENUFILE, patch_menu_list_name) &&
				!game::DB_IsXAssetDefault(game::ASSET_TYPE_MENUFILE, patch_menu_list_name))
			{
				const auto patch_code_list = game::UI_LoadMenus(patch_menu_list_name);
				game::UI_AddMenuList(context, patch_code_list, a3);
			}
		}

		int menus_open_by_name_stub(void* context, const char* name)
		{
			const std::string override_name = "override/"s + name;
			const auto override_menu = game::Menus_FindByName(context, override_name.data());

			if (override_menu)
			{
				game::Menus_Open(context, override_menu, 0);
				return 1;
			}

			const auto menu = game::Menus_FindByName(context, name);
			if (menu)
			{
				game::Menus_Open(context, menu, 0);
				return 1;
			}

			return 0;
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::call(0x1405F0A45, ui_add_menu_list_stub);
			utils::hook::jump(game::Menus_OpenByName, menus_open_by_name_stub);
		}
	};
}

REGISTER_COMPONENT(menus::component)
