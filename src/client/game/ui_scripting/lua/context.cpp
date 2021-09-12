#include <std_include.hpp>
#include "context.hpp"
#include "error.hpp"
#include "../../scripting/execution.hpp"

#include "../../../component/ui_scripting.hpp"

#include "component/game_console.hpp"
#include "component/scheduler.hpp"

#include <utils/string.hpp>

namespace ui_scripting::lua
{
	namespace
	{
		std::unordered_map<std::string, menu> menus;
		std::vector<element*> elements;
		element ui_element;
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

		scripting::script_value convert(const sol::lua_value& value)
		{
			if (value.is<int>())
			{
				return {value.as<int>()};
			}

			if (value.is<unsigned int>())
			{
				return {value.as<unsigned int>()};
			}

			if (value.is<bool>())
			{
				return {value.as<bool>()};
			}

			if (value.is<double>())
			{
				return {value.as<double>()};
			}

			if (value.is<float>())
			{
				return {value.as<float>()};
			}
			if (value.is<std::string>())
			{
				return {value.as<std::string>()};
			}

			if (value.is<scripting::vector>())
			{
				return {value.as<scripting::vector>()};
			}

			return {};
		}

		bool point_in_rect(int px, int py, int x, int y, int w, int h)
		{
			return (px > x && px < x + w && py > y && py < y + h);
		}

		void render_menus()
		{
			check_resize();

			for (const auto& menu : menus)
			{
				if (menu.second.visible)
				{
					menu.second.render();
				}
			}
		}

		std::vector<element*> elements_in_point(int x, int y)
		{
			std::vector<element*> result;

			for (const auto& menu : menus)
			{
				if (!menu.second.visible)
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

		int mouse[2];

		void handle_key_event(sol::state& state, event_handler& handler, const int key, const int down)
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
						{state, mouse[0]},
						{state, mouse[1]},
					};

					handler.dispatch(main_event);

					for (const auto& element : _elements)
					{
						event event;
						event.element = element;
						event.name = utils::string::va("%s%s", key_name, down ? "down" : "up");
						event.arguments =
						{
							{state, mouse[0]},
							{state, mouse[1]},
						};

						handler.dispatch(event);
					}
				}

				if (!down)
				{
					event main_event;
					main_event.element = &ui_element;
					main_event.name = click_name;
					main_event.arguments =
					{
						{state, mouse[0]},
						{state, mouse[1]},
					};

					handler.dispatch(main_event);

					for (const auto& element : _elements)
					{
						event event;
						event.element = element;
						event.name = click_name;
						event.arguments =
						{
							{state, mouse[0]},
							{state, mouse[1]},
						};

						handler.dispatch(event);
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
						{state, mouse[0]},
						{state, mouse[1]},
					};

					handler.dispatch(main_event);

					for (const auto& element : _elements)
					{
						event event;
						event.element = element;
						event.name = key_name;
						event.arguments =
						{
							{state, mouse[0]},
							{state, mouse[1]},
						};

						handler.dispatch(event);
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
				event.arguments =
				{
					{state, key},
				};

				handler.dispatch(event);

				break;
			}
			}
		}

		void handle_char_event(sol::state& state, event_handler& handler, const int key)
		{
			std::string key_str = {(char)key};
			event event;
			event.element = &ui_element;
			event.name = "keypress";
			event.arguments =
			{
				{state, key_str},
			};

			handler.dispatch(event);
		}

		std::vector<element*> previous_elements;
		void handle_mousemove_event(sol::state& state, event_handler& handler, const int x, const int y)
		{
			mouse[0] = x;
			mouse[1] = y;

			{
				event event;
				event.element = &ui_element;
				event.name = "mousemove";
				event.arguments =
				{
					{state, x},
					{state, y},
				};

				handler.dispatch(event);
			}

			const auto _elements = elements_in_point(x, y);
			for (const auto& element : _elements)
			{
				event event;
				event.element = element;
				event.name = "mouseover";

				handler.dispatch(event);
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

					handler.dispatch(event);
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

					handler.dispatch(event);
				}
			}

