#include "Game.h"
#include "../Utils/Logger.h"
#include "../Utils/Utils.h"

float* Game::ArmsLength = nullptr;

uintptr_t SpeedDestroyBlock;

auto Game::init() -> void
{
	logF("Game::init is start runner……");
	//84 C0 74 ? C7 45 ? ? ? ? ? 48 8D 85 ? ? ? ? 48 8D 4D ? 44 0F 2F 25 ? ? ? ? 48 0F 43 C1
	
	auto sigOffset = FindSignature("84 C0 74 ? C7 45 ? ? ? ? ? 48 8D 85 ? ? ? ? 48 8D 4D ? 44 0F 2F 25 ? ? ? ? 48 0F 43 C1");
	auto offset = *reinterpret_cast<int*>(sigOffset + 52);
	ArmsLength = reinterpret_cast<float*>(sigOffset + 56 + offset);//52(22) 56(56-30=26)
	
	if (ArmsLength == 0x0 || sigOffset == 0x0) {
		logF("Survival mode ArmsLength not working!!!");
		ArmsLength = 0x00;
	}
	else {
		DWORD old_Page;
		bool b = VirtualProtect(ArmsLength, sizeof(float), PAGE_READWRITE, &old_Page);
		if (b) {
			logF("[Game::init]Survival mode ArmsLength is %lf,new Modify to %lf", *ArmsLength, 7.0f);
			*ArmsLength = 7.0f;
			VirtualProtect(ArmsLength, sizeof(float), old_Page, &old_Page);
		}
		
	}


	//F3 0F 11 4B ? 33 F6 48 8B 45 ? 48 39 43 ? 0F 8D ? ? ? ? 48 69 8B ? ? ? ? ? ? ? ? 48 03 C8
	SpeedDestroyBlock = FindSignature("F3 0F 11 4B ? 33 F6 48 8B 45 ? 48 39 43 ? 0F 8D ? ? ? ? 48 69 8B ? ? ? ? ? ? ? ? 48 03 C8");
	if (SpeedDestroyBlock == 0x00) {
		logF("Survival mode SpeedDestroyBlock not working!!!");
	}
	else {
		//申请空间
		auto space = VirtualAlloc(0, 22, MEM_COMMIT | MEM_TOP_DOWN, PAGE_EXECUTE_READWRITE);
		if (space != 0x00) {
			//向申请的空间写入汇编数据
			//std::vector<BYTE> run = { 0xC7, 0x43, 0x24,0x00,0x00,0x00,0x00,0x33,0xF6,0x48,0x8B,0x45,0x08,0xEA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
			Utils::WriteMemBytes(space, { 0xC7, 0x43, 0x24,0x00,0x00,0x00,0x00,0xE9,0x00,0x00,0x00,0x00 });
			*(float*)(reinterpret_cast<INT64>(space) + 3) = 1.0f;
			*(int*)(reinterpret_cast<INT64>(space) + 8) = static_cast<int>(static_cast<INT64>(SpeedDestroyBlock) + 5 - (reinterpret_cast<INT64>(space) + 7) - 5);
			//memcpy((void*)space, &run, 21);
			logF("space HMODULE: %llX", space);
			
			DWORD old_Page;
			DWORD old_Page2;
			bool b = VirtualProtect((void*)SpeedDestroyBlock, 5, PAGE_EXECUTE_READWRITE, &old_Page);
			if (b) {
				Utils::WriteMemBytes((void*)SpeedDestroyBlock, { 0xE9, 0x00, 0x00,0x00,0x00 });
				*(int*)(SpeedDestroyBlock + 1) = static_cast<int>(reinterpret_cast<INT64>(space) - SpeedDestroyBlock - 5);
				VirtualProtect((void*)SpeedDestroyBlock, 5, old_Page, &old_Page2);
			}
		}
		else {
			logF("[SpeedDestroyBlock] VirtualAlloc free zone is error");
		}

	}
	

}


auto Game::exit() -> void
{
	//长臂管辖
	if (ArmsLength != 0x00) {
		DWORD old_Page;
		bool b = VirtualProtect(ArmsLength, sizeof(float), PAGE_READWRITE, &old_Page);
		if (b) {
			*ArmsLength = 3.0f;
			logF("[Game::exit]Survival mode ArmsLength Modify to %lf", *ArmsLength);
			VirtualProtect(ArmsLength, sizeof(float), old_Page, &old_Page);
		}
	}
}