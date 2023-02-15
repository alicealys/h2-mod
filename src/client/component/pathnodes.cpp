#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "scheduler.hpp"
#include "command.hpp"
#include "console.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>
#include <utils/memory.hpp>
#include <utils/string.hpp>

namespace pathnodes
{
	namespace
	{
		game::dvar_t* g_connect_paths;

		utils::hook::detour path_init_paths_hook;

		utils::memory::allocator allocator;

		game::pathnode_tree_t* allocate_tree()
		{
			++game::pathData->nodeTreeCount;
			return reinterpret_cast<game::pathnode_tree_t*>(
				game::Hunk_AllocAlignInternal(sizeof(game::pathnode_tree_t), 4));
		}

		game::pathnode_tree_t* build_node_tree(unsigned short* node_indexes, unsigned int num_nodes)
		{
			if (num_nodes < 4)
			{
				const auto result = allocate_tree();
				result->axis = -1;
				result->u.s.nodeCount = num_nodes;
				result->u.s.nodes = node_indexes;
				return result;
			}

			game::vec2_t maxs{};
			game::vec2_t mins{};

			const auto start_node = &game::pathData->nodes[*node_indexes];
			maxs[0] = start_node->constant.vLocalOrigin[0];
			mins[0] = maxs[0];
			maxs[1] = start_node->constant.vLocalOrigin[1];
			mins[1] = maxs[1];

			for (auto i = 1u; i < num_nodes; i++)
			{
				for (auto axis = 0; axis < 2; axis++)
				{
					const auto node = &game::pathData->nodes[node_indexes[i]];
					const auto value = node->constant.vLocalOrigin[axis];
					if (mins[axis] <= value)
					{
						if (value > maxs[axis])
						{
							maxs[axis] = value;
						}
					}
					else
					{
						mins[axis] = value;
					}
				}

			}

			const auto axis = (maxs[1] - mins[1]) > (maxs[0] - mins[0]);
			if ((maxs[axis] - mins[axis]) > 192.f)
			{
				const auto dist = (maxs[axis] + mins[axis]) * 0.5f;
				auto left = 0u;

				for (auto right = num_nodes - 1; ; --right)
				{
					while (dist > game::pathData->nodes[node_indexes[left]].constant.vLocalOrigin[axis])
					{
						++left;
					}

					while (game::pathData->nodes[node_indexes[right]].constant.vLocalOrigin[axis] > dist)
					{
						--right;
					}

					if (left >= right)
					{
						break;
					}

					const auto swap_node = node_indexes[left];
					node_indexes[left] = node_indexes[right];
					node_indexes[right] = swap_node;
					++left;
				}

				while (2 * left < num_nodes &&
					game::pathData->nodes[node_indexes[left]].constant.vLocalOrigin[axis] == dist)
				{
					++left;
				}

				while (2 * left < num_nodes &&
					game::pathData->nodes[node_indexes[left - 1]].constant.vLocalOrigin[axis] == dist)
				{
					--left;
				}

				game::pathnode_tree_t* child[2]{};
				child[0] = build_node_tree(node_indexes, left);
				child[1] = build_node_tree(&node_indexes[left], num_nodes - left);
				const auto result = allocate_tree();
				result->axis = axis;
				result->dist = dist;
				result->u.child[0] = child[0];
				result->u.child[1] = child[1];
				return result;
			}
			else
			{
				const auto result = allocate_tree();
				result->axis = -1;
				result->u.s.nodeCount = num_nodes;
				result->u.s.nodes = node_indexes;
				return result;
			}
		}

		bool is_negotation_link(game::pathnode_t* from, game::pathnode_t* to)
		{
			return from->constant.type == game::NODE_NEGOTIATION_BEGIN &&
				to->constant.type == game::NODE_NEGOTIATION_END &&
				from->constant.target == to->constant.targetname;
		}

		float vec2_normalize(float* v)
		{
			const auto len = std::sqrt(v[0] * v[0] + v[1] * v[1]);

			v[0] /= len;
			v[1] /= len;

			return len;
		}

