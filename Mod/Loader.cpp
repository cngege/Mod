#pragma execution_character_set("utf-8")
#include "Loader.h"
#include "http.hpp"
#include <thread>

#include "Utils/Logger.h"
#include "Utils/Game.h"
#include "Hook/Hook.h"
#include "Utils/Utils.h"
//Modules
#include "Modules/ModuleManager.h"

#include "imgui/HookImgui.h"
#include <io.h>
#include "Utils/config.h"

MH_STATUS hookret;
void* Loader::dllHMODULE = nullptr;



//在线程
void Loader::init(void* hmoudle)
{
	logF("DLL HMODULE: %llX", hmoudle);
	logF("Minecraft.Windows.exe base: %llX", Utils::getBase());
	dllHMODULE = hmoudle;
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
	logF("正在初始化ImGuiHook模块");
	ImguiHooks::InitImgui();
	logF("正在进行游戏进程Hook");
	Hook::init();

	logF("[MH_EnableHook] Hook状态: %s", MH_StatusToString(MH_EnableHook(MH_ALL_HOOKS)));

}

void Loader::exit(void* hmoudle)
{
	if (hookret == MH_OK)
	{
		logF("[Loader::exit] 正在关闭所有Hook");
		Hook::exit();
		logF("[Loader::exit] Hook解除状态: %s", MH_StatusToString(MH_Uninitialize()));
	}
	logF("[Loader::exit] 正在退出Game模块");
	Game::exit();

	logF("Removing logger");
	Logger::Disable();
	//关闭被注入程序的时候会调用
}

