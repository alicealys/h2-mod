#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "achievements.hpp"
#include "console.hpp"
#include "scheduler.hpp"
#include "command.hpp"

#include "game/ui_scripting/execution.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <utils/properties.hpp>

#define ACHIEVEMENT_FILE_VERSION 1
#define ACHIEVEMENT_FILE_SIGNATURE 'AM2H'

namespace achievements
{
	namespace
	{
		std::mutex file_mutex;

		std::array<achievement_t, ACHIEVEMENT_COUNT> achievements =
		{
			achievement_t(ACHIEVEMENT_ALL_ACHIEVEMENTS, "ACHIEVEMENTS_COMPLETED", ACHIEVEMENT_RARITY_3),
			achievement_t(ACHIEVEMENT_1, "BACK_IN_THE_SADDLE", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_2, "DANGER_CLOSE", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_3, "COLD_SHOULDER", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_4, "TAGEM_AND_BAGEM", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_5, "ROYAL_WITH_CHEESE", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_6, "SOAP_ON_A_ROPE", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_7, "DESPERATE_TIMES", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_8, "HOUSTON_WE_HAVE_A_PROBLEM", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_9, "THE_PAWN", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_10, "OUT_OF_THE_FRYING_PAN", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_11, "FOR_THE_RECORD", ACHIEVEMENT_RARITY_1),
			achievement_t(ACHIEVEMENT_12, "THE_PRICE_OF_WAR", ACHIEVEMENT_RARITY_2),
			achievement_t(ACHIEVEMENT_13, "FIRST_DAY_OF_SCHOOL", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_14, "BLACK_DIAMOND", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_15, "TURISTAS", ACHIEVEMENT_RARITY_1),
			achievement_t(ACHIEVEMENT_16, "RED_DAWN", ACHIEVEMENT_RARITY_1),
			achievement_t(ACHIEVEMENT_17, "PRISONER_627", ACHIEVEMENT_RARITY_1),
			achievement_t(ACHIEVEMENT_18, "ENDS_JUSTIFY_THE_MEANS", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_19, "HOME_COMING", ACHIEVEMENT_RARITY_1),
			achievement_t(ACHIEVEMENT_20, "QUEEN_TAKES_ROOK", ACHIEVEMENT_RARITY_1),
			achievement_t(ACHIEVEMENT_21, "OFF_THE_GRID", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_22, "PIT_BOSS", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_23, "GHOST", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_24, "COLONEL_SANDERSON", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_25, "TEN_PLUS_FOOT_MOBILES", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_26, "UNNECESSARY_ROUGHNESS", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_27, "KNOCK_KNOCK", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_28, "SOME_LIKE_IT_HOT", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_29, "TWO_BIRDS_WITH_ONE_STONE", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_30, "THE_ROAD_LESS_TRAVELED", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_31, "LEAVE_NO_STONE_UNTURNED", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_32, "DRIVE_BY", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_33, "THE_HARDER_THEY_FALL", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_34, "ONE_MAN_ARMY", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_35, "LOOK_MA_TWO_HANDS", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_36, "NO_REST_FOR_THE_WARY", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_37, "THREESOME", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_38, "TARGET_CONFIRMED", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_39, "ANGEL_SAVIOR", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_40, "NIGHT_MUSEUM", ACHIEVEMENT_RARITY_1),
			achievement_t(ACHIEVEMENT_41, "STUDENT_MASTER", ACHIEVEMENT_RARITY_1),
			achievement_t(ACHIEVEMENT_42, "REAL_GUN_GAME", ACHIEVEMENT_RARITY_1),
			achievement_t(ACHIEVEMENT_43, "PRECOGNITIVE_PARANOIA", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_44, "IMMORTAL", ACHIEVEMENT_RARITY_2),
			achievement_t(ACHIEVEMENT_45, "SILENT_SKIES", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_46, "CLAYMORE", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_47, "BIRD_HUNTER", ACHIEVEMENT_RARITY_1),
			achievement_t(ACHIEVEMENT_48, "HOT_POTATO", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_49, "CLOWN_IN_TRAINING", ACHIEVEMENT_RARITY_0),
			achievement_t(ACHIEVEMENT_50, "HEADBANGER", ACHIEVEMENT_RARITY_0),
		};

