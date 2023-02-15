#pragma once

#include "updater.hpp"

namespace filesystem
{
	std::string read_file(const std::string& path);
	bool read_file(const std::string& path, std::string* data, std::string* real_path = nullptr);
	bool find_file(const std::string& path, std::string* real_path);
	bool exists(const std::string& path);

	void register_path(const std::filesystem::path& path);
	void unregister_path(const std::filesystem::path& path);

	std::vector<std::string> get_search_paths();
	std::vector<std::string> get_search_paths_rev();

	std::string get_safe_path(const std::filesystem::path& path);
	bool safe_write_file(const std::string& file, const std::string& data, bool append = false);
}
