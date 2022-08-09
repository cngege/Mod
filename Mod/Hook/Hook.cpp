#include "Hook.h"
#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
#include "Player.h"


ClientInstance* instance;

using PlayerKBCALL = void(__fastcall*)(Player*, vec3_t*);
PlayerKBCALL playercall;
uintptr_t playerkb;

using ClientInstance_Tick = void(__fastcall*)(ClientInstance* _this, void* a1);
ClientInstance_Tick clientInstance_Tickcall;
uintptr_t clientInstanceTick;

// 获取一个值 是否在屏幕上显示位置
using Is_ShowCoordinates_Tick = bool(__fastcall*)(void* _this);
Is_ShowCoordinates_Tick is_ShowCoordinates_Tickcall;
uintptr_t is_ShowCoordinatesTick;


using GetHungerValAddress_Tick = void* (__fastcall*)(void* _this, const char* a1,void* a2);
GetHungerValAddress_Tick getHungerValAddress_Tickcall;
uintptr_t getHungerValAddressTick;

//快速破坏相关
using DestroyBlockingAddress = float (__fastcall*)(void* _this, void* a1, int a2, float a3);
DestroyBlockingAddress destroyBlockingAddresscall;
uintptr_t destroyBlockingAddress;

//48 89 5C 24 ? 57 48 83 EC 40 48 8B D9 48 8B FA 48 8B 89 ? ? ? ? 48 8B 01
using NoFallDamage_tick = void* (__fastcall*)(void* _this, float* a1);
NoFallDamage_tick noFallDamage_Tickcall;
uintptr_t noFallDamage_tick;

//0.6 0.2 4
//48 8B C4 48 89 58 ? 48 89 68 ? 56 57 41 56 48 83 EC 70 48 8B EA
using Covers_HitBox_Parts = void* (__fastcall*)(void* _this, void* a1, void* a2);
Covers_HitBox_Parts covers_HitBox_Partscall;
uintptr_t covers_HitBox_Parts;


using Player_Tick = double(__fastcall*)(Player* _this);
Player_Tick player_Tickcall;
uintptr_t player_Tick;


using AllPlayer_Tick = float* (__fastcall*)(Player*, float*, float);
AllPlayer_Tick allPlayer_Tickcall;
uintptr_t allPlayer_Tick;

