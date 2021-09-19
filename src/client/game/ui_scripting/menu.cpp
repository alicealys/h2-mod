#include <std_include.hpp>
#include "menu.hpp"
#include "lua/engine.hpp"
#include "component/ui_scripting.hpp"

namespace ui_scripting
{
	menu::menu()
	{
	}

	void menu::add_child(element* el)
	{
		this->children.push_back(el);
	}

	void menu::open()
	{
		if (this->visible)
		{
			return;
		}

		this->cursor_was_enabled = *game::keyCatchers & 0x40;
		if (this->cursor)
		{
			*game::keyCatchers |= 0x40;
		}

		this->visible = true;
	}

	void menu::close()
	{
		if (!this->visible)
		{
			return;
		}

		if (this->cursor && !this->cursor_was_enabled)
		{
			*game::keyCatchers &= ~0x40;
		}

		this->visible = false;
	}

	void menu::render()
	{
		if (this->cursor && !(*game::keyCatchers & 0x40))
		{
			this->visible = false;
			return;
		}

		for (auto& element : this->children)
		{
			if (element->hidden)
			{
				continue;
			}

			element->render();
		}
	}
}
