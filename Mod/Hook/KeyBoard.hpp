#pragma once

#ifdef MATHLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif

#include <iostream>
#include "windows.h"
#include "../Utils/Logger.h"
#include "../Utils/Utils.h"

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
	KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;		// 包含低级键盘输入事件信息

	/*
	typedef struct tagKBDLLHOOKSTRUCT {
		DWORD     vkCode;		// 按键代号
		DWORD     scanCode;		// 硬件扫描代号，同 vkCode 也可以作为按键的代号。
		DWORD     flags;		// 事件类型，一般按键按下为 0 抬起为 128。
		DWORD     time;			// 消息时间戳
		ULONG_PTR dwExtraInfo;	// 消息附加信息，一般为 0。
	}KBDLLHOOKSTRUCT,*LPKBDLLHOOKSTRUCT,*PKBDLLHOOKSTRUCT;
	*/
	if (ks->flags == 0) {
		logF("Key Down: %s", KeyNames[ks->vkCode]);
	}

	// 将钩子往下传
	return CallNextHookEx(KeyBoard::g_hHook, code, wParam, lParam);
}

auto KeyBoard::init(HMODULE hModule)->void {
	g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hModule, 0);
	if (g_hHook == NULL) {
		// 钩子安装失败
		logF("[SetWindowsHookEx] Install Hook Fail");
	}

	MSG msg;
	while (1)
	{
		if (!Game::ModState) {
			//break;
		}
		// 如果消息队列中有消息
		if (PeekMessageA(
			&msg,		// MSG 接收这个消息
			FindWindowA(NULL, "ABC"),		// 检测消息的窗口句柄，NULL：检索当前线程所有窗口消息
			NULL,		// 检查消息范围中第一个消息的值，NULL：检查所有消息(必须和下面的同时为NULL)
			NULL,		// 检查消息范围中最后一个消息的值，NULL：检查所有消息(必须和上面的同时为NULL)
			PM_REMOVE	// 处理消息的方式，PM_REMOVE：处理后将消息从队列中删除
		)) {
			// 把按键消息传递给字符消息
			TranslateMessage(&msg);

			// 将消息分派给窗口程序
			DispatchMessageW(&msg);
		}
		else
			Sleep(0);    //避免CPU全负载运行
	}
	// 删除钩子
	UnhookWindowsHookEx(KeyBoard::g_hHook);
}

auto KeyBoard::exit(HMODULE hModule)->void {
	UnhookWindowsHookEx(g_hHook);
}