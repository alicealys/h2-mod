#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "console.hpp"

#include <utils/io.hpp>
#include <utils/hook.hpp>

namespace sound
{
	namespace
	{
		void com_sprintf_raw_sound_localized_stub(char* buffer, int size, const char* fmt,
			const char* lang, const char* name, const char* extension)
		{
			sprintf_s(buffer, size, "%s%s", name, extension);
		}

		void com_sprintf_raw_sound_stub(char* buffer, int size, const char* fmt, 
			const char* name, const char* extension)
		{
			sprintf_s(buffer, size, "%s%s", name, extension);
		}

		utils::hook::detour snd_is_music_playing_hook;
		bool snd_is_music_playing_stub(void* a1)
		{
			if (a1 == nullptr)
			{
				return true; // dont play music
			}

			return snd_is_music_playing_hook.invoke<bool>(a1);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// remove raw/sound or raw/language/sound prefix when loading raw sounds
			utils::hook::call(0x140622FEF, com_sprintf_raw_sound_localized_stub);
			utils::hook::call(0x14062306C, com_sprintf_raw_sound_stub);

			// fix playing non-existing music crashing
			snd_is_music_playing_hook.create(0x1407C58A0, snd_is_music_playing_stub);
		}
	};
}

REGISTER_COMPONENT(sound::component)
