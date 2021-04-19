#include "stdinc.hpp"

namespace game
{
	uint64_t base_address;

	void load_base_address()
	{
		const auto module = GetModuleHandle(NULL);
		base_address = uint64_t(module);
	}
}