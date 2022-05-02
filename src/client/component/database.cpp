#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/dvars.hpp"
#include "game/game.hpp"

#include <utils/io.hpp>
#include <utils/hook.hpp>

namespace database
{
	namespace
	{
		game::dvar_t* db_filesysImpl = nullptr;
		utils::hook::detour db_fs_initialize_hook;

		game::DB_FileSysInterface* db_fs_initialize_stub()
		{
			switch (db_filesysImpl->current.integer)
			{
			case 0:
				return reinterpret_cast<game::DB_FileSysInterface*>(0x140BE82F8); // ptr to vtable of BnetTACTVFSManager (implements DB_FileSysInterface)
			case 1:
				return reinterpret_cast<game::DB_FileSysInterface*>(0x140BEFDC0); // ptr to vtable of DiskFS (implements DB_FileSysInterface)
			default:
				return nullptr; // this should not happen
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			static const char* values[] = 
			{
				"BnetTACTVFSManager", // (load files from CASC)
				"DiskFS", // (load files from disk)
				nullptr
			};

			const auto default_value = static_cast<int>(!utils::io::directory_exists("Data/data")
				|| !utils::io::directory_exists("Data/config")
				|| !utils::io::directory_exists("Data/indices"));

			db_filesysImpl = dvars::register_enum("db_filesysImpl", values, default_value, game::DVAR_FLAG_READ);

			if (default_value == 1)
			{
				utils::hook::nop(0x1405A4868, 22); // TACT related stuff that's pointless if we're using DiskFS
				utils::hook::nop(0x14071AF83, 45); // Skip setting Bink file OS callbacks (not necessary since we're loading from disk)
			}
			
			db_fs_initialize_hook.create(game::DB_FSInitialize, db_fs_initialize_stub);
		}
	};
}

REGISTER_COMPONENT(database::component)
