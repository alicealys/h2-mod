#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/dvars.hpp"

#include "fastfiles.hpp"
#include "filesystem.hpp"
#include "console.hpp"
#include "scheduler.hpp"
#include "mapents.hpp"
#include "command.hpp"
#include "game/scripting/functions.hpp"

#include <utils/hook.hpp>
#include <utils/concurrency.hpp>
#include <utils/string.hpp>
#include <utils/io.hpp>

namespace mapents
{
	namespace
	{
		game::dvar_t* addon_mapname = nullptr;
		utils::memory::allocator allocator;

		std::unordered_map<std::string, unsigned int> keys =
		{
			{"code_classname", 172},
			{"classname", 170},
			{"origin", 740},
			{"model", 669},
			{"spawnflags", 989},
			{"target", 1191},
			{"targetname", 1193},
			{"count", 216},
			{"rotation", 902},
			{"animscript", 71},
			{"script_linkname", 920},
			{"script_noteworthy", 922},
			{"angles", 65},
			{"parentname", 749},
			{"type", 1244},
			{"owner", 743},
			{"radius", 851},
			{"height", 488},
			{"customangles", 9555},
			{"speed", 997},
			{"script_vehicle_anim", 40318},

			{"_color", 1398},
			{"skycolor", 34255},
			{"suncolor", 1049},
			{"sundirection", 1050},
			{"modelscale", 23881},
			{"export", 13703},

			{"script_flag", 31190},
			{"script_flag_true", 31196},
			{"script_stealth_function", 31462},
			{"script_stealth", 31460},
			{"script_deathflag", 31114},
			{"script_forcespawn", 31214},
			{"script_stealthgroup", 31463},
			{"script_delay", 916},
			{"script_color_allies", 31096},
			{"script_patroller", 31391},
			{"script_idlereach", 31253},
			{"script_linkto", 31273},
			{"script_animation", 31039},
			{"script_startinghealth", 31454},
			{"script_pet", 9},
			{"script_goalheight", 31236},
			{"script_parameters", 31388},
			{"script_combatmode", 31102},
			{"script_ammo_clip", 31034},
			{"script_moveoverride", 31299},
			{"script_forcegoal", 31212},
			{"script_ammo_max", 31036},
		};

		std::unordered_map<unsigned int, game::scriptType_e> custom_fields;

		unsigned int token_id_start = 0x16000;

		// zonetool/iw4/addonmapents.cpp
		class asset_reader
		{
		public:
			asset_reader(char* data)
				: data_(data)
			{
			}

			template <typename T>
			T read()
			{
				const auto value = *reinterpret_cast<T*>(this->data_);
				this->data_ += sizeof(T);
				return value;
			}

			std::uint32_t read_int()
			{
				const auto type = this->read<char>();
				if (type != 0)
				{
					printf("asset_reader: invalid type %i for int\n", type);
					return 0;
				}

				return this->read<int>();
			}

			template <typename T>
			T* read_array()
			{
				const auto type = this->read<char>();
				if (type != 3)
				{
					printf("asset_reader: invalid type %i for array\n", type);
					return nullptr;
				}

				const auto size = this->read<int>();
				if (size <= 0)
				{
					printf("asset_reader: array size <= 0\n");
					return nullptr;
				}

				const auto array_ = allocator.allocate_array<T>(size);
				const auto total_size = sizeof(T) * size;
				std::memcpy(array_, this->data_, total_size);
				this->data_ += total_size;

				return array_;
			}

		private:
			char* data_ = nullptr;
		};

		std::string parse_mapents(const std::string& source)
		{
			std::string out_buffer{};
			const auto lines = utils::string::split(source, '\n');

			bool in_map_ent = false;
			bool empty = false;
			int line_index{};
			for (const auto& line : lines)
			{
				const auto _0 = gsl::finally([&]()
				{
					line_index++;
				});

				if (line[0] == '{' && !in_map_ent)
				{
					in_map_ent = true;
					out_buffer.append("{\n");
					continue;
				}

				if (line[0] == '{' && in_map_ent)
				{
					throw std::runtime_error(utils::string::va("[addon_map_ents parser] '{' on line %i", line_index));
				}

				if (line[0] == '}' && in_map_ent)
				{
					if (empty)
					{
						out_buffer.append("\n}\n");
					}
					else if (line_index < static_cast<int>(lines.size()) - 1)
					{
						out_buffer.append("}\n");
					}
					else
					{
						out_buffer.append("}\0");
					}

					in_map_ent = false;
					continue;
				}
				
				if (line[0] == '}' && !in_map_ent)
				{
					throw std::runtime_error(utils::string::va("[addon_map_ents parser] Unexpected '}' on line %i", line_index));
				}

				std::regex expr(R"~((.+) "(.*)")~");
				std::smatch match{};
				if (!std::regex_search(line, match, expr))
				{
					console::warn("[addon_map_ents parser] Failed to parse line %i", line_index);
					continue;
				}

				auto key = utils::string::to_lower(match[1].str());
				const auto value = match[2].str();

				if (key.size() <= 0 || value.size() <= 0)
				{
					console::warn("[addon_map_ents parser] Invalid key/value ('%s' '%s') pair on line %i", key.data(), value.data(), line_index);
					continue;
				}

				empty = false;

				auto key_id = std::atoi(key.data());
				if (key_id != 0)
				{
					out_buffer.append(utils::string::va("%i \"%s\"\n", key_id, value.data()));
					continue;
				}

				if (key.size() < 3 || (!key.starts_with("\"") || !key.ends_with("\"")))
				{
					console::warn("[addon_map_ents parser] Bad key '%s' on line %i", key.data(), line_index);
					continue;
				}

				const auto key_ = key.substr(1, key.size() - 2);
				if (keys.find(key_) == keys.end())
				{
					console::warn("[addon_map_ents parser] Key '%s' not found, on line %i", key_.data(), line_index);
					continue;
				}

				out_buffer.append(utils::string::va("%i \"%s\"\n", keys[key_], value.data()));
			}

			return out_buffer;
		}

