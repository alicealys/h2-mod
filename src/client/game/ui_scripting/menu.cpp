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
		*game::keyCatchers |= 0x40;
		this->visible = true;
	}

	void menu::close()
	{
		*game::keyCatchers &= ~0x40;
		this->visible = false;
	}

	void menu::render() const
	{
		if (this->cursor)
		{
			*game::keyCatchers |= 0x40;
		}

		for (auto& element : this->children)
		{
			element->render();
		}
	}
}
