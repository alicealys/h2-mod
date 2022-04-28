#pragma once

namespace filesystem
{
	std::unordered_set<std::string>& get_search_paths();
	std::string read_file(const std::string& path);
	bool read_file(const std::string& path, std::string* data, std::string* real_path = nullptr);
}