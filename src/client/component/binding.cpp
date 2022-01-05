#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace binding
{
	namespace
	{
		std::vector<std::string> custom_binds = {};

		utils::hook::detour cl_execute_key_hook;
		utils::hook::detour key_write_bindings_to_buffer_hook;

		int key_write_bindings_to_buffer_stub(int /*localClientNum*/, char* buffer, const int buffer_size)
		{
			auto bytes_used = 0;
			const auto buffer_size_align = static_cast<std::int32_t>(buffer_size) - 4;

			for (auto key_index = 0; key_index < 256; ++key_index)
			{
				const auto* const key_button = game::Key_KeynumToString(key_index, 0, 1);
				auto value = game::playerKeys->keys[key_index].binding;

				if (value && value < 100)
				{
					const auto len = sprintf_s(&buffer[bytes_used], (buffer_size_align - bytes_used),
					                           "bind %s \"%s\"\n", key_button, game::command_whitelist[value]);

					if (len < 0)
					{
						return bytes_used;
					}

					bytes_used += len;
				}
				else if (value >= 100)
				{
					value -= 100;
					if (static_cast<size_t>(value) < custom_binds.size() && !custom_binds[value].empty())
					{
						const auto len = sprintf_s(&buffer[bytes_used], (buffer_size_align - bytes_used),
						                           "bind %s \"%s\"\n", key_button, custom_binds[value].data());

						if (len < 0)
						{
							return bytes_used;
						}

						bytes_used += len;
					}
				}
			}

			buffer[bytes_used] = 0;
			return bytes_used;
		}

		int get_binding_for_custom_command(const char* command)
		{
			auto index = 0;
			for (auto& bind : custom_binds)
			{
				if (bind == command)
				{
					return index;
				}

				index++;
			}

			custom_binds.emplace_back(command);
			index = static_cast<unsigned int>(custom_binds.size()) - 1;

			return index;
		}

		int key_get_binding_for_cmd_stub(const char* command)
		{
			// original binds
			for (auto i = 0; i <= 100; i++)
			{
				if (game::command_whitelist[i] && !strcmp(command, game::command_whitelist[i]))
				{
					return i;
				}
			}

			// custom binds
			return 100 + get_binding_for_custom_command(command);
		}

		void cl_execute_key_stub(const int local_client_num, int key, const int down, const unsigned int time)
		{
			if (key >= 100)
			{
				key -= 100;

				if (static_cast<size_t>(key) < custom_binds.size() && !custom_binds[key].empty())
				{
					game::Cbuf_AddText(local_client_num, utils::string::va("%s\n", custom_binds[key].data()));
				}

				return;
			}

			cl_execute_key_hook.invoke<void>(local_client_num, key, down, time);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// write all bindings to config file
			key_write_bindings_to_buffer_hook.create(0x3D3840_b, key_write_bindings_to_buffer_stub);

			// links a custom command to an index
			utils::hook::jump(0x59AE30_b, key_get_binding_for_cmd_stub, true);

			// execute custom binds
			cl_execute_key_hook.create(0x3CF1E0_b, &cl_execute_key_stub);
		}
	};
}

REGISTER_COMPONENT(binding::component)
