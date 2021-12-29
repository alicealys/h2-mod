#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "localized_strings.hpp"
#include "scheduler.hpp"
#include "command.hpp"
#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace branding
{
	float color[4] = {0.9f, 0.9f, 0.5f, 1.f};

	void draw()
	{
		const auto font = game::R_RegisterFont("fonts/defaultBold.otf", 22);
		game::R_AddCmdDrawText("h2-mod", 0x7FFFFFFF, font, 15.f,
			15.f + static_cast<float>(font->pixelHeight),
			1.f, 1.f, 0.f, color, 0);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			localized_strings::override("MENU_SP_CAMPAIGN", "H2-MOD");
		}
	};
}

REGISTER_COMPONENT(branding::component) 