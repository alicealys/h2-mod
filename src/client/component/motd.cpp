#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "motd.hpp"
#include "console.hpp"
#include "images.hpp"
#include "command.hpp"
#include "updater.hpp"
#include "config.hpp"

#include <utils/string.hpp>
#include <utils/http.hpp>
#include <utils/concurrency.hpp>
#include <utils/properties.hpp>
#include <utils/io.hpp>
#include <utils/cryptography.hpp>

namespace motd
{
	namespace
	{
		constexpr auto max_featured_tabs = 8;
		constexpr auto cache_max_age = 24h * 5; // 5 days
		constexpr auto cache_file_signature = 'CM2H'; // H2MC (H2-MOD Cache);

		struct wordle_data_t
		{
			bool valid;
			std::uint32_t id;
			std::string solution;
			std::unordered_set<std::string> word_list;
		};

		struct motd_data_t
		{
			links_map_t links;
			nlohmann::json marketing;
			wordle_data_t wordle;
		};

		utils::concurrency::container<motd_data_t, std::recursive_mutex> motd_data;

		struct cached_file_header
		{
			std::uint32_t signature;
			time_t date_created;
		};

		struct parsed_cache_file
		{
			cached_file_header header{};
			std::string data;
		};
		
		std::atomic_bool killed;

		std::filesystem::path get_cache_folder()
		{
			return utils::properties::get_appdata_path() / "cache";
		}

		std::string get_cached_file_name(const std::string& name)
		{
			const auto hash = utils::cryptography::sha1::compute(name, true);
			return (get_cache_folder() / hash).generic_string();
		}

		void cache_file(const std::string& name, const std::string& data)
		{
			std::string buffer;
			const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

			cached_file_header header{};
			header.signature = cache_file_signature;
			header.date_created = now;

			buffer.append(reinterpret_cast<char*>(&header), sizeof(header));
			buffer.append(data);

			const auto path = get_cached_file_name(name);
			utils::io::write_file(path, buffer, false);
		}

		std::optional<cached_file_header> parse_cached_file_header(std::string& data)
		{
			if (data.size() < sizeof(cached_file_header))
			{
				return {};
			}

			auto buffer = data.data();
			const auto header = reinterpret_cast<cached_file_header*>(buffer);
			return {*header};
		}

		std::optional<parsed_cache_file> parse_cached_file(const std::string& path)
		{
			parsed_cache_file parsed{};

			auto data = utils::io::read_file(path);
			const auto header = parse_cached_file_header(data);

			if (!header.has_value())
			{
				return {};
			}

			if (header->signature != cache_file_signature)
			{
				return {};
			}

			const auto file_data = data.data() + sizeof(cached_file_header);
			parsed.header = *header;
			parsed.data = std::string{file_data, data.size() - sizeof(cached_file_header)};

			return {parsed};
		}

		std::optional<std::string> read_cached_file(const std::string& name)
		{
			const auto path = get_cached_file_name(name);
			if (!utils::io::file_exists(path))
			{
				return {};
			}

			const auto parsed = parse_cached_file(path);
			if (!parsed.has_value())
			{
				return {};
			}

			return {parsed->data};
		}

		void delete_old_files()
		{
			const auto path = get_cache_folder().generic_string();
			if (!utils::io::directory_exists(path))
			{
				return;
			}

			const auto now = std::chrono::system_clock::now();
			const auto files = utils::io::list_files(path);

			for (const auto& file : files)
			{
				std::string data{};
				if (!utils::io::read_file(file, &data))
				{
					continue;
				}

				const auto header = parse_cached_file_header(data);
				const auto date = std::chrono::system_clock::from_time_t(header->date_created);
				if (now - date >= cache_max_age)
				{
					utils::io::remove_file(file);
				}
			}
		}

		std::optional<std::string> download_file(const std::string& url, bool from_backend = false)
		{
			if (killed)
			{
				return {};
			}

			const auto cached = read_cached_file(url);
			if (cached.has_value())
			{
				return {cached.value()};
			}

			if (!from_backend)
			{
				console::debug("[HTTP] GET File \"%s\"\n", url.data());
			}

			const auto res = from_backend ?
				updater::get_server_file(url) : 
				utils::http::get_data(url);
			if (res.has_value())
			{
				cache_file(url, res.value());
			}

			return res;
		}

