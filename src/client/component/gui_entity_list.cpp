#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "command.hpp"
#include "gui.hpp"
#include "scripting.hpp"

#include "game/scripting/execution.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>
#include <utils/concurrency.hpp>

namespace entity_list
{
	namespace
	{
		enum entity_type
		{
			type_any,
			actor,
			spawner,
			weapon,
			node,
			count,
		};

		enum entity_team
		{
			team_any,
			neutral,
			allies,
			axis,
			team3
		};

		std::unordered_map<entity_team, std::string> team_names =
		{
			{entity_team::neutral, "neutral"},
			{entity_team::allies, "allies"},
			{entity_team::axis, "axis"},
			{entity_team::team3, "team3"},
		};

		struct entity_info_t
		{
			unsigned int id;
			unsigned int num;
			std::unordered_map<std::string, std::string> fields;
		};

		struct filters_t
		{
			bool filter_by_range;
			float range;
			entity_team team;
			entity_type type;
			std::vector<std::pair<std::string, std::string>> fields;
		};

		struct data_t
		{
			bool auto_update;
			bool force_update;
			filters_t filters;
			std::chrono::milliseconds interval;
			std::chrono::high_resolution_clock::time_point last_call;
			std::vector<entity_info_t> entity_info;
			std::vector<std::function<void()>> tasks;
			std::unordered_map<std::string, bool> selected_fields = 
			{
				{"code_classname", false},
				{"classname", true},
				{"origin", true},
				{"model", false},
				{"spawnflags", false},
				{"target", false},
				{"targetname", false},
				{"count", false},
				{"health", false},
				{"dmg", false},
				{"angles", true},
				{"script_linkname", false},
				{"script_noteworthy", false},
				{"maxhealth", false},
				{"anglelerprate", false},
				{"activator", false},
				{"slidevelocity", false},
				{"disableplayeradsloscheck", false},
				{"type", false},
				{"accuracy", false},
				{"lookforward", false},
				{"lookright", false},
				{"lookup", false},
				{"fovcosine", false},
				{"fovcosinebusy", false},
				{"fovcosinez", false},
				{"upaimlimit", false},
				{"downaimlimit", false},
				{"rightaimlimit", false},
				{"leftaimlimit", false},
				{"maxsightdistsqrd", false},
				{"sightlatency", false},
				{"defaultsightlatency", false},
				{"ignoreclosefoliage", false},
				{"interval", false},
				{"teammovewaittime", false},
				{"damagetaken", false},
				{"damagedir", false},
				{"damageyaw", false},
				{"damagelocation", false},
				{"damageweapon", false},
				{"damagemod", false},
				{"proneok", false},
				{"walkdistfacingmotion", false},
				{"walkdist", false},
				{"desiredangle", false},
				{"pacifist", false},
				{"pacifistwait", false},
				{"footstepdetectdist", false},
				{"footstepdetectdistwalk", false},
				{"footstepdetectdistsprint", false},
				{"reactiontargetpos", false},
				{"newenemyreactiondistsq", false},
				{"ignoreexplosionevents", false},
				{"ignoresuppression", false},
				{"suppressionwait", false},
				{"suppressionduration", false},
				{"suppressionstarttime", false},
				{"suppressionmeter", false},
				{"ignoreplayersuppression", false},
				{"name", false},
				{"weapon", false},
				{"dontavoidplayer", false},
				{"grenadeawareness", false},
				{"grenade", false},
				{"grenadeweapon", false},
				{"grenadeammo", false},
				{"grenadetargetpos", false},
				{"grenadetargetvalid", false},
				{"grenadetossvel", false},
				{"favoriteenemy", false},
				{"highlyawareradius", false},
				{"minpaindamage", false},
				{"allowpain", false},
				{"allowdeath", false},
				{"delayeddeath", false},
				{"diequietly", false},
				{"forceragdollimmediate", false},
				{"providecoveringfire", false},
				{"doingambush", false},
				{"combatmode", false},
				{"alertlevel", false},
				{"alertlevelint", false},
				{"useable", false},
				{"ignoretriggers", false},
				{"pushable", false},
				{"script_pushable", false},
				{"dropweapon", false},
				{"drawoncompass", false},
				{"groundtype", false},
				{"anim_pose", false},
				{"goalradius", false},
				{"goalheight", false},
				{"goalpos", false},
				{"nodeoffsetpos", false},
				{"ignoreforfixednodesafecheck", false},
				{"fixednode", false},
				{"fixednodesaferadius", false},
				{"pathgoalpos", false},
				{"pathrandompercent", false},
				{"usechokepoints", false},
				{"stopanimdistsq", false},
				{"lastenemysightpos", false},
				{"pathenemylookahead", false},
				{"pathenemyfightdist", false},
				{"meleeattackdist", false},
				{"movemode", false},
				{"script_move_distance_override", false},
				{"usecombatscriptatcover", false},
				{"safetochangescript", false},
				{"keepclaimednode", false},
				{"keepclaimednodeifvalid", false},
				{"keepnodeduringscriptedanim", false},
				{"dodangerreact", false},
				{"dangerreactduration", false},
				{"nododgemove", false},
				{"noteammove", false},
				{"leanamount", false},
				{"pitchamount", false},
				{"turnrate", false},
				{"turnanimactive", false},
				{"badplaceawareness", false},
				{"damageshield", false},
				{"nogrenadereturnthrow", false},
				{"noattackeraccuracymod", false},
				{"frontshieldanglecos", false},
				{"lookaheaddir", false},
				{"lookaheaddist", false},
				{"lookaheadhitsstairs", false},
				{"velocity", false},
				{"prevanimdelta", false},
				{"exposedduration", false},
				{"requestarrivalnotify", false},
				{"scriptedarrivalent", false},
				{"goingtoruntopos", false},
				{"engagemindist", false},
				{"engageminfalloffdist", false},
				{"engagemaxdist", false},
				{"engagemaxfalloffdist", false},
				{"usingcovermoveup", false},
				{"finalaccuracy", false},
				{"facemotion", false},
				{"gunblockedbywall", false},
				{"relativedir", false},
				{"lockorientation", false},
				{"maxfaceenemydist", false},
				{"stairsstate", false},
				{"script", false},
				{"prevscript", false},
				{"headicon", false},
				{"headiconteam", false},
				{"coversearchinterval", false},
				{"threatupdateinterval", false},
				{"canclimbladders", false},
				{"swimmer", false},
				{"space", false},
				{"doghandler", false},
				{"sharpturnlookaheaddist", false},
				{"postsharpturnlookaheaddist", false},
				{"sharpturntooclosetodestdist", false},
				{"usepathsmoothingvalues", false},
				{"pathlookaheaddist", false},
				{"maxturnspeed", false},
				{"sharpturn", false},
				{"disablesightandthreatupdate", false},
				{"team", false},
				{"threatbias", false},
				{"threatbiasgroup", false},
				{"node", false},
				{"prevnode", false},
				{"enemy", false},
				{"syncedmeleetarget", false},
				{"lastattacker", false},
				{"lastpusher", false},
				{"ignoreme", false},
				{"ignoreall", false},
				{"maxvisibledist", false},
				{"surprisedbymedistsq", false},
				{"attackeraccuracy", false},
				{"ignorerandombulletdamage", false},
				{"dodamagetoall", false},
				{"turretinvulnerability", false},
				{"useorcaavoidance", false},
				{"reciprocality", false},
				{"avoidanceboundshalfsize", false},
				{"onlygoodnearestnodes", false},
				{"playername", false},
				{"deathinvulnerabletime", false},
				{"criticalbulletdamagedist", false},
				{"attackercount", false},
				{"damagemultiplier", false},
				{"laststand", false},
				{"motiontrackerenabled", false},
				{"veh_speed", false},
				{"veh_pathspeed", false},
				{"veh_transmission", false},
				{"veh_pathdir", false},
				{"veh_pathtype", false},
				{"veh_topspeed", false},
				{"veh_brake", false},
				{"veh_throttle", false},
			};
		};

