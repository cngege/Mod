#include "NoKnockback.h"
#include "Game.h"

bool LongEnable = false;

NoKnockback::NoKnockback() : Module(VK_F4, "NoKnockback", "按下快捷键后不会被击退") {
	setcontrolkeysbind({ VK_CONTROL });
	AddBoolUIValue("开启后持续零击退,热键临时解除", &LongEnable);
	//SetKeyMode(KeyMode::Hold);
}

auto NoKnockback::getBindKeyName()->std::string {
	//ret : (CTRL) CTRL + F4
	std::string name = "(CTRL) ";
	name += Module::getBindKeyName();
	return name;
}

auto NoKnockback::onKnockback(LocalPlayer* lp, vec3_t* v3)->bool {
	if (isEnabled()) {
		if (LongEnable) {
			return Game::IsKeyDown(VK_CONTROL);
		}
		else {
			return !Game::IsKeyDown(VK_CONTROL);
		}
	}
	return true;
}

auto NoKnockback::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", true));
}
auto NoKnockback::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}