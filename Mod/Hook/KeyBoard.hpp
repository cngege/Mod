#pragma once

#ifdef MATHLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif

#include <iostream>
#include "windows.h"
#include "../Utils/Logger.h"

using namespace std;
extern "C" MATHLIBRARY_API BOOL InstallHook();
extern "C" MATHLIBRARY_API BOOL UninstallHook();

class KeyBoard
{
public:
	static HHOOK g_hHook;
public:
	static auto init(HMODULE)->void;
	static auto exit(HMODULE)->void;

};

HHOOK KeyBoard::g_hHook;



/// <summary>
/// 键盘回调
/// </summary>
/// <param name="code"></param>
/// <param name="wParam">虚拟按键的代号</param>
/// <param name="lParam">键状态</param>
/// <returns></returns>
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
	logF("[KeyBoard] Key");
	if (code < 0 || code == HC_NOREMOVE) {
		// 如果代码小于零，则挂钩过程必须将消息传递给CallNextHookEx函数，而无需进一步处理，并且应返回CallNextHookEx返回的值。此参数可以是下列值之一。(来自官网手册)
		return CallNextHookEx(KeyBoard::g_hHook, code, wParam, lParam);
	}
	if (lParam & 0x40000000) {
		// 【第30位的含义】键状态。如果在发送消息之前按下了键，则值为1。如果键被释放，则为0。(来自官网手册)
		// 我们只考虑被按下后松开的状态
		return CallNextHookEx(KeyBoard::g_hHook, code, wParam, lParam);
	}
	char szKeyName[200];
	// 【参数1】LPARAM类型，代表键状态
	// 【参数2】缓冲区
	// 【参数3】缓冲区大小
	GetKeyNameText(lParam, (LPWSTR)szKeyName, 100);
	
	logF("[KeyBoard] KeyDown: %s", szKeyName);

	// 将钩子往下传
	return CallNextHookEx(KeyBoard::g_hHook, code, wParam, lParam);
}

auto KeyBoard::init(HMODULE hModule)->void {
	g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(L"Mod.dll"), 0);
	if (g_hHook == NULL) {
		// 钩子安装失败
		logF("[SetWindowsHookEx] Install Hook Fail");
	}
}

auto KeyBoard::exit(HMODULE hModule)->void {
	UnhookWindowsHookEx(g_hHook);
}