		utils::concurrency::container<data_t> data_{};
		unsigned int selected_entity{};
		bool set_field_window{};
		bool selected_fields_window{};
		int selected_type = game::SCRIPT_INTEGER;

		std::string field_filter{};
		std::string field_name_buffer{};
		std::string field_value_buffer{};
		std::string vector_input[3]{};

		bool verify_entity(unsigned int id)
		{
			const auto type = game::scr_VarGlob->objectVariableValue[id].w.type;
			return type == game::SCRIPT_ENTITY;
		}

		std::optional<scripting::array> get_entity_array(const entity_type type, const entity_team team)
		{
			const auto value = scripting::call("getentarray");
			if (!value.is<scripting::array>())
			{
				return {};
			}

			const auto all = value.as<scripting::array>();

			if (type == entity_type::type_any)
			{
				return {all};
			}

			if (type == entity_type::actor)
			{
				scripting::array result{};

				for (auto i = 0; i < all.size(); i++)
				{
					const auto raw = all[i].get_raw();
					if (raw.type != game::SCRIPT_OBJECT)
					{
						continue;
					}

					if (!verify_entity(raw.u.uintValue))
					{
						continue;
					}

					const auto entity = all[i].as<scripting::entity>();

					const auto classname_value = entity.get("classname");
					if (!classname_value.is<std::string>())
					{
						continue;
					}

					const auto team_value = entity.get("team");
					if (!team_value.is<std::string>())
					{
						continue;
					}

					const auto classname = classname_value.as<std::string>();
					const auto team_ = team_value.as<std::string>();
					if (utils::string::find_lower(classname, "actor_") && 
					    (team == entity_team::team_any || team_ == team_names[team]))
					{
						result.push(entity);
					}
				}

				return result;
			}

			if (type == entity_type::spawner && team == entity_team::team_any)
			{
				return scripting::call("getspawnerarray").as<scripting::array>();
			}

			if (type == entity_type::spawner)
			{
				return scripting::call("getspawnerteamarray", {team_names[team]}).as<scripting::array>();
			}

			if (type == entity_type::weapon)
			{
				return scripting::call("getweaponarray").as<scripting::array>();
			}

			if (type == entity_type::node)
			{
				return scripting::call("getnodearray").as<scripting::array>();
			}

			return {};
		}

