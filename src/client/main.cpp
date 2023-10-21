#include <std_include.hpp>
#include "launcher/launcher.hpp"
#include "loader/loader.hpp"
#include "loader/component_loader.hpp"
#include "game/game.hpp"

#include <utils/string.hpp>
#include <utils/flags.hpp>
#include <utils/io.hpp>
#include <utils/properties.hpp>

DECLSPEC_NORETURN void WINAPI exit_hook(const int code)
{
	component_loader::pre_destroy();
	exit(code);
}

BOOL WINAPI system_parameters_info_a(const UINT uiAction, const UINT uiParam, const PVOID pvParam, const UINT fWinIni)
{
	return SystemParametersInfoA(uiAction, uiParam, pvParam, fWinIni);
}

FARPROC WINAPI get_proc_address(const HMODULE hModule, const LPCSTR lpProcName)
{
	if (lpProcName == "InitializeCriticalSectionEx"s)
	{
		component_loader::post_unpack();
	}

	return GetProcAddress(hModule, lpProcName);
}

launcher::mode detect_mode_from_arguments()
{
	if (utils::flags::has_flag("singleplayer"))
	{
		return launcher::mode::singleplayer;
	}

	return launcher::mode::none;
}

void apply_aslr_patch(std::string* data)
{
	// sp binary
	if (data->size() != 0xE1E0C8)
	{
		throw std::runtime_error("File size mismatch, bad game files");
	}

	auto* dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(&data->at(0));
	auto* nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(&data->at(dos_header->e_lfanew));
	auto* optional_header = &nt_headers->OptionalHeader;

	if (optional_header->DllCharacteristics & IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE)
	{
		optional_header->DllCharacteristics &= ~(IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE);
	}
}

void get_aslr_patched_binary(std::string* binary, std::string* data)
{
	const auto patched_binary = (utils::properties::get_appdata_path() / "bin/h2_sp64_bnet_ship.exe"s).generic_string();

	try
	{
		apply_aslr_patch(data);
		if (!utils::io::file_exists(patched_binary) && !utils::io::write_file(patched_binary, *data, false))
		{
			throw std::runtime_error("Could not write file");
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(
			utils::string::va("Could not create aslr patched binary for %s! %s",
			binary->data(), e.what())
		);
	}

	*binary = patched_binary;
}

FARPROC load_binary(const launcher::mode mode)
{
	loader loader;
	utils::nt::library self;

	loader.set_import_resolver([self](const std::string& library, const std::string& function) -> void*
	{
		if (function == "ExitProcess")
		{
			return exit_hook;
		}
		else if (function == "SystemParametersInfoA")
		{
			return system_parameters_info_a;
		}
		else if (function == "GetProcAddress")
		{
			return get_proc_address;
		}

		return component_loader::load_import(library, function);
	});

	std::string binary = "MW2CR.exe";
	if (!utils::io::file_exists(binary))
	{
		binary = "h2_sp64_bnet_ship.exe";
	}

	std::string data;
	if (!utils::io::read_file(binary, &data))
	{
		throw std::runtime_error(utils::string::va(
			"Failed to read game binary (%s)!\nPlease copy the h2-mod.exe into your Call of Duty: Modern Warfare 2 Campaign Remastered installation folder and run it from there.",
			binary.data()));
	}

#ifdef INJECT_HOST_AS_LIB
	get_aslr_patched_binary(&binary, &data);
	return loader.load_library(binary);
#else
	return loader.load(self, data);
#endif
}

void remove_crash_file()
{
	utils::io::remove_file("__h2Exe");
}

void verify_version()
{
	const auto value = *reinterpret_cast<DWORD*>(0x140123456);
	if (value != 0xE465E151)
	{
		throw std::runtime_error("Unsupported Call of Duty: Modern Warfare 2 Campaign Remastered version");
	}
}

void enable_dpi_awareness()
{
	const utils::nt::library user32{"user32.dll"};
	const auto set_dpi = user32
		                     ? user32.get_proc<BOOL(WINAPI*)(DPI_AWARENESS_CONTEXT)>("SetProcessDpiAwarenessContext")
		                     : nullptr;
	if (set_dpi)
	{
		set_dpi(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	}
}

void limit_parallel_dll_loading()
{
	const utils::nt::library self;
	const auto registry_path = R"(Software\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)" + self.
		get_name();

	HKEY key = nullptr;
	if (RegCreateKeyA(HKEY_LOCAL_MACHINE, registry_path.data(), &key) == ERROR_SUCCESS)
	{
		RegCloseKey(key);
	}

	key = nullptr;
	if (RegOpenKeyExA(
		HKEY_LOCAL_MACHINE, registry_path.data(), 0,
		KEY_ALL_ACCESS, &key) != ERROR_SUCCESS)
	{
		return;
	}

	DWORD value = 1;
	RegSetValueExA(key, "MaxLoaderThreads", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(value));

	RegCloseKey(key);
}

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	FARPROC entry_point;
	enable_dpi_awareness();

	limit_parallel_dll_loading();

	srand(uint32_t(time(nullptr)));
	remove_crash_file();

	{
		auto premature_shutdown = true;
		const auto _ = gsl::finally([&premature_shutdown]()
		{
			if (premature_shutdown)
			{
				component_loader::pre_destroy();
			}
		});

		try
		{
			if (!component_loader::post_start())
			{
				return 0;
			}

			auto mode = detect_mode_from_arguments();
			if (mode == launcher::mode::none)
			{
				const launcher launcher;
				mode = launcher.run();
				if (mode == launcher::mode::none)
				{
					return 0;
				}
			}

			game::environment::set_mode(mode);

			entry_point = load_binary(mode);
			if (!entry_point)
			{
				throw std::runtime_error("Unable to load binary into memory");
			}

			verify_version();

			if (!component_loader::post_load())
			{
				return 0;
			}

			premature_shutdown = false;
		}
		catch (std::exception& e)
		{
			MessageBoxA(nullptr, e.what(), "ERROR", MB_ICONERROR);
			return 1;
		}
	}

	return static_cast<int>(entry_point());
}

int __stdcall WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
	return main();
}
