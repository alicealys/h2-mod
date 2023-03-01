#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "motd.hpp"
#include "console.hpp"
#include "images.hpp"
#include "command.hpp"

#include <utils/string.hpp>
#include <utils/http.hpp>
#include <utils/concurrency.hpp>

#define MAX_FEATURED_TABS 8

namespace motd
{
	namespace
	{
		utils::concurrency::container<nlohmann::json, std::recursive_mutex> marketing;

		std::unordered_map<std::string, std::string> image_cache;

		std::optional<std::string> download_image(const std::string& url)
		{
			if (image_cache.contains(url))
			{
				return {image_cache.at(url)};
			}

			const auto res = utils::http::get_data(url);
			if (res.has_value())
			{
				image_cache[url] = res.value();
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

		void download_images()
		{
			marketing.access([&](nlohmann::json& data)
			{
				if (!data.is_object())
				{
					return;
				}

				download_motd_image(data);
				download_featured_tabs_images(data);
			});
		}

		void init(bool load_images = true)
		{
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
						if (load_images)
						{
							download_images();
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
