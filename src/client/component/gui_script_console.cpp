#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "command.hpp"
#include "gui.hpp"

#include "game/scripting/lua/context.hpp"
#include "game/scripting/lua/engine.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>
#include <utils/concurrency.hpp>

namespace gui_script_console
{
	namespace
	{
		bool auto_scroll = true;
		bool multi_line_input = false;
		int history_index = -1;
		std::string input{};
		std::string filter{};
		std::deque<std::string> history;

		struct menu_data_t
		{
			std::deque<std::string> command_queue;
			std::deque<std::string> output;
		};

		utils::concurrency::container<menu_data_t> menu_data;

		std::string run_command(const std::string& code)
		{
			const auto result = scripting::lua::engine::load(code);
			if (result.has_value())
			{
				return result.value();
			}
			else
			{
				return "Script not running";
			}
		}

		void run_commands()
		{
			menu_data.access([](menu_data_t& menu_data_)
			{
				for (const auto& command : menu_data_.command_queue)
				{
					menu_data_.output.push_back(run_command(command));
				}

				menu_data_.command_queue.clear();
			});
		}

		int multi_line_input_text_edit(ImGuiInputTextCallbackData* data)
		{
			if (data->EventKey == ImGuiKey_Tab)
			{
				data->InsertChars(data->CursorPos, "\t");
			}

			return 0;
		}

		int input_text_edit(ImGuiInputTextCallbackData* data)
		{
			switch (data->EventFlag)
			{
			case ImGuiInputTextFlags_CallbackHistory:
			{
				if (data->EventKey == ImGuiKey_UpArrow)
				{
					if (++history_index >= history.size())
					{
						history_index = static_cast<int>(history.size()) - 1;
					}

					data->DeleteChars(0, data->BufTextLen);

					if (history_index != -1)
					{
						const auto text = history.at(history_index).data();
						data->InsertChars(0, text, text + strlen(text));
					}
				}
				else if (data->EventKey == ImGuiKey_DownArrow)
				{
					if (--history_index < -1)
					{
						history_index = -1;
					}

					data->DeleteChars(0, data->BufTextLen);

					if (history_index != -1)
					{
						const auto text = history.at(history_index).data();
						data->InsertChars(0, text, text + strlen(text));
					}
				}

				break;
			}
			}

			return 0;
		}

		void on_frame()
		{
			if (!gui::enabled_menus["script_console"])
			{
				return;
			}

			menu_data.access([](menu_data_t& menu_data_)
			{
				if (!game::CL_IsCgameInitialized())
				{
					menu_data_.command_queue.clear();
				}

				static const float footer_height = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
				static const auto input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion |
													 ImGuiInputTextFlags_CallbackHistory;

				ImGui::Begin("Script console", &gui::enabled_menus["script_console"]);

				if (ImGui::BeginPopup("Options"))
				{
					ImGui::Checkbox("Auto-scroll", &auto_scroll);
					ImGui::Checkbox("Multi-line input", &multi_line_input);
					ImGui::EndPopup();
				}

				if (ImGui::Button("Clear"))
				{
					menu_data_.output.clear();
					input.clear();
					history.clear();
				}

				ImGui::Separator();

				if (ImGui::Button("Options"))
				{
					ImGui::OpenPopup("Options");
				}

				ImGui::SameLine();
				ImGui::InputText("Filter", &filter);

				ImGui::BeginChild("script_console_scroll", ImVec2(0, -1 * footer_height - 80.f * multi_line_input), false);

				for (const auto& line : menu_data_.output)
				{
					ImGui::Text(line.data());
					ImGui::Separator();
				}

				if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				{
					ImGui::SetScrollHereY(1.0f);
				}

				ImGui::EndChild();

				bool execute = false;
				if (multi_line_input)
				{
					ImGui::InputTextMultiline("", &input, ImVec2(0, 100), 
						ImGuiInputTextFlags_CallbackCompletion, multi_line_input_text_edit);
					ImGui::SameLine();
					execute = ImGui::Button("Execute", ImVec2(100, 100));
				}
				else
				{
					execute = ImGui::InputText("Input", &input, input_text_flags, input_text_edit);
				}

				if (execute)
				{
					menu_data_.output.push_back(input);
					menu_data_.command_queue.push_back(input);

					if (history_index != -1)
					{
						const auto itr = history.begin() + history_index;

						if (*itr == input)
						{
							history.erase(history.begin() + history_index);
						}
					}

					history.push_front(input);
					if (history.size() > 10)
					{
						history.erase(history.begin() + 10);
					}

					ImGui::SetKeyboardFocusHere(-1);
					history_index = -1;
					input.clear();
				}

				ImGui::End();
			});
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			gui::on_frame(on_frame);
			scheduler::loop(run_commands, scheduler::pipeline::server);
		}
	};
}

REGISTER_COMPONENT(gui_script_console::component)
