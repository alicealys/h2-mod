#pragma once

namespace achievements
{
	enum achievement_rarity
	{
		ACHIEVEMENT_RARITY_0,
		ACHIEVEMENT_RARITY_1,
		ACHIEVEMENT_RARITY_2,
		ACHIEVEMENT_RARITY_3,
	};

	enum achievement_id
	{
		ACHIEVEMENT_ALL_ACHIEVEMENTS = 0,
		ACHIEVEMENT_START = 1,
		ACHIEVEMENT_1 = 1,
		ACHIEVEMENT_2,
		ACHIEVEMENT_3,
		ACHIEVEMENT_4,
		ACHIEVEMENT_5,
		ACHIEVEMENT_6,
		ACHIEVEMENT_7,
		ACHIEVEMENT_8,
		ACHIEVEMENT_9,
		ACHIEVEMENT_10,
		ACHIEVEMENT_11,
		ACHIEVEMENT_12,
		ACHIEVEMENT_13,
		ACHIEVEMENT_14,
		ACHIEVEMENT_15,
		ACHIEVEMENT_16,
		ACHIEVEMENT_17,
		ACHIEVEMENT_18,
		ACHIEVEMENT_19,
		ACHIEVEMENT_20,
		ACHIEVEMENT_21,
		ACHIEVEMENT_22,
		ACHIEVEMENT_23,
		ACHIEVEMENT_24,
		ACHIEVEMENT_25,
		ACHIEVEMENT_26,
		ACHIEVEMENT_27,
		ACHIEVEMENT_28,
		ACHIEVEMENT_29,
		ACHIEVEMENT_30,
		ACHIEVEMENT_31,
		ACHIEVEMENT_32,
		ACHIEVEMENT_33,
		ACHIEVEMENT_34,
		ACHIEVEMENT_35,
		ACHIEVEMENT_36,
		ACHIEVEMENT_37,
		ACHIEVEMENT_38,
		ACHIEVEMENT_39,
		ACHIEVEMENT_40,
		ACHIEVEMENT_41,
		ACHIEVEMENT_42,
		ACHIEVEMENT_43,
		ACHIEVEMENT_44,
		ACHIEVEMENT_45,
		ACHIEVEMENT_46,
		ACHIEVEMENT_47,
		ACHIEVEMENT_48,
		ACHIEVEMENT_49,
		ACHIEVEMENT_50,
		ACHIEVEMENT_ORIGINAL_COUNT = 51,
		ACHIEVEMENT_51 = 51,
		ACHIEVEMENT_52,
		ACHIEVEMENT_TOTAL_COUNT,
	};

	struct achievement_t
	{
		achievement_id id;
		std::string_view code;
		achievement_rarity rarity;
		bool secret{};
	};

	struct achievement_file_t
	{
		std::uint32_t signature;
		std::uint8_t version;
		bool achievements[ACHIEVEMENT_TOTAL_COUNT];
	};

	void get_achievements(achievement_file_t* file);

	int get_count();
	bool has_achievement(int id);

	std::string get_name(int id);
	std::string get_details(int id);
	std::optional<std::string> get_background(int id);
	int get_rarity(int id);
	bool is_secret(int id);
}