		void download_motd_image(nlohmann::json& data)
		{
			if (!data["motd"].is_object() || !data["motd"]["image_url"].is_string())
			{
				return;
			}

			const auto url = data["motd"]["image_url"].get<std::string>();
			const auto image_data = download_file(url);
			if (image_data.has_value())
			{
				const auto& image = image_data.value();
				images::override_texture("motd_image", image);
			}
		}

		void download_featured_tabs_images(nlohmann::json& data)
		{
			if (!data["featured"].is_array())
			{
				return;
			}

			auto index = 0;
			for (const auto& [key, tab] : data["featured"].items())
			{
				index++;
				if (index >= max_featured_tabs + 1)
				{
					return;
				}

				if (!tab.is_object() || !tab["image_url"].is_string())
				{
					continue;
				}

				const auto download_image_ = [&](const std::string& field, const std::string& image_name)
				{
					const auto url = tab[field].get<std::string>();
					const auto image_data = download_file(url);
					if (image_data.has_value())
					{
						const auto& image = image_data.value();
						images::override_texture(std::format("{}_{}", image_name, index), image);
					}
				};

				download_image_("image_url", "featured_panel");
				download_image_("thumbnail_url", "featured_panel_thumbnail");
			}
		}

		void download_images(nlohmann::json& data)
		{
			if (!data.is_object())
			{
				return;
			}

			download_motd_image(data);
			download_featured_tabs_images(data);
		}

		void init_links(links_map_t& map)
		{
			map =
			{
				{"github", "https://github.com/alicealys/h2-mod"},
				{"donate", "https://github.com/alicealys/h2-mod"},
				{"specops", "https://github.com/alicealys/h2-specops-mod"},
				{"credits_1", "https://github.com/momo5502"},
				{"credits_2", "https://github.com/VladWinner"},
				{"credits_3", "https://github.com/diamante0018"},
				{"credits_4", "https://github.com/JariKCoding"},
				{"credits_5", "https://github.com/netadr"},
				{"credits_6", "https://github.com/Joelrau"},
				{"credits_7", "https://github.com/xensik"},
				{"credits_8", "https://github.com/ZoneTool/zonetool"},
			};
		}

		void add_links(motd_data_t& motd_data_)
		{
			init_links(motd_data_.links);
			if (!motd_data_.marketing.is_object() || !motd_data_.marketing["links"].is_object())
			{
				return;
			}

			for (const auto& [link, url] : motd_data_.marketing["links"].items())
			{
				if (!url.is_string())
				{
					continue;
				}

				motd_data_.links.insert(std::make_pair(link, url.get<std::string>()));
			}
		}

		std::string get_wordle_url()
		{
			const auto now = std::chrono::system_clock::now();
			const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};
			const auto y = ymd.year();
			const auto m = ymd.month();
			const auto d = ymd.day();

			return utils::string::va("https://www.nytimes.com/svc/wordle/v2/%04i-%02i-%02i.json", y, m, d);
		}

		bool get_wordle_list(motd_data_t& motd_data_)
		{
			const auto data = download_file("h2-mod/wordle_list.txt", true);
			if (!data.has_value())
			{
				console::debug("failed to get wordle list\n");
				return false;
			}

			const auto words = utils::string::split(data.value(), '\n');
			for (const auto& word : words)
			{
				motd_data_.wordle.word_list.insert(word);
			}

			return !motd_data_.wordle.word_list.empty();
		}

		bool get_current_wordle(motd_data_t& motd_data_)
		{
			const auto url = get_wordle_url();
			const auto data = utils::http::get_data(url);
			if (!data.has_value())
			{
				console::debug("failed to get current wordle %s\n", url.data());
				return false;
			}

			try
			{
				auto json = nlohmann::json::parse(data.value());
				motd_data_.wordle.id = json["id"].get<std::uint32_t>();
				motd_data_.wordle.solution = json["solution"].get<std::string>();
				return true;
			}
			catch (const std::exception& e)
			{
				console::error("failed to parse wordle data: %s\n", e.what());
				return false;
			}
		}
		
		void init_wordle(motd_data_t& motd_data_)
		{
			if (get_wordle_list(motd_data_) && get_current_wordle(motd_data_))
			{
				motd_data_.wordle.valid = true;
			}
		}

