#include "Hook.h"
#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
#include "../Utils/Game.h"
#include "../Loader.h"
#include "imgui.h"

#include "Player.h"
#include "ServerPlayer.h"
#include "LocalPlayer.h"
#include "RemotePlayer.h"
#include "Actor.h"
#include "Level.h"
#include "GameMode.h"
#include "MinecraftUIRenderContext.h"
#include "ActorMovementProxy.h"
#include "Item.h"
#include "FishingHook.h"
#include "ItemInstance.h"
#include "ItemStack.h"
#include "Block.h"
#include "LoopbackPacketSender.h"

#include "../Modules/ModuleManager.h"
#include "../Modules/Modules/HitBox.h"
#include "../Modules/Modules/InstantDestroy.h"
#include "../Modules/Modules/ShowCoordinates.h"
#include "../Modules/Modules/FastViewPerspective.h"
#include "../Modules/Modules/LockControlInput.h"
#include "../Modules/Modules/HundredTimesMoreDrops.h"

#include "../Modules/Modules/Debug.h"

#include "../imgui/imgui_uwp_wndProc.h"

using LockControl = void*(__fastcall*)(void* thi, void* a2, void* a3, void* a4, void* a5, void* a6);
LockControl LockControlInputcall;
int LockControlInput_offset = 0;

using ClientInstance_Tick = uintptr_t(__fastcall*)(ClientInstance* _this, void* a1);
ClientInstance_Tick clientInstance_Tickcall;
uintptr_t clientInstanceTick;

// 获取一个值 是否在屏幕上显示位置
using Is_ShowCoordinates_Tick = bool(__fastcall*)(void* _this, void* a2, void* a3);
Is_ShowCoordinates_Tick is_ShowCoordinates_Tickcall;
uintptr_t is_ShowCoordinatesTick;

//0.6 0.2 4
//48 8B C4 48 89 58 ? 48 89 68 ? 56 57 41 56 48 83 EC 70 48 8B EA
using Covers_HitBox_Parts = void* (__fastcall*)(void* _this, void* a1, void* a2);
Covers_HitBox_Parts covers_HitBox_Partscall;
uintptr_t covers_HitBox_Parts;


//using LocalPlayer_getCameraOffset = vec2_t*(__fastcall*)(LocalPlayer* _this);
//LocalPlayer_getCameraOffset localplayer_getCameraOffsetcall;
//uintptr_t localplayer_getCameraOffset;


using AllActor_Tick = float* (__fastcall*)(Actor*, float*, float);
AllActor_Tick allActor_Tickcall;
//uintptr_t allActor_Tick;

using AllPlayer_Tick = char(__fastcall*)(void*, Player*);
AllPlayer_Tick allPlayer_Tickcall;

using KeyUpdate = void* (__fastcall*)(__int64 key, int isdown);
KeyUpdate keyupdatecall;
//uintptr_t keyupdate;

using MouseUpdata = void(__fastcall*)(__int64, char, char, __int16, __int16, __int16, __int16, char);
MouseUpdata mouseupdatecall;
//uintptr_t mouseupdate;

using SendChatMessage = uint8_t(__fastcall*)(void*, std::mcstring*);
SendChatMessage sendChatMessagecall;
uintptr_t sendChatMessage;

using GetViewPerspective = int(__fastcall*)(void*);
GetViewPerspective getLocalPlayerViewPerspectivecall;

//using LPLP = void* (__fastcall*)(void*, void*, void*, void*, int, void*, char, void*, void*, void*, void*, void*);
//LPLP lplpcall;

using LoopbackPacketSenderTOServer = void* (__fastcall*)(LoopbackPacketSender*, Packet*);
LoopbackPacketSenderTOServer LoopbackPacketSenderToServerCall;

using ClientInstanceOnDimensionChanged = void* (__fastcall*)(ClientInstance*);
ClientInstanceOnDimensionChanged ClientInstanceOnDimensionChangedCall;

using BlockPlayerDestroy = char*(__fastcall*)(Block* block, Player* player, vec3_ti pos);
BlockPlayerDestroy blockPlayerDestroyCall;

char* Block_playerDestroy(Block* block, Player* player, vec3_ti pos) {
	//logF("block: %llX", block);
	static HundredTimesMoreDrops* HTMD = Game::GetModuleManager()->GetModule<HundredTimesMoreDrops*>();
	if (HTMD->isEnabled() && !player->isLocalPlayer()) {
		
		if (HTMD->onlyLocalPlayer) {
			LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
			if (lp && (*player->getOrCreateUniqueID() == *lp->getOrCreateUniqueID())) {
				for (int i = 0; i < HTMD->multiple; i++) {
					blockPlayerDestroyCall(block, player, pos);
				}
			}
		}
		else {
			for (int i = 0; i < HTMD->multiple; i++) {
				blockPlayerDestroyCall(block, player, pos);
			}
		}
	}

	return blockPlayerDestroyCall(block, player, pos);
}

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_UWP_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using WndProcFn = LRESULT(WINAPI*)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
WndProcFn WndProcCall;

//https://learn.microsoft.com/zh-cn/windows/win32/inputmsg/messages
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_UWP_WndProcHandler(hWnd, msg, wParam, lParam)) {
		return true;
	}

	return WndProcCall(hWnd, msg, wParam, lParam);
	//return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

using BlockPlayerDestroy2 = char* (__fastcall*)(void* a1, void* a2);
BlockPlayerDestroy2 blockPlayerDestroyCall2;
void* Block_playerDestroy2(void* a1, void* a2) {
	logF("[Hook::init] 触发了！");
	for (int i = 0; i < 99; i++) {
		blockPlayerDestroyCall2(a1,a2);
	}

	return blockPlayerDestroyCall2(a1,a2);
}

