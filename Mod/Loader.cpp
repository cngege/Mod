#include "Loader.h"
#include "version.h"
#include "http.hpp"
#include <thread>
#include "Utils/Logger.h"
#include "Utils/Game.h"
#include "Hook/Hook.h"
#include "Utils/Utils.h"
//Modules
#include "Modules/ModuleManager.h"
//imgui hook
#include "imgui/HookImgui.h"
#include <io.h>
//config
#include "Utils/config.h"

MH_STATUS hookret;
void* Loader::dllHMODULE		= nullptr;
bool  Loader::Eject_Signal		= false;
bool  Loader::RemoteFreeLib		= false;
bool  Loader::EnableEjectKey	= true;

static DWORD WINAPI FreeLibraryThread(LPVOID lpParam);

//在线程
void Loader::init(void* hmodule)
{
	logF("DLL VER: %s at %s , HMODULE: %llX", FILE_VERSION_FILE_VERSION_STRING, __TIMESTAMP__ , hmodule);
	std::string mcVersion = Utils::getMCVersion();
	logF("Minecraft.Windows.exe base: %llX, Ver: %s", Utils::getBase(), mcVersion.c_str());

	dllHMODULE = hmodule;
	hookret = MH_Initialize();
	if (hookret != MH_OK)
	{
		logF("MH_Initialize Error ret is %s ,Mod Return", MH_StatusToString(hookret));
		return;
	}
	
	//创建相关文件夹与 下载字体
	{
		std::wstring modPath = Logger::GetRoamingFolderPath() + Utils::stringToWstring("\\Mod\\");
		std::wstring assetsPath = Logger::GetRoamingFolderPath() + Utils::stringToWstring("\\Mod\\Assets\\");
		std::wstring configPath = Logger::GetRoamingFolderPath() + Utils::stringToWstring("\\Mod\\Config\\");
		std::string font_JNMYT = Utils::WStringToString(Logger::GetRoamingFolderPath()) + std::string("\\Mod\\Assets\\JNMYT.ttf");

		//检测 资源等相关文件夹是否存在并创建
		CreateDirectory((LPCWSTR)(modPath).c_str(), NULL);
		CreateDirectory((LPCWSTR)(assetsPath).c_str(), NULL);
		CreateDirectory((LPCWSTR)(configPath).c_str(), NULL);
		{
			//检测字体文件是否存在及下载
			if (_access(font_JNMYT.c_str(),0 /*F_OK*/) == -1) {
				//线程网络下载
				std::thread netdownload([font_JNMYT]() {
					logF("[Loader::init] 线程中: 正在下载字体文件 JNMYT.ttf");

					auto client = NetClient::GetAsync("https://raw.githubusercontent.com/cngege/Mod/master/Assets/Fonts/JNMYT.ttf");
					if (!client.StatusSuccess()) {
						logF("[Loader::init Error]Font Download fail AS StatusSuccess = false, statuscode = %i",client.StatusCode);
						return;
					}
					auto buff = client.GetAsyncAsBuffer();
					if (!NetClient::WriteFile(font_JNMYT, buff)) {
						logF("[Loader::init Error]Font Download fail AS WriteFile fail");
						return;
					}
					logF("[Loader::init Success]Font Download Success, restart work");
				});
				netdownload.detach();
			}
		}
	}
	logF("正在初始化Game模块");
	Game::init();
	logF("正在初始化ImGuiHook模块");		// Hook 的先后顺序无关紧要，因为都是统一的开启Hook MH_EnableHook(MH_ALL_HOOKS)
	ImguiHooks::InitImgui();			// 23/9/21 最后检测结果, 注释此条则Mod不会崩溃
	logF("正在进行游戏进程Hook");
	Hook::init();

	logF("[MH_EnableHook] Hook状态: %s", MH_StatusToString(MH_EnableHook(MH_ALL_HOOKS)));
	CreateThread(NULL, NULL, FreeLibraryThread, hmodule, NULL, NULL);
}

static DWORD WINAPI FreeLibraryThread(LPVOID lpParam) {
	while (!Loader::Eject_Signal)
	{
		Sleep(500);
		if (Loader::RemoteFreeLib) {
			logF("[thread while] 检测远程释放库信号.");
			return 0;
		}
	}
	logF("[thread while] 检测到退出信号.");
	::FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), 0);		//只能退出 CreateThread 创建的线程
	return 0;
}

void Loader::exit(void* hmodule)
{
	if (Game::ModState) {
		logF("=============================="); // =x30
		if (hookret == MH_OK)
		{
			logF("[Loader::exit] 正在关闭所有Hook");
			Hook::exit();
			logF("[Loader::exit] Hook解除状态: %s", MH_StatusToString(MH_Uninitialize()));
			hookret = MH_STATUS::MH_UNKNOWN;
		}
		logF("[Loader::exit] 正在关闭D3D12渲染");
		ImguiHooks::CloseImGui();
		logF("[Loader::exit] 正在退出Game模块");
		Game::exit();
		logF("[Loader::exit] Removing logger");
		Logger::Disable();
		//关闭被注入程序的时候会调用
	}
}

