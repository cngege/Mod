#pragma execution_character_set("utf-8")
#include "Hook.h"
#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
#include "../Utils/Game.h"
#include "imgui.h"
#include "Player.h"
#include "ServerPlayer.h"
#include "LocalPlayer.h"
#include "RemotePlayer.h"
#include "Actor.h"
#include "Level.h"
#include "GameMode.h"
#include "MinecraftUIRenderContext.h"

#include "Item.h"
#include "FishingHook.h"
#include "ItemInstance.h"
#include "ItemStack.h"

#include "../Modules/ModuleManager.h"
#include "../Modules/Modules/HitBox.h"
#include "../Modules/Modules/InstantDestroy.h"
#include "../Modules/Modules/ShowCoordinates.h"
#include "../Modules/Modules/FastViewPerspective.h"
#include "../Modules/Modules/AutoSprinting.h"
#include "../Modules/Modules/AutoWalking.h"

using LockSprinting = void(__fastcall*)(void* a, void* b);
LockSprinting lockSprintingcall;
int LockSprinting_offset = 0;

using ClientInstance_Tick = void(__fastcall*)(ClientInstance* _this, void* a1);
ClientInstance_Tick clientInstance_Tickcall;
uintptr_t clientInstanceTick;

// 获取一个值 是否在屏幕上显示位置
using Is_ShowCoordinates_Tick = bool(__fastcall*)(void* _this);
Is_ShowCoordinates_Tick is_ShowCoordinates_Tickcall;
uintptr_t is_ShowCoordinatesTick;


//using GetHungerValAddress_Tick = void* (__fastcall*)(void* _this, const char* a1,void* a2);
//GetHungerValAddress_Tick getHungerValAddress_Tickcall;
//uintptr_t getHungerValAddressTick;

//48 89 5C 24 ? 57 48 83 EC 40 48 8B D9 48 8B FA 48 8B 89 ? ? ? ? 48 8B 01
using NoFallDamage_tick = void* (__fastcall*)(void* _this, float* a1);
NoFallDamage_tick noFallDamage_Tickcall;
uintptr_t noFallDamage_tick;

//0.6 0.2 4
//48 8B C4 48 89 58 ? 48 89 68 ? 56 57 41 56 48 83 EC 70 48 8B EA
using Covers_HitBox_Parts = void* (__fastcall*)(void* _this, void* a1, void* a2);
Covers_HitBox_Parts covers_HitBox_Partscall;
uintptr_t covers_HitBox_Parts;


using LocalPlayer_getCameraOffset = vec2_t*(__fastcall*)(LocalPlayer* _this);
LocalPlayer_getCameraOffset localplayer_getCameraOffsetcall;
uintptr_t localplayer_getCameraOffset;


using AllActor_Tick = float* (__fastcall*)(Actor*, float*, float);
AllActor_Tick allActor_Tickcall;
uintptr_t allActor_Tick;


//48 89 5C 24 ? 57 48 83 EC ? F3 0F 10 02 48 8B D9 F3 0F 58 81
//这里第二个参数是三个float（三个四字节）
//using Actor_moveBBs = void* (__fastcall*)(Actor*, vec3_t*);
//Actor_moveBBs actor_moveBBscall;
//uintptr_t actor_moveBBs;

using KeyUpdate = void* (__fastcall*)(__int64 key, int isdown);
KeyUpdate keyupdatecall;
uintptr_t keyupdate;

using MouseUpdate = void(__fastcall*)(__int64, char, char, __int16, __int16, __int16, __int16, char);
MouseUpdate mouseupdatecall;
uintptr_t mouseupdate;

using RenderDetour = void(__fastcall*)(void*, MinecraftUIRenderContext*);
RenderDetour renderDetourcall;
uintptr_t renderDetour;

using SendChatMessage = uint8_t(__fastcall*)(void*, TextHolder*);
SendChatMessage sendChatMessagecall;
uintptr_t sendChatMessage;

using GetViewPerspective = int(__fastcall*)(void*);
GetViewPerspective getLocalPlayerViewPerspectivecall;

using LPLP = void* (__fastcall*)(void*, void*, void*, void*, int, void*, char, void*, void*, void*, void*, void*);
LPLP lplpcall;