auto Hook::init() -> void
{
	//logF_Debug("WndProc2: %llX", (uintptr_t*)GetWindowLongPtr((HWND)Game::ChildWindowsHandle, -4));
	//SetWindowLongPtr((HWND)Game::WindowsHandle, -4, (LONG_PTR)&WndProc);
	logF("[Hook::init] 正在初始化");
	// WndProc Hook
	{
		MH_CreateHookEx((LPVOID)GetWindowLongPtr((HWND)Game::ChildWindowsHandle, GWLP_WNDPROC), &WndProc, &WndProcCall);
	}
	
	//48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B E8 4C 8B FA 48 8B F1
	//Block::playerDestroy
	{
		// 这就是本函数
		const char* memcode_call = "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B E8 4C 8B FA 48 8B F1 0F";
		// 这是函数的调用者
		const char* memcode_offset = "E8 ? ? ? ? 48 8B 47 ? 48 8B 90 ? ? ? ? 80 BA ? ? ? ? ? 75 ? 48 8B 8A";

		SignCode sign("Block::playerDestroy");
		//sign.AddSign(memcode_call);
		sign << memcode_call;
		sign.AddSignCall(memcode_offset);
		if (sign) {
			MH_CreateHookEx((LPVOID)*sign, &Block_playerDestroy, &blockPlayerDestroyCall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", *sign, sign.ValidSign());
		}
	}
	{
		//MH_CreateHookEx((LPVOID)(Utils::getBase() + 0x304DF60), &Block_playerDestroy2, &blockPlayerDestroyCall2);
	}

	// 锁定疾跑 强制按下疾跑键 | 并不能确定被Hook的这个函数是什么 a2+offset
	{
		//Hook这个函数, 这个函数的参数有控制键结构的基址, 有基址到结构的偏移
		//首先找到这个函数, 从这个函数中找到基址到结构的偏移, 和确定基址是哪个参数(a2)
		//怎么找这个函数,首先找到能够锁定控制键的结构,的开头(下蹲的控制地址)，找访问,定位到始终执行的反汇编项
		const char* memcode_call = "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 0F 10 42 ? 48 8B D9";
		//auto LockControlInputSign = FindSignature(memcode_call);
		//if (LockControlInputSign != 0x00) {
		//	auto LockControlInputOffset = Utils::FindSignatureRelay(LockControlInputSign, "0F 10 42", 32);
		//	if (LockControlInputOffset != 0x00) {
		//		LockControlInput_offset = (int)*reinterpret_cast<byte*>(LockControlInputOffset + 3);
		//		MH_CreateHookEx((LPVOID)LockControlInputSign, &Hook::LockControlInputCallBack, &LockControlInputcall);
		//		logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", LockControlInputSign, memcode_call);
		//	}
		//	else {
		//		logF("[Hook::FindSignature] [Error] 控制键锁定 查找偏移未找到");
		//	}
		//}
		//else {
		//	logF("[Hook::FindSignature] [Error] 控制键锁定 函数未找到");
		//}


		SignCode sign("锁定疾跑");
		sign << memcode_call;
		if (sign) {
			auto LockControlInputOffset = Utils::FindSignatureRelay(*sign, "0F 10 42", 32);
			if (LockControlInputOffset != 0x00) {
				LockControlInput_offset = (int)*reinterpret_cast<byte*>(LockControlInputOffset + 3);
				MH_CreateHookEx((LPVOID)*sign, &Hook::LockControlInputCallBack, &LockControlInputcall);
				logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", *sign, sign.ValidSign());
			}
			else {
				logF("[Hook::FindSignature] [Error] 控制键锁定 查找偏移未找到");
			}
		}


	}

	//clientInstance::Tick // 1.20 虚表 - 19号
	{
		const char* memcode = "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B DA 48 8B F9 0F 57 C0 0F 11 44 24 ? 0F";
		//clientInstanceTick = FindSignature(memcode);
		//if (clientInstanceTick != 0x00) {
		//	MH_CreateHookEx((LPVOID)clientInstanceTick, &Hook::ClientInstance_Tick, &clientInstance_Tickcall);
		//	logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", clientInstanceTick, memcode);
		//}
		//else {
		//	logF("[Hook error] [%s] is no found Hook point", "clientInstanceTick");
		//}

		SignCode sign("clientInstance::Tick");
		//sign.AddSign(memcode_call);
		sign << memcode;
		if (sign) {
			MH_CreateHookEx((LPVOID)*sign, &Hook::ClientInstance_Tick, &clientInstance_Tickcall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", *sign, sign.ValidSign());
		}
	}
	
	//是否显示坐标 Tick
	{
		// 均调用者(1.20.31原函数定位太长了)48 89 5C 24 10 57 48 83 EC ? 48 8B D1 48 8B 41 30 48 85 C0 0F 84 ? ? ? ? 48 83 38 00 0F 84 ? ? ? ? 48 8B 41 ? 48 85 ? 74 ? F0 FF 40 ? 48 8B 49 ? 48 89 4C 24 ? 48 8B 5A ? 48 89 5C 24 ? 48 8B 09 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 8B F8 48 85
		const char* memcode1 = "E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? F2";		//+5EE1A1
		const char* memcode2 = "E8 ? ? ? ? E9 ? ? ? ? 41 81 F8 82 FB 70 6D 75 ? E8"; //+5EE052
		const char* memcode3 = "E8 ? ? ? ? 84 C0 74 ? 48 8B 4B ? E8 ? ? ? ? 3C"; //+789DC4
		//is_ShowCoordinatesTick = FindSignature(memcode1);
		//if (is_ShowCoordinatesTick == 0x00) {
		//	is_ShowCoordinatesTick = FindSignature(memcode2);
		//}
		//if (is_ShowCoordinatesTick == 0x00) {
		//	is_ShowCoordinatesTick = FindSignature(memcode3);
		//}
		//if (is_ShowCoordinatesTick == 0x00) {
		//	logF("[Hook error] [%s] is no found Hook point", "is_ShowCoordinatesTick");
		//}
		//else {
		//	MH_CreateHookEx((LPVOID)Utils::FuncFromSigOffset<uintptr_t*>(is_ShowCoordinatesTick, 1), &Hook::Is_ShowCoordinates_Tick, &is_ShowCoordinates_Tickcall);
		//	logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", is_ShowCoordinatesTick, "...");
		//}

		SignCode sign("是否显示坐标");
		sign.AddSignCall(memcode1);
		sign.AddSignCall(memcode2);
		sign.AddSignCall(memcode3);
		if (sign) {
			MH_CreateHookEx((LPVOID)*sign, &Hook::Is_ShowCoordinates_Tick, &is_ShowCoordinates_Tickcall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", sign.ValidPtr(), sign.ValidSign());
		}
	}


	// https://github.com/cngege/Mod/wiki/%E5%85%B3%E4%BA%8EHook%E5%87%BD%E6%95%B0%E6%88%96%E5%81%8F%E7%A7%BB%E7%9A%84%E6%9F%A5%E6%89%BE%E4%BB%A5%E5%8F%8A%E6%9D%82%E9%A1%B9#%E6%89%80%E6%9C%89%E7%8E%A9%E5%AE%B6tick
	//所有玩家TICK ,所有生物TICK,对玩家来说应该是渲染相关得函数,只有看向 那个玩家，那个玩家才会触发这个函数，其中出现的玩家指针是Player 不是ServerPlayer
	//非掉落物实体，都只会在视野中才会触发  这个特征码比较旧了，不知道Hook的函数正不正确
	{
		const char* memcode = "48 89 5C 24 ? 48 89 6C 24 ? 57 48 83 EC ? 48 8B 01 41 8B E8 48 8B FA";
		//allActor_Tick = FindSignature(memcode);
		//if (allActor_Tick != 0x00) {
		//	MH_CreateHookEx((LPVOID)allActor_Tick, &Hook::AllActor_Tick, &allActor_Tickcall);
		//	logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", allActor_Tick, memcode);
		//}
		//else {
		//	logF("[Hook error] [%s] is no found Hook point", "allActor_Tick");
		//}
		SignCode sign("allActor_Tick Hook");
		sign << memcode;
		if (sign) {
			MH_CreateHookEx((LPVOID)*sign, &Hook::AllActor_Tick, &allActor_Tickcall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", sign.ValidPtr(), sign.ValidSign());
		}
	}

	// 所有玩家Tick（非服务玩家）
	{
		SignCode sign("所有玩家Tick");
		sign << "48 83 EC ? 48 8B 02 4C 8B C2 4C 8B 88";	//没有直接调用者(应该是虚表函数)，只能通过函数本身特征码定位
		if (sign) {
			MH_CreateHookEx((LPVOID)*sign, &Hook::AllPlayer_Tick, &allPlayer_Tickcall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", sign.ValidPtr(), sign.ValidSign());
		}
	}

	//KeyUpdate
	{
		const char* memcode = "48 83 EC ? 0F B6 C1 4C 8D 05 ? ? ? ? 89 54 24";
		//keyupdate = FindSignature(memcode);
		//if (keyupdate != 0x00) {
		//	MH_CreateHookEx((LPVOID)keyupdate, &Hook::KeyUpdate, &keyupdatecall);
		//	logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", keyupdate, memcode);
		//}
		//else {
		//	logF("[Hook error] [%s] is no found Hook point", "keyupdate");
		//}

		SignCode sign("KeyUpdate");
		sign << memcode;
		if (sign) {
			MH_CreateHookEx((LPVOID)*sign, &Hook::KeyUpdate, &keyupdatecall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", *sign, sign.ValidSign());
		}
	}
	
	//MouseUpdate
	{
		const char* memcode = "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F";
		//mouseupdate = FindSignature(memcode);
		//if (keyupdate != 0x00) {
		//	MH_CreateHookEx((LPVOID)mouseupdate, &Hook::MouseUpdate, &mouseupdatecall);
		//	logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", mouseupdate, memcode);
		//}
		//else {
		//	logF("[Hook error] [%s] is no found Hook point", "mouseupdate");
		//}

		SignCode sign("MouseUpdate");
		sign << memcode;
		if (sign) {
			MH_CreateHookEx((LPVOID)*sign, &Hook::MouseUpdate, &mouseupdatecall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", *sign, sign.ValidSign());
		}

	}
	

	//SendChatMessage 这个暂时不修复,因为没用上
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

	// https://github.com/cngege/Mod/wiki/Player.hpp-(ServerPlayer---LocalPlayer---ALL)#%E8%AE%BE%E7%BD%AE%E7%8E%A9%E5%AE%B6%E4%BA%BA%E7%A7%B0%E8%A7%86%E8%A7%92-perspective-%E7%AC%ACn%E4%BA%BA%E7%A7%B0%E8%A7%86%E8%A7%92
	// 获取本地玩家人称视角函数
	// 从找调用可以看出，这是某个类的虚表函数
	{
		const char* memcode = "48 83 EC ? 48 8B ? 48 8D 54 24 ? 41 B8 03 00 00 00";
		//auto findptr = FindSignature(memcode);
		//if (findptr != 0x00) {
		//	MH_CreateHookEx((LPVOID)findptr, &Hook::getLocalPlayerViewPerspective, &getLocalPlayerViewPerspectivecall);
		//	logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", findptr, memcode);
		//}
		//else {
		//	logF("[Hook error] [%s] is no found Hook point", "getLocalPlayerViewPerspective");
		//}

		SignCode sign("获取本地玩家人称视角");
		sign << memcode;
		if (sign) {
			MH_CreateHookEx((LPVOID)*sign, &Hook::getLocalPlayerViewPerspective, &getLocalPlayerViewPerspectivecall);
			logF_Debug("[Hook::FindSignature] Find MemCode result=%llX , MemCode=%s", *sign, sign.ValidSign());
		}
	}


	//ItemStack虚表 来自Player::completeUsingItem
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 85 ? ? ? ? 44 89 BD";
		//auto ItemStackVT_sigOffset = FindSignature(memcode);
		//if (ItemStackVT_sigOffset == 0x00) {
		//	logF("[ItemStack::SetVtables] [Error]Find ItemStackVTable_sigOffset is no working!!!");
		//}
		//else {
		//	auto ItemStackVT = Utils::FuncFromSigOffset<uintptr_t**>(ItemStackVT_sigOffset, 3);
		//	logF_Debug("[ItemStack::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", ItemStackVT, ItemStackVT_sigOffset, memcode);
		//	ItemStack::SetVFtables(ItemStackVT);
		//}

		SignCode sign("ItemStack虚表");
		sign.AddSignCall(memcode, 3);
		if (sign) {
			logF_Debug("[ItemStack::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", *sign, sign.ValidPtr(), sign.ValidSign());
			ItemStack::SetVFtables((uintptr_t**)*sign);
		}
	}

	//ItemStackBase虚表 来自Player::completeUsingItem
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 45 ? 45 33 FF 4C";
		//auto ItemStackBaseVT_sigOffset = FindSignature(memcode);
		//if (ItemStackBaseVT_sigOffset == 0x00) {
		//	logF("[ItemStackBase::SetVtables] [Error]Find ItemStackBaseVTable_sigOffset is no working!!!");
		//}
		//else {
		//	auto ItemStackBaseVT = Utils::FuncFromSigOffset<uintptr_t**>(ItemStackBaseVT_sigOffset, 3);
		//	logF_Debug("[ItemStackBase::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", ItemStackBaseVT, ItemStackBaseVT_sigOffset, memcode);
		//	ItemStackBase::SetVFtables(ItemStackBaseVT);
		//}

		SignCode sign("ItemStackBase虚表");
		sign.AddSignCall(memcode, 3);
		if (sign) {
			logF_Debug("[ItemStackBase::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", *sign, sign.ValidPtr(), sign.ValidSign());
			ItemStackBase::SetVFtables((uintptr_t**)*sign);
		}
	}

	//ItemInstance 虚表 来自Player::completeUsingItem
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 45 ? 49 8D 96";
		//auto ItemInstanceVT_sigOffset = FindSignature(memcode);
		//if (ItemInstanceVT_sigOffset == 0x00) {
		//	logF("[ItemInstance::SetVtables] [Error]Find ItemInstanceVTable_sigOffset is no working!!!");
		//}
		//else {
		//	auto ItemInstanceVT = Utils::FuncFromSigOffset<uintptr_t**>(ItemInstanceVT_sigOffset, 3);
		//	logF_Debug("[ItemInstance::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", ItemInstanceVT, ItemInstanceVT_sigOffset, memcode);
		//	ItemInstance::SetVFtables(ItemInstanceVT);
		//}

		SignCode sign("ItemInstance虚表");
		sign.AddSignCall(memcode, 3);
		if (sign) {
			logF_Debug("[ItemInstance::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", *sign, sign.ValidPtr(), sign.ValidSign());
			ItemInstance::SetVFtables((uintptr_t**)*sign);
		}
	}

	// 虚表函数核对时间 : 1.20.41.02
	//GameMode虚表及相关Hook
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 01 48 89 51 ? 48 C7 41 ? ? ? ? ? C7 41 ? ? ? ? ? 44 88 61";
		//auto GameModeVTable_sigOffset = FindSignature(memcode);
		//if (GameModeVTable_sigOffset == 0x00) {
		//	logF("[GameMode::SetVtables] [Error]Find GameMode GameModeVTable_sigOffset is no working!!!");
		//}
		//else {
		//	auto GameModeVTables = Utils::FuncFromSigOffset<uintptr_t**>(GameModeVTable_sigOffset, 3);
		//	logF_Debug("[GameMode::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", GameModeVTables, GameModeVTable_sigOffset, memcode);
		//	GameMode::SetVFtables(GameModeVTables);
		//	//Hook GameMode_startDestroyBlock
		//	MH_CreateHookEx((LPVOID)GameMode::GetVFtableFun(1), &Hook::GameMode_startDestroyBlock, &GameMode::startDestroyBlockCall);
		//	//Hook GameMode_tick
		//	MH_CreateHookEx((LPVOID)GameMode::GetVFtableFun(9), &Hook::GameMode_tick, &GameMode::tickCall);
		//	//Hook GameMode_useItem
		//	MH_CreateHookEx((LPVOID)GameMode::GetVFtableFun(11), &Hook::GameMode_useItem, &GameMode::useItemCall);
		//	//Hook GameMode_useItemOn
		//	MH_CreateHookEx((LPVOID)GameMode::GetVFtableFun(12), &Hook::GameMode_useItemOn, &GameMode::useItemOnCall);
		//	//Hook GameMode_attack
		//	MH_CreateHookEx((LPVOID)GameMode::GetVFtableFun(14), &Hook::GameMode_attack, &GameMode::attackCall);
		//}

		SignCode sign("GameMode虚表");
		sign.AddSignCall(memcode, 3);
		if (sign) {
			logF_Debug("[GameMode::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", *sign, sign.ValidPtr(), sign.ValidSign());
			GameMode::SetVFtables((uintptr_t**)*sign);
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

	// 虚表函数核对时间 : 1.20.41.02
	//Actor 虚表及相关Hook  构造函数特征码：48 89 5C 24 ? 55  56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 8B D8 48 8B FA 4C 8B F1
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 01 49 8B 00 48 89 41 ? 41 8B 40 ? 89 41";
		//auto ActorVTable_sigOffset = FindSignature(memcode);
		//if (ActorVTable_sigOffset == 0x00) {
		//	logF("[Actor::SetVtables] [Error]Find Actor ActorVTable_sigOffset is no working!!!");
		//}
		//else
		//{
		//	auto ActorVTable = Utils::FuncFromSigOffset<uintptr_t**>(ActorVTable_sigOffset, 3);
		//	logF_Debug("[Actor::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", ActorVTable, ActorVTable_sigOffset, memcode);
		//	Actor::SetVFtables(ActorVTable);
		//	//虚表Hook
		//	//Actor::setVelocity
		//	MH_CreateHookEx((LPVOID)Actor::GetVFtableFun(27), &Hook::SetVelocity, &Actor::setVelocityCallptr);				
		//	MH_CreateHookEx((LPVOID)Actor::GetVFtableFun(37), &Hook::Actor_isInvisible, &Actor::isInvisibleCallptr);
		//	//MH_CreateHookEx((LPVOID)Actor::GetVFtableFun(64), &Hook::Actor_isInWater, &Actor::isInWaterCallptr);
		//	MH_CreateHookEx((LPVOID)Actor::GetVFtableFun(42), &Hook::Actor_getShadowRadius, &Actor::getShadowRadiusCallptr);
		//}

		SignCode sign("Actor虚表");
		sign.AddSignCall(memcode, 3);
		if (sign) {
			logF_Debug("[Actor::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", *sign, sign.ValidPtr(), sign.ValidSign());
			Actor::SetVFtables((uintptr_t**)*sign);
			//虚表Hook
			//Actor::setVelocity
			MH_CreateHookEx((LPVOID)Actor::GetVFtableFun(27), &Hook::SetVelocity, &Actor::setVelocityCallptr);
			MH_CreateHookEx((LPVOID)Actor::GetVFtableFun(37), &Hook::Actor_isInvisible, &Actor::isInvisibleCallptr);
			//MH_CreateHookEx((LPVOID)Actor::GetVFtableFun(64), &Hook::Actor_isInWater, &Actor::isInWaterCallptr);
			MH_CreateHookEx((LPVOID)Actor::GetVFtableFun(42), &Hook::Actor_getShadowRadius, &Actor::getShadowRadiusCallptr);
		}
	}

	//三参数Actor构造函数核对时间 : 1.20.12
	//Actor构造函数下的 获取Actor类中的Level指针偏移
	{
		const char* memcode = "4C 89 B6 ? ? ? ? E8 ? ? ? ? 48 8B D0 48 8D 8E ? ? ? ? E8";		//来自三参数Actor构造函数
		const char* memcode2 = "48 8D 05 ? ? ? ? 48 89 01 49 8B 01 48 89 41 ? 41 8B 41";		//来自四参数Actor构造函数(四参数构造函数参数好像没有Level)
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
	/*
	// Mob 虚表及相关Hook  XX Mob虚表的定位有问题，直接从Player虚表获取 该部分函数待删除,因为现在改为从后面Player部分获取
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
	*/
	// 虚表函数核对时间 : 1.20.41.02
	//Player 虚表及相关Hook
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 06 33 C0 48 89 86 ? ? ? ? 48 89 86 ? ? ? ? 48 89 86";
		auto PlayerVTable_sigOffset = FindSignature(memcode);
		if (PlayerVTable_sigOffset == 0x00) {
			logF("[Player::SetVtables] [Error]Find Player PlayerVTable_sigOffset is no working!!!");
		}
		else {
			auto PlayerVTable = Utils::FuncFromSigOffset<uintptr_t**>(PlayerVTable_sigOffset, 3);
			logF_Debug("[Player::SetVtables] [Success] 虚表地址= %llX , sigoffset= %llX , memcode=%s", PlayerVTable, PlayerVTable_sigOffset, memcode);
			Player::SetVFtables(PlayerVTable);
			//虚表Hook
			//MH_CreateHookEx((LPVOID)Player::GetVFtableFun(80), &Hook::LocalPlayer_getCameraOffset, &localplayer_getCameraOffsetcall); // 没找到
			//Player::getShadowRadius
			MH_CreateHookEx((LPVOID)Player::GetVFtableFun(42), &Hook::Player_getShadowRadius, &Player::getShadowRadiusCallptr);
			//Player::startSwimming
			//MH_CreateHookEx((LPVOID)Player::GetVFtableFun(121), &Hook::Player_startSwimming, &Player::startSwimmingCallptr);
			//Player::tickWorld
			MH_CreateHookEx((LPVOID)Player::GetVFtableFun(221), &Hook::Player_tickWorld, &Player::tickWorldCallptr);
			// Mob 虚表及相关Hook
			{
				// 在 PlayerVTable_sigOffset 地址定位之前的汇编是 E8 X X X X 90 ....
				// 所以 将 PlayerVTable_sigOffset-5 就是 Mob::Mob 此构造函数
				// 流程: 这里找到了Player虚表 ,Player继承自Mob, 所以这里有Mob的Call, 进入Mob::Mob, 往下找到
				auto Mob_Constructor = Utils::FuncFromSigOffset(PlayerVTable_sigOffset - 5, 0);	// Mob 构造函数地址
				auto offset = Utils::FindSignatureRelay(Mob_Constructor, "48 8D 05", 200);	// offset 值大概是 35
				if (offset != 0x00) {
					auto MobVTable = Utils::FuncFromSigOffset<uintptr_t**>(offset, 3);
					logF_Debug("[Mob::SetVtables] [Success] 虚表地址= %llX , sigoffset= %llX ,from PlayerVTable", MobVTable, Mob_Constructor);
					Mob::SetVFtables(MobVTable);
				}
				else {
					logF("[Mob::SetVtables] [Error] 没能从 Player 虚表获取函数中找到Mob虚表(进入Mob构造函数,看看虚表赋值从48 8D 05变为了什么)");
				}
			}
		}
	}

	// 虚表函数核对时间 : 1.20.41.02
	//ServerPlayer 虚表及相关Hook
	{
		//48 8D 05 ? ? ? ? 48 89 06 4C 89 BE ? ? ? ? 48 8D 9E ? ? ? ? 48
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 06 4C 89 ? ? ? ? ? 48 8D 9E ? ? ? ? 48 89 5D";
		auto ServerPlayerVTable_sigOffset = FindSignature(memcode);
		if (ServerPlayerVTable_sigOffset == 0x00) {
			logF("[ServerPlayer::SetVtables] [Error]Find ServerPlayer ServerPlayerVTable_sigOffset is no working!!!");
		}
		else {
			auto ServerPlayerVTable = Utils::FuncFromSigOffset<uintptr_t**>(ServerPlayerVTable_sigOffset, 3);
			logF_Debug("[ServerPlayer::SetVtables] [Success] 虚表地址= %llX , sigoffset= %llX , memcode=%s", ServerPlayerVTable, ServerPlayerVTable_sigOffset, memcode);
			ServerPlayer::SetVFtables(ServerPlayerVTable);

			//ServerPlayer::respawn 获取实现 Actor::getRotationEx 的关键偏移(342) +9
			//logF("ServerPlayer::GetVFtableFun(342) %d", reinterpret_cast<uintptr_t>(ServerPlayer::GetVFtableFun(342)));
			//DebugBreak(); //ServerPlayer::respawn?
			Actor::GetRotationOffset = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(ServerPlayer::GetVFtableFun(229)) + 9);	// 检查版本 1.20.41


			//虚表Hook
			// 为什么不调用
			MH_CreateHookEx((LPVOID)ServerPlayer::GetVFtableFun(221), &Hook::ServerPlayer_TickWorld, &ServerPlayer::tickWorldCall); // （记得同步修改RemotePlayer）检查版本 1.20

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
			//TickWorld 不能Hook这个函数,因为函数的内容为 ret 0000 (C2 00 00) 302 304 这里的虚表位置应该和ServerPlayer::tick 的虚表位置是一样的
			MH_CreateHookEx((LPVOID)RemotePlayer::GetVFtableFun(221), &Hook::RemotePlayer_TickWorld, &RemotePlayer::tickWorldCallptr);
		}
	}

	//LocalPlayer虚表及相关Hook	LocalPlayer 构造函数第二个参数应该就是 CI
	//啥用没有 虚函数没找到能用的  仅在 Actor::isLocalPlayer() 中使用
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 07 C6 87 ? ? ? ? 00 45 33 ? 4C 89 ? ? ? ? ? 48";
		auto LocalPlayerVTable_sigOffset = FindSignature(memcode);
		if (LocalPlayerVTable_sigOffset == 0x00) {
			logF("[LocalPlayer::SetVtables] [Error]Find LocalPlayer LocalPlayerVTable_sigOffset is no working!!!");
			// LocalPlayer构造函数调用+10
			auto LocalP_LocalP_SigOffset = FindSignature("4D 8B C7 48 8B D5 48 8B CF E8 ? ? ? ? 48 8B");	
			if (LocalP_LocalP_SigOffset != 0x00) {
				auto lplp = Utils::FuncFromSigOffset(LocalP_LocalP_SigOffset, 10);
				logF_Debug("LocalPlayer 构造函数位置: %llX", lplp);
			}
		}
		else {
			auto LocalPlayerVTable = Utils::FuncFromSigOffset<uintptr_t**>(LocalPlayerVTable_sigOffset, 3);
			logF_Debug("[LocalPlayer::SetVtables] [Success] 虚表地址= %llX , sigoffset= %llX , memcode=%s", LocalPlayerVTable, LocalPlayerVTable_sigOffset, memcode);
			LocalPlayer::SetVFtables(LocalPlayerVTable);

			//找本地玩家到ClientInstance的指针偏移
			auto LP_CIoffset = Utils::FindSignatureRelay(LocalPlayerVTable_sigOffset, "48 89 B7", 500);
			if (LP_CIoffset) {
				LocalPlayer::toCIoffset = *reinterpret_cast<int*>(LP_CIoffset + 3);
				logF_Debug("[LocalPlayer::SetVtables] [Success] Find LocalPlayer To ClientInstance offset");
			}
			else {
				logF("[LocalPlayer::SetVtables] [Error] 寻找 LocalPlayer To ClientInstance 偏移地址失败,LP_CIoffset:%llX", LP_CIoffset);
			}
			//虚表Hook
			MH_CreateHookEx((LPVOID)LocalPlayer::GetVFtableFun(221), &Hook::LocalPlayer_TickWorld, &LocalPlayer::tickWorldCall);

		}
	}
// FishingHook 虚表地址特征码：48 8D 05 ? ? ? ? 48 89 03 C7 83 ? ? ? ? ? ? ? ? C7 83 ? ? ? ? ? ? ? ? 48 C7 83

	// 关于虚表怎么找 看:https://github.com/cngege/Mod/wiki/%E9%87%8D%E5%A4%A7%E5%8F%91%E7%8E%B0_%E7%90%86%E8%AE%BA%E5%8F%AF%E4%BB%A5%E8%8E%B7%E5%8F%96%E6%89%80%E6%9C%89%E7%B1%BB%E7%9A%84%E8%99%9A%E8%A1%A8
	// Level VFT
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 01 48 8D 05 ? ? ? ? 48 89 41 ? 48 8D 05 ? ? ? ? 48 89 41 ? 48 81 C1 ? ? ? ? E8";
		const char* memcode2 = "48 8D 05 ? ? ? ? 48 89 07 48 8D 05 ? ? ? ? 48 89 47 ? 48 8D  05 ? ? ? ? 48 89 47 ? 33";
		auto levelVF_sigOffset = FindSignature(memcode);
		if (levelVF_sigOffset == 0x00) {
			logF_Debug("[Level::SetVFtables] [Watn] 第一个特征码失效");
			levelVF_sigOffset = FindSignature(memcode2);
		}

		if (levelVF_sigOffset == 0x00) {
			logF("[Level::SetVFtables] [Error] Level获取虚表错误,所有特征码失效");
		}
		else {
			auto LevelVT = Utils::FuncFromSigOffset<uintptr_t**>(levelVF_sigOffset, 3);
			Level::SetVFtables(LevelVT);
			logF_Debug("[Level::SetVtables] [Success] 虚表地址= %llX , sigoffset= %llX , memcode=%s", LevelVT, levelVF_sigOffset, memcode);

			MH_CreateHookEx((LPVOID)Level::GetVFtableFun(2), &Hook::level_startLeaveGame, &Level::startLeaveGameCall);
			MH_CreateHookEx((LPVOID)Level::GetVFtableFun(98), &Hook::Level_Tick, &Level::tickCall);

		}
	}

	//LoopbackPacketSender::LoopbackPacketSender
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 8B 5C 24 ? 48 89 06 33";
		//auto lppackVF_sigoffset = FindSignature(memcode);
		//if (lppackVF_sigoffset == 0x00) {
		//	logF("[LoopbackPacketSender VF] LoopbackPacketSender虚表地址获取失败");
		//}
		//else {
		//	auto LoopbackPacketSenderVT = Utils::FuncFromSigOffset<uintptr_t**>(lppackVF_sigoffset, 3);
		//	LoopbackPacketSender::SetVFtables(LoopbackPacketSenderVT);
		//	logF_Debug("[LoopbackPacketSender::SetVtables] [Success] 虚表地址= %llX , sigoffset= %llX , memcode=%s", LoopbackPacketSenderVT, lppackVF_sigoffset, memcode);
		//	//LoopbackPacketSender::sendToServer
		//	MH_CreateHookEx((LPVOID)LoopbackPacketSender::GetVFtableFun(2), &Hook::LoopbackPacketSender_SendServer, &LoopbackPacketSenderToServerCall);
		//}

		SignCode sign("LoopbackPacketSender虚表");
		sign.AddSignCall(memcode, 3);
		if (sign) {
			logF_Debug("[LoopbackPacketSender::SetVtables] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", *sign, sign.ValidPtr(), sign.ValidSign());
			LoopbackPacketSender::SetVFtables((uintptr_t**)*sign);
			//LoopbackPacketSender::sendToServer
			MH_CreateHookEx((LPVOID)LoopbackPacketSender::GetVFtableFun(2), &Hook::LoopbackPacketSender_SendServer, &LoopbackPacketSenderToServerCall);
		}
	}

	//CI 虚表
	{
		const char* memcode = "48 8D 05 ? ? ? ? 48 89 06 48 8D 05 ? ? ? ? 48 89 46 ? 48 8D 05 ? ? ? ? 48 89 86";
		SignCode sign("CI虚表");
		sign.AddSignCall(memcode, 3);
		sign.AddSignCall("48 8D 05 ? ? ? ? 48 89 01 48 8D 05 ? ? ? ? 48 89 41 ? 48 8D 05 ? ? ? ? 48 89 81", 3);
		if (sign) {
			logF_Debug("[CI] [Success] 虚表指针= %llX , sigoffset= %llX , memcode=%s", *sign, sign.ValidPtr(), sign.ValidSign());
			ClientInstance::SetVFtables((uintptr_t**)*sign);
			// 记得同步修改 Clientinstance::getMinecraftGame()
			ClientInstance::getMinecraftGameOffset = *reinterpret_cast<int*>((uintptr_t)ClientInstance::GetVFtableFun(140) + 3);	//必须在Hook前获取
			MH_CreateHookEx((LPVOID)ClientInstance::GetVFtableFun(140), &Hook::ClientInstance_onDimensionChanged, &ClientInstanceOnDimensionChangedCall);
		}
	}
}

