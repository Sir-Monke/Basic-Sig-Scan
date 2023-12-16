#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

MODULEINFO getModuleInfo(const char* szModule) {
    MODULEINFO modInfo = { 0 };
    wchar_t wszModule[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, szModule, -1, wszModule, MAX_PATH);
    HMODULE hModule = GetModuleHandle(wszModule);
    if (hModule != NULL) {
        GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
    }
    return modInfo;
}

void WriteToMemory(uintptr_t addressToWrite, char* valueToWrite, int byteNum) {
    DWORD oldProtection;
    VirtualProtect((LPVOID)(addressToWrite), byteNum, PAGE_EXECUTE_READWRITE, &oldProtection);
    memcpy((LPVOID)addressToWrite, valueToWrite, byteNum);
    VirtualProtect((LPVOID)(addressToWrite), byteNum, oldProtection, NULL);
}

DWORD FindPattern(const char* module, const char* pattern, const char* mask) {
    MODULEINFO mInfo = getModuleInfo(module);
    uintptr_t base = reinterpret_cast<uintptr_t>(mInfo.lpBaseOfDll);
    DWORD size = mInfo.SizeOfImage;
    DWORD patternLength = static_cast<DWORD>(strlen(mask));
    for (DWORD i = 0; i < size - patternLength; i++) {
        bool found = true;
        for (DWORD j = 0; j < patternLength; j++) {
            if (mask[j] != '?' && pattern[j] != *reinterpret_cast<char*>(base + i + j)) {
                found = false;
                break;
            }
        }
        if (found) {
            return static_cast<DWORD>(base + i);
        }
    }
    return NULL;
}