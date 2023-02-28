#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "filesystem.hpp"
#include "console.hpp"
#include "mods.hpp"
#include "language.hpp"

#include "game/game.hpp"

#include <utils/io.hpp>
#include <utils/hook.hpp>
#include <utils/flags.hpp>
#include <utils/properties.hpp>

namespace filesystem
{
	namespace
	{
		bool initialized = false;

		std::deque<std::filesystem::path>& get_search_paths_internal()
		{
			static std::deque<std::filesystem::path> search_paths{};
			return search_paths;
		}

		void fs_startup_stub(const char* name)
		{
			console::info("[FS] Startup\n");

			initialized = true;

			filesystem::register_path(utils::properties::get_appdata_path() / CLIENT_DATA_FOLDER);
			filesystem::register_path(L".");
			filesystem::register_path(L"h2-mod");

			const auto mod_path = utils::flags::get_flag("mod");
			if (mod_path.has_value())
			{
				mods::set_mod(mod_path.value());
			}

			utils::hook::invoke<void>(0x14060BF50, name);
		}

		std::vector<std::filesystem::path> get_paths(const std::filesystem::path& path)
		{
			std::vector<std::filesystem::path> paths{};

			const auto code = game::SEH_GetCurrentLanguageName();

			paths.push_back(path);
			paths.push_back(path / code);

			return paths;
		}

		bool can_insert_path(const std::filesystem::path& path)
		{
			const auto& paths = get_search_paths_internal();
			return std::ranges::none_of(paths.cbegin(), paths.cend(), [path](const auto& elem)
			{
				return elem == path;
			});
		}

		const char* sys_default_install_path_stub()
		{
			static auto current_path = std::filesystem::current_path().string();
			return current_path.data();
		}

		bool is_parent_path(const std::filesystem::path& parent, const std::filesystem::path& child)
		{
			std::filesystem::path iter = child;

			while (iter != iter.parent_path())
			{
				if (iter == parent)
				{
					return true;
				}

				iter = iter.parent_path();
			}

			return false;
		}
	}

	std::string read_file(const std::string& path)
	{
		for (const auto& search_path : get_search_paths_internal())
		{
			const auto path_ = search_path / path;
			if (utils::io::file_exists(path_.generic_string()))
			{
				return utils::io::read_file(path_.generic_string());
			}
		}

		return {};
	}

	bool read_file(const std::string& path, std::string* data, std::string* real_path)
	{
		for (const auto& search_path : get_search_paths_internal())
		{
			const auto path_ = search_path / path;
			if (utils::io::read_file(path_.generic_string(), data))
			{
				if (real_path != nullptr)
				{
					*real_path = path_.generic_string();
				}

				return true;
			}
		}

		return false;
	}

	bool find_file(const std::string& path, std::string* real_path)
	{
		for (const auto& search_path : get_search_paths_internal())
		{
			const auto path_ = search_path / path;
			if (utils::io::file_exists(path_.generic_string()))
			{
				*real_path = path_.generic_string();
				return true;
			}
		}

		return false;
	}

	bool exists(const std::string& path)
	{
		for (const auto& search_path : get_search_paths_internal())
		{
			const auto path_ = search_path / path;
			if (utils::io::file_exists(path_.generic_string()))
			{
				return true;
			}
		}

		return false;
	}

	void register_path(const std::filesystem::path& path)
	{
		if (!initialized)
		{
			return;
		}

		const auto paths = get_paths(path);
		for (const auto& path_ : paths)
		{
			if (can_insert_path(path_))
			{
				console::info("[FS] Registering path '%s'\n", path_.generic_string().data());
				get_search_paths_internal().push_front(path_);
			}
		}
	}

	void unregister_path(const std::filesystem::path& path)
	{
		if (!initialized)
		{
			return;
		}

		const auto paths = get_paths(path);
		for (const auto& path_ : paths)
		{
			auto& search_paths = get_search_paths_internal();
			for (auto i = search_paths.begin(); i != search_paths.end();)
			{
				if (*i == path_)
				{
					console::info("[FS] Unregistering path '%s'\n", path_.generic_string().data());
					i = search_paths.erase(i);
				}
				else
				{
					++i;
				}
			}
		}
	}

	std::vector<std::string> get_search_paths()
	{
		std::vector<std::string> paths{};

		for (const auto& path : get_search_paths_internal())
		{
			paths.push_back(path.generic_string());
		}

		return paths;
	}

	std::vector<std::string> get_search_paths_rev()
	{
		std::vector<std::string> paths{};
		const auto& search_paths = get_search_paths_internal();

		for (auto i = search_paths.rbegin(); i != search_paths.rend(); ++i)
		{
			paths.push_back(i->generic_string());
		}

		return paths;
	}

	void check_path(const std::filesystem::path& path)
	{
		if (path.generic_string().find("..") != std::string::npos)
		{
			throw std::runtime_error("directory traversal is not allowed");
		}
	}

	std::string get_safe_path(const std::filesystem::path& path)
	{
		check_path(path);
		const auto absolute = std::filesystem::weakly_canonical(path);

		static std::vector<std::filesystem::path> allowed_directories =
		{
			{std::filesystem::weakly_canonical("mods")},
			{std::filesystem::weakly_canonical("h2-mod")},
			{std::filesystem::weakly_canonical("players2/default")},
		};

		auto is_allowed = false;
		for (const auto& dir : allowed_directories)
		{
			if (is_parent_path(dir, absolute))
			{
				is_allowed = true;
				break;
			}
		}

		if (!is_allowed)
		{
			throw std::runtime_error(std::format("Disallowed access to directory \"{}\"", path.generic_string()));
		}

		return path.generic_string();
	}

	bool safe_write_file(const std::string& file, const std::string& data, bool append)
	{
		const auto path = filesystem::get_safe_path(file);
		return utils::io::write_file(path, data, append);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::call(0x14060B052, fs_startup_stub);
			utils::hook::jump(0x140624050, sys_default_install_path_stub);
		}
	};
}

REGISTER_COMPONENT(filesystem::component)