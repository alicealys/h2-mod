#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "command.hpp"
#include "gui.hpp"

#include "game/scripting/lua/context.hpp"
#include "game/scripting/lua/engine.hpp"
#include "game/scripting/execution.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>
#include <utils/concurrency.hpp>

namespace gui_debug
{
	namespace
	{
		game::dvar_t* cl_paused = nullptr;

		enum object_type
		{
			square,
			cube,
			cube_mesh
		};

		float camera[3] = {};
		float axis[3][3] = {};

		struct draw_settings
		{
			bool enabled;
			bool native_rendering;
			bool camera_locked;
			float camera[3] = {};
			float range = 500.f;
			float color[4] = {1.f, 0.f, 0.f, 1.f};
			float mesh_thickness = 1.f;
			float size = 10.f;
			object_type type;
		};

		struct : draw_settings
		{
			float link_thickness = 1.f;
			bool draw_linked_nodes;
		} path_node_settings{};

		float vector_dot(float* a, float* b)
		{
			return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
		}

		bool world_pos_to_screen_pos(float* origin, float* out)
		{
			float local[3] =
			{
				origin[0] - camera[0],
				origin[1] - camera[1],
				origin[2] - camera[2]
			};

			float transform[3] =
			{
				vector_dot(local, axis[1]),
				vector_dot(local, axis[2]),
				vector_dot(local, axis[0])
			};

			if (transform[2] < 0.1f)
			{
				return false;
			}

			const auto width = game::ScrPlace_GetViewPlacement()->realViewportSize[0];
			const auto height = game::ScrPlace_GetViewPlacement()->realViewportSize[1];

			out[0] = (width / 2) * (1 - transform[0] / game::refdef->fovX / transform[2]);
			out[1] = (height / 2) * (1 - transform[1] / game::refdef->fovY / transform[2]);

			return true;
		}

		void draw_line(float* start, float* end, float* color, float thickness)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();

			float start_screen[2] = {};
			float end_screen[2] = {};

			auto result = 1;
			result *= world_pos_to_screen_pos(start, start_screen);
			result *= world_pos_to_screen_pos(end, end_screen);

			if (!result)
			{
				return;
			}

			const auto start_ = ImVec2(start_screen[0], start_screen[1]);
			const auto end_ = ImVec2(end_screen[0], end_screen[1]);
			const auto color_ = ImGui::GetColorU32({color[0], color[1], color[2], color[3]});

			window->DrawList->AddLine(start_, end_, color_, thickness);
		}

		void draw_square(float* origin, float width, float* color)
		{
			const auto half = width / 2.f;
			float p1[3] = {origin[0] - half, origin[1] + half, origin[2]};
			float p2[3] = {origin[0] + half, origin[1] + half, origin[2]};
			float p3[3] = {origin[0] + half, origin[1] - half, origin[2]};
			float p4[3] = {origin[0] - half, origin[1] - half, origin[2]};

			float p1_screen[2] = {};
			float p2_screen[2] = {};
			float p3_screen[2] = {};
			float p4_screen[2] = {};

			auto result = 1;
			result *= world_pos_to_screen_pos(p1, p1_screen);
			result *= world_pos_to_screen_pos(p2, p2_screen);
			result *= world_pos_to_screen_pos(p3, p3_screen);
			result *= world_pos_to_screen_pos(p4, p4_screen);

			if (!result)
			{
				return;
			}

			ImGuiWindow* window = ImGui::GetCurrentWindow();

			ImVec2 points[4] =
			{
				ImVec2(p1_screen[0], p1_screen[1]),
				ImVec2(p2_screen[0], p2_screen[1]),
				ImVec2(p3_screen[0], p3_screen[1]),
				ImVec2(p4_screen[0], p4_screen[1])
			};

			const auto color_ = ImGui::GetColorU32({color[0], color[1], color[2], color[3]});
			window->DrawList->AddConvexPolyFilled(points, 4, color_);
		}

