#include "AutoSprinting.h"
#include "LocalPlayer.h"
#include "../../Utils/Game.h"

AutoSprinting::AutoSprinting() : Module(VK_F6, "AutoSprinting", "自动疾跑") {
	setEnabled(true);
}


auto AutoSprinting::onTick(GameMode* gm)->void {
	if (!isEnabled()) {
		return;
	}

	LocalPlayer* lp = Game::localplayer;
	if (lp != nullptr) {
		if (lp->getSpeed().magnitudexy() > 0.05f) {
			lp->setSprintingEx(true);					//setSprintingEx 不会被HIVE踢，原版虚表函数会，可能是没有检测而直接发包的原因
		}
	}
}