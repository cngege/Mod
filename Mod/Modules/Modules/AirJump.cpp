#pragma execution_character_set("utf-8")
#include "AirJump.h"
#include "LocalPlayer.h"
#include "GameMode.h"

AirJump::AirJump() : Module(0, "AirJump", "¿ÕÆøÌø") {

}

auto AirJump::onTick(GameMode* gm)->void {
	if (isEnabled()) {
		LocalPlayer* lp = gm->GetLocalPlayer();
		if (lp != nullptr) {
			*lp->isOnGround() = true;
		}
	}
}

auto AirJump::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", false));
}
auto AirJump::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}