		float vec2_length(const float* v)
		{
			return std::sqrt(v[0] * v[0] + v[1] * v[1]);
		}

		float vec3_normalize(float* v)
		{
			const auto len = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

			v[0] /= len;
			v[1] /= len;
			v[2] /= len;

			return len;
		}

		void vector_scale(float* v, const int size, const float scale)
		{
			for (auto i = 0; i < size; i++)
			{
				v[i] *= scale;
			}
		}

		bool is_deflection_ok(const float* start, const float* origin, const float* move_dir)
		{
			game::vec2_t deflection{};
			deflection[0] = origin[0] - start[0];
			deflection[1] = origin[1] - start[1];
			const auto value = ((deflection[0] * move_dir[0]) + (deflection[1] * move_dir[1])) * -1.f;
			const auto d = (
				((value * move_dir[0]) + deflection[0]) * ((value * move_dir[0]) + deflection[0]) +
				((value * move_dir[1]) + deflection[1]) * ((value * move_dir[1]) + deflection[1])
				);
			return 0.3f > d;
		}

		void vector_copy(const float* a, float* b, const int size)
		{
			for (auto i = 0; i < size; i++)
			{
				b[i] = a[i];
			}
		}

		bool vector_cmp(const float* a, const float* b, const int size)
		{
			for (auto i = 0; i < size; i++)
			{
				if (a[i] != b[i])
				{
					return false;
				}
			}

			return true;
		}

		float distance_squared(const float* a, const float* b)
		{
			return ((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]));
		}

		void actor_physics(game::pmove_t* pm, game::pml_t* pml)
		{
			pml->previous_velocity[0] = pm->ps->velocity[0];
			pml->previous_velocity[1] = pm->ps->velocity[1];
			pml->previous_velocity[2] = pm->ps->velocity[2];

			pml->previous_origin[0] = pm->ps->origin[0];
			pml->previous_origin[1] = pm->ps->origin[1];
			pml->previous_origin[2] = pm->ps->origin[2];

			pml->groundTrace.hitId = 3998;

			game::PM_GroundTrace(pm, pml);
			if (pml->walking)
			{
				game::PM_WalkMove(pm, pml);
			}
			else
			{
				game::PM_AirMove(pm, pml);
			}
		}

		void vector_cross(const float* a, const float* b, float* out)
		{
			out[0] = (a[1] * b[2]) - (a[2] * b[1]);
			out[1] = (a[0] * b[2]) - (a[2] * b[0]);
			out[2] = (a[0] * b[1]) - (a[1] * b[0]);
		}

		bool phys_trace_passed(const float* from, const float* to, float* dist)
		{
			game::pml_t pml{};
			game::pmove_t pm{};

			pm.tracemask = 0x281C011;
			pm.bounds = *reinterpret_cast<game::Bounds*>(0x140984950);

			pm.ps = reinterpret_cast<game::playerState_s*>(&game::g_entities[0].client);
			pm.ps->origin[0] = from[0];
			pm.ps->origin[1] = from[1];
			pm.ps->origin[2] = from[2];
			pm.ps->gravity = 800;

			pm.ps->velocity[0] = 0.f;
			pm.ps->velocity[1] = 0.f;
			pm.ps->velocity[2] = 0.f;

			pml.previous_origin[0] = from[0];
			pml.previous_origin[1] = from[1];
			pml.previous_origin[2] = from[2];

			pml.msec = 50;
			pml.frametime = static_cast<float>(pml.msec) * 0.001f;

			game::vec3_t move_dir{};
			move_dir[0] = to[0] - from[0];
			move_dir[1] = to[1] - from[1];

			game::Vec2Normalize(move_dir);

			pml.forward[0] = move_dir[0];
			pml.forward[1] = move_dir[1];
			pml.forward[2] = move_dir[2];

			pml.up[0] = 0.f;
			pml.up[1] = 0.f;
			pml.up[2] = 1.f;

			vector_cross(pml.forward, pml.up, pml.right);

			pm.cmd.forwardmove = 127;
			pm.cmd.rightmove = 0;
			pm.cmd.unk_float = 1.f;

			auto dist_squared = 100000.f;
			auto last_ground_plane_altitude = -3.4028235e38f;

			for (auto i = 0; i < 96; i++)
			{
				dist_squared = (
					((to[0] - pm.ps->origin[0]) * (to[0] - pm.ps->origin[0])) +
					((to[1] - pm.ps->origin[1]) * (to[1] - pm.ps->origin[1]))
				);

				if (dist_squared <= 16.f)
				{
					break;
				}

				game::vec3_t start{};
				vector_copy(pm.ps->origin, start, 3);

				actor_physics(&pm, &pml);
				if (vector_cmp(start, pm.ps->origin, 3) ||
					distance_squared(pm.ps->origin, to) > 65536.f)
				{
					return false;
				}

				const auto has_ground_plane = pml.groundPlane && pml.groundTrace.normal[2] >= 0.3f;
				if (has_ground_plane)
				{
					last_ground_plane_altitude = pm.ps->origin[2];
				}

				if ((last_ground_plane_altitude - pm.ps->origin[2]) > 32.f)
				{
					return false;
				}

				if (pml.groundTrace.hitId != 3998 && !is_deflection_ok(from, pm.ps->origin, move_dir))
				{
					return false;
				}
			}

			if ((last_ground_plane_altitude - to[2]) > 32.f)
			{
				return false;
			}

			*dist = std::sqrtf(dist_squared);
			return dist_squared <= 16.f && std::abs(pm.ps->origin[2] - to[2]) <= 18.f;
		}

