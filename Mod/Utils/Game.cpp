#include "Game.h"
#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
#include "Player.h"
#include "Actor.h"
#include "AttributeInstance.h"
#include "GameMode.h"
#include "../Modules/ModuleManager.h"

bool Game::ModState = false;
ModuleManager* Game::modmag = nullptr;
LocalPlayer* Game::localplayer = nullptr;
BitmapFont* Game::mcfont = nullptr;
ClientInstance* Game::Cinstance = nullptr;

uintptr_t Game::KeyMap = 0x00;

auto Game::init() -> void
{
	logF("[Game::init] is start runner");

	Game::ModState = true;
	Game::modmag = new ModuleManager;
	Game::modmag->Init();
	logF("[ModuleManager::Disable] now init");


	//获取生物位置指针的偏移
	{

		Mob::setSprintingFunAddr = FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B ? ? ? ? ? 0F B6 ? 48 8B F1");
		if (Mob::setSprintingFunAddr == 0x00) {
			logF("[Game::init] [Warn]Find Mob::setSprintingFunAddr FunAddr is no working!!!,Mob::setSprintingFunAddr=0");
			return;
		}
	}
	//获取玩家视角的偏移地址 +15
	{
		auto PlayerView_sigOffset = FindSignature("0F B6 D0 48 8B CE E8 ? ? ? ? F2 0F 10 86 ? ? ? ? F2 0F 11 86");
		if (PlayerView_sigOffset == 0x00) {
			logF("[Game::init] [Warn]Find Player PlayerView_sigOffset Offset is no working!!!,PlayerView_sigOffset=0");
			return;
		}
		auto offset = *reinterpret_cast<int*>(PlayerView_sigOffset + 15);
		Player::Rot1 = offset;
		Player::Rot2 = offset + 8;
	}

	//获取 获取玩家血量函数的相关偏移
	{
		auto getHealthFun_sigOffset = FindSignature("48 8D 15 ? ? ? ? 48 8B CE FF 90 ? ? ? ? F3 0F 10 88 ? ? ? ? F3 0F 2C C9 66 0F 6E C1 0F 5B C0 8D 41 ? 0F 2F C1 0F 42 C8 85 C9");
		if (getHealthFun_sigOffset == 0x00) {
			logF("[Game::init] [Warn]Find Player getHealthFun_sigOffset Offset is no working!!!,getHealthFun_sigOffset=0");
			return;
		}
		auto offset = *reinterpret_cast<int*>(getHealthFun_sigOffset + 3);
		auto HEALTHVal = *reinterpret_cast<__int64*>(getHealthFun_sigOffset + 7 + offset);

		auto vtoffset = *reinterpret_cast<int*>(getHealthFun_sigOffset + 12);
		auto gvoffset = *reinterpret_cast<int*>(getHealthFun_sigOffset + 20);
		Actor::GetAttributeInstance_HealthFunVT = vtoffset;
		AttributeInstance::getCurrentValueoffset = gvoffset;


		if (Attribute::HEALTH != HEALTHVal) {
			logF("[Game::init] [Error]Attribute::HEALTH is Error,should is :%I64d", HEALTHVal);
		}
		if (vtoffset == 0x00) {
			logF("[Game::init] [Error]vtoffset == 0x00 ");
		}
		if (gvoffset == 0x00) {
			logF("[Game::init] [Error]gvoffset == 0x00 ");
		}
	}

	//获取Actor::isSneaking 函数位置
	{
		auto Actor_Sneaking_sigOffset = FindSignature("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 4F ? 48 85 C9 74 ? 48 83");
		if (Actor_Sneaking_sigOffset == 0x00) {
			logF("[Game::init] [Warn]Find Player Actor_Sneaking_sigOffset Offset is no working!!!,Actor_Sneaking_sigOffset=0");
			return;
		}
		auto offset = *reinterpret_cast<int*>(Actor_Sneaking_sigOffset + 1);
		Actor::isSneakingCallptr = Actor_Sneaking_sigOffset + offset + 5;
	}

	//KeyMap
	{
		auto sigOffset = FindSignature("4C 8D 05 ? ? ? ? 89 54 24 ? 88 4C 24");
		if (sigOffset == 0x0) {
			logF("[Game::init] [Warn]Find KeyMap Offset is no working!!!,KeyMap=0");
			return;
		}
		int offset = *reinterpret_cast<int*>((sigOffset + 3));      // Get Offset from code
		KeyMap = sigOffset + offset + /*length of instruction*/ 7;  // Offset is relative
		logF("KeyMap: %llX", KeyMap);
	}
}


auto Game::exit() -> void
{
	Game::modmag->Disable();
	delete[] Game::modmag;
	logF("[ModuleManager::Disable] now Disale");

	Game::ModState = false;
}

auto Game::GetModuleManager()->ModuleManager* {
	return Game::modmag;
}


auto Game::IsKeyDown(int key)->bool {
	// All keys are mapped as bools, though aligned as ints (4 byte)
	// key0 00 00 00 key1 00 00 00 key2 00 00 00 ...
	return *reinterpret_cast<bool*>(KeyMap + ((uintptr_t)key * 0x4));
}

/// <summary>
/// 判断一个按键是否按下
/// </summary>
/// <param name="key"></param>
/// <returns>如果判断时已经按下，则等到松开时再返回true</returns>
auto Game::IsKeyPressed(int key)->bool {
	if (IsKeyDown(key)) {
		while (IsKeyDown(key))
			Sleep(1);
		return true;
	}
	return false;
}
