#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "gui.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace gui
{
	namespace
	{
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

		void gui_draw()
		{
			
		}

		void on_frame()
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
			scheduler::loop(on_frame, scheduler::pipeline::renderer);
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
