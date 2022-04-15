#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/dvars.hpp"
#include "game/game.hpp"

#include <utils/flags.hpp>
#include <utils/hook.hpp>

namespace database
{
	namespace
	{
		game::dvar_t* db_filesysImpl = nullptr;

		utils::hook::detour sub_140272EC0_hook;

		game::DB_FileSysInterface* sub_140272EC0_stub()
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
			static const char* values[] = {
				"BnetTACTVFSManager", // (load files from CASC)
				"DiskFS", // (load files from disk)
				nullptr
			};

			int default_value = utils::flags::has_flag("disk") ? 1 : 0;
			db_filesysImpl = dvars::register_enum("db_filesysImpl", values, default_value, game::DVAR_FLAG_READ);

			if (default_value == 1)
			{
				utils::hook::nop(0x1405A4868, 22); // TACT related stuff that's pointless if we're using DiskFS
			}
			
			sub_140272EC0_hook.create(0x140272EC0, sub_140272EC0_stub);
		}
	};
}

REGISTER_COMPONENT(database::component)