		void draw_square_from_points(float* p1, float* p2, float* p3, float* p4, float* color)
		{
			float p1_screen[2] = {};
			float p2_screen[2] = {};
			float p3_screen[2] = {};
			float p4_screen[2] = {};

			auto result = 1;
			result *= world_pos_to_screen_pos(p1, p1_screen);
			result *= world_pos_to_screen_pos(p2, p2_screen);
			result *= world_pos_to_screen_pos(p3, p3_screen);
			result *= world_pos_to_screen_pos(p4, p4_screen);

			if (!result)
			{
				return;
			}

			ImGuiWindow* window = ImGui::GetCurrentWindow();

			ImVec2 points[4] =
			{
				ImVec2(p1_screen[0], p1_screen[1]),
				ImVec2(p2_screen[0], p2_screen[1]),
				ImVec2(p3_screen[0], p3_screen[1]),
				ImVec2(p4_screen[0], p4_screen[1])
			};

			const auto color_ = ImGui::GetColorU32({color[0], color[1], color[2], color[3]});
			window->DrawList->AddConvexPolyFilled(points, 4, color_);
		}

		void draw_cube(float* origin, float width, float* color)
		{
			const auto half = width / 2.f;

			float p1[3] = {origin[0] - half, origin[1] + half, origin[2]};
			float p2[3] = {origin[0] + half, origin[1] + half, origin[2]};
			float p3[3] = {origin[0] + half, origin[1] - half, origin[2]};
			float p4[3] = {origin[0] - half, origin[1] - half, origin[2]};

			float p1_top[3] = {p1[0], p1[1], origin[2] + width};
			float p2_top[3] = {p2[0], p2[1], origin[2] + width};
			float p3_top[3] = {p3[0], p3[1], origin[2] + width};
			float p4_top[3] = {p4[0], p4[1], origin[2] + width};

			draw_square_from_points(p1, p2, p3, p4, color);
			draw_square_from_points(p1_top, p2_top, p3_top, p4_top, color);

			draw_square_from_points(p3, p2, p2_top, p3_top, color);
			draw_square_from_points(p4, p1, p1_top, p4_top, color);

			draw_square_from_points(p1, p2, p2_top, p1_top, color);
			draw_square_from_points(p4, p3, p3_top, p4_top, color);
		}

		void draw_cube_mesh(float* origin, float width, float* color, float thickness)
		{
			const auto half = width / 2.f;

			float p1[3] = {origin[0] - half, origin[1] + half, origin[2]};
			float p2[3] = {origin[0] + half, origin[1] + half, origin[2]};
			float p3[3] = {origin[0] + half, origin[1] - half, origin[2]};
			float p4[3] = {origin[0] - half, origin[1] - half, origin[2]};

			float p1_top[3] = {p1[0], p1[1], origin[2] + width};
			float p2_top[3] = {p2[0], p2[1], origin[2] + width};
			float p3_top[3] = {p3[0], p3[1], origin[2] + width};
			float p4_top[3] = {p4[0], p4[1], origin[2] + width};

			draw_line(p1, p2, color, thickness);
			draw_line(p2, p3, color, thickness);
			draw_line(p3, p4, color, thickness);
			draw_line(p4, p1, color, thickness);

			draw_line(p1_top, p2_top, color, thickness);
			draw_line(p2_top, p3_top, color, thickness);
			draw_line(p3_top, p4_top, color, thickness);
			draw_line(p4_top, p1_top, color, thickness);

			draw_line(p1, p1_top, color, thickness);
			draw_line(p2, p2_top, color, thickness);
			draw_line(p3, p3_top, color, thickness);
			draw_line(p4, p4_top, color, thickness);
		}