		std::filesystem::path get_achievements_path()
		{
			return utils::properties::get_appdata_path() / "player/achievements.bin";
		}



		void write_achievements(achievement_file_t* data)
		{
			std::lock_guard _0(file_mutex);

			data->version = ACHIEVEMENT_FILE_VERSION;
			data->signature = ACHIEVEMENT_FILE_SIGNATURE;

			const auto path = get_achievements_path();
			const auto str = std::string(reinterpret_cast<char*>(data), sizeof(achievement_file_t));
			utils::io::write_file(path.generic_string(), str, false);
		}

		bool has_achievement(achievement_file_t* file, int id)
		{
			if (id >= ACHIEVEMENT_COUNT)
			{
				return false;
			}

			return file->achievements[id];
		}

		std::optional<int> get_achievement_id(const std::string& name)
		{
			for (auto i = 0; i < ACHIEVEMENT_COUNT; i++)
			{
				const auto achievement = &achievements[i];
				if (name == achievement->code)
				{
					return {achievement->id};
				}
			}

			return {};
		}

		void notifiy_achievement_id(int id)
		{
			scheduler::once([=]
			{
				ui_scripting::notify("earned_achievement", {{"id", id}});
			}, scheduler::lui);
		}

		bool should_get_platinum(achievement_file_t* file)
		{
			if (has_achievement(file, ACHIEVEMENT_ALL_ACHIEVEMENTS))
			{
				return false;
			}

			for (auto i = static_cast<int>(ACHIEVEMENT_START); i < ACHIEVEMENT_COUNT; i++)
			{
				if (!file->achievements[i])
				{
					return false;
				}
			}

			return true;
		}

		void give_achievement_id_internal(achievement_file_t* file, int id)
		{
			achievements::get_achievements(file);
			if (has_achievement(file, id))
			{
				return;
			}

			file->achievements[id] = true;
			write_achievements(file);
			notifiy_achievement_id(id);
		}

		void give_achievement_id(achievement_file_t* file, int id)
		{
			give_achievement_id_internal(file, id);
			if (should_get_platinum(file))
			{
				give_achievement_id_internal(file, ACHIEVEMENT_ALL_ACHIEVEMENTS);
			}
		}

		void give_achievement(const std::string& name)
		{
			achievement_file_t file{};

			const auto id = get_achievement_id(name);
			if (!id.has_value())
			{
				return;
			}

			give_achievement_id(&file, id.value());
		}

		void scr_give_achievement_stub()
		{
			give_achievement(game::Scr_GetString(0));
		}
	}

	void get_achievements(achievement_file_t* file)
	{
		std::lock_guard _0(file_mutex);
		const auto path = get_achievements_path().generic_string();

		if (!utils::io::file_exists(path))
		{
			return;
		}

		const auto data = utils::io::read_file(path);
		if (data.size() < sizeof(achievement_file_t))
		{
			return;
		}

		std::memcpy(file, data.data(), sizeof(achievement_file_t));
		if (file->signature != ACHIEVEMENT_FILE_SIGNATURE)
		{
			std::memset(file, 0, sizeof(achievement_file_t));
		}
	}

	int get_count()
	{
		return ACHIEVEMENT_COUNT;
	}

	bool has_achievement(int id)
	{
		achievement_file_t file{};
		get_achievements(&file);
		return has_achievement(&file, id);
	}

	std::string get_name(int id)
	{
		return utils::string::va("ACHIEVEMENT_NAME_%i", id);
	}

	std::string get_details(int id)
	{
		return utils::string::va("ACHIEVEMENT_DETAIL_%i", id);
	}

	int get_rarity(int id)
	{
		if (id >= ACHIEVEMENT_COUNT)
		{
			return 0;
		}

		return achievements[id].rarity;
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::jump(0x1404B6240, scr_give_achievement_stub);

			command::add("reset_achievements", []
			{
				achievement_file_t file{};
				write_achievements(&file);
			});

			command::add("give_achievement", [](const command::params& params)
			{
				if (params.size() < 2)
				{
					return;
				}

				const auto id = std::atoi(params.get(1));
				achievement_file_t file{};
				get_achievements(&file);
				give_achievement_id_internal(&file, id);
			});
		}
	};
}

REGISTER_COMPONENT(achievements::component)
