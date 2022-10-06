//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "Loader.h"

#include "httplib.h"
#include <Windows.h>

#include "Utils/Logger.h"
#include "Utils/Game.h"
#include "Hook/Hook.h"
#include "Utils/Utils.h"

#include "Player.h"
#include "Modules/ModuleManager.h"

#include "imgui/HookImgui.h"
#include <io.h>
#include <thread>

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
	
	{
		std::wstring modPath = Logger::GetRoamingFolderPath() + Utils::stringToWstring("\\Mod\\");
		std::wstring assetsPath = Logger::GetRoamingFolderPath() + Utils::stringToWstring("\\Mod\\Assets\\");
		std::string font_JNMYT = Utils::WStringToString(Logger::GetRoamingFolderPath()) + std::string("\\Mod\\Assets\\JNMYT.ttf");

		//检测 资源等相关文件夹是否存在并创建
		CreateDirectory((LPCWSTR)(modPath).c_str(), NULL);
		CreateDirectory((LPCWSTR)(assetsPath).c_str(), NULL);
		{
			//检测字体文件是否存在及下载
			if (_access(font_JNMYT.c_str(),0 /*F_OK*/) == -1) {
				//线程网络下载
				//std::thread netdownload([font_JNMYT]() {
				//	logF("[Loader::init] 线程中: 正在下载字体文件");
				//	httplib::Client C_Font_JNMYT("https://raw.githubusercontent.com");
				//	//httplib::Client client("https://raw.githubusercontent.com/cngege/Mod/master/Assets/Fonts/FGMRT.ttf");
				//	C_Font_JNMYT.enable_server_certificate_verification(false);
				//	auto res = C_Font_JNMYT.Get("/cngege/Mod/master/Assets/Fonts/JNMYT.ttf");
				//	if (res)
				//	{
				//		if (res->status == 200) {
				//			std::ofstream out;
				//			out.open(font_JNMYT.c_str(), std::ios_base::binary | std::ios::out);
				//			if (out.is_open())
				//			{
				//				out << res->body;
				//				out.close();
				//				logF("[Loader::init] 字体文件下载完成,下次启动时自动加载");
				//			}
				//			else
				//			{
				//				logF("[Loader::init] 字体文件下载失败,打开文件准备写入时打开失败");
				//			}
				//		}
				//	}
				//	else 
				//	{
				//		logF("[Loader::init] 下载字体文件失败：%s", httplib::to_string(res.error()).c_str());
				//	}
				//});
				//netdownload.detach();
			}
		}
	}
	
	Game::init();
	ImguiHooks::InitImgui();
	Hook::init();

	logF("[MH_EnableHook] hook is: %s", MH_StatusToString(MH_EnableHook(MH_ALL_HOOKS)));
}

void Loader::exit(void* hmoudle)
{
	if (hookret == MH_OK)
	{
		logF("[Hook::exit] Hook IS UnLoad");
		Hook::exit();
		logF("[MH_Uninitialize] is: %s", MH_StatusToString(MH_Uninitialize()));
	}
	Game::exit();

	logF("Removing logger");
	Logger::Disable();
	//关闭被注入程序的时候会调用
}
