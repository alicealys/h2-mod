#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "motd.hpp"
#include "console.hpp"
#include "images.hpp"
#include "command.hpp"

#include <utils/string.hpp>
#include <utils/http.hpp>
#include <utils/concurrency.hpp>
#include <utils/properties.hpp>
#include <utils/io.hpp>
#include <utils/cryptography.hpp>

#define MAX_FEATURED_TABS 8

#define CACHE_MAX_AGE 24h * 5
#define CACHE_FILE_SIGNATURE 'CM2H' // H2MC (H2-MOD Cache)

namespace motd
{
	namespace
	{
		utils::concurrency::container<links_map_t> links;
		utils::concurrency::container<nlohmann::json, std::recursive_mutex> marketing;

		struct cached_file_header
		{
			std::uint32_t signature;
			time_t date_created;
		};

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
			auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

			cached_file_header header{};
			header.signature = CACHE_FILE_SIGNATURE;
			header.date_created = now;

			buffer.append(reinterpret_cast<char*>(&header), sizeof(header));
			buffer.append(data);

			const auto path = get_cached_file_name(name);
			utils::io::write_file(path, buffer, false);
		}

		std::optional<std::string> read_cached_file(const std::string& name)
		{
			const auto path = get_cached_file_name(name);
			if (!utils::io::file_exists(path))
			{
				return {};
			}

			auto now = std::chrono::system_clock::now();
			auto data = utils::io::read_file(path);
			if (data.size() < sizeof(cached_file_header))
			{
				return {};
			}

			auto buffer = data.data();
			const auto header = reinterpret_cast<cached_file_header*>(buffer);
			if (header->signature != CACHE_FILE_SIGNATURE)
			{
				return {};
			}

			const auto date = std::chrono::system_clock::from_time_t(header->date_created);
			if (now - date >= CACHE_MAX_AGE)
			{
				utils::io::remove_file(path);
				return {};
			}

			const auto file_data = buffer + sizeof(cached_file_header);
			return std::string{file_data, data.size() - sizeof(cached_file_header)};
		}

		std::optional<std::string> download_image(const std::string& url)
		{
			const auto cached = read_cached_file(url);
			if (cached.has_value())
			{
				return {cached.value()};
			}

			const auto res = utils::http::get_data(url);
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
			const auto image_data = download_image(url);
			if (image_data.has_value())
			{
				const auto& image = image_data.value();
				console::debug("Downloaded motd image\n");
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
				if (index >= MAX_FEATURED_TABS + 1)
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
					const auto image_data = download_image(url);
					if (image_data.has_value())
					{
						const auto& image = image_data.value();
						console::debug("Downloaded featured tab image %i\n", index);
						images::override_texture(image_name + std::format("_{}", index), image);
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
				{"github", "https://github.com/fedddddd/h2-mod"},
				{"donate", "https://www.paypal.com/donate/?hosted_button_id=LM5BA9UABEV4Q"},
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

		void add_links(nlohmann::json& data)
		{
			links.access([&](links_map_t& map)
			{
				init_links(map);
				if (!data.is_object() || !data["links"].is_object())
				{
					return;
				}

				for (const auto& [link, url] : data["links"].items())
				{
					if (!url.is_string())
					{
						continue;
					}

					map.insert(std::make_pair(link, url.get<std::string>()));
				}
			});
		}

		void init(bool load_images = true)
		{
			links.access([](links_map_t& map)
			{
				init_links(map);
			});

			marketing.access([&](nlohmann::json& data)
			{
				image_cache.clear();
				data.clear();

				const auto marketing_data = utils::http::get_data("https://master.fed0001.xyz/h2-mod/marketing.json");
				if (marketing_data.has_value())
				{
					try
					{
						const auto& value = marketing_data.value();
						data = nlohmann::json::parse(value);

						add_links(data);
						if (load_images)
						{
							download_images(data);
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
		return links.access<links_map_t>([&](links_map_t& map)
		{
			return map;
		});
	}

	int get_num_featured_tabs()
	{
		return marketing.access<nlohmann::json>([&](nlohmann::json& data)
			-> nlohmann::json
		{
			if (!data.is_object() || !data["featured"].is_array())
			{
				return 0;
			}

			return std::min(MAX_FEATURED_TABS, static_cast<int>(data["featured"].size()));
		});
	}

	nlohmann::json get_featured_tab(const int index)
	{
		return marketing.access<nlohmann::json>([&](nlohmann::json& data)
			-> nlohmann::json
		{
			if (!data.is_object() || !data["featured"].is_array())
			{
				return {};
			}

			if (index >= data["featured"].size())
			{
				return {};
			}

			return data["featured"][index];
		});
	}

	nlohmann::json get_motd()
	{
		return marketing.access<nlohmann::json>([](nlohmann::json& data)
			-> nlohmann::json
		{
			if (!data.is_object() || !data["motd"].is_object())
			{
				return {};
			}

			return data["motd"];
		});
	}

	bool has_motd()
	{
		return marketing.access<bool>([](nlohmann::json& data)
			-> nlohmann::json
		{
			return data.is_object() && data["motd"].is_object();
		});
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			init();

			command::add("reloadmotd", []()
			{
				init(true);
			});

			command::add("reloadmotd_noimages", []()
			{
				init(false);
			});
		}
	};
}

REGISTER_COMPONENT(motd::component)