		bool can_link_nodes(game::pathnode_t* from, game::pathnode_t* to, float* dist, bool* negotiation_link)
		{
			if (is_negotation_link(from, to))
			{
				*negotiation_link = true;
				*dist = 15.f;
				return true;
			}
			else
			{
				game::vec3_t delta{};
				delta[0] = to->constant.vLocalOrigin[0] - from->constant.vLocalOrigin[0];
				delta[1] = to->constant.vLocalOrigin[1] - from->constant.vLocalOrigin[1];
				delta[2] = to->constant.vLocalOrigin[2] - from->constant.vLocalOrigin[2];

				if (std::abs(delta[2]) > 128.f)
				{
					return false;
				}

				if ((delta[0] * delta[0] + delta[1] * delta[1]) > 65536.f)
				{
					return false;
				}

				game::vec2_t move_dir{};
				move_dir[0] = to->constant.vLocalOrigin[0] - from->constant.vLocalOrigin[0];
				move_dir[1] = to->constant.vLocalOrigin[1] - from->constant.vLocalOrigin[1];

				*dist = game::Vec2Normalize(move_dir);
				*negotiation_link = false;

				return phys_trace_passed(from->constant.vLocalOrigin, to->constant.vLocalOrigin, dist);
			}
		}

		bool try_link_nodes(game::pathnode_t* from, game::pathnode_t* to,
			game::pathlink_s* links, int max_links)
		{
			float dist{};
			bool negotiation_link{};

			if (max_links <= 0)
			{
				console::error("[Connect paths] Out of available links, increase link buffer size\n");
				return false;
			}

			if (!can_link_nodes(from, to, &dist, &negotiation_link))
			{
				return false;
			}

			const auto link = &links[from->constant.totalLinkCount++];
			link->nodeNum = static_cast<unsigned short>(to - game::pathData->nodes);
			link->fDist = dist;
			link->disconnectCount = 0;
			link->negotiationLink = negotiation_link;
			return true;
		}

