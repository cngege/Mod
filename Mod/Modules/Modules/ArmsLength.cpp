#include "ArmsLength.h"
#include "../../Utils/Logger.h"

ArmsLength::ArmsLength() : Module(VK_F10, "ArmsLength", "修改玩家攻击距离") {
	setcontrolkeysbind({ VK_SHIFT });
	AddFloatUIValue("攻击距离", &distance, 0, 7.f, true);
	auto sigOffset = FindSignature("84 C0 74 ? C7 45 ? ? ? ? ? 48 8D 85 ? ? ? ? 48 8D 4D ? 44 0F 2F ? ? ? ? ? 48 0F 43 C1");
	if (sigOffset == 0x00) {
		logF("[ArmsLength::ArmsLength] [error] FindSignature sigOffset NoFound");
		return;
	}
	auto offset = *reinterpret_cast<int*>(sigOffset + 52);
	arms = reinterpret_cast<float*>(sigOffset + 56 + offset);//指向玩家攻击距离的指针 52(22) 56(56-30=26)
	//setEnabled(true);	//默认开启
}

auto ArmsLength::onEnable()->void {
	DWORD old_Page;
	bool b = VirtualProtect(arms, sizeof(float), PAGE_READWRITE, &old_Page);
	if (b) {
		*arms = distance;
		VirtualProtect(arms, sizeof(float), old_Page, &old_Page);
	}
}

auto ArmsLength::onDisable()->void {
	//长臂管辖
	DWORD old_Page;
	bool b = VirtualProtect(arms, sizeof(float), PAGE_READWRITE, &old_Page);
	if (b) {
		*arms = 3.0f;
		VirtualProtect(arms, sizeof(float), old_Page, &old_Page);
	}
}

auto ArmsLength::isEnabled()->bool {
	return *arms != 3.f;
}

auto ArmsLength::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", true));
	distance = config::readDataFromJson<float>(data, "distance", 7.f);
}
auto ArmsLength::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
	data["distance"] = distance;
}