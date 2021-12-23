#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "command.hpp"
#include "game_console.hpp"
#include "gui.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>
#include <utils/concurrency.hpp>

namespace gui_console
{
	namespace
	{
		bool auto_scroll = true;
		int history_index = -1;
		std::string input{};
		std::string filter{};
		std::vector<std::string> matches{};

		int input_text_edit(ImGuiInputTextCallbackData* data)
		{
			switch (data->EventFlag)
			{
			case ImGuiInputTextFlags_CallbackCompletion:
			{
				matches.clear();
				const std::string text = data->Buf;

				if (text.find(" ") != std::string::npos)
				{
					game_console::find_matches(text.substr(0, text.find(" ")), matches, true);
				}
				else
				{
					game_console::find_matches(text, matches, false);
				}

				if (matches.size() < 24)
				{
					const auto match = matches[0].data();
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, match, match + strlen(match));
				}

				break;
			}
			case ImGuiInputTextFlags_CallbackHistory:
			{
				const auto history = game_console::get_history();

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

		std::string get_filtered_text()
		{
			std::string text{};

			const auto output = game_console::get_output();
			for (const auto& line : output)
			{
				if (utils::string::find_lower(line, filter))
				{
					text.append(line.data());
					text.append("\n");
				}
			}

			if (text[text.size() - 1] == '\n')
			{
				text.pop_back();
			}

			return text;
		}

		void on_frame()
		{
			if (!gui::enabled_menus["console"])
			{
				return;
			}

			const auto filtered_text = get_filtered_text();
			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			static const auto input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | 
												 ImGuiInputTextFlags_CallbackHistory;
			
			ImGui::Begin("Console", &gui::enabled_menus["console"]);

			if (ImGui::BeginPopup("Options"))
			{
				ImGui::Checkbox("Auto-scroll", &auto_scroll);
				ImGui::EndPopup();
			}

			if (ImGui::Button("Clear"))
			{
				game_console::clear_console();
			}

			ImGui::SameLine();

			if (ImGui::Button("Copy"))
			{
				utils::string::set_clipboard_data(filtered_text);
				gui::notification("Console", "Text copied to clipboard");
			}

			ImGui::Separator();

			if (ImGui::Button("Options"))
			{
				ImGui::OpenPopup("Options");
			}

			ImGui::SameLine();
			ImGui::InputText("Filter", &filter);

			ImGui::BeginChild("console_scroll", ImVec2(0, -footer_height_to_reserve), false);

			ImGui::Text(filtered_text.data(), ImVec2(-1, -1));

			if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			{
				ImGui::SetScrollHereY(1.0f);
			}

			ImGui::EndChild();
			
			if (ImGui::InputText("Input", &input, input_text_flags, input_text_edit))
			{
				auto history = game_console::get_history();

				if (history_index != -1)
				{
					const auto itr = history.begin() + history_index;

					if (*itr == input)
					{
						history.erase(history.begin() + history_index);
					}
				}

				ImGui::SetKeyboardFocusHere(-1);
				game_console::add(input.data());
				history_index = -1;
				input.clear();
			}

			ImGui::End();
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			gui::on_frame(on_frame);
		}
	};
}

REGISTER_COMPONENT(gui_console::component)