			previous_elements = _elements;
		}

		bool valid_dvar_name(const std::string& name)
		{
			for (const auto c : name)
			{
				if (!isalnum(c))
				{
					return false;
				}
			}
			
			return true;
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

		void setup_types(sol::state& state, event_handler& handler, scheduler& scheduler)
		{
			clear_menus();

			auto vector_type = state.new_usertype<scripting::vector>("scripting::vector", sol::constructors<scripting::vector(float, float, float)>());
			vector_type["x"] = sol::property(&scripting::vector::get_x, &scripting::vector::set_x);
			vector_type["y"] = sol::property(&scripting::vector::get_y, &scripting::vector::set_y);
			vector_type["z"] = sol::property(&scripting::vector::get_z, &scripting::vector::set_z);

			vector_type["r"] = sol::property(&scripting::vector::get_x, &scripting::vector::set_x);
			vector_type["g"] = sol::property(&scripting::vector::get_y, &scripting::vector::set_y);
			vector_type["b"] = sol::property(&scripting::vector::get_z, &scripting::vector::set_z);

			vector_type[sol::meta_function::addition] = sol::overload(
				[](const scripting::vector& a, const scripting::vector& b)
			{
				return scripting::vector(
					a.get_x() + b.get_x(),
					a.get_y() + b.get_y(),
					a.get_z() + b.get_z()
				);
			},
				[](const scripting::vector& a, const int value)
			{
				return scripting::vector(
					a.get_x() + value,
					a.get_y() + value,
					a.get_z() + value
				);
			}
			);

			vector_type[sol::meta_function::subtraction] = sol::overload(
				[](const scripting::vector& a, const scripting::vector& b)
			{
				return scripting::vector(
					a.get_x() - b.get_x(),
					a.get_y() - b.get_y(),
					a.get_z() - b.get_z()
				);
			},
				[](const scripting::vector& a, const int value)
			{
				return scripting::vector(
					a.get_x() - value,
					a.get_y() - value,
					a.get_z() - value
				);
			}
			);

			vector_type[sol::meta_function::multiplication] = sol::overload(
				[](const scripting::vector& a, const scripting::vector& b)
			{
				return scripting::vector(
					a.get_x() * b.get_x(),
					a.get_y() * b.get_y(),
					a.get_z() * b.get_z()
				);
			},
				[](const scripting::vector& a, const int value)
			{
				return scripting::vector(
					a.get_x() * value,
					a.get_y() * value,
					a.get_z() * value
				);
			}
			);

			vector_type[sol::meta_function::division] = sol::overload(
				[](const scripting::vector& a, const scripting::vector& b)
			{
				return scripting::vector(
					a.get_x() / b.get_x(),
					a.get_y() / b.get_y(),
					a.get_z() / b.get_z()
				);
			},
				[](const scripting::vector& a, const int value)
			{
				return scripting::vector(
					a.get_x() / value,
					a.get_y() / value,
					a.get_z() / value
				);
			}
			);

			vector_type[sol::meta_function::equal_to] = [](const scripting::vector& a, const scripting::vector& b)
			{
				return a.get_x() == b.get_x() &&
					   a.get_y() == b.get_y() &&
					   a.get_z() == b.get_z();
			};

			vector_type[sol::meta_function::length] = [](const scripting::vector& a)
			{
				return sqrt((a.get_x() * a.get_x()) + (a.get_y() * a.get_y()) + (a.get_z() * a.get_z()));
			};

			vector_type[sol::meta_function::to_string] = [](const scripting::vector& a)
			{
				return utils::string::va("{x: %f, y: %f, z: %f}", a.get_x(), a.get_y(), a.get_z());
			};

			auto element_type = state.new_usertype<element>("element", "new", []()
			{
				const auto el = new element();
				elements.push_back(el);
				return el;
			});

			element_type["setvertalign"] = &element::set_vertalign;
			element_type["sethorzalign"] = &element::set_horzalign;
			element_type["setrect"] =&element::set_rect;
			element_type["setfont"] = &element::set_font;
			element_type["settext"] = &element::set_text;
			element_type["setmaterial"] = &element::set_material;
			element_type["setcolor"] = &element::set_color;
			element_type["setbackcolor"] = &element::set_background_color;
			element_type["setbordercolor"] = &element::set_border_color;
			element_type["setborderwidth"] = sol::overload(
				static_cast<void(element::*)(float)>(&element::set_border_width),
				static_cast<void(element::*)(float, float)>(&element::set_border_width),
				static_cast<void(element::*)(float, float, float)>(&element::set_border_width),
				static_cast<void(element::*)(float, float, float, float)>(&element::set_border_width)
			);
			element_type["settextoffset"] = &element::set_text_offset;
			element_type["setslice"] = &element::set_slice;

			element_type["onnotify"] = [&handler](element& element, const std::string& event,
												  const event_callback& callback)
			{
				event_listener listener{};
				listener.callback = callback;
				listener.element = &element;
				listener.event = event;
				listener.is_volatile = false;

				return handler.add_event_listener(std::move(listener));
			};

			element_type["getrect"] = [](const sol::this_state s, element& element)
			{
				auto rect = sol::table::create(s.lua_state());
				rect["x"] = element.x;
				rect["y"] = element.y;
				rect["w"] = element.w + element.border_width[1] + element.border_width[3];
				rect["h"] = element.h + element.border_width[0] + element.border_width[2];

				return rect;
			};

			element_type["onnotifyonce"] = [&handler](element& element, const std::string& event,
													  const event_callback& callback)
			{
				event_listener listener{};
				listener.callback = callback;
				listener.element = &element;
				listener.event = event;
				listener.is_volatile = true;

				return handler.add_event_listener(std::move(listener));
			};

			auto menu_type = state.new_usertype<menu>("menu");

			menu_type["addchild"] = [](const sol::this_state s, menu& menu, element& element)
			{
				menu.add_child(&element);
			};

			menu_type["cursor"] = sol::property(
				[](menu& menu)
				{
					return menu.cursor;
				},
				[](menu& menu, bool cursor)
				{
					menu.cursor = cursor;
				}
			);

			struct game
			{
			};
			auto game_type = state.new_usertype<game>("game_");
			state["game"] = game();

			game_type["newmenu"] = [](const sol::lua_value&, const std::string& name)
			{
				menus[name] = {};
				return &menus[name];
			};

			game_type["getmouseposition"] = [](const sol::this_state s, const game&)
			{
				auto pos = sol::table::create(s.lua_state());
				pos["x"] = mouse[0];
				pos["y"] = mouse[1];

				return pos;
			};

			game_type["openmenu"] = [](const game&, const std::string& name)
			{
				if (menus.find(name) == menus.end())
				{
					return;
				}

				menus[name].open();
			};

			game_type["closemenu"] = [](const game&, const std::string& name)
			{
				if (menus.find(name) == menus.end())
				{
					return;
				}

				menus[name].close();
			};

			game_type["onframe"] = [&scheduler](const game&, const sol::protected_function& callback)
			{
				return scheduler.add(callback, 0, false);
			};

			game_type["onnotify"] = [&handler](const game&, const std::string& event,
												  const event_callback& callback)
			{
				event_listener listener{};
				listener.callback = callback;
				listener.element = &ui_element;
				listener.event = event;
				listener.is_volatile = false;

				return handler.add_event_listener(std::move(listener));
			};

			game_type["onnotifyonce"] = [&handler](const game&, const std::string& event,
													  const event_callback& callback)
			{
				event_listener listener{};
				listener.callback = callback;
				listener.element = &ui_element;
				listener.event = event;
				listener.is_volatile = true;

				return handler.add_event_listener(std::move(listener));
			};

			game_type["isingame"] = []()
			{
				return ::game::CL_IsCgameInitialized() && ::game::g_entities[0].client;
			};

			game_type["getdvar"] = [](const game&, const sol::this_state s, const std::string& name)
			{
				const auto dvar = ::game::Dvar_FindVar(name.data());
				if (!dvar)
				{
					return sol::lua_value{s, sol::lua_nil};
				}

				const std::string value = ::game::Dvar_ValueToString(dvar, nullptr, &dvar->current);
				return sol::lua_value{s, value};
			};

			game_type["getdvarint"] = [](const game&, const sol::this_state s, const std::string& name)
			{
				const auto dvar = ::game::Dvar_FindVar(name.data());
				if (!dvar)
				{
					return sol::lua_value{s, sol::lua_nil};
				}

				const auto value = atoi(::game::Dvar_ValueToString(dvar, nullptr, &dvar->current));
				return sol::lua_value{s, value};
			};

			game_type["getdvarfloat"] = [](const game&, const sol::this_state s, const std::string& name)
			{
				const auto dvar = ::game::Dvar_FindVar(name.data());
				if (!dvar)
				{
					return sol::lua_value{s, sol::lua_nil};
				}

				const auto value = atof(::game::Dvar_ValueToString(dvar, nullptr, &dvar->current));
				return sol::lua_value{s, value};
			};

			game_type["setdvar"] = [](const game&, const std::string& name, const sol::lua_value& value)
			{
				if (!valid_dvar_name(name))
				{
					throw std::runtime_error("Invalid DVAR name, must be alphanumeric");
				}

				const auto hash = ::game::generateHashValue(name.data());
				std::string string_value;

				if (value.is<bool>())
				{
					string_value = utils::string::va("%i", value.as<bool>());
				} 
				else if (value.is<int>())
				{
					string_value = utils::string::va("%i", value.as<int>());
				} 
				else if (value.is<float>())
				{
					string_value = utils::string::va("%f", value.as<float>());
				}
				else if (value.is<scripting::vector>())
				{
					const auto v = value.as<scripting::vector>();
					string_value = utils::string::va("%f %f %f", 
						v.get_x(),
						v.get_y(),
						v.get_z()
					);
				}

				if (value.is<std::string>())
				{
					string_value = value.as<std::string>();
				}

				::game::Dvar_SetCommand(hash, "", string_value.data());
			};

			game_type["drawmaterial"] = [](const game&, float x, float y, float width, float height, float s0, float t0, float s1, float t1,
				const sol::lua_value& color_value, const std::string& material)
			{
				const auto color = color_value.as<std::vector<float>>();
				float _color[4] =
				{
					color[0],
					color[1],
					color[2],
					color[3],
				};

				const auto _material = ::game::Material_RegisterHandle(material.data());
				::game::R_AddCmdDrawStretchPic(x, y, width, height, s0, t0, s1, t1, _color, _material);
			};

			game_type["playsound"] = [](const game&, const std::string& sound)
			{
				::game::UI_PlayLocalSoundAlias(0, sound.data());
			};

			game_type["getwindowsize"] = [](const game&, const sol::this_state s)
			{
				const auto size = ::game::ScrPlace_GetViewPlacement()->realViewportSize;

				auto screen = sol::table::create(s.lua_state());
				screen["x"] = size[0];
				screen["y"] = size[1];

				return screen;
			};

			struct player
			{
			};
			auto player_type = state.new_usertype<player>("player_");
			state["player"] = player();

			player_type["notify"] = [](const player&, const sol::this_state s, const std::string& name, sol::variadic_args va)
			{
				if (!::game::CL_IsCgameInitialized() || !::game::g_entities[0].client)
				{
					throw std::runtime_error("Not in game");
				}

				std::vector<scripting::script_value> arguments{};

				for (auto arg : va)
				{
					arguments.push_back(convert({s, arg}));
				}

				::scheduler::once([s, va, name, arguments]()
				{
					const auto player = scripting::call("getentbynum", {0}).as<scripting::entity>();
					scripting::notify(player, name, arguments);
				}, ::scheduler::pipeline::server);
			};

			player_type["getorigin"] = [](const player&)
			{
				if (!::game::CL_IsCgameInitialized() || !::game::g_entities[0].client)
				{
					throw std::runtime_error("Not in game");
				}

				return scripting::vector(
					::game::g_entities[0].origin[0],
					::game::g_entities[0].origin[1],
					::game::g_entities[0].origin[2]
				);
			};

			player_type["setorigin"] = [](const player&, const scripting::vector& velocity)
			{
				if (!::game::CL_IsCgameInitialized() || !::game::g_entities[0].client)
				{
					throw std::runtime_error("Not in game");
				}

				::game::g_entities[0].origin[0] = velocity.get_x();
				::game::g_entities[0].origin[1] = velocity.get_y();
				::game::g_entities[0].origin[2] = velocity.get_z();
			};

			player_type["getvelocity"] = [](const player&)
			{
				if (!::game::CL_IsCgameInitialized() || !::game::g_entities[0].client)
				{
					throw std::runtime_error("Not in game");
				}

				return scripting::vector(
					::game::g_entities[0].client->velocity[0],
					::game::g_entities[0].client->velocity[1],
					::game::g_entities[0].client->velocity[2]
				);
			};

			player_type["setvelocity"] = [](const player&, const scripting::vector& velocity)
			{
				if (!::game::CL_IsCgameInitialized() || !::game::g_entities[0].client)
				{
					throw std::runtime_error("Not in game");
				}

				::game::g_entities[0].client->velocity[0] = velocity.get_x();
				::game::g_entities[0].client->velocity[1] = velocity.get_y();
				::game::g_entities[0].client->velocity[2] = velocity.get_z();
			};
		}
	}

	void open_menu(const std::string& name)
	{
		if (menus.find(name) == menus.end())
		{
			return;
		}

		menus[name].open();
	}

	void close_menu(const std::string& name)
	{
		if (menus.find(name) == menus.end())
		{
			return;
		}

		menus[name].close();
	}

	context::context(std::string folder)
		: folder_(std::move(folder))
		  , scheduler_(state_)
		  , event_handler_(state_)

	{
		this->state_.open_libraries(sol::lib::base,
		                            sol::lib::package,
		                            sol::lib::io,
		                            sol::lib::string,
		                            sol::lib::os,
		                            sol::lib::math,
		                            sol::lib::table);

		this->state_["include"] = [this](const std::string& file)
		{
			this->load_script(file);
		};

		sol::function old_require = this->state_["require"];
		auto base_path = utils::string::replace(this->folder_, "/", ".") + ".";
		this->state_["require"] = [base_path, old_require](const std::string& path)
		{
			return old_require(base_path + path);
		};

		this->state_["scriptdir"] = [this]()
		{
			return this->folder_;
		};

		setup_types(this->state_, this->event_handler_, this->scheduler_);

		printf("Loading ui script '%s'\n", this->folder_.data());
		this->load_script("__init__");
	}

	context::~context()
	{
		this->state_.collect_garbage();
		this->scheduler_.clear();
		this->state_ = {};
	}

	void context::run_frame()
	{
		render_menus();
		this->scheduler_.run_frame();
		this->state_.collect_garbage();
	}

	void context::ui_event(const std::string& type, const std::vector<int>& arguments)
	{
		if (type == "key")
		{
			handle_key_event(this->state_, this->event_handler_, arguments[0], arguments[1]);
		}

		if (type == "char")
		{
			handle_char_event(this->state_, this->event_handler_, arguments[0]);
		}

		if (type == "mousemove")
		{
			handle_mousemove_event(this->state_, this->event_handler_, 
				relative_mouse(arguments[0]), relative_mouse(arguments[1]));
		}
	}

	void context::load_script(const std::string& script)
	{
		if (!this->loaded_scripts_.emplace(script).second)
		{
			return;
		}

		const auto file = (std::filesystem::path{this->folder_} / (script + ".lua")).generic_string();
		handle_error(this->state_.safe_script_file(file, &sol::script_pass_on_error));
	}
}
