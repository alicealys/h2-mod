#include <stdinc.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "component/game_console.hpp"

#pragma warning(disable:4996)

DWORD WINAPI console(LPVOID)
{
    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);

    std::string cmd;

    while (true)
    {
        std::getline(std::cin, cmd);

        game_console::execute(cmd.data());
    }

    return 0;
}

void init()
{
    CreateThread(0, 0, console, 0, 0, 0);

    game::load_base_address();

    component_loader::post_unpack();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        init();
    }

    return TRUE;
}