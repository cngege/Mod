// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include "Loader.h"

#ifdef _DEBUG
    #pragma comment(lib, "libMinHook.x64-v143-mdd.lib")
#elif NDEBUG
    #pragma comment(lib, "libMinHook.x64-v143-md.lib")
#endif

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "version")

//临时消除出现在Json.hpp 4281行的警告
//#pragma warning(disable:26800)

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
        DisableThreadLibraryCalls(hModule);                                     //应用程序及其DLL的线程创建与销毁不再对此DLL进行通知
        break;
    }
    case DLL_PROCESS_DETACH:
        if (!Loader::Eject_Signal) {
            // 表示是外部卸载这个库
            Loader::RemoteFreeLib = true;
            Loader::Eject_Signal = true;
            Sleep(600);
            Loader::exit(hModule);
        }
        else {
            Loader::exit(hModule);      // 这个有锁 多次调用没有关系
        }
        break;
    }
    return TRUE;
}

