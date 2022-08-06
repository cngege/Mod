// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "Loader.h"

#pragma comment(lib, "MinHook.x64.lib")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Loader::init, hModule, NULL, NULL);
        DisableThreadLibraryCalls(hModule);
    }
    break;
    case DLL_PROCESS_DETACH:
        Loader::exit(hModule);
        break;
    }
    return TRUE;
}

