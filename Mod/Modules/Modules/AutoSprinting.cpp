#include "AutoSprinting.h"
#include "LocalPlayer.h"
#include "GameMode.h"

#include "../../Utils/Logger.h"

AutoSprinting::AutoSprinting() : Module(VK_F6, "AutoSprinting", "自动疾跑") {
	setcontrolkeysbind({ VK_CONTROL });
}

auto AutoSprinting::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", true));
}
auto AutoSprinting::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}