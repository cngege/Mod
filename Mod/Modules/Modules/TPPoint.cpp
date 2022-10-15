#include "TPPoint.h"
#include "../../Utils/Game.h"
#include "LocalPlayer.h"
#include "TextHolder.h"
#pragma execution_character_set("utf-8")

TPPoint::TPPoint() : Module(VK_F3, "TPPoint", "传送坐标点") {
	SetKeyMode(KeyMode::Trigger);
	point = new vec3_t();
}

TPPoint::~TPPoint() {
	delete[] point;
}


auto TPPoint::onTrigger()->void {
	if (!Game::localplayer) {
		return;
	}
	//TextHolder chat = TextHolder("AAAA");
	//Game::localplayer->displayChatMessage(&chat,&chat);
	//Game::localplayer->jumpFromGround();

	//记录传送点
	if (Game::IsKeyDown(VK_CONTROL)) {
		*point = *Game::localplayer->getPosition();
		
	}

	//读取并传送过去
	if (Game::IsKeyDown(VK_SHIFT) && point && (point->x || point->y || point->z)) {
		//Game::localplayer->setPos(point);
		Game::localplayer->teleportTo(point,true,0,1);
	}
}

auto TPPoint::getBindKeyName()->std::string {
	//ret : CTRL + SHIFT + F
	std::string name = Utils::getKeybindName(VK_SHIFT);
	name += "/";
	name += Utils::getKeybindName(VK_CONTROL);
	name += "+";
	name += Utils::getKeybindName(this->getKeybind());
	return name;
}

auto TPPoint::onloadConfigFile(json& data)->void {
	//setEnabled(config::readDataFromJson<bool>(data, "enable", true));
}
auto TPPoint::onsaveConfigFile(json& data)->void {
	//data["enable"] = isEnabled();
}