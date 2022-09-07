#include "Traverse.h"
#include "../../Utils/Game.h"
#include "Actor.h"
#include "LocalPlayer.h"

Traverse::Traverse() : Module(VK_F2, "Traverse", "向所视方向前进一格") {
	SetKeyMode(KeyMode::Trigger);
}


int rotToCoordinateX(float X) {
	int ret = 0;
	if (X < 157.5f && X > 22.5f) {
		return -1;
	}
	if (X > -157.5f && X < -22.5f) {
		return 1;
	}
	return 0;
}

int rotToCoordinateZ(float X) {
	int ret = 0;
	if ((X < 180.f && X > 112.5f) || (X < -112.5f && X > -180.f)) {
		return -1;
	}
	if (X > -67.5f && X < 67.5f) {
		return 1;
	}
	return 0;
}

auto Traverse::onTrigger()->void {
	LocalPlayer* lp = Game::localplayer;
	if (lp == nullptr) {
		return;
	}

	vec2_t rot = lp->getViewYX();
	vec3_t pos = *lp->getPosition();
	vec3_t toPos = vec3_t(pos.x + (float)rotToCoordinateX(rot.y), pos.y, pos.z + (float)rotToCoordinateZ(rot.y));
	//lp->setPos(vec3_t(pos.x + rotToCoordinateX(rot.y), pos.y, pos.z + rotToCoordinateZ(rot.y)));
	lp->setPos(&toPos);
	//lp->teleportTo(&toPos, true, 0, 1);
}