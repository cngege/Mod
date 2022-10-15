#include "HitBox.h"
#include "Actor.h"
#include "Player.h"
#include "../Mod/Utils/Game.h"
#include "../ModuleManager.h"
#include "NoAttackFriend.h"
#pragma execution_character_set("utf-8")

HitBox::HitBox() : Module(0, "HitBox", "增大其他玩家的碰撞体积,更容易击中") {
	AddFloatUIValue("宽度", &width, 0, 5.f, 0.05f);
	AddFloatUIValue("高度", &height, 0, 5.f, 0.05f);
}

auto HitBox::onDisable()->void {
	//获取所有非本地玩家,然后重设HitBox
}

auto HitBox::getBindKeyName()->std::string {
	return Utils::getKeybindName(VK_CAPITAL);
}

auto HitBox::onKeyUpdate(int key, bool isDown)->void {
	setEnabled(GETKEYSTATE(VK_CAPITAL));
}

auto HitBox::onActorTick(Actor* actor)->void {
	if (actor->isLocalPlayer()) {
		return;
	}
	//if (Game::localplayer == nullptr) {
	//	return;
	//}
	//if (actor == (Actor*)Game::localplayer) {
	//	return;
	//}
	//判断是否是玩家 大写锁定
	static NoAttackFriend* noAttackFriend = Game::GetModuleManager()->GetModule<NoAttackFriend*>();
	if (actor->isPlayer()) {
		if (isEnabled()) {
			if (noAttackFriend->isEnabled() && noAttackFriend->IsFriend((Player*)actor)) {
				actor->resetHitBox();
			}
			else {
				actor->setHitBox(vec2_t(width, height));
			}
		}
		else {
			actor->resetHitBox();
		}
	}
}

auto HitBox::onloadConfigFile(json& data)->void {
	//setEnabled(config::readDataFromJson<bool>(data, "enable", true));
	width = config::readDataFromJson<float>(data, "width", 5.f);
	height = config::readDataFromJson<float>(data, "height", 3.f);
}
auto HitBox::onsaveConfigFile(json& data)->void {
	//data["enable"] = isEnabled();
	data["width"] = width;
	data["height"] = height;
}