		void update_entity_list()
		{
			data_.access([](data_t& data)
			{
				for (const auto& task : data.tasks)
				{
					task();
				}

				data.tasks = {};

				const auto now = std::chrono::high_resolution_clock::now();
				if (!data.force_update && (!data.auto_update || (now - data.last_call < data.interval)))
				{
					return;
				}

				data.last_call = now;
				data.force_update = false;

				const auto value = get_entity_array(data.filters.type, data.filters.team);
				if (!value.has_value())
				{
					return;
				}

				data.entity_info.clear();

				const auto array = value.value();

				for (auto i = 0; i < array.size(); i++)
				{
					const auto raw = array[i].get_raw();
					if (raw.type != game::SCRIPT_OBJECT)
					{
						continue;
					}

					if (!verify_entity(raw.u.uintValue))
					{
						continue;
					}

					const auto entity = array[i].as<scripting::entity>();
					entity_info_t info{};

					info.id = raw.u.uintValue;
					info.num = entity.get_entity_reference().entnum;

					if (data.filters.filter_by_range)
					{
						const auto player = scripting::call("getentbynum", {0}).as<scripting::entity>();
						const auto distance = scripting::call("distance", {player.get("origin"), entity.get("origin")}).as<float>();

						if (distance > data.filters.range)
						{
							continue;
						}
					}

					auto match_count = 0;
					for (const auto& field : data.selected_fields)
					{
						if (!field.second)
						{
							continue;
						}

						try
						{
							const auto field_value = entity.get(field.first);
							const auto value_string = field_value.to_string();
							info.fields[field.first] = value_string;

							for (const auto& filter : data.filters.fields)
							{
								if (field_value.is<std::string>() && 
									utils::string::find_lower(field.first, filter.first) &&
									utils::string::find_lower(value_string, filter.second))
								{
									match_count++;
								}
							}
						}
						catch (...)
						{
						}
					}

					if (match_count == data.filters.fields.size())
					{
						data.entity_info.push_back(info);
					}
				}
			});
		}

		void teleport_to(data_t& data, unsigned int num)
		{
			data.tasks.push_back([num]()
			{
				try
				{
					const auto entity = scripting::call("getentbynum", {num}).as<scripting::entity>();
					const auto player = scripting::call("getentbynum", {0}).as<scripting::entity>();
					player.call("setorigin", {entity.get("origin")});
				}
				catch (...)
				{
					gui::notification("Error", utils::string::va("^1error teleporting player to entity num %i!", num));
				}
			});
		}

		void teleport_to_reverse(data_t& data, unsigned int num)
		{
			data.tasks.push_back([num]()
			{
				try
				{
					const auto entity = scripting::call("getentbynum", {num}).as<scripting::entity>();
					const auto player = scripting::call("getentbynum", {0}).as<scripting::entity>();
					entity.set("origin", player.get("origin"));
				}
				catch (...)
				{
					gui::notification("Error", utils::string::va("^1error teleporting entity num %i to player!", num));
				}
			});
		}

		void delete_entity(data_t& data, unsigned int num)
		{
			data.tasks.push_back([num]()
			{
				try
				{
					const auto entity = scripting::call("getentbynum", {num}).as<scripting::entity>();
					entity.call("delete");
				}
				catch (...)
				{
					gui::notification("Error", utils::string::va("^1error deleting entity num %i!", num));
				}
			});
		}

