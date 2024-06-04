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
		game::dvar_t* snd_music_volume = nullptr;
		game::dvar_t* snd_music_volume_boost = nullptr;
		game::dvar_t** snd_music_disabled_for_custom_sndtrack = nullptr;
		int music_volmod_index = -1;

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

		bool is_sound_music(game::snd_alias_t* sound)
		{
			return sound->dspBusIndex == *game::music_dsp_bus_index ||
				sound->volModIndex == music_volmod_index;
		}

		float get_snd_volume(game::snd_alias_t* sound, float original_volume)
		{
			if (is_sound_music(sound) &&
				snd_music_disabled_for_custom_sndtrack &&
				(*snd_music_disabled_for_custom_sndtrack)->current.enabled)
			{
				return 0.f;
			}

			if (is_sound_music(sound))
			{
				return original_volume * snd_music_volume->current.value * snd_music_volume_boost->current.value;
			}

			return original_volume;
		}

		void snd_update_channel_stub(utils::hook::assembler& a)
		{
			a.pushad64();
			a.mov(rcx, qword_ptr(rdi, 0xD0));
			a.call_aligned(get_snd_volume);
			a.movaps(xmm1, xmm0);
			a.popad64();

			a.jmp(0x1407CD8C7);
		}

		double atof_stub(const char* str)
		{
			if (!std::strncmp(game::sound_data->volmods[*game::volmod_index].name, "music", sizeof(game::volmod_t::name)))
			{
				music_volmod_index = *game::volmod_index;
			}

			return std::atof(str);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			snd_music_volume = dvars::register_float("snd_musicVolume", 1.f, 0.0f, 1.f, game::DVAR_FLAG_SAVED, "Music volume scale");
			snd_music_volume_boost = dvars::register_float("snd_musicVolumeBoost", 1.f, 0.0f, 5.f, game::DVAR_FLAG_SAVED, "Music volume boost");
			snd_music_disabled_for_custom_sndtrack = reinterpret_cast<game::dvar_t**>(0x151B818C8);

			// remove raw/sound or raw/language/sound prefix when loading raw sounds
			utils::hook::call(0x140622FEF, com_sprintf_raw_sound_localized_stub);
			utils::hook::call(0x14062306C, com_sprintf_raw_sound_stub);

			// fix playing non-existing music crashing
			snd_is_music_playing_hook.create(0x1407C58A0, snd_is_music_playing_stub);

			utils::hook::jump(0x1407CD8A5, utils::hook::assemble(snd_update_channel_stub), true);
			utils::hook::call(0x1407C5F2F, atof_stub);
		}
	};
}

REGISTER_COMPONENT(sound::component)
