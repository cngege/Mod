#include "ArmsLength.h"
#include "../../Utils/Logger.h"

/*
* 需要说明： （函数原理, 判断是否创造模式->攻击距离比较7或比较3-> 比较生存攻击距离是否小于等于3
* 原来的方案是直接修改 这个3为指定的数
* 但新版 这个3有好多地方在用, 如果修改会同时影响多个地方
* 于是只能退而求其次, 修改实现逻辑, 判断如果不是创造, 直接跳到处理攻击的地方（原版是跳到判断攻击距离3）, 而原版的前一步就是越过这个判断
*/

// 这里修改的地址的数, 有很多地方共用, 比如除了这里，还有划船速度也在使用, 检查摔落高度也在使用
ArmsLength::ArmsLength() : Module(VK_F10, "ArmsLength", "修改玩家攻击距离") {
	setcontrolkeysbind({ VK_SHIFT });
	
	auto sigOffset = FindSignature("84 C0 74 ? C7 45 ? ? ? ? ? 48 8D 85 ? ? ? ? 48 8D 4D ? 44 0F 2F ? ? ? ? ? 48 0F 43 C1");
	if (sigOffset == 0x00) {
		logF("[ArmsLength::ArmsLength] [error] FindSignature sigOffset NoFound");
		return;
	}
	// new
	jumpaddr = reinterpret_cast<BYTE*>(sigOffset + 3);
	sJumpaddr = *jumpaddr;
	//auto offset = *reinterpret_cast<int*>(sigOffset + 52);
	//arms = reinterpret_cast<float*>(sigOffset + 56 + offset);//指向玩家攻击距离的指针 52(22) 56(56-30=26)
	//setEnabled(true);	//默认开启

}

auto ArmsLength::onEnable()->void {
	DWORD old_Page;
	bool b = VirtualProtect(jumpaddr, sizeof(BYTE), PAGE_READWRITE, &old_Page);
	if (b) {
		*jumpaddr = sJumpaddr - 2;
		VirtualProtect(jumpaddr, sizeof(BYTE), old_Page, &old_Page);
	}


	//DWORD old_Page;
	//bool b = VirtualProtect(arms, sizeof(float), PAGE_READWRITE, &old_Page);
	//if (b) {
	//	*arms = distance;
	//	VirtualProtect(arms, sizeof(float), old_Page, &old_Page);
	//}
}

auto ArmsLength::onDisable()->void {
	//长臂管辖

	// new
	DWORD old_Page;
	bool b = VirtualProtect(jumpaddr, sizeof(BYTE), PAGE_READWRITE, &old_Page);
	if (b) {
		*jumpaddr = sJumpaddr;
		VirtualProtect(jumpaddr, sizeof(BYTE), old_Page, &old_Page);
	}

	//DWORD old_Page;
	//bool b = VirtualProtect(arms, sizeof(float), PAGE_READWRITE, &old_Page);
	//if (b) {
	//	*arms = 3.0f;
	//	VirtualProtect(arms, sizeof(float), old_Page, &old_Page);
	//}
}

auto ArmsLength::isEnabled()->bool {
	return *jumpaddr != sJumpaddr;
}

auto ArmsLength::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", true));
	//distance = config::readDataFromJson<float>(data, "distance", 7.f);
}
auto ArmsLength::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
	//data["distance"] = distance;
}