		void set_entity_field(data_t& data, unsigned int num,
			const std::string& name, const std::string& string_value, int type)
		{
			data.tasks.push_back([num, name, type, string_value, &data]()
			{
				scripting::script_value value{};

				if (type == game::SCRIPT_INTEGER)
				{
					value = atoi(string_value.data());
				}

				if (type == game::SCRIPT_FLOAT)
				{
					value = atof(string_value.data());
				}

				if (type == game::SCRIPT_STRING)
				{
					value = string_value;
				}

				try
				{
					const auto entity = scripting::call("getentbynum", {num}).as<scripting::entity>();
					scripting::set_entity_field(entity, name, value);
					data.force_update = true;
				}
				catch (...)
				{
					gui::notification("Error", utils::string::va("^1error setting field '%s'!", name.data()));
				}
			});
		}

		void set_entity_field_vector(data_t& data, unsigned int num,
			const std::string& name, const scripting::vector& value)
		{
			data.tasks.push_back([num, name, value, &data]()
			{
				try
				{
					const auto entity = scripting::call("getentbynum", {num}).as<scripting::entity>();
					scripting::set_entity_field(entity, name, value);
					data.force_update = true;
				}
				catch (...)
				{
					gui::notification("Error", utils::string::va("^1error setting field '%s'!", name.data()));
				}
			});
		}

		void show_set_field_window(data_t& data)
		{
			ImGui::SetNextWindowSizeConstraints(ImVec2(300, 300), ImVec2(300, 300));
			ImGui::Begin("Set entity field", &set_field_window);
			ImGui::SetWindowSize(ImVec2(300, 300));
			ImGui::Text(utils::string::va("Entity id %i", selected_entity));

			if (ImGui::TreeNode("Type"))
			{
				ImGui::RadioButton("integer", &selected_type, game::SCRIPT_INTEGER);
				ImGui::RadioButton("float", &selected_type, game::SCRIPT_FLOAT);
				ImGui::RadioButton("vector", &selected_type, game::SCRIPT_VECTOR);
				ImGui::RadioButton("string", &selected_type, game::SCRIPT_STRING);

				ImGui::TreePop();
			}

			ImGui::InputText("name", &field_name_buffer);
			if (selected_type == game::SCRIPT_VECTOR)
			{
				ImGui::InputText("x", &vector_input[0]);
				ImGui::InputText("y", &vector_input[1]);
				ImGui::InputText("z", &vector_input[2]);
			}
			else
			{
				ImGui::InputText("value", &field_value_buffer);
			}

			if (ImGui::Button("set", ImVec2(300, 0)))
			{
				if (selected_type == game::SCRIPT_VECTOR)
				{
					const scripting::vector vector
					{
						static_cast<float>(atof(vector_input[0].data())),
						static_cast<float>(atof(vector_input[1].data())),
						static_cast<float>(atof(vector_input[2].data()))
					};

					set_entity_field_vector(data, selected_entity, 
						field_name_buffer, vector);
				}
				else
				{
					set_entity_field(data, selected_entity, field_name_buffer, 
						field_value_buffer, selected_type);
				}
			}

			ImGui::End();
		}

