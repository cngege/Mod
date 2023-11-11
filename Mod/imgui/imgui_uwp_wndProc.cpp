#include "imgui_uwp_wndProc.h"

#include <windows.h>
#include <windowsx.h> // GET_X_LPARAM(), GET_Y_LPARAM()

#include "Logger.h"

ImGuiMouseSource GetMouseSourceFromMessageExtraInfo()
{
    LPARAM extra_info = ::GetMessageExtraInfo();
    if ((extra_info & 0xFFFFFF80) == 0xFF515700)
        return ImGuiMouseSource_Pen;
    if ((extra_info & 0xFFFFFF80) == 0xFF515780)
        return ImGuiMouseSource_TouchScreen;
    return ImGuiMouseSource_Mouse;
}

void ImGui_ImplUWP_AddKeyEvent(ImGuiKey key, bool down, int native_keycode, int native_scancode = -1)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddKeyEvent(key, down);
	io.SetKeyEventNativeData(key, native_keycode, native_scancode); // To support legacy indexing (<1.87 user code)
	IM_UNUSED(native_scancode);
}

IMGUI_IMPL_API LRESULT ImGui_UWP_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

#ifdef IMGUIINPUT_USE_WNDPROC

	//logF_Debug("msg: %02x", msg);
	//if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
	//	return 1;
	//}

	switch (msg)
	{
	case DM_POINTERHITTEST:
		break;
	case WM_NCPOINTERDOWN:				// 鼠标按下?
	case WM_POINTERDOWN:
		break;
	case WM_NCPOINTERUP:
	case WM_POINTERUP:
		break;
	case WM_NCPOINTERUPDATE:
	case WM_POINTERUPDATE:	//ok
		break;
	case WM_PARENTNOTIFY:				// 创建销毁子窗口时的消息
		break;
	case WM_POINTERACTIVATE:			// 忽略
		break;
	case WM_POINTERCAPTURECHANGED:		// 可能是鼠标移开游戏窗口
		break;
	case WM_POINTERDEVICECHANGE:		// 忽略 当显示模式缩放时?
	case WM_POINTERDEVICEINRANGE:
	case WM_POINTERDEVICEOUTOFRANGE:
		break;
	case WM_POINTERENTER:				// 鼠标悬停或移动
		break;
	case WM_POINTERLEAVE:				// 移出窗口 ok
		break;
	case WM_POINTERROUTEDAWAY:			// 什么路由到下一进程
	case WM_POINTERROUTEDRELEASED:		// 和跨进程相关
	case WM_POINTERROUTEDTO:
		break;
	case WM_POINTERWHEEL:				// 鼠标滚轮
		break;
	case WM_POINTERHWHEEL:				// 横向滚轮
		break;
	case WM_TOUCHHITTESTING:
		break;
		// 以下是非官方链接中给出的消息
	case WM_MOUSEMOVE:					//0x200
		break;

	case WM_SETCURSOR:	//0x20
	case WM_MOUSEACTIVATE://0x21
	case WM_NCHITTEST:	//0x84 命中测试
	case WM_IME_SETCONTEXT://0x281
	case WM_IME_NOTIFY://0x282
	case WM_SETFOCUS://0x7
	case WM_KILLFOCUS://0x8
	case WM_INPUT://0xFF
	case WM_SYSKEYDOWN://0x104
	case WM_SYSKEYUP://0x105	直接按下F10之类的健
	case WM_SYSCHAR://0x106		ALT+字符健
	case WM_SYSCOMMAND://0x112

		break;
	case 0xC07D://可能是指切换窗口
	case 0xC1D0:
	case 0x349:
		break;
	default:
		logF_Debug("defaultmsg: %02x", msg);
		break;
	}
#endif // IMGUIINPUT_USE_WNDPROC

	switch (msg)
	{
	case WM_SETFOCUS://0x7
	case WM_KILLFOCUS://0x8
		{
			ImGuiIO& io = ImGui::GetIO();
			//io.AddFocusEvent(msg == WM_SETFOCUS);
		}
	default:
		break;
	}

    return 0;
}