		void draw_square_native(float* origin, float width, float* color)
		{
			const auto half = width / 2.f;

			float p1[3] = {origin[0] - half, origin[1] + half, origin[2]};
			float p2[3] = {origin[0] + half, origin[1] + half, origin[2]};
			float p3[3] = {origin[0] + half, origin[1] - half, origin[2]};
			float p4[3] = {origin[0] - half, origin[1] - half, origin[2]};

			float p1_screen[2] = {};
			float p2_screen[2] = {};
			float p3_screen[2] = {};
			float p4_screen[2] = {};

			auto result = 1;
			result *= world_pos_to_screen_pos(p1, p1_screen);
			result *= world_pos_to_screen_pos(p2, p2_screen);
			result *= world_pos_to_screen_pos(p3, p3_screen);
			result *= world_pos_to_screen_pos(p4, p4_screen);

			if (!result)
			{
				return;
			}

			game::rectangle rect;
			rect.p0.x = p1_screen[0];
			rect.p0.y = p1_screen[1];
			rect.p0.f2 = 0.f;
			rect.p0.f3 = 1.f;

			rect.p1.x = p2_screen[0];
			rect.p1.y = p2_screen[1];
			rect.p1.f2 = 0.f;
			rect.p1.f3 = 1.f;

			rect.p2.x = p3_screen[0];
			rect.p2.y = p3_screen[1];
			rect.p2.f2 = 0.f;
			rect.p2.f3 = 1.f;

			rect.p3.x = p4_screen[0];
			rect.p3.y = p4_screen[1];
			rect.p3.f2 = 0.f;
			rect.p3.f3 = 1.f;

			const auto material = game::Material_RegisterHandle("white");
			game::R_DrawRectangle(&rect, 0.f, 0.f, 1.f, 1.f, color, material);
		}

