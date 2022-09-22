#include "AutoSprinting.h"
#include "LocalPlayer.h"
#include "GameMode.h"

#include "../../Utils/Logger.h"

AutoSprinting::AutoSprinting() : Module(VK_F6, "AutoSprinting", "自动疾跑") {
	setEnabled(true);
}


auto AutoSprinting::onTick(GameMode* gm)->void {
	if (!isEnabled()) {
		return;
	}
	if (gm->GetLocalPlayer() == nullptr) {
		logF("[AutoSprinting] gm->GetLocalPlayer() localPlayer is nullptr");
		setEnabled(false);
	}

	auto speed = gm->GetLocalPlayer()->getSpeed().magnitudexz();
	if (speed > 0.10f && speed < 0.15f) {
		gm->GetLocalPlayer()->setSprintingEx(true);					//setSprintingEx 不会被HIVE踢，原版虚表函数会，可能是没有检测而直接发包的原因
	}
}