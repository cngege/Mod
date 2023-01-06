#pragma execution_character_set("utf-8")
#include "AutoWalking.h"

AutoWalking::AutoWalking() : Module(VK_F7, "AutoWalking", "自动前进") {
	setcontrolkeysbind({ VK_CONTROL });
}

auto AutoWalking::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", true));
}
auto AutoWalking::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}