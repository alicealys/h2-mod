#include <std_include.hpp>
#include "engine.hpp"
#include "context.hpp"

#include "../../../component/scheduler.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>

namespace ui_scripting::lua::engine
{
	void notify(const event& e);

	namespace
	{
		float screen_max[2];

		void check_resize()
		{
			screen_max[0] = game::ScrPlace_GetViewPlacement()->realViewportSize[0];
			screen_max[1] = game::ScrPlace_GetViewPlacement()->realViewportSize[1];
		}

		int relative_mouse(int value)
		{
			return (int)(((float)value / screen_max[0]) * 1920.f);
		}

		int relative(int value)
		{
			return (int)(((float)value / 1920.f) * screen_max[0]);
		}

		float relative(float value)
		{
			return (value / 1920.f) * screen_max[0];
		}

		bool point_in_rect(int px, int py, int x, int y, int w, int h)
		{
			return (px > x && px < x + w && py > y && py < y + h);
		}

		bool is_menu_visible(const menu& menu)
		{
			return menu.visible || (menu.type == menu_type::overlay && game::Menu_IsMenuOpenAndVisible(0, menu.overlay_menu.data()));
		}

		std::vector<element*> elements_in_point(int x, int y)
		{
			std::vector<element*> result;

			for (const auto& menu : menus)
			{
				if (!is_menu_visible(menu.second))
				{
					continue;
				}

				for (const auto& child : menu.second.children)
				{
					const auto in_rect = point_in_rect(
						x, y,
						(int)child->x,
						(int)child->y,
						(int)child->w + (int)child->border_width[1] + (int)child->border_width[3],
						(int)child->h + (int)child->border_width[0] + (int)child->border_width[2]
					);

					if (in_rect)
					{
						result.push_back(child);
					}
				}
			}

			return result;
		}

		void handle_key_event(const int key, const int down)
		{
			const auto _elements = elements_in_point(mouse[0], mouse[1]);

			switch (key)
			{
			case game::K_MOUSE2:
			case game::K_MOUSE1:
			{
				const auto click_name = key == game::K_MOUSE1
					? "click"
					: "rightclick";

				const auto key_name = key == game::K_MOUSE1
					? "mouse"
					: "rightmouse";

				{
					event main_event;
					main_event.element = &ui_element;
					main_event.name = utils::string::va("%s%s", key_name, down ? "down" : "up");
					main_event.arguments =
					{
						mouse[0],
						mouse[1],
					};

					engine::notify(main_event);

					for (const auto& element : _elements)
					{
						event event;
						event.element = element;
						event.name = utils::string::va("%s%s", key_name, down ? "down" : "up");
						event.arguments =
						{
							mouse[0],
							mouse[1],
						};

						engine::notify(event);
					}
				}

				if (!down)
				{
					event main_event;
					main_event.element = &ui_element;
					main_event.name = click_name;
					main_event.arguments =
					{
						mouse[0],
						mouse[1],
					};

					engine::notify(main_event);

					for (const auto& element : _elements)
					{
						event event;
						event.element = element;
						event.name = click_name;
						event.arguments =
						{
							mouse[0],
							mouse[1],
						};

						engine::notify(event);
					}
				}

				break;
			}
			case game::K_MWHEELUP:
			case game::K_MWHEELDOWN:
			{
				const auto key_name = key == game::K_MWHEELUP
					? "scrollup"
					: "scrolldown";

				if (!down)
				{
					break;
				}

				{
					event main_event;
					main_event.element = &ui_element;
					main_event.name = key_name;
					main_event.arguments =
					{
						mouse[0],
						mouse[1],
					};

					engine::notify(main_event);

					for (const auto& element : _elements)
					{
						event event;
						event.element = element;
						event.name = key_name;
						event.arguments = {mouse[0], mouse[1]};

						engine::notify(event);
					}
				}

				break;
			}
			default:
			{
				event event;
				event.element = &ui_element;
				event.name = down
					? "keydown"
					: "keyup";
				event.arguments = {key};

				notify(event);

				break;
			}
			}
		}

