#pragma once

#include "structs.hpp"
#include <string>

namespace dvars
{
	struct dvar_info
	{
		dvar_info(const std::string& name, const std::string& description);

		std::string name;
		std::string description;
		int hash;
	};

	extern game::dvar_t* con_inputBoxColor;
	extern game::dvar_t* con_inputHintBoxColor;
	extern game::dvar_t* con_outputBarColor;
	extern game::dvar_t* con_outputSliderColor;
	extern game::dvar_t* con_outputWindowColor;
	extern game::dvar_t* con_inputDvarMatchColor;
	extern game::dvar_t* con_inputDvarValueColor;
	extern game::dvar_t* con_inputDvarInactiveValueColor;
	extern game::dvar_t* con_inputCmdMatchColor;

	extern game::dvar_t* jump_enableFallDamage;
	extern game::dvar_t* jump_ladderPushVel;
	extern game::dvar_t* jump_spreadAdd;

	extern game::dvar_t* pm_bouncing;

	extern game::dvar_t* r_fontFallbackMod;

	extern game::dvar_t* r_fullbright;

	extern game::dvar_t* cg_legacyCrashHandling;

	extern game::dvar_t* g_enableElevators;

	extern std::vector<dvar_info> dvar_list;

	std::string dvar_get_vector_domain(const int components, const game::dvar_limits& domain);
	std::string dvar_get_domain(const game::dvar_type type, const game::dvar_limits& domain);
	std::string dvar_get_description(const std::string& name);
	std::optional<dvar_info> get_dvar_info_from_hash(const int hash);

	game::dvar_t* register_int(const std::string& name, int value, int min, int max,
		unsigned int flags, const std::string& description);
	game::dvar_t* register_bool(const std::string& name, bool value,
		unsigned int flags, const std::string& description);
	game::dvar_t* register_string(const std::string& name, const char* value,
		unsigned int flags, const std::string& description);
	game::dvar_t* register_float(const std::string& name, float value, float min, float max,
		unsigned int flags, const std::string& description);
	game::dvar_t* register_vec4(const std::string& name, float x, float y, float z, float w, float min,
		float max, unsigned int flags, const std::string& description);
	game::dvar_t* register_enum(const std::string& name, const char** value_list, int default_index,
		unsigned int flags, const std::string& description);
}