		void init(bool load_images = true)
		{
			delete_old_files();

			motd_data.access([&](motd_data_t& motd_data_)
			{
				init_links(motd_data_.links);
				init_wordle(motd_data_);
				motd_data_.marketing.clear();

				const auto marketing_data = updater::get_server_file("h2-mod/marketing.json");
				if (marketing_data.has_value())
				{
					try
					{
						const auto& value = marketing_data.value();
						motd_data_.marketing = nlohmann::json::parse(value);

						add_links(motd_data_);
						if (load_images)
						{
							download_images(motd_data_.marketing);
						}
					}
					catch (const std::exception& e)
					{
						console::error("Failed to load marketing.json: %s\n", e.what());
					}
				}
			});
		}
	}

	links_map_t get_links()
	{
		return motd_data.access<links_map_t>([&](motd_data_t& motd_data_)
		{
			return motd_data_.links;
		});
	}

	int get_num_featured_tabs()
	{
		return motd_data.access<nlohmann::json>([&](motd_data_t& motd_data_)
			-> nlohmann::json
		{
			if (!motd_data_.marketing.is_object() || !motd_data_.marketing["featured"].is_array())
			{
				return 0;
			}

			return std::min(max_featured_tabs, static_cast<int>(motd_data_.marketing["featured"].size()));
		});
	}

	nlohmann::json get_featured_tab(const int index)
	{
		return motd_data.access<nlohmann::json>([&](motd_data_t& motd_data_)
			-> nlohmann::json
		{
			if (!motd_data_.marketing.is_object() || !motd_data_.marketing["featured"].is_array())
			{
				return {};
			}

			if (index >= motd_data_.marketing["featured"].size())
			{
				return {};
			}

			return motd_data_.marketing["featured"][index];
		});
	}

	nlohmann::json get_motd()
	{
		return motd_data.access<nlohmann::json>([](motd_data_t& motd_data_)
			-> nlohmann::json
		{
			if (!motd_data_.marketing.is_object() || !motd_data_.marketing["motd"].is_object())
			{
				return {};
			}

			return motd_data_.marketing["motd"];
		});
	}

	bool has_motd()
	{
		return motd_data.access<bool>([](motd_data_t& motd_data_)
		{
			return motd_data_.marketing.is_object() && motd_data_.marketing["motd"].is_object();
		});
	}

	bool has_wordle()
	{
		return motd_data.access<bool>([](motd_data_t& motd_data_)
		{
			return motd_data_.wordle.valid;
		});
	}

	std::string get_wordle_solution()
	{
		return motd_data.access<std::string>([](motd_data_t& motd_data_)
		{
			return motd_data_.wordle.solution;
		});
	}

	std::uint32_t get_wordle_id()
	{
		return motd_data.access<std::uint32_t>([](motd_data_t& motd_data_)
		{
			return motd_data_.wordle.id;
		});
	}

	bool has_solved_wordle()
	{
		return motd_data.access<bool>([](motd_data_t& motd_data_)
		{
			const auto last_wordle = config::get<std::uint32_t>("motd_last_wordle");
			if (!last_wordle.has_value())
			{
				return false;
			}

			return last_wordle >= motd_data_.wordle.id;
		});
	}

	std::uint32_t get_wordle_score()
	{
		const auto score_opt = config::get<std::uint32_t>("motd_wordle_score");
		return score_opt.value_or(0u);
	}

	void solve_wordle(bool success)
	{
		motd_data.access([&](motd_data_t& motd_data_)
		{
			const auto last_wordle = config::get<std::uint32_t>("motd_last_wordle");
			if (last_wordle.has_value() && last_wordle.value() >= motd_data_.wordle.id)
			{
				return;
			}

			config::set<std::uint32_t>("motd_last_wordle", motd_data_.wordle.id);
			if (success)
			{
				config::set<std::uint32_t>("motd_wordle_score", get_wordle_score() + 1);
			}
		});
	}

	bool check_worlde_word(const std::string& word)
	{
		return motd_data.access<bool>([&](motd_data_t& motd_data_)
		{
			const auto lower = utils::string::to_lower(word);
			return motd_data_.wordle.word_list.contains(lower);
		});
	}

	std::thread init_thread;

	class component final : public component_interface
	{
	public:
		void post_start() override
		{
			init_thread = std::thread([]
			{
				init();
			});
		}

		void post_unpack() override
		{
			if (init_thread.joinable())
			{
				init_thread.join();
			}

			command::add("reloadmotd", []()
			{
				init(true);
			});

			command::add("reloadmotd_noimages", []()
			{
				init(false);
			});
		}

		void pre_destroy() override
		{
			killed = true;
			if (init_thread.joinable())
			{
				init_thread.join();
			}
		}
	};
}

REGISTER_COMPONENT(motd::component)