auto Hook::init() -> void
{
	logF("[Hook::init] 正在初始化");

	// 锁定疾跑 强制按下疾跑键 | 并不能确定被Hook的这个函数是什么
	{
		const char* memcode = "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 0F B6 41";
		auto lockSprintingSign = FindSignature(memcode);
		if (lockSprintingSign != 0x00) {
			MH_CreateHookEx((LPVOID)lockSprintingSign, &Hook::LockSprinting, &lockSprintingcall);
			LockSprinting_offset = (int)*reinterpret_cast<byte*>(lockSprintingSign + 18);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", lockSprintingSign, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "lockSprintingSign");
		}
	}

	//clientInstance::Tick
	{
		const char* memcode = "48 89 5C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 48 8B F9 48 8B 01";
		clientInstanceTick = FindSignature(memcode);
		if (clientInstanceTick != 0x00) {
			MH_CreateHookEx((LPVOID)clientInstanceTick, &Hook::ClientInstance_Tick, &clientInstance_Tickcall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", clientInstanceTick, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "clientInstanceTick");
		}
	}



	//是否显示坐标 Tick
	{
		const char* memcode = "48 83 EC ? 48 8B 49 ? 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 85 C0 74 ? 48 8B 88";
		is_ShowCoordinatesTick = FindSignature(memcode);
		if (is_ShowCoordinatesTick != 0x00) {
			MH_CreateHookEx((LPVOID)is_ShowCoordinatesTick, &Hook::Is_ShowCoordinates_Tick, &is_ShowCoordinates_Tickcall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", is_ShowCoordinatesTick, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "is_ShowCoordinatesTick");
		}
	}

	//获取饥饿值的地址的函数  实际上被用来做修改玩家速度
	//没有作用，准备删除
	//{
	//	const char* memcode = "4C 8B D1 44 0F B6 CA 49 BB ? ? ? ? ? ? ? ? 48 B8 ? ? ? ? ? ? ? ? 4C 33 C8 8B C2 4D 0F AF CB C1 E8 08 44 0F B6 C0 8B C2 4D 33 C8 C1 E8 10 4D 8B 42 08 4D 0F AF CB 0F B6 C8 4C 33 C9 8B C2 49 8B 4A 30 4D 0F AF CB 48 C1 E8 18 4C 33 C8 4D 0F AF CB 49 23 C9 48 C1 E1 04 49 03 4A 18 48 8B 41 08 49 3B C0 74 27 48 8B 09 3B 50 10 74 0E 48 3B C1 74 1A 48 8B 40 08 3B 50 10 75 F2 48 85 C0 49 0F 44 C0 49 3B C0 74 05 ? ? ? ? C3 48 8D 05 ? ? ? ? C3";
	//	getHungerValAddressTick = FindSignature(memcode);
	//	if (getHungerValAddressTick != 0x00) {
	//		MH_CreateHookEx((LPVOID)getHungerValAddressTick, &Hook::GetHungerValAddress_Tick, &getHungerValAddress_Tickcall);
	//		logBF("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", getHungerValAddressTick, memcode);
	//	}
	//	else {
	//		logF("[Hook error] [%s] is no found Hook point", "getHungerValAddressTick");
	//	}
	//}

	//掉落无伤 仅本地有效
	{
		const char* memcode = "48 89 5C 24 ? 57 48 83 EC 40 48 8B D9 48 8B FA 48 8B 89 ? ? ? ? 48 8B 01";
		noFallDamage_tick = FindSignature(memcode);
		if (noFallDamage_tick != 0x00) {
			MH_CreateHookEx((LPVOID)noFallDamage_tick, &Hook::NoFallDamage_Tick, &noFallDamage_Tickcall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", noFallDamage_tick, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "noFallDamage_tick");
		}
	}

	//Level::Tick 无直接调用者
	{
		const char* memcode = "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B F1 0F 57 C0";
		auto level_tick = FindSignature(memcode);
		if (level_tick != 0x00) {
			MH_CreateHookEx((LPVOID)level_tick, &Hook::Level_Tick, &Level::tickCall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", level_tick, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "level_tick");
		}
	}

	// 本地玩家 Tick  改为获取虚表后Hook
	//{
	//	const char* memcode = "48 83 EC 28 48 8B 91 ? ? ? ? 45 33 C0 48 8B 81 ? ? ? ? 48 2B C2 48 C1 F8 03 66 44 3B C0 73 ? 48 8B 02";
	//	localplayer_getCameraOffset = FindSignature(memcode);
	//	if (localplayer_getCameraOffset != 0x00) {
	//		MH_CreateHookEx((LPVOID)localplayer_getCameraOffset, &Hook::LocalPlayer_getCameraOffset, &localplayer_getCameraOffsetcall);
	//		logF("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", localplayer_getCameraOffset, memcode);
	//	}
	//	else {
	//		logF("[Hook error] [%s] is no found Hook point", "player_getCameraOffset");
	//	}
	//}

	//所有玩家TICK ,所有生物TICK,对玩家来说应该是渲染相关得函数,只有看向 那个玩家，那个玩家才会触发这个函数，其中出现的玩家指针是Player 不是ServerPlayer
	//非掉落物实体，都只会在视野中才会触发  这个特征码比较旧了，不知道Hook的函数正不正确
	{
		const char* memcode = "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 30 48 8B 01 48 8B F2 0F 29 74 24 ? 48 8B D9 0F 28 F2";
		allActor_Tick = FindSignature(memcode);
		if (allActor_Tick != 0x00) {
			MH_CreateHookEx((LPVOID)allActor_Tick, &Hook::AllActor_Tick, &allActor_Tickcall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", allActor_Tick, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "allActor_Tick");
		}
	}

	//生物移动消息
	{
		const char* memcode = "40 53 48 83 EC ? 48 8B 81 ? ? ? ? 48 8B D9 48 85 C0 0F 84 ? ? ? ? F3 0F 10";
		auto actor_moveBBs = FindSignature(memcode);
		if (actor_moveBBs != 0x00) {
			Actor::AABBOffset = *reinterpret_cast<int*>(actor_moveBBs + 9);//这个结果应该是由Actor指向AABB类的偏移
			//Actor::PosXOffset1 = Xoffset;
			//Actor::PosYOffset1 = Xoffset + 4;
			//Actor::PosZOffset1 = Xoffset + 8;
			//Actor::PosXOffset2 = Xoffset + 12;
			//Actor::PosYOffset2 = Xoffset + 16;
			//Actor::PosZOffset2 = Xoffset + 20;

			//Actor::XHitBoxOffset = Xoffset + 24;
			//Actor::YHitBoxOffset = Xoffset + 28;
			//MH_CreateHookEx((LPVOID)actor_moveBBs, &Hook::Actor_moveBBs, &actor_moveBBscall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", actor_moveBBs, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "actor_moveBBs");
		}
	}

	//KeyUpdate
	{
		const char* memcode = "48 83 EC ? 0F B6 C1 4C 8D 05 ? ? ? ? 89 54 24";
		keyupdate = FindSignature(memcode);
		if (keyupdate != 0x00) {
			MH_CreateHookEx((LPVOID)keyupdate, &Hook::KeyUpdate, &keyupdatecall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", keyupdate, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "keyupdate");
		}
	}

	//MouseUpdate
	{
		const char* memcode = "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F";
		mouseupdate = FindSignature(memcode);
		if (keyupdate != 0x00) {
			MH_CreateHookEx((LPVOID)mouseupdate, &Hook::MouseUpdate, &mouseupdatecall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", mouseupdate, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "mouseupdate");
		}
	}

	// 渲染
	{
		const char* memcode = "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? ? ? 70 B8 ? ? 78 A8 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C";
		renderDetour = FindSignature(memcode);
		if (renderDetour != 0x00) {
			MH_CreateHookEx((LPVOID)renderDetour, &Hook::RenderDetour,&renderDetourcall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", renderDetour, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "renderDetour");
		}
	}

	{
		uintptr_t muicDrawText;
		const char* memcode = "48 89 5C 24 ? 55 56 57 48 83 EC ? 4C 8B 94 24 ? ? ? ? 48 8D";
		muicDrawText = FindSignature(memcode);
		if (muicDrawText != 0x00) {
			MH_CreateHookEx((LPVOID)muicDrawText, &Hook::Draw_Text, &MinecraftUIRenderContext::drawtextCall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", muicDrawText, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "muicDrawText");
		}
	}

	//SendChatMessage
	{
		const char* memcode = "E8 ? ? ? ? 3C ? 75 ? 48 8B 8F ? ? ? ? 48 8B 01 4C 89 75";
		auto findptr = FindSignature(memcode);
		if (findptr != 0x00) {
			auto offset = *reinterpret_cast<int*>(findptr + 1);
			sendChatMessage = findptr + 5 + offset;
			MH_CreateHookEx((LPVOID)sendChatMessage, &Hook::sendMessage, &sendChatMessagecall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", findptr, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "sendChatMessage");
		}
	}

	//获取本地玩家人称视角函数
	{
		const char* memcode = "48 83 EC ? 48 8B ? 48 8D 54 24 ? 41 B8 02 00 00 00";
		auto findptr = FindSignature(memcode);
		if (findptr != 0x00) {
			MH_CreateHookEx((LPVOID)findptr, &Hook::getLocalPlayerViewPerspective, &getLocalPlayerViewPerspectivecall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", findptr, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "getLocalPlayerViewPerspective");
		}
	}

	//寻找 Level::foreachplayer 的call 非Hook
	{
		const char* memcode = "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B EA 48";
		Level::forEachPlayerCall = (uintptr_t*)FindSignature(memcode);
		if (Level::forEachPlayerCall != 0x00) {
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", Level::forEachPlayerCall, memcode);
		}
		else {
			logF("[FindCallPtr error] [%s] is no found ptr point", "Level::forEachPlayerCall");
			return;
		}
	}

	//Level::startLeaveGame Hook
	{
		const char* memcode = "4C 8B DC 49 89 5B ? 49 89 73 ? 57 48 81 EC ? ? ? ? 48 8D 05";
		auto findptr = FindSignature(memcode);
		if (findptr != 0x00) {
			MH_CreateHookEx((LPVOID)findptr, &Hook::level_startLeaveGame, &Level::startLeaveGameCall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", findptr, memcode);
		}
		else {
			logF("[Hook error] [%s] is no found Hook point", "Level::startLeaveGame");
		}
	}

	//ItemStack虚表 来自Player::completeUsingItem
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 85 ? ? ? ? 44 89 BD";
		auto ItemStackVT_sigOffset = FindSignature(memcode);
		if (ItemStackVT_sigOffset == 0x00) {
			logF("[ItemStack::SetVtables] [Error]Find ItemStackVTable_sigOffset is no working!!!");
		}
		else {
			auto ItemStackVT = Utils::FuncFromSigOffset<uintptr_t**>(ItemStackVT_sigOffset, 3);
			logF_Debug("[ItemStack::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", ItemStackVT, ItemStackVT_sigOffset, memcode);
			ItemStack::SetVFtables(ItemStackVT);
		}
	}

	//ItemStackBase虚表 来自Player::completeUsingItem
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 45 ? 45 33 FF 4C";
		auto ItemStackBaseVT_sigOffset = FindSignature(memcode);
		if (ItemStackBaseVT_sigOffset == 0x00) {
			logF("[ItemStackBase::SetVtables] [Error]Find ItemStackBaseVTable_sigOffset is no working!!!");
		}
		else {
			auto ItemStackBaseVT = Utils::FuncFromSigOffset<uintptr_t**>(ItemStackBaseVT_sigOffset, 3);
			logF_Debug("[ItemStackBase::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", ItemStackBaseVT, ItemStackBaseVT_sigOffset, memcode);
			ItemStackBase::SetVFtables(ItemStackBaseVT);
		}
	}

	//ItemInstance 虚表 来自Player::completeUsingItem
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 45 ? 49 8D 96";
		auto ItemInstanceVT_sigOffset = FindSignature(memcode);
		if (ItemInstanceVT_sigOffset == 0x00) {
			logF("[ItemInstance::SetVtables] [Error]Find ItemInstanceVTable_sigOffset is no working!!!");
		}
		else {
			auto ItemInstanceVT = Utils::FuncFromSigOffset<uintptr_t**>(ItemInstanceVT_sigOffset, 3);
			logF_Debug("[ItemInstance::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", ItemInstanceVT, ItemInstanceVT_sigOffset, memcode);
			ItemInstance::SetVFtables(ItemInstanceVT);
		}
	}

	//GameMode虚表及相关Hook
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 01 48 89 51 ? 48 C7 41 ? ? ? ? ? C7 41 ? ? ? ? ? 44 88 61";
		auto GameModeVTable_sigOffset = FindSignature(memcode);
		if (GameModeVTable_sigOffset == 0x00) {
			logF("[GameMode::SetVtables] [Error]Find GameMode GameModeVTable_sigOffset is no working!!!");
		}
		else {
			auto GameModeVTables = Utils::FuncFromSigOffset<uintptr_t**>(GameModeVTable_sigOffset, 3);
			logF_Debug("[GameMode::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", GameModeVTables, GameModeVTable_sigOffset, memcode);
			GameMode::SetVFtables(GameModeVTables);
			//Hook GameMode_startDestroyBlock
			MH_CreateHookEx((LPVOID)GameMode::GetVFtableFun(1), &Hook::GameMode_startDestroyBlock, &GameMode::startDestroyBlockCall);
			//Hook GameMode_tick
			MH_CreateHookEx((LPVOID)GameMode::GetVFtableFun(9), &Hook::GameMode_tick, &GameMode::tickCall);
			//Hook GameMode_useItem
			MH_CreateHookEx((LPVOID)GameMode::GetVFtableFun(11), &Hook::GameMode_useItem, &GameMode::useItemCall);
			//Hook GameMode_useItemOn
			MH_CreateHookEx((LPVOID)GameMode::GetVFtableFun(12), &Hook::GameMode_useItemOn, &GameMode::useItemOnCall);
			//Hook GameMode_attack
			MH_CreateHookEx((LPVOID)GameMode::GetVFtableFun(14), &Hook::GameMode_attack, &GameMode::attackCall);
		}
	}

	//Actor 虚表及相关Hook  构造函数特征码：48 89 5C 24 ? 55  56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 8B D8 48 8B FA 4C 8B F1
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 01 49 8B 00 48 89 41 ? 41 8B 40 ? 89 41";
		auto ActorVTable_sigOffset = FindSignature(memcode);
		if (ActorVTable_sigOffset == 0x00) {
			logF("[Actor::SetVtables] [Error]Find Actor ActorVTable_sigOffset is no working!!!");
		}
		else
		{
			auto ActorVTable = Utils::FuncFromSigOffset<uintptr_t**>(ActorVTable_sigOffset, 3);
			logF_Debug("[GameMode::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", ActorVTable, ActorVTable_sigOffset, memcode);
			Actor::SetVFtables(ActorVTable);
			//虚表Hook
			//Actor::setVelocity
			MH_CreateHookEx((LPVOID)Actor::GetVFtableFun(48), &Hook::SetVelocity, &Actor::setVelocityCallptr);
			Actor::SpeedOffset = *reinterpret_cast<int*>((uintptr_t)Actor::GetVFtableFun(48) + 7);
			MH_CreateHookEx((LPVOID)Actor::GetVFtableFun(82), &Hook::Actor_getShadowRadius, &Actor::getShadowRadiusCallptr);

		}
	}

	//Actor构造函数下的 获取Actor类中的Level指针偏移
	{
		const char* memcode = "49 89 BE ? ? ? ? E8 ? ? ? ? 48 8B D0 49 8D 8E ? ? ? ? E8";		//来自三参数Actor构造函数
		const char* memcode2 = "48 8D 05 ? ? ? ? 48 89 01 49 8B 01 48 89 41 ? 41 8B 41";		//来自四参数Actor构造函数
		auto ActorGetLevel_sigOffset = FindSignature(memcode);
		if (ActorGetLevel_sigOffset == 0x00) {
			logF_Debug("[Hook::FindSignature] [%s] [!] 使用第一个特征码查找\"地址偏移\"失败","Actor::LevelOffset");
			ActorGetLevel_sigOffset = FindSignature(memcode2);
			if (ActorGetLevel_sigOffset == 0x00) {
				logF("[Hook::FindSignature] [%s] [Error] 使用两个特征码查找\"地址偏移\"全部失败", "Actor::LevelOffset");
			}
			else {
				Actor::LevelOffset = *reinterpret_cast<int*>(ActorGetLevel_sigOffset + 3);
				logF_Debug("[Actor::LevelOffset] [Success] 偏移地址= %i , sigoffset= %llX , memcode=%s", Actor::LevelOffset, ActorGetLevel_sigOffset, memcode2);
			}
		}
		else {
			Actor::LevelOffset = *reinterpret_cast<int*>(ActorGetLevel_sigOffset + 3);
			logF_Debug("[Actor::LevelOffset] [Success] 偏移地址= %i , sigoffset= %llX , memcode=%s", Actor::LevelOffset, ActorGetLevel_sigOffset, memcode);
		}
	}

	//FishingHook 虚表
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 03 C7 83 ? ? ? ? ? ? ? ? C7 83 ? ? ? ? ? ? ? ? 48 C7 83";
		auto FishingHookVTable_sigoffset = FindSignature(memcode);
		if (FishingHookVTable_sigoffset == 0x00) {
			logF("[FishingHook::SetVtables] [Error]Find VTable_sigOffset is no working!!!");
		}
		else {
			auto FishingHookVT = Utils::FuncFromSigOffset<uintptr_t**>(FishingHookVTable_sigoffset, 3);
			logF_Debug("[FishingHook::SetVtables] [Success] 虚表地址= %llX , sigoffset= %llX , memcode=%s", FishingHookVT, FishingHookVTable_sigoffset, memcode);
			FishingHook::SetVFtables(FishingHookVT);
		}

	}

	// Mob 虚表及相关Hook
	{
		const char* memcode = "40 53 56 57 48 81 EC ? ? ? ? 49 8B D8 48 8B F9 48 89 4C 24 ? E8 ? ? ? ? 90 48 8D 05";	//来自三参数Mob构造函数
		const char* memcode2 = "48 8D 05 ? ? ? ? 48 89 07 33 F6 48 89 B7 ? ? ? ? 48 89 B7 ? ? ? ? 48 89 B7";		//来自四参数Mob构造函数
		auto MobVTable_sigOffset = FindSignature(memcode); //+31
		if (MobVTable_sigOffset == 0x00) {
			logF("[Mob::SetVtables] [Error]Find Mob MobVTable_sigOffset is no working!!!");
		}
		else
		{
			auto offsize = *reinterpret_cast<int*>(MobVTable_sigOffset + 31);
			auto MobVTable = reinterpret_cast<uintptr_t**>(MobVTable_sigOffset + offsize + 35);
			logF_Debug("[Mob::SetVtables] [Success] 虚表地址= %llX , sigoffset= %llX , memcode=%s", MobVTable, MobVTable_sigOffset, memcode);
			Mob::SetVFtables(MobVTable);
			//虚表Hook
		}
	}

	//Player 虚表及相关Hook
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 07 45 33 E4 44 89 A7 ? ? ? ? 44 88 A7";
		auto PlayerVTable_sigOffset = FindSignature(memcode);
		if (PlayerVTable_sigOffset == 0x00) {
			logF("[Player::SetVtables] [Error]Find Player PlayerVTable_sigOffset is no working!!!");
		}
		else {
			auto PlayerVTable = Utils::FuncFromSigOffset<uintptr_t**>(PlayerVTable_sigOffset, 3);
			logF_Debug("[Player::SetVtables] [Success] 虚表地址= %llX , sigoffset= %llX , memcode=%s", PlayerVTable, PlayerVTable_sigOffset, memcode);
			Player::SetVFtables(PlayerVTable);
			//虚表Hook
			MH_CreateHookEx((LPVOID)Player::GetVFtableFun(80), &Hook::LocalPlayer_getCameraOffset, &localplayer_getCameraOffsetcall);
			//Player::getShadowRadius
			MH_CreateHookEx((LPVOID)Player::GetVFtableFun(82), &Hook::Player_getShadowRadius, &Player::getShadowRadiusCallptr);
			//Player::tickWorld
			MH_CreateHookEx((LPVOID)Player::GetVFtableFun(369), &Hook::Player_tickWorld, &Player::tickWorldCallptr);
		}
	}


	//ServerPlayer 虚表及相关Hook
	{
		//48 8D 05 ? ? ? ? 48 89 06 4C 89 A6 ? ? ? ? 48 8D 9E ? ? ? ? 48 89 5D ? 48 89 7B
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 06 4C 89 A6 ? ? ? ? 48 8D 9E ? ? ? ? 48 89 5D ? 48 89 7B";
		auto ServerPlayerVTable_sigOffset = FindSignature(memcode);
		if (ServerPlayerVTable_sigOffset == 0x00) {
			logF("[ServerPlayer::SetVtables] [Error]Find ServerPlayer ServerPlayerVTable_sigOffset is no working!!!");
		}
		else {
			auto ServerPlayerVTable = Utils::FuncFromSigOffset<uintptr_t**>(ServerPlayerVTable_sigOffset, 3);
			logF_Debug("[ServerPlayer::SetVtables] [Success] 虚表地址= %llX , sigoffset= %llX , memcode=%s", ServerPlayerVTable, ServerPlayerVTable_sigOffset, memcode);
			ServerPlayer::SetVFtables(ServerPlayerVTable);
			//ServerPlayer::respawn 获取实现 Actor::getRotationEx 的关键偏移(379) +9
			Actor::GetRotationOffset = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(ServerPlayer::GetVFtableFun(379)) + 9);

			//虚表Hook
			MH_CreateHookEx((LPVOID)ServerPlayer::GetVFtableFun(369), &Hook::ServerPlayer_TickWorld, &ServerPlayer::tickWorldCall);

		}
	}

	//RemotePlayer::RemotePlayer 虚表及相关功能Hook
	{
		//48 8D 05 ? ? ? ? 48 89 07 48 8D 05 ? ? ? ? 48 89 06
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 07 48 8D 05 ? ? ? ? 48 89 06";
		auto RemotePlayerVTable_sigOffset = FindSignature(memcode);
		if (RemotePlayerVTable_sigOffset == 0x00) {
			logF("[RemotePlayer::SetVtables] [Error]Find RemotePlayerVTable_sigOffset is no working!!!");
		}
		else {
			auto RemotePlayerVT = Utils::FuncFromSigOffset<uintptr_t**>(RemotePlayerVTable_sigOffset, 3);
			logF_Debug("[RemotePlayer::SetVtables] [Success] 虚表地址= %llX , sigoffset= %llX , memcode=%s", RemotePlayerVT, RemotePlayerVTable_sigOffset, memcode);
			RemotePlayer::SetVFtables(RemotePlayerVT);
			//虚表Hook
			//TickWorld 不能Hook这个函数,因为函数的内容为 ret 0000
			MH_CreateHookEx((LPVOID)RemotePlayer::GetVFtableFun(369), &Hook::RemotePlayer_TickWorld, &RemotePlayer::tickWorldCallptr);
		}
	}

	//LocalPlayer虚表及相关Hook
	//啥用没有 虚函数没找到能用的  仅在 Actor::isLocalPlayer() 中使用
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 07 C6 87 ? ? ? ? 0 4C 89 B7";
		auto LocalPlayerVTable_sigOffset = FindSignature(memcode);
		if (LocalPlayerVTable_sigOffset == 0x00) {
			logF("[LocalPlayer::SetVtables] [Error]Find LocalPlayer LocalPlayerVTable_sigOffset is no working!!!");
		}
		else {
			auto LocalPlayerVTable = Utils::FuncFromSigOffset<uintptr_t**>(LocalPlayerVTable_sigOffset, 3);
			logF_Debug("[LocalPlayer::SetVtables] [Success] 虚表地址= %llX , sigoffset= %llX , memcode=%s", LocalPlayerVTable, LocalPlayerVTable_sigOffset, memcode);
			LocalPlayer::SetVFtables(LocalPlayerVTable);

			//找本地玩家到ClientInstance的指针偏移
			char* LP_CIoffset = (char*)LocalPlayerVTable_sigOffset;
			for (int i = 0; i <= 500; i++) {
				if (i == 500) {
					logF("[LocalPlayer::SetVtables] [Error] 寻找 LocalPlayer To ClientInstance 偏移地址失败,LP_CIoffset:%llX", LP_CIoffset);
					break;
				}
				if (*LP_CIoffset == (char)0x48 && *(LP_CIoffset + 1) == (char)0x89 && *(LP_CIoffset + 2) == (char)0xB7/*(char)0xBE*/) {
					LocalPlayer::toCIoffset = *reinterpret_cast<int*>(LP_CIoffset + 3);
					logF_Debug("[LocalPlayer::SetVtables] [Success] Find LocalPlayer To ClientInstance i:%d",i);
					break;
				}
				LP_CIoffset++;
			}
			//虚表Hook
			//MH_CreateHookEx((LPVOID)ServerPlayer::GetVFtableFun(374), &Hook::ServerPlayer_TickWorld, &ServerPlayer::tickWorldCall);

		}
	}
// FishingHook 虚表地址特征码：48 8D 05 ? ? ? ? 48 89 03 C7 83 ? ? ? ? ? ? ? ? C7 83 ? ? ? ? ? ? ? ? 48 C7 83
}

auto Hook::exit() -> void {
	logF("[MH_DisableHook] Hook关闭状态: %s", MH_StatusToString(MH_DisableHook(MH_ALL_HOOKS)));
	Sleep(10);
}

auto Hook::LockSprinting(void* a, void* b) -> void
{
	static AutoSprinting* as = Game::GetModuleManager()->GetModule<AutoSprinting*>();
	static AutoWalking* aw = Game::GetModuleManager()->GetModule<AutoWalking*>();
	if (as->isEnabled()) {
		*(bool*)((uintptr_t)a + LockSprinting_offset) = true;
	}
	if (aw->isEnabled()) {
		// TODO: 这个没必要放在Tick里面，应该只用修改一次就可以了,后面尝试改一下
		*(bool*)((uintptr_t)a + LockSprinting_offset + 3) = true;
	}
	lockSprintingcall(a, b);
}

//无击退效果
auto Hook::SetVelocity(Player* player,vec3_t* kb)->void*
{
	if (!Game::GetModuleManager()->onKnockback((LocalPlayer*)player, kb)) {
		return nullptr;
	}
	return player->setVelocity(kb);
}

auto Hook::Actor_getShadowRadius(Actor* actor)->float {
	Game::GetModuleManager()->onActorSightTick(actor);
	return actor->getShadowRadius();
}

auto Hook::ClientInstance_Tick(ClientInstance* _this, void* a1) -> void
{
	if (_this != nullptr) Game::Cinstance = _this;
	clientInstance_Tickcall(_this, a1);
}


auto Hook::Is_ShowCoordinates_Tick(void* _this)->bool
{
	static ShowCoordinates* sc = Game::GetModuleManager()->GetModule<ShowCoordinates*>();
	bool ret = is_ShowCoordinates_Tickcall(_this);
	if (sc && sc->isEnabled()) {
		ret = true;
	}
	return ret;
}


// 实际很多变量都调用这个函数，速度 血量，饥饿，附加血量等
//auto Hook::GetHungerValAddress_Tick(void* _this, const char* a1, void* a2)->void*
//{
//	auto ret = getHungerValAddress_Tickcall(_this, a1, a2);	// +0x84 = 132
//	/*auto hunger = reinterpret_cast<float*>(reinterpret_cast<INT64>(ret) + 0x84);
//	//速度
//	if (*hunger == 0.1299999952f) {
//		*hunger = 0.133f;
//	}*/
//	return ret;
//}


//仅仅在本地房间时有效 _this 应该是serverplayer
auto Hook::NoFallDamage_Tick(Player* _this, float* a1)->void*
{
	//this + 1D4
	//*reinterpret_cast<float*>(reinterpret_cast<INT64>(_this) + 0x1D4) = 0.0f;
	return noFallDamage_Tickcall(_this, a1);
}

auto Hook::Level_Tick(Level* level)->void
{
	Game::GetModuleManager()->onLevelTick(level);
	level->Tick();
}



auto Hook::LocalPlayer_getCameraOffset(LocalPlayer* _this)->vec2_t*
{
	static INT64 p = 0;
	auto thisp = reinterpret_cast<INT64>(_this);
	if (thisp != p) {
		p = thisp;
		Game::localplayer = _this;
		logF_Debug("[%s] 本地玩家地址: %llX,虚表 = %llX","LocalPlayer_getCameraOffset", thisp, *(INT64*)thisp);
		logF_Debug("[%s] Clientinstance: %llX ,通过本地玩家获取的CI: %llX ,虚表 = %llX","LocalPlayer_getCameraOffset", Game::Cinstance, _this->getClientInstance(), *(uintptr_t*)_this->getClientInstance());
		//Level* l = _this->getLevel();
		//logF("Level VT = %llX , Level::startLeaveGame addr= %llX", *reinterpret_cast<uintptr_t*>(l),Utils::GetVTFPtr(*reinterpret_cast<uintptr_t*>(l), 2));
	}
	Game::GetModuleManager()->onLocalPlayerTick(_this);
	return localplayer_getCameraOffsetcall(_this);
}

//在非本地房间 只有本地玩家才会触发Tick
auto Hook::Player_tickWorld(Player* player, Tick* tick)->void
{
	Game::GetModuleManager()->onPlayerTick(player);
	player->tickWorld(tick);
}

auto Hook::Player_getShadowRadius(Player* player) -> float
{
	Game::GetModuleManager()->onPlayerSightTick(player);
	return player->getShadowRadius();
}

//一直调用 且每位玩家都调用 这个函数好像是Player::getShadowRadius 不确定
auto Hook::AllActor_Tick(Actor* _this, float* a1, float a2)->float* {
	Game::GetModuleManager()->onActorTick(_this);
	return allActor_Tickcall(_this, a1, a2);
}

//就是把生物当前位置加上这个值 v3
//auto Hook::Actor_moveBBs(Actor* _this, vec3_t* v3)->void* {
//	_this->onMoveBBs(*v3);
//	return actor_moveBBscall(_this, v3);
//}

auto Hook::KeyUpdate(__int64 key, int isdown)->void* {
	Game::GetModuleManager()->onKeyUpdate((int)key, isdown == 1);
	if (ImGui::GetCurrentContext() != nullptr) {
		ImGui::GetIO().KeysDown[key] = isdown == 1;
		if (ImGui::GetIO().WantTextInput) {
			//if (key > 0 && key < 0x10000 && isdown == 1) ImGui::GetIO().AddInputCharacterUTF16((UINT)key);
			return 0;
		}
		if (ImGui::GetIO().WantCaptureKeyboard) {
			return 0;
		}
	}
	return keyupdatecall(key, isdown);
}



//触发: 鼠标在窗口中经过就会触发 mousebutton=0,isDown=0
//mousebutton 1:鼠标左键,2鼠标右键,3:鼠标中键,4:鼠标滚轮滚动(isDown 为+-120左右的值),
auto Hook::MouseUpdate(__int64 a1, char mousebutton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8)->void {
	Game::MouseKeyDown[mousebutton] = isDown;
	Game::GetModuleManager()->onMouseUpdate(mousebutton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY);
	if (ImGui::GetCurrentContext() != nullptr) {
		switch (mousebutton) {
		case 1:
			ImGui::GetIO().MouseDown[0] = isDown;
			break;
		case 2:
			ImGui::GetIO().MouseDown[1] = isDown;
			break;
		case 3:
			ImGui::GetIO().MouseDown[2] = isDown;
			break;
		case 4:
			ImGui::GetIO().MouseWheel = isDown < 0 ? -0.5f : 0.5f; //For scrolling
			break;
		default:
			break;
		}
		if (!ImGui::GetIO().WantCaptureMouse)
			mouseupdatecall(a1, mousebutton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
	}
	else {
		mouseupdatecall(a1, mousebutton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
	}
}

int frame = 0;		// 按照视频作者的说法，这个函数会在三层每层都调用一次，也就是每一帧调用三次
auto Hook::RenderDetour(void* _this, MinecraftUIRenderContext* ctx)->void {
	renderDetourcall(_this, ctx);
	if (frame >= 3) {
		frame = 0;
		Game::GetModuleManager()->onRenderDetour(ctx);
	}
	frame++;
}


auto Hook::Draw_Text(MinecraftUIRenderContext* _this, BitmapFont* a1, RectangleArea const& a2, TextHolder* a3, UIColor const& a4, float a5, float a6, TextMeasureData* a7, uintptr_t* a8)->void {
	if (Game::mcfont != a1) {
		if (Game::mcfont == nullptr) {
			Game::mcfont = a1;
		}
		//logF("mcfont = %llX,Text = %s", a1,a3->getText());
	}
	
	reinterpret_cast<MUICDrawText>(MinecraftUIRenderContext::drawtextCall)(_this, a1, a2, a3, a4, a5, a6, a7, a8);
}


auto Hook::sendMessage(void* a1, TextHolder* a2)->__int64 {
	//if (strcmp(a2->getText(), "1") == 0)
	auto ret = Game::GetModuleManager()->onSendMessage(a2);
	if (!ret) {
		return 0;
	}
	return sendChatMessagecall(a1, a2);
}

//返回值0-2 分别对应玩家第一二三人称视角
auto Hook::getLocalPlayerViewPerspective(void* thi)->int {
	static FastViewPerspective* idy = Game::GetModuleManager()->GetModule<FastViewPerspective*>();
	if (idy) {
		if (idy->isEnabled() && idy->isToggle()) {
			return 2;
		}
	}
	return getLocalPlayerViewPerspectivecall(thi);
}

auto Hook::level_startLeaveGame(Level* level) -> void
{
	Game::localplayer = nullptr;
	Game::GetModuleManager()->onstartLeaveGame(level);
	level->startLeaveGame();
}

//虚表Hook
auto Hook::GameMode_startDestroyBlock(GameMode* _this, vec3_ti* a2, uint8_t* face, void* a3, void* a4)->bool {
	static InstantDestroy* idy = Game::GetModuleManager()->GetModule<InstantDestroy*>();
	if (idy) {
		idy->onStartDestroyBlock(_this, a2, face);
	}
	return _this->startDestroyBlock(a2,face,a3,a4);
}

auto Hook::GameMode_useItem(GameMode* gm, class ItemStack* item)->bool
{
	if (!Game::GetModuleManager()->useItem(gm,item)) {
		return false;
	}
	return gm->useItem(item);
}

//对着方块右键 会一直触发
auto Hook::GameMode_useItemOn(GameMode* gm, class ItemStack* item, vec3_ti* bpos, uint8_t* face, vec3_t* f, class Block* block)->bool
{
	if (!Game::GetModuleManager()->useItemOn(gm, item,bpos,face,f,block)) {
		return false;
	}
	return gm->useItemOn(item,bpos,face,f,block);
}


auto Hook::GameMode_tick(GameMode* _this)->void* {
	logF_Debug("[Hook::GameMode_tick] GameMode_tick 在该版本中恢复工作。。。");
	Game::GetModuleManager()->onTick(_this);
	return _this->tick();
}

auto Hook::GameMode_attack(GameMode* _this, Actor* actor)->bool {
	if (_this->GetLocalPlayer()->isLocalPlayer()) {
		//logF("attack Actor ptr= %llX, ActorType = %i, sizex = %f, sizey = %f, isplayer=%i, islocalplayer=%i", actor, actor->getEntityTypeId(),actor->getHitBox().x, actor->getHitBox().y,actor->isPlayer(),actor->isLocalPlayer());
		//_this->GetLocalPlayer()->getSelectedItem()->use(_this->GetLocalPlayer());	//必须是服务玩家才能有效
		//_this->useItem(_this->GetLocalPlayer()->getSelectedItem());
	}

	if (!Game::GetModuleManager()->onAttack(actor)) {
		return false;
	}
	return _this->attack(actor);
}

auto Hook::ServerPlayer_TickWorld(ServerPlayer* _this, void* tick)->void* {
	Game::GetModuleManager()->onServerPlayerTick(_this);
	//_this->onAllPlayerTick();				//这里应该是 所有serverplayer tick
	return _this->tickWorld(tick);
}

auto Hook::RemotePlayer_TickWorld(RemotePlayer* _this) -> void*
{
	//判断虚表是否是远程玩家
	if (_this && (uintptr_t)_this > 0xFFFFFFFF && *(uintptr_t***)_this == RemotePlayer::GetVFtables()) {
		Game::GetModuleManager()->onRemotePlayerTick(_this);
	}
	return _this->tickWorld();
}