auto Hook::exit() -> void {
	logF("[MH_DisableHook] Hook关闭状态: %s", MH_StatusToString(MH_DisableHook(MH_ALL_HOOKS)));
	logF("[MH_RemoveHook] Hook移除状态: %s", MH_StatusToString(MH_RemoveHook(MH_ALL_HOOKS)));
	Sleep(10);
}





/*
__int64 __fastcall sub_141C45E20(
__int64** a1,
_DWORD* a2,
__int64* a3,
__int64 a4,
__int64 a5,
__int64 a6,
__int64 a7,
__int64 a8,
__int64 a9,
__int64 a10,
__int64 a11)
*/
//41 0F 10 47 ? 0F 11 45 00 41 0F 10 4F
//auto Hook::LockControlInput(void* thi, void* a2, void* a3, void* a4, void* a5, void* a6) -> void*
auto Hook::LockControlInputCallBack(void* thi, void* a2, void* a3, void* a4, void* a5, void* a6) -> void*
{
	static LockControlInput* lci = Game::GetModuleManager()->GetModule<LockControlInput*>();
	if (lci) {
		lci->ControlTick((LockControlInput::ControlKey*)((uintptr_t)a2 + LockControlInput_offset));
	}
	return LockControlInputcall(thi, a2, a3, a4, a5, a6);
}

