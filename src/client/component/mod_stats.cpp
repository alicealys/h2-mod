#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "console.hpp"
#include "filesystem.hpp"
#include "mods.hpp"
#include "mod_stats.hpp"
#include "command.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>
#include <utils/concurrency.hpp>
#include <utils/thread.hpp>
#include <utils/properties.hpp>

namespace mod_stats
{
	namespace
	{
		struct
		{
			utils::concurrency::container<mod_stats_t> current_stats;
			std::atomic_bool modified_stats;
			std::atomic_bool kill_thread;
			std::thread stats_write_thread;
		} globals{};

		std::optional<std::string> get_mod_basename()
		{
			const auto mod = mods::get_mod();
			if (!mod.has_value())
			{
				return {};
			}

			const auto& value = mod.value();
			const auto last_index = value.find_last_of('/');
			const auto basename = value.substr(last_index + 1);
			return {basename};
		}

		std::optional<std::string> get_stats_path()
		{
			const auto current_mod = get_mod_basename();
			if (!current_mod.has_value())
			{
				return {};
			}

			const auto path = utils::properties::get_appdata_path() / 
				"player/modstats" / (current_mod.value() + ".json");
			return {path.generic_string()};
		}

		nlohmann::json default_mod_stats()
		{
			return nlohmann::json::object();
		}

		nlohmann::json verify_mod_stats(const nlohmann::json& json)
		{
			if (!json.is_object())
			{
				return default_mod_stats();
			}

			return json;
		}

		nlohmann::json parse_mod_stats()
		{
			const auto stats_file = get_stats_path();
			if (!stats_file.has_value())
			{
				return default_mod_stats();
			}

			const auto& stats_file_value = stats_file.value();
			if (!utils::io::file_exists(stats_file_value))
			{
				return default_mod_stats();
			}

			const auto data = utils::io::read_file(stats_file_value);
			try
			{
				auto json = nlohmann::json::parse(data);
				return verify_mod_stats(json);
			}
			catch (const std::exception& e)
			{
				console::error("Failed to parse json mod stats file \"%s\": %s", 
					stats_file_value.data(), e.what());
			}

			return default_mod_stats();
		}
	}

	void initialize()
	{
		globals.modified_stats = false;
		globals.current_stats.access([](mod_stats_t& stats)
		{
			stats = parse_mod_stats();
		});
	}

	utils::concurrency::container<mod_stats_t>& get_stats()
	{
		return globals.current_stats;
	}

	void set_modified()
	{
		globals.modified_stats = true;
	}

	void write()
	{
		const auto path = get_stats_path();
		if (!path.has_value())
		{
			return;
		}

		console::debug("[Mod stats] writing stats\n");
		globals.current_stats.access([&](mod_stats_t& stats)
		{
			const auto verified_stats = verify_mod_stats(stats);
			const auto dump = verified_stats.dump(4);
			const auto& path_value = path.value();
			utils::io::write_file(path_value, dump, false);
			globals.modified_stats = false;
		});
	}

	nlohmann::json get(const std::string& key, const nlohmann::json& default_value)
	{
		return get_stats().access<nlohmann::json>([&](mod_stats_t& stats)
			-> nlohmann::json
		{	
			if (!stats.is_object() || stats[key].is_null())
			{
				return default_value;
			}

			return stats[key];
		});
	}

	nlohmann::json get_all()
	{
		return get_stats().access<nlohmann::json>([&](mod_stats_t& stats)
			-> nlohmann::json
		{
			return stats;
		});
	}

	nlohmann::json get_struct(const std::string& name, const std::string& field, const nlohmann::json& default_value)
	{
		return get_stats().access<nlohmann::json>([&](mod_stats_t& stats)
			-> nlohmann::json
		{
			if (!stats.is_object() || !stats[name].is_object() || stats[name][field].is_null())
			{
				return default_value;
			}

			return stats[name][field];
		});
	}

	void set(const std::string& key, const nlohmann::json& value)
	{
		get_stats().access([&](mod_stats_t& stats)
		{
			stats[key] = value;
			set_modified();
		});
	}

	void set_struct(const std::string& name, const std::string& field, const nlohmann::json& value)
	{
		get_stats().access([&](mod_stats_t& stats)
		{
			stats[name][field] = value;
			set_modified();
		});
	}

	void set_all(const nlohmann::json& value)
	{
		get_stats().access([&](mod_stats_t& stats)
		{
			stats = value;
			set_modified();
		});
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			globals.stats_write_thread = utils::thread::create_named_thread("Stats Write (H2-Mod)", []()
			{
				while (!globals.kill_thread)
				{
					const auto _0 = gsl::finally([]
					{
						std::this_thread::sleep_for(50ms);
					});

					if (!globals.modified_stats)
					{
						continue;
					}

					write();
				}
			});

			command::add("stats_initialize", initialize);
		}

		void pre_destroy() override
		{
			globals.kill_thread = true;
			if (globals.stats_write_thread.joinable())
			{
				globals.stats_write_thread.join();
			}
		}
	};
}

REGISTER_COMPONENT(mod_stats::component)
