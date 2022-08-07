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