		std::optional<std::string> get_mapents_data(std::string* real_path = nullptr)
		{
			std::string data{};
			if (addon_mapname->current.string != ""s && 
				filesystem::read_file("addon_map_ents/"s + addon_mapname->current.string + ".mapents"s, &data, real_path))
			{
				return {data};
			}

			static const auto mapname = game::Dvar_FindVar("mapname");

			if (filesystem::read_file("addon_map_ents/"s + mapname->current.string + ".mapents"s, &data, real_path))
			{
				return {data};
			}

			return {};
		}

		bool should_load_addon_mapents()
		{
			return get_mapents_data().has_value();
		}

		void try_parse_mapents(const std::string& path, const std::string& data, game::AddonMapEnts* mapents)
		{
			const auto parsed = parse_mapents(data);
			utils::io::write_file("parsed_mapents.txt", parsed, false);

			mapents->entityString = allocator.duplicate_string(parsed.data());
			mapents->numEntityChars = static_cast<int>(parsed.size()) + 1;

			std::string triggers{};
			if (utils::io::read_file(path + ".triggers", &triggers))
			{
				asset_reader reader(triggers.data());

				mapents->trigger.modelCount = reader.read_int();
				mapents->trigger.models = reader.read_array<game::TriggerModel>();

				mapents->trigger.hullCount = reader.read_int();
				mapents->trigger.hulls = reader.read_array<game::TriggerHull>();

				mapents->trigger.slabCount = reader.read_int();
				mapents->trigger.slabs = reader.read_array<game::TriggerSlab>();
			}
		}

		game::XAssetHeader db_find_xasset_header_stub(game::XAssetType type, const char* name, int allow_create_default)
		{
			const auto _0 = gsl::finally(&mapents::clear_dvars);

			const auto mapents = allocator.allocate<game::AddonMapEnts>();
			mapents->name = allocator.duplicate_string(name);
			mapents->entityString = allocator.duplicate_string("{\n}");
			mapents->numEntityChars = 4;

			std::string real_path{};
			const auto data = get_mapents_data(&real_path);
			if (!data.has_value())
			{
				return static_cast<game::XAssetHeader>(mapents);
			}

			try
			{
				try_parse_mapents(real_path, data.value(), mapents);
			}
			catch (const std::exception& e)
			{
				console::error("Failed to parse addon_map_ents file %s: %s\n", name, e.what());
			}

			return static_cast<game::XAssetHeader>(mapents);
		}

		void cm_trigger_model_bounds_stub(void* a1, void* a2)
		{
			__try
			{
				utils::hook::invoke<void>(0x14058BEB0, a1, a2);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				game::Com_Error(game::ERR_DROP, "CM_TriggerModelBounds: you are probably missing a mapents.triggers file");
			}
		}

		void add_field(const std::string& name, game::scriptType_e type)
		{
			const auto id = token_id_start++;
			custom_fields[id] = type;
			keys[name] = id;
			scripting::token_map[name] = id;
		}

		utils::hook::detour scr_find_field_hook;
		unsigned int scr_find_field_stub(unsigned int name, game::scriptType_e* type)
		{
			if (custom_fields.find(name) != custom_fields.end())
			{
				*type = custom_fields[name];
				return name;
			}

			return scr_find_field_hook.invoke<unsigned int>(name, type);
		}
	}

	void clear_dvars()
	{
		game::Dvar_SetString(addon_mapname, "");
	}

	void clear()
	{
		allocator.clear();
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			scr_find_field_hook.create(0x1405C5240, scr_find_field_stub);

			scheduler::once([]()
			{
				addon_mapname = dvars::register_string("addon_mapname", "", 0, "");
			}, scheduler::pipeline::main);

			command::add("dumpMapEnts", []()
			{
				if (!game::SV_Loaded())
				{
					console::info("Not in game\n");
					return;
				}

				static const auto mapname = game::Dvar_FindVar("mapname");
				const auto name = utils::string::va("maps/%s.d3dbsp", mapname->current.string);
				const auto mapents = game::DB_FindXAssetHeader(game::ASSET_TYPE_MAP_ENTS, 
					name, false).mapents;
				if (mapents == nullptr)
				{
					console::info("Failed to dump mapents\n");
					return;
				}

				const auto dest = utils::string::va("dumps/%s.ents", name);
				const auto data = std::string(mapents->entityString, mapents->numEntityChars);
				utils::io::write_file(dest, data, false);
				console::info("Mapents dumped to %s\n", dest);
			});

			utils::hook::call(0x14058BDD3, db_find_xasset_header_stub);
			utils::hook::call(0x14058BD6B, should_load_addon_mapents);
			utils::hook::call(0x1406B3384, cm_trigger_model_bounds_stub);

			add_field("script_specialops", game::SCRIPT_INTEGER);
		}
	};
}

REGISTER_COMPONENT(mapents::component)
