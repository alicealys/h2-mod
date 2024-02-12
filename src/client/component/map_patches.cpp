#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include <utils/hook.hpp>

namespace map_patches
{
	struct GfxLightGridTree
	{
		unsigned char index;
		unsigned char maxDepth;
		char unused[2];
		int nodeCount;
		int leafCount;
		int coordMinGridSpace[3];
		int coordMaxGridSpace[3];
		int coordHalfSizeGridSpace[3];
		int defaultColorIndexBitCount;
		int defaultLightIndexBitCount;
		unsigned int* p_nodeTable;
		int leafTableSize;
		unsigned char* p_leafTable;
	};

	enum leaf_table_version : std::int8_t
	{
		h2 = 0,
		h1 = 0,
		s1 = 0,
		iw6 = 1,
	};

	utils::hook::detour r_decode_light_grid_block_hook;
	void r_decode_light_grid_block_stub(GfxLightGridTree* p_tree, int child_mask, 
		char child_index, int encoded_node_address, char* p_node_raw, char* p_leaf_raw)
	{
		constexpr auto p_address = 0x140766F7F;
		if (p_tree->unused[0] == leaf_table_version::iw6)
		{
			utils::hook::set<uint8_t>(p_address, 6); // iw6
		}
		else
		{
			utils::hook::set<uint8_t>(p_address, 7); // s1,h1,h2
		}

		r_decode_light_grid_block_hook.invoke<void>(p_tree, child_mask, 
			child_index, encoded_node_address, p_node_raw, p_leaf_raw);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// patch iw6 leafTable decoding
			r_decode_light_grid_block_hook.create(0x140765420, r_decode_light_grid_block_stub);
		}
	};
}

REGISTER_COMPONENT(map_patches::component)
