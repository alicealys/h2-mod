#pragma once

namespace motd
{
	int get_num_featured_tabs();
	nlohmann::json get_motd();
	nlohmann::json get_featured_tab(const int index);
}
