#include "Traverse.h"
#include "../../Utils/Game.h"
#include "Actor.h"
#include "LocalPlayer.h"
#include "ClientInstance.h"

Traverse::Traverse() : Module(VK_F2, "Traverse", "向所视方向前进一格") {
	//SetKeyMode(KeyMode::Trigger);
	//setEnabled(true);
	AddBoolUIValue("需要按下Ctrl键", &needCtrl);
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


auto Traverse::getBindKeyName()->std::string {
	std::string name = "(MButton)";
	name += Module::getBindKeyName();
	return name;
}

auto Traverse::onMouseUpdate(char mousebutton, char isdown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY)->void {
	if (isEnabled()) {
		if (mousebutton == VK_CANCEL && isdown) {
			//LocalPlayer* lp = Game::localplayer;
			LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
			if (lp == nullptr || !lp->isValid()) {
				return;
			}
			if (needCtrl) {
				if (!Game::IsKeyDown(VK_CONTROL)) {
					return;
				}
			}

			vec2_t rot = *lp->getRotEx1();
			vec3_t pos = *lp->getPosition();
			if (rot.x > 67.5f) {
				//如果玩家看向地面 则向下tp一格以穿过地面
				vec3_t toPos = vec3_t(pos.x, pos.y-1, pos.z);
				lp->setPos(&toPos);
			}
			else {
				vec3_t toPos = vec3_t(pos.x + (float)rotToCoordinateX(rot.y), pos.y, pos.z + (float)rotToCoordinateZ(rot.y));
				//lp->setPos(vec3_t(pos.x + rotToCoordinateX(rot.y), pos.y, pos.z + rotToCoordinateZ(rot.y)));
				lp->setPos(&toPos);
				//lp->teleportTo(&toPos, true, 0, 1);
			}
		}
	}
}

auto Traverse::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", true));
	needCtrl = config::readDataFromJson<bool>(data, "needCtrl", true);
}
auto Traverse::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
	data["needCtrl"] = needCtrl;
}