		void handle_char_event(const int key)
		{
			std::string key_str = {(char)key};
			event event;
			event.element = &ui_element;
			event.name = "keypress";
			event.arguments = {key_str};

			engine::notify(event);
		}

		std::vector<element*> previous_elements;
		void handle_mousemove_event(const int x, const int y)
		{
			if (mouse[0] == x && mouse[1] == y)
			{
				return;
			}

			mouse[0] = x;
			mouse[1] = y;

			{
				event event;
				event.element = &ui_element;
				event.name = "mousemove";
				event.arguments = {x, y};

				engine::notify(event);
			}

			const auto _elements = elements_in_point(x, y);
			for (const auto& element : _elements)
			{
				event event;
				event.element = element;
				event.name = "mouseover";

				engine::notify(event);
			}

			for (const auto& element : previous_elements)
			{
				auto found = false;

				for (const auto& _element : _elements)
				{
					if (element == _element)
					{
						found = true;
					}
				}

				if (!found)
				{
					event event;
					event.element = element;
					event.name = "mouseleave";

					engine::notify(event);
				}
			}

			for (const auto& element : _elements)
			{
				auto found = false;

				for (const auto& _element : previous_elements)
				{
					if (element == _element)
					{
						found = true;
					}
				}

				if (!found)
				{
					event event;
					event.element = element;
					event.name = "mouseenter";

					engine::notify(event);
				}
			}

			previous_elements = _elements;
		}

		auto& get_scripts()
		{
			static std::vector<std::unique_ptr<context>> scripts{};
			return scripts;
		}

		void load_scripts()
		{
			const auto script_dir = "ui_scripts/"s;

			if (!utils::io::directory_exists(script_dir))
			{
				return;
			}

			const auto scripts = utils::io::list_files(script_dir);

			for (const auto& script : scripts)
			{
				if (std::filesystem::is_directory(script) && utils::io::file_exists(script + "/__init__.lua"))
				{
					get_scripts().push_back(std::make_unique<context>(script));
				}
			}
		}

		void render_menus()
		{
			check_resize();

			for (const auto& menu : menus)
			{
				if (is_menu_visible(menu.second))
				{
					menu.second.render();
				}
			}
		}

		void close_all_menus()
		{
			for (auto& menu : menus)
			{
				if (!is_menu_visible(menu.second))
				{
					continue;
				}

				event event;
				event.element = &menu.second;
				event.name = "close";
				notify(event);

				menu.second.close();
			}
		}

		void clear_menus()
		{
			menus.clear();

			for (const auto element : elements)
			{
				delete element;
			}

			elements.clear();
		}
	}

	void open_menu(const std::string& name)
	{
		if (menus.find(name) == menus.end())
		{
			return;
		}

		const auto menu = &menus[name];

		event event;
		event.element = menu;
		event.name = "open";
		notify(event);

		menu->open();
	}

	void close_menu(const std::string& name)
	{
		if (menus.find(name) == menus.end())
		{
			return;
		}

		const auto menu = &menus[name];

		event event;
		event.element = menu;
		event.name = "close";
		notify(event);

		menu->close();
	}

	void start()
	{
		close_all_menus();
		get_scripts().clear();
		clear_menus();
		load_scripts();
	}

	void stop()
	{
		close_all_menus();
		get_scripts().clear();
		clear_menus();
	}

	void ui_event(const std::string& type, const std::vector<int>& arguments)
	{
		::scheduler::once([type, arguments]()
		{
			if (type == "key")
			{
				handle_key_event(arguments[0], arguments[1]);
			}

			if (type == "char")
			{
				handle_char_event(arguments[0]);
			}

			if (type == "mousemove")
			{
				handle_mousemove_event(relative_mouse(arguments[0]), relative_mouse(arguments[1]));
			}
		}, ::scheduler::pipeline::renderer);
	}

	void notify(const event& e)
	{
		for (auto& script : get_scripts())
		{
			script->notify(e);
		}
	}

	void run_frame()
	{
		check_resize();
		render_menus();

		for (auto& script : get_scripts())
		{
			script->run_frame();
		}
	}
}
