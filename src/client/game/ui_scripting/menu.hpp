#pragma once
#include "game/game.hpp"
#include "element.hpp"

namespace ui_scripting
{
	class menu final
	{
	public:
		menu();

		bool visible = false;
		bool cursor = false;
		bool cursor_was_enabled = false;

		void open();
		void close();

		void add_child(element* el);
		void render() const;

		std::vector<element*> children{};
	};
}
