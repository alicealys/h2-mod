#pragma once
#include "game/game.hpp"
#include "element.hpp"

namespace ui_scripting
{
	enum menu_type
	{
		normal,
		overlay
	};

	class menu final
	{
	public:
		menu();

		bool visible = false;
		bool cursor = false;

		void open();
		void close();

		void add_child(element* el);
		void render() const;

		menu_type type = normal;

		std::string overlay_menu;
		std::vector<element*> children{};
	};
}