//无击退效果
auto Hook::SetVelocity(Actor* actor,vec3_t* kb)->void*
{
	if (actor->isLocalPlayer() && !Game::GetModuleManager()->onKnockback((LocalPlayer*)actor, kb)) {
		return nullptr;
	}
	return actor->setVelocity(kb);
}


auto Hook::Actor_isInvisible(Actor* _this) -> bool
{
	static FastViewPerspective* idy = Game::GetModuleManager()->GetModule<FastViewPerspective*>();
	if (idy && idy->isEnabled() && idy->isToggle() && idy->Hide())
	{
		if (_this->isLocalPlayer()) {
			return true;
		}
	}
	return _this->isInvisible();
}

auto Hook::Actor_getShadowRadius(Actor* actor)->float {
	Game::GetModuleManager()->onActorSightTick(actor);
	return actor->getShadowRadius();
}

auto Hook::ClientInstance_Tick(ClientInstance* _this, void* a1) -> uintptr_t
{
	if (Game::Cinstance == nullptr) {
		logF_Debug("Game::Cinstance : %llX", (uintptr_t)_this);
		Game::Cinstance = _this;
	}
	return clientInstance_Tickcall(_this, a1);
}


auto Hook::Is_ShowCoordinates_Tick(void* _this, void* a2, void* a3)->bool
{
	static ShowCoordinates* sc = Game::GetModuleManager()->GetModule<ShowCoordinates*>();
	bool ret = is_ShowCoordinates_Tickcall(_this, a2, a3);
	if (sc && sc->isEnabled()) {
		ret = true;
	}
	return ret;
}


