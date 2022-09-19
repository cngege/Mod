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
			lp->setSprinting(true);
		}
	}
}