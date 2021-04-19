#include "stdinc.hpp"

#pragma warning(disable:4996)

DWORD WINAPI dwConsole(LPVOID)
{
    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);

    std::string cmd;

    while (true)
    {
        std::cout << "\n";
        std::getline(std::cin, cmd);

        game::Cbuf_AddText(0, cmd.data());
    }

    return 0;
}

void init()
{
    CreateThread(0, 0, dwConsole, 0, 0, 0);

    game::load_base_address();

    utils::hook::set(game::base_address + 0xBE7F83C, true); // disable bnet popup

    command::init();
    input::init();
    scheduler::init();
    game_console::init();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        init();
    }

    return TRUE;
}