// 定位这个函数的特征码 48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 8B D9 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48
auto Hook::Level_Tick(Level* level)->void*
{
	Game::GetModuleManager()->onLevelTick(level);
	return level->Tick();
}

//在非本地房间 只有本地玩家才会触发Tick
// 为什么远程玩家不会触发这个tick，因为远程玩家重载了tick方法，且其中没有调用此tick
auto Hook::Player_tickWorld(Player* player, Tick* tick)->void
{
	//Game::GetModuleManager()->onPlayerTick(player);
	player->tickWorld(tick);
}

auto Hook::Player_getShadowRadius(Player* player) -> float
{
	Game::GetModuleManager()->onPlayerSightTick(player);
	return player->getShadowRadius();
}

//auto Hook::Player_startSwimming(Player* player) ->void
//{
//	static NoWaterResistence* nwr = Game::GetModuleManager()->GetModule<NoWaterResistence*>();
//	if (nwr->isEnabled()) {
//		return;
//	}
//	player->startSwimming();
//}

//一直调用 且每位玩家都调用 这个函数好像是Player::getShadowRadius 不确定
auto Hook::AllActor_Tick(Actor* _this, float* a1, float a2)->float* {
	Game::GetModuleManager()->onActorTick(_this);
	return allActor_Tickcall(_this, a1, a2);
}

