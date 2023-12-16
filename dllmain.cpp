#include "pch.h"
#include <windows.h>
#include <iostream>
#include "Helper.h"

using namespace std;

DWORD WINAPI MainThread(HMODULE hModule) {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    while (true) {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
            DWORD ammoAddy = FindPattern("ac_client.exe", "\x0f\xbf\x88\x00\x00\x00\x00\x8b\x56\x00\x89\x0a\x8b\x76 ", "xxx????xx?xxxx");
            ammoAddy += 15;
            std::cout << ammoAddy << std::endl;
            char AmmoOpCode[] = "\x90\x90"; //NOP
            WriteToMemory(ammoAddy, AmmoOpCode, 2);
        }
    }
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, NULL, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        break;
    default:
        break;
    }
	return true;
}