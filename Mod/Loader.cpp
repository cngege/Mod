#include "Loader.h"
#include "Utils/Logger.h"
#include "Utils/Game.h"
#include "Hook/Hook.h"
#include "Utils/Utils.h"
#include "Player.h"
#include "Modules/ModuleManager.h"

#include "imgui/HookImgui.h"

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
