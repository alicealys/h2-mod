#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

#define ORIGINAL_BIND_COUNT 110

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

				if (value && value < ORIGINAL_BIND_COUNT)
				{
					const auto len = sprintf_s(&buffer[bytes_used], (buffer_size_align - bytes_used),
					                           "bind %s \"%s\"\n", key_button, game::command_whitelist[value]);

					if (len < 0)
					{
						return bytes_used;
					}

					bytes_used += len;
				}
				else if (value >= ORIGINAL_BIND_COUNT)
				{
					value -= ORIGINAL_BIND_COUNT;
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
			for (auto i = 0; i <= ORIGINAL_BIND_COUNT; i++)
			{
				if (game::command_whitelist[i] && !strcmp(command, game::command_whitelist[i]))
				{
					return i;
				}
			}

			// custom binds
			return ORIGINAL_BIND_COUNT + get_binding_for_custom_command(command);
		}
		
		std::optional<std::string> get_custom_binding_for_key(int key)
		{
			key -= ORIGINAL_BIND_COUNT;

			if (static_cast<size_t>(key) < custom_binds.size() && !custom_binds[key].empty())
			{
				return {custom_binds[key]};
			}

			return {};
		}

		void cl_execute_key_stub(const int local_client_num, int key, const int down, const unsigned int time)
		{
			if (key >= ORIGINAL_BIND_COUNT)
			{
				const auto bind = get_custom_binding_for_key(key);
				if (!bind.has_value())
				{
					return;
				}

				return game::Cbuf_AddText(local_client_num, utils::string::va("%s\n", bind.value().data()));
			}

			cl_execute_key_hook.invoke<void>(local_client_num, key, down, time);
		}

		const char* cmd_get_binding_for_key_stub(unsigned int key)
		{
			if (key >= ORIGINAL_BIND_COUNT)
			{
				const auto bind = get_custom_binding_for_key(key);
				if (!bind.has_value())
				{
					return "";
				}
				return utils::string::va("%s", bind.value().data());
			}

			return game::command_whitelist[key];
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// write all bindings to config file
			key_write_bindings_to_buffer_hook.create(0x1403D3840, key_write_bindings_to_buffer_stub);

			// links a custom command to an index
			utils::hook::jump(0x14059AE30, key_get_binding_for_cmd_stub, true);

			// execute custom binds
			cl_execute_key_hook.create(0x1403CF1E0, &cl_execute_key_stub);

			utils::hook::jump(0x14059AE90, cmd_get_binding_for_key_stub);
		}
	};
}

REGISTER_COMPONENT(binding::component)
