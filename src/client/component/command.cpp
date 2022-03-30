#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "game/scripting/execution.hpp"

#include "command.hpp"
#include "scheduler.hpp"
#include "game_console.hpp"
#include "fastfiles.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/memory.hpp>
#include <utils/io.hpp>

namespace command
{
	namespace
	{
		utils::hook::detour dvar_command_hook;

		std::unordered_map<std::string, std::function<void(params&)>> handlers;

		void main_handler()
		{
			params params = {};

			const auto command = utils::string::to_lower(params[0]);
			if (handlers.find(command) != handlers.end())
			{
				handlers[command](params);
			}
		}

		game::dvar_t* dvar_command_stub()
		{
			const params args;

			if (args.size() <= 0)
			{
				return 0;
			}

			const auto dvar = game::Dvar_FindVar(args[0]);

			if (dvar)
			{
				if (args.size() == 1)
				{
					const auto current = game::Dvar_ValueToString(dvar, nullptr, &dvar->current);
					const auto reset = game::Dvar_ValueToString(dvar, nullptr, &dvar->reset);

					game_console::print(game_console::con_type_info, "\"%s\" is: \"%s\" default: \"%s\" hash: 0x%08lX",
						args[0], current, reset, dvar->name);

					game_console::print(game_console::con_type_info, "   %s\n",
						dvars::dvar_get_domain(dvar->type, dvar->domain).data());
				}
				else
				{
					char command[0x1000] = { 0 };
					game::Dvar_GetCombinedString(command, 1);
					game::Dvar_SetCommand(dvar->name, "", command);
				}

				return dvar;
			}

			return 0;
		}
	}

	params::params()
		: nesting_(game::cmd_args->nesting)
	{
	}

	int params::size() const
	{
		return game::cmd_args->argc[this->nesting_];
	}

	const char* params::get(const int index) const
	{
		if (index >= this->size())
		{
			return "";
		}

		return game::cmd_args->argv[this->nesting_][index];
	}

	std::string params::join(const int index) const
	{
		std::string result = {};

		for (auto i = index; i < this->size(); i++)
		{
			if (i > index) result.append(" ");
			result.append(this->get(i));
		}
		return result;
	}

	void add_raw(const char* name, void (*callback)())
	{
		game::Cmd_AddCommandInternal(name, callback, utils::memory::get_allocator()->allocate<game::cmd_function_s>());
	}

	void add(const char* name, const std::function<void(const params&)>& callback)
	{
		const auto command = utils::string::to_lower(name);

		if (handlers.find(command) == handlers.end())
		{
			add_raw(name, main_handler);
		}

		handlers[command] = callback;
	}

	void add(const char* name, const std::function<void()>& callback)
	{
		add(name, [callback](const params&)
		{
			callback();
		});
	}