		void draw_window()
		{
			if (!gui::enabled_menus["debug"])
			{
				return;
			}

			ImGui::Begin("Debug", &gui::enabled_menus["debug"]);

			if (ImGui::TreeNode("Path nodes"))
			{
				ImGui::Checkbox("Draw", &path_node_settings.enabled);
				ImGui::Checkbox("Native rendering", &path_node_settings.native_rendering);
				ImGui::Checkbox("Lock camera", &path_node_settings.camera_locked);

				if (!path_node_settings.native_rendering)
				{
					ImGui::Checkbox("Draw linked nodes", &path_node_settings.draw_linked_nodes);

					if (ImGui::TreeNode("Object type"))
					{
						ImGui::RadioButton("square", reinterpret_cast<int*>(&path_node_settings.type), object_type::square);
						ImGui::RadioButton("cube", reinterpret_cast<int*>(&path_node_settings.type), object_type::cube);
						ImGui::RadioButton("cube mesh", reinterpret_cast<int*>(&path_node_settings.type), object_type::cube_mesh);

						ImGui::TreePop();
					}
				}

				ImGui::SliderFloat("range", &path_node_settings.range, 0.f, 10000.f);
				ImGui::SliderFloat("size", &path_node_settings.size, 5.f, 100.f);

				if (path_node_settings.draw_linked_nodes)
				{
					ImGui::SliderFloat("link thickness", &path_node_settings.link_thickness, 1.f, 20.f);
				}

				if (path_node_settings.type == object_type::cube_mesh)
				{
					ImGui::SliderFloat("mesh thickness", &path_node_settings.mesh_thickness, 1.f, 20.f);
				}

				if (ImGui::TreeNode("Color picker"))
				{
					ImGui::ColorPicker4("color", path_node_settings.color);
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			ImGui::End();
		}

		float distance_2d(float* a, float* b)
		{
			return sqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]));
		}

		void get_pathnode_origin(game::pathnode_t* node, float* out)
		{
			out[0] = node->vLocalOrigin[0];
			out[1] = node->vLocalOrigin[1];
			out[2] = node->vLocalOrigin[2];

			game::PathNode_WorldifyPosFromParent(node, out);
		}

		void draw_linked_nodes(game::pathnode_t* node, float* origin)
		{
			for (unsigned int i = 0; i < node->totalLinkCount; i++)
			{
				float linked_origin[3] = {};
				const auto num = node->Links[i].nodeNum;
				const auto linked = &game::pathData->nodes[num];

				get_pathnode_origin(linked, linked_origin);
				if (distance_2d(path_node_settings.camera, linked_origin) < path_node_settings.range)
				{
					draw_line(origin, linked_origin, path_node_settings.color, 
						path_node_settings.link_thickness);
				}
			}
		}

		void draw_node_object(float* origin)
		{
			switch (path_node_settings.type)
			{
			case object_type::square:
				draw_square(origin, path_node_settings.size, path_node_settings.color);
				break;
			case object_type::cube:
				draw_cube(origin, path_node_settings.size, path_node_settings.color);
				break;
			case object_type::cube_mesh:
				draw_cube_mesh(origin, path_node_settings.size, path_node_settings.color, 
					path_node_settings.mesh_thickness);
				break;
			}
		}

		void draw_nodes()
		{
			if (!game::SV_Loaded() || path_node_settings.native_rendering || 
				!path_node_settings.enabled || cl_paused->current.integer)
			{
				return;
			}

			const auto& io = ImGui::GetIO();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
			ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0f, 0.0f, 0.0f, 0.0f});
			ImGui::Begin("debug window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

			ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
			ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

			for (unsigned int i = 0; i < game::pathData->nodeCount; i++)
			{
				float origin[3] = {};
				const auto node = &game::pathData->nodes[i];

				get_pathnode_origin(node, origin);
				if (distance_2d(path_node_settings.camera, origin) >= path_node_settings.range)
				{
					continue;
				}

				draw_node_object(origin);
				if (path_node_settings.draw_linked_nodes)
				{
					draw_linked_nodes(node, origin);
				}
			}

			ImGuiWindow* window = ImGui::GetCurrentWindow();
			window->DrawList->PushClipRectFullScreen();

			ImGui::End();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar(2);
		}

		void update_camera()
		{
			camera[0] = game::refdef->org[0];
			camera[1] = game::refdef->org[1];
			camera[2] = game::refdef->org[2];

			axis[0][0] = game::refdef->axis[0][0];
			axis[0][1] = game::refdef->axis[0][1];
			axis[0][2] = game::refdef->axis[0][2];

			axis[1][0] = game::refdef->axis[1][0];
			axis[1][1] = game::refdef->axis[1][1];
			axis[1][2] = game::refdef->axis[1][2];

			axis[2][0] = game::refdef->axis[2][0];
			axis[2][1] = game::refdef->axis[2][1];
			axis[2][2] = game::refdef->axis[2][2];

			if (!path_node_settings.camera_locked)
			{
				path_node_settings.camera[0] = camera[0];
				path_node_settings.camera[1] = camera[1];
				path_node_settings.camera[2] = camera[2];
			}
		}

		void draw_nodes_native()
		{
			if (!game::SV_Loaded() || !path_node_settings.enabled || cl_paused->current.integer)
			{
				return;
			}

			update_camera();

			if (!path_node_settings.native_rendering)
			{
				return;
			}

			for (unsigned int i = 0; i < game::pathData->nodeCount; i++)
			{
				float origin[3] = {};
				const auto node = &game::pathData->nodes[i];

				get_pathnode_origin(node, origin);
				if (distance_2d(path_node_settings.camera, origin) < path_node_settings.range)
				{
					draw_square_native(origin, path_node_settings.size, path_node_settings.color);
				}
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			gui::on_frame(draw_window);
			gui::on_frame(draw_nodes, true);

			scheduler::on_game_initialized([]()
			{
				cl_paused = game::Dvar_FindVar("cl_paused");
			});

			scheduler::loop(draw_nodes_native, scheduler::pipeline::renderer);
		}
	};
}

REGISTER_COMPONENT(gui_debug::component)