// 所有玩家Tick 这里当前是本地和远程玩家
auto Hook::AllPlayer_Tick(void* a1, Player* player) -> char
{
	Game::GetModuleManager()->onPlayerTick(player);
	return allPlayer_Tickcall(a1,player);
}

//就是把生物当前位置加上这个值 v3
//auto Hook::Actor_moveBBs(Actor* _this, vec3_t* v3)->void* {
//	_this->onMoveBBs(*v3);
//	return actor_moveBBscall(_this, v3);
//}

ImGuiKey ImGui_VirtualKeyToImGuiKey(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_TAB: return ImGuiKey_Tab;
	case VK_LEFT: return ImGuiKey_LeftArrow;
	case VK_RIGHT: return ImGuiKey_RightArrow;
	case VK_UP: return ImGuiKey_UpArrow;
	case VK_DOWN: return ImGuiKey_DownArrow;
	case VK_PRIOR: return ImGuiKey_PageUp;
	case VK_NEXT: return ImGuiKey_PageDown;
	case VK_HOME: return ImGuiKey_Home;
	case VK_END: return ImGuiKey_End;
	case VK_INSERT: return ImGuiKey_Insert;
	case VK_DELETE: return ImGuiKey_Delete;
	case VK_BACK: return ImGuiKey_Backspace;
	case VK_SPACE: return ImGuiKey_Space;
	case VK_RETURN: return ImGuiKey_Enter;
	case VK_ESCAPE: return ImGuiKey_Escape;
	case VK_OEM_7: return ImGuiKey_Apostrophe;
	case VK_OEM_COMMA: return ImGuiKey_Comma;
	case VK_OEM_MINUS: return ImGuiKey_Minus;
	case VK_OEM_PERIOD: return ImGuiKey_Period;
	case VK_OEM_2: return ImGuiKey_Slash;
	case VK_OEM_1: return ImGuiKey_Semicolon;
	case VK_OEM_PLUS: return ImGuiKey_Equal;
	case VK_OEM_4: return ImGuiKey_LeftBracket;
	case VK_OEM_5: return ImGuiKey_Backslash;
	case VK_OEM_6: return ImGuiKey_RightBracket;
	case VK_OEM_3: return ImGuiKey_GraveAccent;
	case VK_CAPITAL: return ImGuiKey_CapsLock;
	case VK_SCROLL: return ImGuiKey_ScrollLock;
	case VK_NUMLOCK: return ImGuiKey_NumLock;
	case VK_SNAPSHOT: return ImGuiKey_PrintScreen;
	case VK_PAUSE: return ImGuiKey_Pause;
	case VK_NUMPAD0: return ImGuiKey_Keypad0;
	case VK_NUMPAD1: return ImGuiKey_Keypad1;
	case VK_NUMPAD2: return ImGuiKey_Keypad2;
	case VK_NUMPAD3: return ImGuiKey_Keypad3;
	case VK_NUMPAD4: return ImGuiKey_Keypad4;
	case VK_NUMPAD5: return ImGuiKey_Keypad5;
	case VK_NUMPAD6: return ImGuiKey_Keypad6;
	case VK_NUMPAD7: return ImGuiKey_Keypad7;
	case VK_NUMPAD8: return ImGuiKey_Keypad8;
	case VK_NUMPAD9: return ImGuiKey_Keypad9;
	case VK_DECIMAL: return ImGuiKey_KeypadDecimal;
	case VK_DIVIDE: return ImGuiKey_KeypadDivide;
	case VK_MULTIPLY: return ImGuiKey_KeypadMultiply;
	case VK_SUBTRACT: return ImGuiKey_KeypadSubtract;
	case VK_ADD: return ImGuiKey_KeypadAdd;
	case (VK_RETURN + 256): return ImGuiKey_KeypadEnter;
	case VK_LSHIFT: return ImGuiKey_LeftShift;
	case VK_LCONTROL: return ImGuiKey_LeftCtrl;
	case VK_LMENU: return ImGuiKey_LeftAlt;
	case VK_LWIN: return ImGuiKey_LeftSuper;
	case VK_RSHIFT: return ImGuiKey_RightShift;
	case VK_RCONTROL: return ImGuiKey_RightCtrl;
	case VK_RMENU: return ImGuiKey_RightAlt;
	case VK_RWIN: return ImGuiKey_RightSuper;
	case VK_APPS: return ImGuiKey_Menu;
	case '0': return ImGuiKey_0;
	case '1': return ImGuiKey_1;
	case '2': return ImGuiKey_2;
	case '3': return ImGuiKey_3;
	case '4': return ImGuiKey_4;
	case '5': return ImGuiKey_5;
	case '6': return ImGuiKey_6;
	case '7': return ImGuiKey_7;
	case '8': return ImGuiKey_8;
	case '9': return ImGuiKey_9;
	case 'A': return ImGuiKey_A;
	case 'B': return ImGuiKey_B;
	case 'C': return ImGuiKey_C;
	case 'D': return ImGuiKey_D;
	case 'E': return ImGuiKey_E;
	case 'F': return ImGuiKey_F;
	case 'G': return ImGuiKey_G;
	case 'H': return ImGuiKey_H;
	case 'I': return ImGuiKey_I;
	case 'J': return ImGuiKey_J;
	case 'K': return ImGuiKey_K;
	case 'L': return ImGuiKey_L;
	case 'M': return ImGuiKey_M;
	case 'N': return ImGuiKey_N;
	case 'O': return ImGuiKey_O;
	case 'P': return ImGuiKey_P;
	case 'Q': return ImGuiKey_Q;
	case 'R': return ImGuiKey_R;
	case 'S': return ImGuiKey_S;
	case 'T': return ImGuiKey_T;
	case 'U': return ImGuiKey_U;
	case 'V': return ImGuiKey_V;
	case 'W': return ImGuiKey_W;
	case 'X': return ImGuiKey_X;
	case 'Y': return ImGuiKey_Y;
	case 'Z': return ImGuiKey_Z;
	case VK_F1: return ImGuiKey_F1;
	case VK_F2: return ImGuiKey_F2;
	case VK_F3: return ImGuiKey_F3;
	case VK_F4: return ImGuiKey_F4;
	case VK_F5: return ImGuiKey_F5;
	case VK_F6: return ImGuiKey_F6;
	case VK_F7: return ImGuiKey_F7;
	case VK_F8: return ImGuiKey_F8;
	case VK_F9: return ImGuiKey_F9;
	case VK_F10: return ImGuiKey_F10;
	case VK_F11: return ImGuiKey_F11;
	case VK_F12: return ImGuiKey_F12;
	case VK_F13: return ImGuiKey_F13;
	case VK_F14: return ImGuiKey_F14;
	case VK_F15: return ImGuiKey_F15;
	case VK_F16: return ImGuiKey_F16;
	case VK_F17: return ImGuiKey_F17;
	case VK_F18: return ImGuiKey_F18;
	case VK_F19: return ImGuiKey_F19;
	case VK_F20: return ImGuiKey_F20;
	case VK_F21: return ImGuiKey_F21;
	case VK_F22: return ImGuiKey_F22;
	case VK_F23: return ImGuiKey_F23;
	case VK_F24: return ImGuiKey_F24;
	case VK_BROWSER_BACK: return ImGuiKey_AppBack;
	case VK_BROWSER_FORWARD: return ImGuiKey_AppForward;
	default: return ImGuiKey_None;
	}
}

