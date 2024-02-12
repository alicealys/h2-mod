#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "images.hpp"
#include "console.hpp"
#include "filesystem.hpp"

#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/image.hpp>
#include <utils/io.hpp>
#include <utils/concurrency.hpp>
#include <utils/http.hpp>

namespace images
{
	namespace
	{
		utils::hook::detour load_texture_hook;
		utils::hook::detour setup_texture_hook;
		utils::concurrency::container<std::unordered_map<std::string, std::string>> overriden_textures;

		std::optional<std::string> load_image(game::GfxImage* image)
		{
			std::string data{};
			overriden_textures.access([&](const std::unordered_map<std::string, std::string>& textures)
			{
				if (const auto i = textures.find(image->name); i != textures.end())
				{
					data = i->second;
				}
			});

			if (data.empty() && !filesystem::read_file(utils::string::va("images/%s.png", image->name), &data))
			{
				return {};
			}

			return {std::move(data)};
		}

		std::optional<utils::image> load_raw_image_from_file(game::GfxImage* image)
		{
			const auto image_file = load_image(image);
			if (!image_file)
			{
				return {};
			}

			return utils::image(*image_file);
		}

		bool load_custom_texture(game::GfxImage* image)
		{
			auto raw_image = load_raw_image_from_file(image);
			if (!raw_image)
			{
				return false;
			}

			image->mapType = game::MAPTYPE_2D;
			image->semantic = 2;
			image->category = 3;
			image->flags = 0;

			D3D11_SUBRESOURCE_DATA data{};
			data.SysMemPitch = raw_image->get_width() * 4;
			data.SysMemSlicePitch = data.SysMemPitch * raw_image->get_height();
			data.pSysMem = raw_image->get_buffer();

			game::Image_Setup(image, raw_image->get_width(), raw_image->get_height(), image->depth, image->numElements,
				image->mapType, DXGI_FORMAT_R8G8B8A8_UNORM, 0, image->name, &data);

			return true;
		}

		void load_texture_stub(void* a1, game::GfxImage* image)
		{
			try
			{
				if (load_custom_texture(image))
				{
					return;
				}
			}
			catch (std::exception& e)
			{
				console::error("Failed to load image %s: %s\n", image->name, e.what());
			}

			load_texture_hook.invoke<void>(a1, image);
		}

		int setup_texture_stub(game::GfxImage* image, void* a2, void* a3)
		{
			if (image->resourceSize == -1)
			{
				return 0;
			}

			return setup_texture_hook.invoke<bool>(image, a2, a3);
		}
	}

	void override_texture(std::string name, std::string data)
	{
		overriden_textures.access([&](std::unordered_map<std::string, std::string>& textures)
		{
			textures[std::move(name)] = std::move(data);
		});
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			setup_texture_hook.create(0x1402A7940, setup_texture_stub);
			load_texture_hook.create(0x14074A390, load_texture_stub);
		}
	};
}

REGISTER_COMPONENT(images::component)