	void execute(std::string command, const bool sync)
	{
		command += "\n";

		if (sync)
		{
			game::Cmd_ExecuteSingleCommand(0, 0, command.data());
		}
		else
		{
			game::Cbuf_AddText(0, command.data());
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::jump(0x1405A74F0, dvar_command_stub, true);

			add("quit", game::Quit);

			add("startmap", [](const params& params)
			{
				const auto map = params.get(1);

				const auto exists = utils::hook::invoke<bool>(0x140412B50, map, 0);

				if (!exists)
				{
					game_console::print(game_console::con_type_error, "map '%s' not found\n", map);
					return;
				}

				// SV_SpawnServer
				utils::hook::invoke<void>(0x1406B3AA0, map, 0, 0, 0, 0);
			});

			add("listassetpool", [](const params& params)
			{
				if (params.size() < 2)
				{
					game_console::print(game_console::con_type_info, "listassetpool <poolnumber>: list all the assets in the specified pool\n");

					for (auto i = 0; i < game::XAssetType::ASSET_TYPE_COUNT; i++)
					{
						game_console::print(game_console::con_type_info, "%d %s\n", i, game::g_assetNames[i]);
					}
				}
				else
				{
					const auto type = static_cast<game::XAssetType>(atoi(params.get(1)));

					if (type < 0 || type >= game::XAssetType::ASSET_TYPE_COUNT)
					{
						game_console::print(game_console::con_type_info, "Invalid pool passed must be between [%d, %d]\n", 0, game::XAssetType::ASSET_TYPE_COUNT - 1);
						return;
					}

					game_console::print(game_console::con_type_info, "Listing assets in pool %s\n", game::g_assetNames[type]);

					fastfiles::enum_assets(type, [type](const game::XAssetHeader header)
					{
						const auto asset = game::XAsset{type, header};
						const auto* const asset_name = game::DB_GetXAssetName(&asset);
						//const auto entry = game::DB_FindXAssetEntry(type, asset_name);
						//TODO: display which zone the asset is from
						game_console::print(game_console::con_type_info, "%s\n", asset_name);
					}, true);
				}
			});

			add("commandDump", []()
			{
				printf("======== Start command dump =========\n");

				game::cmd_function_s* cmd = (*game::cmd_functions);

				while (cmd)
				{
					if (cmd->name)
					{
						game_console::print(game_console::con_type_info, "%s\n", cmd->name);
					}

					cmd = cmd->next;
				}

				printf("======== End command dump =========\n");
			});

			add("god", []()
			{
				if (!game::SV_Loaded())
				{
					return;
				}

				game::g_entities[0].flags ^= game::FL_GODMODE;
				game::CG_GameMessage(0, utils::string::va("godmode %s",
					game::g_entities[0].flags & game::FL_GODMODE
					? "^2on"
					: "^1off"));
			});

			add("demigod", []()
			{
				if (!game::SV_Loaded())
				{
					return;
				}

				game::g_entities[0].flags ^= game::FL_DEMI_GODMODE;
				game::CG_GameMessage(0, utils::string::va("demigod mode %s",
					game::g_entities[0].flags & game::FL_DEMI_GODMODE
					? "^2on"
					: "^1off"));
			});

			add("notarget", []()
			{
				if (!game::SV_Loaded())
				{
					return;
				}

				game::g_entities[0].flags ^= game::FL_NOTARGET;
				game::CG_GameMessage(0, utils::string::va("notarget %s",
					game::g_entities[0].flags & game::FL_NOTARGET
					? "^2on"
					: "^1off"));
			});

			add("noclip", []()
			{
				if (!game::SV_Loaded())
				{
					return;
				}

				game::g_entities[0].client->flags ^= 1;
				game::CG_GameMessage(0, utils::string::va("noclip %s",
					game::g_entities[0].client->flags & 1
					? "^2on"
					: "^1off"));
			});

			add("ufo", []()
			{
				if (!game::SV_Loaded())
				{
					return;
				}

				game::g_entities[0].client->flags ^= 2;
				game::CG_GameMessage(
					0, utils::string::va("ufo %s", game::g_entities[0].client->flags & 2 ? "^2on" : "^1off"));
			});

			add("give", [](const params& params)
			{
				if (!game::SV_Loaded())
				{
					return;
				}

				if (params.size() < 2)
				{
					game::CG_GameMessage(0, "You did not specify a weapon name");
					return;
				}

				try
				{
					const auto arg = params.get(1);
					const scripting::entity player = scripting::call("getentbynum", {0}).as<scripting::entity>();
					auto ps = game::g_entities[0].client;

					if (arg == "ammo"s)
					{
						const auto weapon = player.call("getcurrentweapon").as<std::string>();
						player.call("givemaxammo", {weapon});
					}
					else if (arg == "allammo"s)
					{
						const auto weapons = player.call("getweaponslist").as<scripting::array>();
						for (auto i = 0; i < weapons.size(); i++)
						{
							player.call("givemaxammo", {weapons[i]});
						}
					}
					else if (arg == "health"s)
					{
						if (params.size() > 2)
						{
							const auto amount = atoi(params.get(2));
							const auto health = player.get("health").as<int>();
							player.set("health", {health + amount});
						}
						else
						{
							const auto amount = game::Dvar_FindVar("g_player_maxhealth")->current.integer;
							player.set("health", {amount});
						}
					}
					else if (arg == "all"s)
					{
						const auto type = game::XAssetType::ASSET_TYPE_WEAPON;
						fastfiles::enum_assets(type, [&player, type](const game::XAssetHeader header)
						{
							const auto asset = game::XAsset{type, header};
							const auto* const asset_name = game::DB_GetXAssetName(&asset);

							player.call("giveweapon", {asset_name});
						}, true);
					}
					else
					{
						const auto wp = game::G_GetWeaponForName(arg);
						if (wp)
						{
							if (game::G_GivePlayerWeapon(ps, wp, 0, 0, 0, 0))
							{
								game::G_InitializeAmmo(ps, wp, 0);
								game::G_SelectWeapon(0, wp);
							}
						}
						else
						{
							game::CG_GameMessage(0, "Weapon does not exist");
						}
					}
				}
				catch (...)
				{
				}
			});

			add("dropweapon", [](const params& params)
			{
				if (!game::SV_Loaded())
				{
					return;
				}

				try
				{
					const scripting::entity player = scripting::call("getentbynum", {0}).as<scripting::entity>();
					const auto weapon = player.call("getcurrentweapon");
					player.call("dropitem", {weapon});
				}
				catch (...)
				{
				}
			});

			add("take", [](const params& params)
			{
				if (!game::SV_Loaded())
				{
					return;
				}

				if (params.size() < 2)
				{
					game::CG_GameMessage(0, "You did not specify a weapon name");
					return;
				}

				const auto weapon = params.get(1);

				try
				{
					const auto player = scripting::call("getentbynum", {0}).as<scripting::entity>();
					if (weapon == "all"s)
					{
						player.call("takeallweapons");
					}
					else
					{
						player.call("takeweapon", {weapon});
					}
				}
				catch (...)
				{
				}
			});

			add("kill", [](const params& params)
			{
				if (!game::SV_Loaded())
				{
					return;
				}

				scheduler::once([]()
				{
					try
					{
						const scripting::entity player = scripting::call("getentbynum", {0}).as<scripting::entity>();
						player.call("kill");
					}
					catch (...)
					{
					}
				}, scheduler::pipeline::server);
			});
		}
	};
}

REGISTER_COMPONENT(command::component)