auto Hook::KeyUpdate(__int64 key, int isdown)->void* {

	// 防止在ImGui输入框中触发热键
	if (ImGui::GetCurrentContext() == nullptr || !ImGui::GetIO().WantTextInput) {
		Game::GetModuleManager()->onKeyUpdate((int)key, isdown == 1);
	}
	
	//快捷键反注入
	if (Loader::EnableEjectKey && key == 'L' && isdown && Game::IsKeyDown(VK_CONTROL)) {
		Loader::Eject_Signal = true;
		return 0;
	}
	
#ifndef IMGUIINPUT_USE_WNDPROC
	try
	{
		//IMGUI 按键信号传递
		if (ImGui::GetCurrentContext() != nullptr) {
			ImGuiIO& io = ImGui::GetIO();
			bool is_key_down = (isdown == 1);
			auto scancode = MapVirtualKey(static_cast<UINT>(key), MAPVK_VK_TO_VSC);

			io.AddKeyEvent(ImGuiMod_Ctrl, KEY_DOWN(VK_CONTROL));
			io.AddKeyEvent(ImGuiMod_Shift, KEY_DOWN(VK_SHIFT));
			io.AddKeyEvent(ImGuiMod_Alt, KEY_DOWN(VK_MENU));
			io.AddKeyEvent(ImGuiMod_Super, KEY_DOWN(VK_APPS));

			ImGuiKey imKey = ImGui_VirtualKeyToImGuiKey(key);
			if (imKey != ImGuiKey_None && imKey != ImGuiKey_Tab) {//屏蔽TAB键切换焦点
				//io.AddKeyEvent(imKey, isdown == 1);
				ImGui_ImplUWP_AddKeyEvent(imKey, is_key_down, static_cast<int>(key), scancode);
			}

			if (key == VK_SHIFT)
			{
				// Important: Shift keys tend to get stuck when pressed together, missing key-up events are corrected in ImGui_ImplWin32_ProcessKeyEventsWorkarounds()
				if (KEY_DOWN(VK_LSHIFT) == is_key_down) { ImGui_ImplUWP_AddKeyEvent(ImGuiKey_LeftShift, is_key_down, VK_LSHIFT, scancode); }
				if (KEY_DOWN(VK_RSHIFT) == is_key_down) { ImGui_ImplUWP_AddKeyEvent(ImGuiKey_RightShift, is_key_down, VK_RSHIFT, scancode); }
			}
			else if (key == VK_CONTROL)
			{
				if (KEY_DOWN(VK_LCONTROL) == is_key_down) { ImGui_ImplUWP_AddKeyEvent(ImGuiKey_LeftCtrl, is_key_down, VK_LCONTROL, scancode); }
				if (KEY_DOWN(VK_RCONTROL) == is_key_down) { ImGui_ImplUWP_AddKeyEvent(ImGuiKey_RightCtrl, is_key_down, VK_RCONTROL, scancode); }
			}
			else if (key == VK_MENU)
			{
				if (KEY_DOWN(VK_LMENU) == is_key_down) { ImGui_ImplUWP_AddKeyEvent(ImGuiKey_LeftAlt, is_key_down, VK_LMENU, scancode); }
				if (KEY_DOWN(VK_RMENU) == is_key_down) { ImGui_ImplUWP_AddKeyEvent(ImGuiKey_RightAlt, is_key_down, VK_RMENU, scancode); }
			}


			if (io.WantTextInput) {
				if (is_key_down) {
					//if (Game::Cinstance) Game::Cinstance->setSuspendInput(false);
					// 这里不负责粘贴部分的工作
					BYTE kb[256];
					if (GetKeyboardState(kb)) {
						wchar_t ch[6] = { 0 };
						int ret = ToUnicode(static_cast<UINT>(key), scancode, kb, (LPWSTR)ch, 5, 0);
						if (ret > 0) {
							io.AddInputCharacterUTF16(ch[0]);
						}

					}
				}
				return 0;
			}
			if (ImGui::GetIO().WantCaptureKeyboard) {	// 这里应该是指焦点在ImGui上, 可能不在输入框里
				return 0;
			}
		}
	}
	catch (const std::exception& ex)
	{
		logF_Debug("KeyUpdate 异常");
		logF_Debug("%s", ex.what());
	}

#endif // IMGUIINPUT_USE_WNDPROC == false

	return keyupdatecall(key, isdown);
}