auto Hook::init() -> void
{
	logF("Hook::init is start runner……");

	playerkb = FindSignature("8B 02 89 81 ? ? 00 00 8B 42 04 89 81 ? ? 00 00 8B 42 08 89 81 ? ? 00 00 C3");
	if (playerkb != 0x00) {
		Player::SpeedXOffset = *reinterpret_cast<int*>(playerkb + 4);
		Player::SpeedYOffset = *reinterpret_cast<int*>(playerkb + 13);
		Player::SpeedZOffset = *reinterpret_cast<int*>(playerkb + 22);
		MH_CreateHookEx((LPVOID)playerkb, &Hook::PlayerKB, &playercall);
		MH_EnableHook((LPVOID)playerkb);
	}else {
		logF("[Hook error] [%s] is no found Hook point", "playerkb");
	}

	//clientInstance::Tick
	clientInstanceTick = FindSignature("48 89 5C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 48 8B F9 48 8B 01");
	if (clientInstanceTick != 0x00) {
		MH_CreateHookEx((LPVOID)clientInstanceTick, &Hook::ClientInstance_Tick, &clientInstance_Tickcall);
		MH_EnableHook((LPVOID)clientInstanceTick);
	}
	else {
		logF("[Hook error] [%s] is no found Hook point", "clientInstanceTick");
	}


	//是否显示坐标 Tick
	is_ShowCoordinatesTick = FindSignature("48 83 EC ? 48 8B 49 ? 48 8B 01 FF 90 ? ? ? ? 48 85 C0 74 ? 48 8B 88");
	if (is_ShowCoordinatesTick != 0x00) {
		MH_CreateHookEx((LPVOID)is_ShowCoordinatesTick, &Hook::Is_ShowCoordinates_Tick, &is_ShowCoordinates_Tickcall);
		MH_EnableHook((LPVOID)is_ShowCoordinatesTick);
	}
	else {
		logF("[Hook error] [%s] is no found Hook point", "is_ShowCoordinatesTick");
	}

	//获取饥饿值的地址的函数  实际上被用来做修改玩家速度
	getHungerValAddressTick = FindSignature("4C 8B D1 44 0F B6 CA 49 BB ? ? ? ? ? ? ? ? 48 B8 ? ? ? ? ? ? ? ? 4C 33 C8 8B C2 4D 0F AF CB C1 E8 08 44 0F B6 C0 8B C2 4D 33 C8 C1 E8 10 4D 8B 42 08 4D 0F AF CB 0F B6 C8 4C 33 C9 8B C2 49 8B 4A 30 4D 0F AF CB 48 C1 E8 18 4C 33 C8 4D 0F AF CB 49 23 C9 48 C1 E1 04 49 03 4A 18 48 8B 41 08 49 3B C0 74 27 48 8B 09 3B 50 10 74 0E 48 3B C1 74 1A 48 8B 40 08 3B 50 10 75 F2 48 85 C0 49 0F 44 C0 49 3B C0 74 05 ? ? ? ? C3 48 8D 05 ? ? ? ? C3");
	if (getHungerValAddressTick != 0x00) {
		MH_CreateHookEx((LPVOID)getHungerValAddressTick, &Hook::GetHungerValAddress_Tick, &getHungerValAddress_Tickcall);
		MH_EnableHook((LPVOID)getHungerValAddressTick);
	}
	else {
		logF("[Hook error] [%s] is no found Hook point", "getHungerValAddressTick");
	}


	//获取能够加上偏移得出破坏进度的地址(第一个参数) 可用来快速破坏方块
	destroyBlockingAddress = FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC 30 48 8B D9 0F 29 74 24 ? 48 8B 49 08");
	if (destroyBlockingAddress != 0x00) {
		MH_CreateHookEx((LPVOID)destroyBlockingAddress, &Hook::DestroyBlocking, &destroyBlockingAddresscall);
		MH_EnableHook((LPVOID)destroyBlockingAddress);
	}else {
		logF("[Hook error] [%s] is no found Hook point", "destroyBlockingAddress");
	}

	//掉落无伤 仅本地有效
	noFallDamage_tick = FindSignature("48 89 5C 24 ? 57 48 83 EC 40 48 8B D9 48 8B FA 48 8B 89 ? ? ? ? 48 8B 01");
	if (noFallDamage_tick != 0x00) {
		MH_CreateHookEx((LPVOID)noFallDamage_tick, &Hook::NoFallDamage_Tick, &noFallDamage_Tickcall);
		MH_EnableHook((LPVOID)noFallDamage_tick);
	}else {
		logF("[Hook error] [%s] is no found Hook point", "noFallDamage_tick");
	}

	//获取 HitBox所要用的关键指针
	covers_HitBox_Parts = FindSignature("48 8B C4 48 89 58 ? 48 89 68 ? 56 57 41 56 48 83 EC 70 48 8B EA");
	if (covers_HitBox_Parts != 0x00) {
		MH_CreateHookEx((LPVOID)covers_HitBox_Parts, &Hook::Covers_HitBox_Parts, &covers_HitBox_Partscall);
		MH_EnableHook((LPVOID)covers_HitBox_Parts);
	}
	else {
		logF("[Hook error] [%s] is no found Hook point", "covers_HitBox_Parts");
	}


	// 本地玩家 Tick
	player_Tick = FindSignature("48 83 EC 28 48 8B 91 ? ? ? ? 45 33 C0 48 8B 81 ? ? ? ? 48 2B C2 48 C1 F8 03 66 44 3B C0 73 ? 48 8B 02");
	if (player_Tick != 0x00) {
		MH_CreateHookEx((LPVOID)player_Tick, &Hook::Player_Tick, &player_Tickcall);
		MH_EnableHook((LPVOID)player_Tick);
	}
	else {
		logF("[Hook error] [%s] is no found Hook point", "player_Tick");
	}


	//所有玩家TICK
	allPlayer_Tick = FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 30 48 8B 01 48 8B F2 0F 29 74 24 ? 48 8B D9 0F 28 F2");
	if (allPlayer_Tick != 0x00) {
		MH_CreateHookEx((LPVOID)allPlayer_Tick, &Hook::AllPlayer_Tick, &allPlayer_Tickcall);
		MH_EnableHook((LPVOID)allPlayer_Tick);
	}
	else {
		logF("[Hook error] [%s] is no found Hook point", "allPlayer_Tick");
	}
}

