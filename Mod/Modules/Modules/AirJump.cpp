#include "AirJump.h"
#include "LocalPlayer.h"
#include "GameMode.h"

AirJump::AirJump() : Module(0, "AirJump", "空气跳") {

}

auto AirJump::onTick(GameMode* gm)->void {
	if (isEnabled()) {
		LocalPlayer* lp = gm->GetLocalPlayer();
		if (lp != nullptr) {
			*lp->isOnGround() = true;
		}
	}
}

auto AirJump::onLocalPlayerTick(LocalPlayer* lp)->void
{
	if (isEnabled()) {
		*lp->isOnGround() = true;
	}
}

auto AirJump::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", false));
}
auto AirJump::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}