//触发: 鼠标在窗口中经过就会触发 mousebutton=0,isDown=0
//mousebutton 1:鼠标左键,2鼠标右键,3:鼠标中键,4:鼠标滚轮滚动(isDown 为+-120左右的值),
auto Hook::MouseUpdate(__int64 a1, char mousebutton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8)->void {
	Game::MouseKeyDown[mousebutton] = isDown;
	Game::GetModuleManager()->onMouseUpdate(mousebutton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY);
#ifndef IMGUIINPUT_USE_WNDPROC
	try
	{
		if (ImGui::GetCurrentContext() != nullptr) {
			ImGuiMouseSource mouse_source = GetMouseSourceFromMessageExtraInfo();
			ImGuiIO& io = ImGui::GetIO();
			io.AddMouseSourceEvent(mouse_source);
			switch (mousebutton) {
			case 1:
				//io.MouseDown[0] = isDown;
				io.AddMouseButtonEvent(0, isDown);
				break;
			case 2:
				//io.MouseDown[1] = isDown;
				io.AddMouseButtonEvent(1, isDown);
				break;
			case 3:
				//io.MouseDown[2] = isDown;
				io.AddMouseButtonEvent(2, isDown);
				break;
			case 4:
				//io.MouseWheel = isDown < 0 ? -0.5f : 0.5f; //For scrolling
				io.AddMouseWheelEvent(0.f, isDown < 0 ? -1.f : 1.f);
				break;
			default:
				io.AddMousePosEvent(mouseX, mouseY);
				break;
			}
			if (/*io.WantCaptureMouse && */io.WantCaptureMouseUnlessPopupClose)
				return;
		}
	}
	catch (const std::exception& ex)
	{
		logF_Debug("MouseUpdate 异常");
		logF_Debug("%s", ex.what());
	}

#endif // IMGUIINPUT_USE_WNDPROC == false

	mouseupdatecall(a1, mousebutton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
}

/**
int frame = 0;		// 按照视频作者的说法，这个函数会在三层每层都调用一次，也就是每一帧调用三次
auto Hook::RenderDetour(void* _this, MinecraftUIRenderContext* ctx)->void {
	renderDetourcall(_this, ctx);
	if (frame >= 3) {
		frame = 0;
		Game::Cinstance = ctx->CI;
		//Game::GetModuleManager()->onRenderDetour(ctx);
	}
	frame++;
}
*/
/*
auto Hook::Draw_Text(MinecraftUIRenderContext* _this, BitmapFont* a1, RectangleArea const& a2, TextHolder* a3, UIColor const& a4, float a5, float a6, TextMeasureData* a7, CaretMeasureData* a8)->void {
	if (Game::mcfont != a1) {
		if (Game::mcfont == nullptr) {
			Game::mcfont = a1;
		}
	}
	reinterpret_cast<MUICDrawText>(MinecraftUIRenderContext::drawtextCall)(_this, a1, a2, a3, a4, a5, a6, a7, a8);
}
*/


auto Hook::sendMessage(void* a1, std::mcstring* a2)->__int64 {
	//if (strcmp(a2->getText(), "1") == 0)
	auto ret = Game::GetModuleManager()->onSendMessage(a2);
	if (!ret) {
		return 0;
	}
	return sendChatMessagecall(a1, a2);
}

//返回值0-2 分别对应玩家第一二三人称视角
auto Hook::getLocalPlayerViewPerspective(void* thi)->int {
	int _sourceViewPerspective = getLocalPlayerViewPerspectivecall(thi);

	static FastViewPerspective* idy = Game::GetModuleManager()->GetModule<FastViewPerspective*>();
	if (idy) {
		if (idy->isEnabled()) {
			return idy->getViewPerspective(_sourceViewPerspective);
		}
	}
	return _sourceViewPerspective;
}

//此特征码可直接定位该函数 "40 53 48 83 EC ? 48 8B D9 48 81 C1 ? ? ? ? E8 ? ? ? ? 48 8D 8B ? ? ? ? E8 ? ? ? ? 48 8D 8B ? ? ? ?  E8 ? ? ? ? 48 8B ? 48"
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
auto Hook::GameMode_useItemOn(GameMode* gm, class ItemStack* item, class ItemInstance* itemins, vec3_ti* bpos, uint8_t* face, vec3_t* f, class Block* block)->bool
{
	if (!Game::GetModuleManager()->useItemOn(gm, item, itemins,bpos,face,f,block)) {
		return false;
	}
	return gm->useItemOn(item, itemins,bpos,face,f,block);
}


auto Hook::GameMode_tick(GameMode* _this)->void* {
	logF_Debug("[Hook::GameMode_tick] GameMode_tick 在该版本中恢复工作。。。");
	Game::GetModuleManager()->onTick(_this);
	return _this->tick();
}


auto Hook::GameMode_attack(GameMode* _this, Actor* actor)->bool {
// 本地房间攻击的时候 会调用两次, 先调用的是本地玩家,后调用的可能是服务玩家，在他人房间只会调用一次 就是本地玩家
	static auto debug = Game::GetModuleManager()->GetModule<Debug*>();
	if (debug) {
		if (_this->GetLocalPlayer()->isLocalPlayer()) {
			//logF("attack Actor ptr= %llX, ActorType = %i, sizex = %f, sizey = %f, isplayer=%i, islocalplayer=%i", actor, actor->getEntityTypeId(),actor->getHitBox().x, actor->getHitBox().y,actor->isPlayer(),actor->isLocalPlayer());
			//_this->GetLocalPlayer()->getSelectedItem()->use(_this->GetLocalPlayer());	//必须是服务玩家才能有效， 否则可能崩溃
			//_this->useItem(_this->GetLocalPlayer()->getSelectedItem());
		}

		if (debug->isEnabled()) {
			if (debug->GameMode_attack_Print) {
				logF_Debug("LocalPlayer: %llX, CI: %llX, attackActor: %llX", (uintptr_t)_this->GetLocalPlayer(), (uintptr_t)_this->GetLocalPlayer()->getClientInstance(), (uintptr_t)actor);
			}
			
			if (debug->GameMode_attack_UseItem) {
				// TODO: use这个函数坏了, 没修, 后面再考虑
				if (!_this->GetLocalPlayer()->isClientSide()) {
					//	_this->GetLocalPlayer()->getSelectedItem()->use(_this->GetLocalPlayer());
					_this->useItem(_this->GetLocalPlayer()->getSelectedItem());
					// 更新背包
				}
			}
		}
	}

	bool ret = false;
	if (Game::GetModuleManager()->onAttackBefore(_this, actor)) {
		ret = _this->attack(actor);
	}
	Game::GetModuleManager()->onAttackAfter(_this, actor);
	return ret;
}

auto Hook::LocalPlayer_TickWorld(LocalPlayer* _this, void* tick) -> void*
{
	static INT64 p = 0;
	auto thisp = reinterpret_cast<INT64>(_this);
	if (thisp != p) {
		p = thisp;
		Game::localplayer = _this;
		logF_Debug("[%s] 本地玩家地址: %llX,虚表 = %llX", "LocalPlayer_tick", thisp, *(INT64*)thisp);
		logF_Debug("[%s] Clientinstance: %llX ,通过本地玩家获取的CI: %llX ,虚表 = %llX", "LocalPlayer_tick", Game::Cinstance, _this->getClientInstance(), *(uintptr_t*)_this->getClientInstance());
		//logF_Debug("本地玩家所在的维度的指针: %llX", _this->getDimensionConst());
	}


	Game::GetModuleManager()->onLocalPlayerTick(_this);
	using Fn = void*(__fastcall*)(LocalPlayer*, void*);
	return reinterpret_cast<Fn>(LocalPlayer::tickWorldCall)(_this, tick);
}

auto Hook::ServerPlayer_TickWorld(ServerPlayer* _this, void* tick)->void* {
	Game::GetModuleManager()->onServerPlayerTick(_this);
	//_this->onAllPlayerTick();				//这里应该是 所有serverplayer tick
	if (!Game::Cinstance) {
		Game::localServerPlayer = nullptr;
	}
	auto lp = Game::Cinstance->getCILocalPlayer();
	if (!lp) {
		Game::localServerPlayer = nullptr;
	}
	if (*_this->getOrCreateUniqueID() == *lp->getOrCreateUniqueID()) {
		Game::localServerPlayer = _this;
	}

	return _this->tickWorld(tick);
}

// 1.20 没有远程玩家调用这个函数
auto Hook::RemotePlayer_TickWorld(RemotePlayer* _this) -> void*
{
	//判断虚表是否是远程玩家
	if (_this && (uintptr_t)_this > 0xFFFFFFFF && *(uintptr_t***)_this == RemotePlayer::GetVFtables()) {
		Game::GetModuleManager()->onRemotePlayerTick(_this);
	}
	return _this->tickWorld();
}

auto Hook::LoopbackPacketSender_SendServer(LoopbackPacketSender* _this, Packet* pack)-> void* {
	
	if (!Game::GetModuleManager()->onSendPacketToServer(_this, pack)) {
		return nullptr;
	}
	return LoopbackPacketSenderToServerCall(_this, pack);
}

auto Hook::ClientInstance_onDimensionChanged(ClientInstance* ci) -> void*
{
	Game::GetModuleManager()->onDimensionChanged(ci);
	return ClientInstanceOnDimensionChangedCall(ci);
}