auto Hook::exit() -> void {
	MH_DisableHook(MH_ALL_HOOKS);
	Sleep(10);
}

//无击退效果
auto Hook::PlayerKB(Player* player,vec3_t* kb) -> void
{
	kb->x *= 0.1f;
	kb->y *= 0.1f;
	kb->z *= 0.1f;
	playercall(player,kb);
	/*
	auto p = (Player*)(instance->LocalPlayer());
	if(p != nullptr)
		p->setSpeed(vec3_t(0.0f, 2.0f, 0.0f));
	*/
	/*
	logF("this fun Player Ptr at: %llX", player);
	if (instance != nullptr) {
		logF("instance Ptr at: %llX", instance);
		logF("LocalPlayer Ptr at: %llX", instance->LocalPlayer());
		logF("no offset");
	}
	*/
}
//2.Hook后的函数 3.具有原来的功能的可以调用的函数
//MH_CreateHook(&MessageBoxW, &DetourMessageBoxW, reinterpret_cast<LPVOID*>(&fpMessageBoxW))
//Utils::FindSignatureModule("Minecraft.Windows.exe","8B 02 89 81 ? ? 00 00 8B 42 04 89 81 ? ? 00 00 8B 42 08 89 81 ? ? 00 00 C3");

auto Hook::ClientInstance_Tick(ClientInstance* _this, void* a1) -> void
{
	if (_this != nullptr) instance = _this;
	clientInstance_Tickcall(_this, a1);
}


auto Hook::Is_ShowCoordinates_Tick(void* _this)->bool
{
	is_ShowCoordinates_Tickcall(_this);
	return true;
}


// 实际很多变量都调用这个函数，速度 血量，饥饿，附加血量等
auto Hook::GetHungerValAddress_Tick(void* _this, const char* a1, void* a2)->void*
{
	auto ret = getHungerValAddress_Tickcall(_this, a1, a2);	// +0x84 = 132
	auto hunger = reinterpret_cast<float*>(reinterpret_cast<INT64>(ret) + 0x84);
	//速度
	if (*hunger == 0.1299999952f) {
		*hunger = 0.133f;
	}
	return ret;
}



auto Hook::DestroyBlocking(void* _this, void* a1, int a2, float a3)->float
{
	auto speedDestroy = reinterpret_cast<float*>(reinterpret_cast<INT64>(_this) + 0x24);
	if (*speedDestroy>0.0f&&*speedDestroy<1.0f) {
		*speedDestroy = 1.0f;
	}
	auto ret = destroyBlockingAddresscall(_this, a1,a2,a3);
	return ret;
}

//仅仅在本地房间时有效
auto Hook::NoFallDamage_Tick(void* _this, float* a1)->void*
{
	//this + 1D4
	*reinterpret_cast<float*>(reinterpret_cast<INT64>(_this) + 0x1D4) = 0.0f;
	return noFallDamage_Tickcall(_this, a1);
}


//HitBox
auto Hook::Covers_HitBox_Parts(void* _this, void* a1, void* a2)->void*
{
	// a1 + 4D0 无效
	// ret + 4D0
	auto ret = covers_HitBox_Partscall(_this, a1, a2);
	float* hitboxaddr = reinterpret_cast<float*>(reinterpret_cast<INT64>(ret) + 0x4D0);
	if (*hitboxaddr == 0.6000000238f) {
		*hitboxaddr = 4.0f;
	}
	return ret;
}


auto Hook::Player_Tick(Player* _this)->double
{
	static INT64 p = 0;
	auto thisp = reinterpret_cast<INT64>(_this);
	if (thisp != p) {
		p = thisp;
		logF("Player_Tick localplayer ptr : %llX，Clientinstance ptr : %llX", thisp, instance);
	}
	_this->onLocalPlayerTick();
	return player_Tickcall(_this);
}

auto Hook::AllPlayer_Tick(Player* _this, float* a1, float a2)->float* {

	if (_this != Player::LocalPlayer) {
		_this->setHitBox(vec2_t(6.0f, 6.0f));
	}
	return allPlayer_Tickcall(_this, a1, a2);
}