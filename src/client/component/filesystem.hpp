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

	template <typename R>
	std::function<R(const std::string& str)>
		safe_io_func(const std::function<R(const std::string& str)>& func)
	{
		return [func](const std::string& path)
		{
			const auto safe_path = filesystem::get_safe_path(path);
			return func(safe_path);
		};
	}
}