		void show_entity_list_window(data_t& data)
		{
			ImGui::SetNextWindowSizeConstraints(ImVec2(500, 500), ImVec2(1000, 1000));
			ImGui::Begin("Entity list", &gui::enabled_menus["entity_list"]);

			if (ImGui::Button("Update list"))
			{
				data.force_update = true;
			}

			ImGui::Checkbox("Auto update", &data.auto_update);
			ImGui::Checkbox("Field list", &selected_fields_window);

			auto interval = static_cast<int>(data.interval.count());
			if (data.auto_update && ImGui::SliderInt("Interval", &interval, 0, 1000 * 30))
			{
				data.interval = std::chrono::milliseconds(interval);
			}

			ImGui::Separator();

			if (ImGui::CollapsingHeader("Filters"))
			{
				ImGui::Checkbox("Filter by distance", &data.filters.filter_by_range);

				if (data.filters.filter_by_range)
				{
					if (ImGui::SliderFloat("range", &data.filters.range, 0.f, 10000.f))
					{
						data.force_update = true;
					}
				}

				if (ImGui::TreeNode("Entity type"))
				{
					auto result = 0;

					result += ImGui::RadioButton("any", reinterpret_cast<int*>(&data.filters.type), entity_type::type_any);
					result += ImGui::RadioButton("actor", reinterpret_cast<int*>(&data.filters.type), entity_type::actor);
					result += ImGui::RadioButton("spawner", reinterpret_cast<int*>(&data.filters.type), entity_type::spawner);
					result += ImGui::RadioButton("weapon", reinterpret_cast<int*>(&data.filters.type), entity_type::weapon);
					result += ImGui::RadioButton("node", reinterpret_cast<int*>(&data.filters.type), entity_type::node);

					if (result)
					{
						data.force_update = true;
					}

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Entity team"))
				{
					auto result = 0;

					result += ImGui::RadioButton("any", reinterpret_cast<int*>(&data.filters.team), entity_team::team_any);
					result += ImGui::RadioButton("neutral", reinterpret_cast<int*>(&data.filters.team), entity_team::neutral);
					result += ImGui::RadioButton("allies", reinterpret_cast<int*>(&data.filters.team), entity_team::allies);
					result += ImGui::RadioButton("axis", reinterpret_cast<int*>(&data.filters.team), entity_team::axis);
					result += ImGui::RadioButton("team3", reinterpret_cast<int*>(&data.filters.team), entity_team::team3);

					if (result)
					{
						data.force_update = true;
					}

					ImGui::TreePop();
				}

				ImGui::Text("Fields");

				auto index = 0;
				for (auto i = data.filters.fields.begin(); i != data.filters.fields.end(); ++i)
				{
					if (ImGui::TreeNode(utils::string::va("Filter #%i", index++)))
					{
						ImGui::InputText("name", &i->first);
						ImGui::InputText("value", &i->second);

						if (ImGui::Button("Erase"))
						{
							data.filters.fields.erase(i);
							--i;
						}

						ImGui::TreePop();
					}
				}

				if (ImGui::Button("Add field filter"))
				{
					data.filters.fields.push_back({});
				}
			}

			ImGui::Separator();
			ImGui::BeginChild("entity list");

			for (const auto& info : data.entity_info)
			{
				if (ImGui::TreeNode(utils::string::va("Entity num %i", info.num)))
				{
					ImGui::Text("Info");

					const auto num_str = utils::string::va("%i", info.num);

					ImGui::Text("Entity num");
					ImGui::SameLine();
					if (ImGui::Button(num_str))
					{
						gui::copy_to_clipboard(num_str);
					}

					ImGui::Text("Commands");

					if (ImGui::Button("Set field"))
					{
						set_field_window = true;
						selected_entity = info.num;
					}

					if (ImGui::Button("Teleport to"))
					{
						teleport_to(data, info.num);
						data.force_update = true;
					}

					if (ImGui::Button("Teleport to you"))
					{
						teleport_to_reverse(data, info.num);
						data.force_update = true;
					}

					if (info.num != 0 && ImGui::Button("Delete"))
					{
						delete_entity(data, info.num);
						data.force_update = true;
					}

					ImGui::Text("Fields");

					for (const auto& field : info.fields)
					{
						if (field.second.empty())
						{
							continue;
						}

						if (ImGui::Button(field.first.data()))
						{
							gui::copy_to_clipboard(field.first);
						}

						ImGui::SameLine();

						if (ImGui::Button(field.second.data()))
						{
							gui::copy_to_clipboard(field.second);
						}
					}

					ImGui::TreePop();
				}
			}

			ImGui::EndChild();
			ImGui::End();
		}

		void show_selected_fields_window(data_t& data)
		{
			ImGui::SetNextWindowSizeConstraints(ImVec2(500, 500), ImVec2(1000, 1000));
			ImGui::Begin("Selected fields", &selected_fields_window);

			ImGui::InputText("field name", &field_filter);
			ImGui::BeginChild("field list");

			for (auto& field : data.selected_fields)
			{
				if (utils::string::find_lower(field.first, field_filter) &&
					ImGui::Checkbox(field.first.data(), &field.second))
				{
					data.force_update = true;
				}
			}

			ImGui::EndChild();
			ImGui::End();
		}

		void on_frame()
		{
			if (!gui::enabled_menus["entity_list"])
			{
				return;
			}

			data_.access([](data_t& data)
			{
				if (!game::CL_IsCgameInitialized())
				{
					selected_entity = 0;
					data.entity_info = {};
					data.tasks = {};
				}


				show_entity_list_window(data);

				if (selected_fields_window)
				{
					show_selected_fields_window(data);
				}

				if (set_field_window)
				{
					show_set_field_window(data);
				}
			});
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			gui::on_frame(on_frame);
			scheduler::loop([]()
			{
				try
				{
					update_entity_list();
				}
				catch (...)
				{
				}
			}, scheduler::pipeline::server);
		}
	};
}

REGISTER_COMPONENT(entity_list::component)
