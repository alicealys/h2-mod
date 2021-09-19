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
	void draw()
	{
		const auto x = 15.f;
		const auto y = 15.f;
		const auto scale = 1.0f;
		float color[4] = { 0.9f, 0.9f, 0.5f, 1.f };
		const auto* text = "h2-mod";

		auto* font = game::R_RegisterFont("fonts/defaultBold.otf", 22);

		if (!font) return;

		game::R_AddCmdDrawText(text, 0x7FFFFFFF, font, static_cast<float>(x),
			y + static_cast<float>(font->pixelHeight) * scale,
			scale, scale, 0.0f, color, 0);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			localized_strings::override("MENU_SP_CAMPAIGN", "H2-Mod");
		}
	};
}

REGISTER_COMPONENT(branding::component) 