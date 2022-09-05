#include "ArmsLength.h"
#include "../../Utils/Logger.h"

ArmsLength::ArmsLength() : Module(0, "ArmsLength", "玩家攻击距离") {
	setEnabled(true);	//默认开启
}

auto ArmsLength::onEnable()->void {
	static auto sigOffset = FindSignature("84 C0 74 ? C7 45 ? ? ? ? ? 48 8D 85 ? ? ? ? 48 8D 4D ? 44 0F 2F 25 ? ? ? ? 48 0F 43 C1");
	auto offset = *reinterpret_cast<int*>(sigOffset + 52);

	//指向玩家攻击距离的指针
	arms = reinterpret_cast<float*>(sigOffset + 56 + offset);//52(22) 56(56-30=26)

	if (arms == 0x0 || sigOffset == 0x0) {
		logF("[ArmsLength::onEnable] Survival mode ArmsLength not working!!!");
		arms = 0x00;
	}
	else {
		DWORD old_Page;
		bool b = VirtualProtect(arms, sizeof(float), PAGE_READWRITE, &old_Page);
		if (b) {
			logF("[ArmsLength::onEnable] Survival mode ArmsLength is %lf,new Modify to %lf", *arms, 8.0f);
			*arms = 8.0f;
			VirtualProtect(arms, sizeof(float), old_Page, &old_Page);
		}

	}
}

auto ArmsLength::onDisable()->void {
	//长臂管辖
	if (arms != 0x00) {
		DWORD old_Page;
		bool b = VirtualProtect(arms, sizeof(float), PAGE_READWRITE, &old_Page);
		if (b) {
			*arms = 3.0f;
			logF("[ArmsLength::onDisable] Survival mode ArmsLength Modify to %lf", *arms);
			VirtualProtect(arms, sizeof(float), old_Page, &old_Page);
		}
	}
}