#include "TPPoint.h"
#include "../../Utils/Game.h"
#include "LocalPlayer.h"
#include "ClientInstance.h"
#include "TextHolder.h"

#include "imgui.h"

TPPoint::TPPoint() : Module(VK_F3, "TPPoint", "传送坐标点") {
	SetKeyMode(KeyMode::Trigger);
	point = new vec3_t();
}

TPPoint::~TPPoint() {
	delete point;
}


auto TPPoint::onTrigger()->void {
	if (!Game::Cinstance) {
		return;
	}
	LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
	if (!lp) {
		return;
	}
	//TextHolder chat = TextHolder("AAAA");
	//Game::localplayer->displayChatMessage(&chat,&chat);
	//Game::localplayer->jumpFromGround();

	//记录传送点
	if (Game::IsKeyDown(VK_CONTROL)) {
		*point = *lp->getPosition();
		return;
	}

	//读取并传送过去
	if (Game::IsKeyDown(VK_SHIFT) && point && (point->x || point->y || point->z)) {
		//Game::localplayer->setPos(point);
		lp->teleportTo(point,true,0,1);
	}
}

auto TPPoint::getBindKeyName()->std::string {
	//ret : CTRL + SHIFT + F
	std::string name = Utils::getKeybindName(VK_SHIFT);
	name += "/";
	name += Utils::getKeybindName(VK_CONTROL);
	name += "+";
	name += Module::getBindKeyName();
	return name;
}

auto TPPoint::onInternalImGUIRender() -> void
{
	ImGui::Text("传送点: x:%.3f, y:%.3f, z:%.3f", point->x, point->y, point->z);
}

auto TPPoint::onloadConfigFile(json& data)->void {
	//setEnabled(config::readDataFromJson<bool>(data, "enable", true));
}
auto TPPoint::onsaveConfigFile(json& data)->void {
	//data["enable"] = isEnabled();
}