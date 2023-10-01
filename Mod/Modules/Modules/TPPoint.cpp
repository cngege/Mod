#include "TPPoint.h"
#include "../../Utils/Game.h"
#include "LocalPlayer.h"
#include "ClientInstance.h"

#include "imgui.h"

TPPoint::TPPoint() : Module(VK_F3, "TPPoint", "传送坐标点") {
	SetKeyMode(KeyMode::Trigger);
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
		point = *lp->getPosition();
		return;
	}

	//读取并传送过去
	if (Game::IsKeyDown(VK_SHIFT) && point.has_value()) {
		//Game::localplayer->setPos(point);
		vec3_t tpPos = *point;
		lp->teleportTo(&tpPos,true,0,1);
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
	if (point.has_value()) {
		ImGui::Text("快捷键传送点: x:%.3f, y:%.3f, z:%.3f", point->x, point->y, point->z);
	}
	else {
		ImGui::Text("未记录传送点");
	}

	ImGui::DragFloat3("手动按钮传送点", btnTpPoint, 0.1f);
	if (ImGui::Button("读取")) {
		if (!Game::Cinstance) {
			return;
		}
		LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
		if (!lp) {
			return;
		}
		vec3_t pos = *lp->getPosition();
		btnTpPoint[0] = pos.x;
		btnTpPoint[1] = pos.y;
		btnTpPoint[2] = pos.z;
	}
	ImGui::SameLine();
	if (ImGui::Button("传送")) {
		if (!Game::Cinstance) {
			return;
		}
		LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
		if (!lp) {
			return;
		}
		vec3_t pos(btnTpPoint[0], btnTpPoint[1], btnTpPoint[2]);
		lp->teleportTo(&pos, true, 0, 1);
	}
}

auto TPPoint::onloadConfigFile(json& data)->void {
	//setEnabled(config::readDataFromJson<bool>(data, "enable", true));
}
auto TPPoint::onsaveConfigFile(json& data)->void {
	//data["enable"] = isEnabled();
}