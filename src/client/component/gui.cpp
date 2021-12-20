#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "gui.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>
#include <utils/concurrency.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace gui
{
	std::unordered_map<std::string, bool> enabled_menus;

	namespace
	{
		utils::concurrency::container<std::vector<std::function<void()>>> on_frame_callbacks;
		utils::concurrency::container<std::vector<notification_t>> notifications;

		ID3D11Device* device;
		ID3D11DeviceContext* device_context;
		bool initialized = false;
		bool toggled = false;

		void initialize_gui_context()
		{
			ImGui::CreateContext();
			ImGui::StyleColorsDark();

			ImGui_ImplWin32_Init(*game::hWnd);
			ImGui_ImplDX11_Init(device, device_context);

			initialized = true;
		}

		void new_gui_frame()
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			*game::keyCatchers |= 0x10;
		}

		void end_gui_frame()
		{
			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

		void toggle_menu(const std::string& name)
		{
			enabled_menus[name] = !enabled_menus[name];
		}

		void show_notifications()
		{
			static auto window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | 
									   ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | 
									   ImGuiWindowFlags_NoMove;

			notifications.access([](std::vector<notification_t>& notifications_)
			{
				auto index = 0;
				for (auto i = notifications_.begin(); i != notifications_.end(); ++i)
				{
					const auto now = std::chrono::high_resolution_clock::now();
					if (now - i->creation_time >= i->duration)
					{
						notifications_.erase(i--);
						continue;
					}

					const auto title = i->title.size() <= 34
						? i->title
						: i->title.substr(0, 31) + "...";

					const auto text = i->text.size() <= 34
						? i->text
						: i->text.substr(0, 31) + "...";

					ImGui::SetNextWindowSizeConstraints(ImVec2(250, 50), ImVec2(250, 50));
					ImGui::SetNextWindowBgAlpha(0.6f);
					ImGui::Begin(utils::string::va("Notification #%i", index), nullptr, window_flags);

					ImGui::SetWindowPos(ImVec2(10, 30.f + static_cast<float>(index) * 60.f));
					ImGui::SetWindowSize(ImVec2(250, 0));
					ImGui::Text(title.data());
					ImGui::Text(text.data());

					ImGui::End();

					index++;
				}
			});
		}

		void gui_draw()
		{
			show_notifications();

			on_frame_callbacks.access([](std::vector<std::function<void()>>& callbacks)
			{
				for (const auto& callback : callbacks)
				{
					callback();
				}
			});

			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Windows"))
				{
					ImGui::Checkbox("Asset list", &enabled_menus["asset_list"]);
					ImGui::Checkbox("Entity list", &enabled_menus["entity_list"]);

					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
		}

		void gui_on_frame()
		{
			if (!initialized)
			{
				initialize_gui_context();
			}

			if (toggled)
			{
				new_gui_frame();
				gui_draw();
				end_gui_frame();
			}
		}

		HRESULT d3d11_create_device_stub(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software,
			UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, 
			ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext)
		{
			const auto result = D3D11CreateDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels,
				FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);

			if (ppDevice != nullptr && ppImmediateContext != nullptr)
			{
				device = *ppDevice;
				device_context = *ppImmediateContext;
			}

			return result;
		}

		void dxgi_swap_chain_present_stub(utils::hook::assembler& a)
		{
			a.pushad64();
			a.call_aligned(gui_on_frame);
			a.popad64();

			a.mov(r8d, esi);
			a.mov(edx, r15d);
			a.mov(rcx, rdi);
			a.call_aligned(rbx);
			a.mov(ecx, eax);

			a.jmp(0x7A14D1_b);
		}

		utils::hook::detour wnd_proc_hook;
		LRESULT wnd_proc_stub(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			if (toggled && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			{
				return TRUE;
			}

			return wnd_proc_hook.invoke<LRESULT>(hWnd, msg, wParam, lParam);
		}
	}

	bool gui_key_event(const int local_client_num, const int key, const int down)
	{
		if (key == game::K_F10 && down)
		{
			toggled = !toggled;
			return false;
		}

		if (key == game::K_ESCAPE && down && toggled)
		{
			toggled = false;
			return false;
		}

		return !toggled;
	}

	bool gui_char_event(const int local_client_num, const int key)
	{
		return !toggled;
	}

	bool gui_mouse_event(const int local_client_num, int x, int y)
	{
		return !toggled;
	}

	void on_frame(const std::function<void()>& callback)
	{
		on_frame_callbacks.access([callback](std::vector<std::function<void()>>& callbacks)
		{
			callbacks.push_back(callback);
		});
	}

	bool is_menu_open(const std::string& name)
	{
		return enabled_menus[name];
	}

	void notification(const std::string& title, const std::string& text, const std::chrono::milliseconds duration)
	{
		notification_t notification{};
		notification.title = title;
		notification.text = text;
		notification.duration = duration;
		notification.creation_time = std::chrono::high_resolution_clock::now();

		notifications.access([notification](std::vector<notification_t>& notifications_)
		{
			notifications_.insert(notifications_.begin(), notification);
		});
	}

	class component final : public component_interface
	{
	public:
		void* load_import(const std::string& library, const std::string& function) override
		{
			if (function == "D3D11CreateDevice")
			{
				return d3d11_create_device_stub;
			}

			return nullptr;
		}

		void post_unpack() override
		{
			utils::hook::jump(0x7A14C4_b, utils::hook::assemble(dxgi_swap_chain_present_stub), true);
			wnd_proc_hook.create(0x650F10_b, wnd_proc_stub);
		}

		void pre_destroy() override
		{
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}
	};
}

REGISTER_COMPONENT(gui::component)
