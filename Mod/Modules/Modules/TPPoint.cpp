#include "TPPoint.h"
#include "../../Utils/Game.h"
#include "LocalPlayer.h"

TPPoint::TPPoint() : Module(VK_F3, "TPPoint", "传送坐标点") {
	SetKeyMode(KeyMode::Trigger);
	point = new vec3_t();
}

TPPoint::~TPPoint() {
	delete[] point;
}


auto TPPoint::onTrigger()->void {

	//记录传送点
	if (Game::IsKeyDown(VK_CONTROL)) {
		if (Game::localplayer) {
			auto p = Game::localplayer->getPos();
			point->x = p.x;
			point->y = p.y;
			point->z = p.z;
		}
	}

	//读取传送过去
	if (Game::IsKeyDown(VK_SHIFT) && point && (point->x || point->y || point->z)) {
		if (Game::localplayer) {
			//Game::localplayer->setPos(*point);
			Game::localplayer->teleportTo(point,true,0,1);
		}
	}
}

auto TPPoint::getBindKeyName()->std::string {
	//ret : CTRL + SHIFT + F
	std::string name = Utils::getKeybindName(VK_SHIFT);
	name += "/";
	name += Utils::getKeybindName(VK_CONTROL);
	return name;
}