		void link_pathnodes()
		{
			constexpr auto max_links = 0x80000;
			const auto links_buffer = allocator.allocate_array<game::pathlink_s>(max_links);
			auto total_link_count = 0;

			for (auto i = 0u; i < game::pathData->nodeCount; i++)
			{
				const auto node = &game::pathData->nodes[i];
				if ((node->constant.spawnflags & 1) || !node->constant.type)
				{
					continue;
				}

				for (auto o = 0u; o < game::pathData->nodeCount; o++)
				{
					const auto other = &game::pathData->nodes[o];
					if (o == i || (other->constant.spawnflags & 1) || !other->constant.type)
					{
						continue;
					}

					try_link_nodes(node, other, &links_buffer[total_link_count], max_links - total_link_count);
				}

				total_link_count += node->constant.totalLinkCount;
				if (node->constant.totalLinkCount == 0)
				{
					console::info("[Connect paths] Pathnode at (%f %f %f) has no links\n",
						node->constant.vLocalOrigin[0],
						node->constant.vLocalOrigin[1],
						node->constant.vLocalOrigin[2]
					);
				}
			}

			console::info("[Connect paths] Total links: %i\n", total_link_count);

			auto accounted_links = 0;
			for (auto i = 0u; i < game::pathData->nodeCount; i++)
			{
				if (game::pathData->nodes[i].constant.totalLinkCount)
				{
					game::pathData->nodes[i].constant.Links = &links_buffer[accounted_links];
					accounted_links += game::pathData->nodes[i].constant.totalLinkCount;
				}
			}
		}

		float distance(float* a, float* b)
		{
			return std::sqrtf((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]));
		}

		void connect_paths()
		{
			console::info("[Connect paths] Node count: %i\n", game::pathData->nodeCount);

			auto original_link_count = 0;
			for (auto i = 0u; i < game::pathData->nodeCount; i++)
			{
				original_link_count += game::pathData->nodes[i].constant.totalLinkCount;
			}

			console::info("[Connect paths] Original link count: %i\n", original_link_count);

			for (auto i = 0u; i < game::pathData->nodeCount; i++)
			{
				if (game::pathData->nodes[i].constant.Links != nullptr)
				{
					console::warn("[Connect paths] Path nodes already linked\n");
					return;
				}
			}

			game::pathData->dynamicNodeGroupCount = 0;
			game::pathData->visBytes = 0;

			link_pathnodes();

			const auto node_indexes = allocator.allocate_array<unsigned short>(game::pathData->nodeCount);
			for (auto i = 0u; i < game::pathData->nodeCount; i++)
			{
				node_indexes[i] = static_cast<unsigned short>(i);
			}

			console::info("[Connect paths] Building pathnode trees...\n");
			game::pathData->nodeTreeCount = 0;
			game::pathData->nodeTree = build_node_tree(node_indexes, game::pathData->nodeCount);
			console::info("[Connect paths] Total trees: %i\n", game::pathData->nodeTreeCount);
		}

		float pm_cmd_scale_walk_stub(void*, void*, void*)
		{
			return 1.f;
		}

		char patch_bytes[2][5]{};

		void patch_functions()
		{
			std::memcpy(&patch_bytes[0], reinterpret_cast<void*>(0x1406887D0), 5);
			std::memcpy(&patch_bytes[1], reinterpret_cast<void*>(0x1403C8414), 5);

			utils::hook::jump(0x1406887D0, pm_cmd_scale_walk_stub);
			utils::hook::nop(0x1403C8414, 5);
		}

		void restore_code(const size_t ptr, char* data, const size_t size)
		{
			const auto ptr_ = reinterpret_cast<char*>(ptr);
			DWORD old_protect;
			VirtualProtect(ptr_, size, PAGE_EXECUTE_READWRITE, &old_protect);

			for (auto i = 0; i < size; i++)
			{
				ptr_[i] = data[i];
			}

			VirtualProtect(ptr_, size, old_protect, &old_protect);
			FlushInstructionCache(GetCurrentProcess(), ptr_, size);
		}

		void restore_functions()
		{
			restore_code(0x1406887D0, patch_bytes[0], 5);
			restore_code(0x1403C8414, patch_bytes[1], 5);
		}

		void path_init_paths_stub(void* a1, void* a2, void* a3, void* a4)
		{
			path_init_paths_hook.invoke<void>(a1, a2, a3, a4);

			if (g_connect_paths->current.enabled)
			{
				patch_functions();
				const auto _0 = gsl::finally(restore_functions);
				connect_paths();
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			g_connect_paths = dvars::register_bool("g_connectPaths", false, 0, "Connect paths");
			path_init_paths_hook.create(0x140522250, path_init_paths_stub);
		}
	};
}

REGISTER_COMPONENT(pathnodes::component)
