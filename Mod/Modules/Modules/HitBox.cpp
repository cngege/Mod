#include "HitBox.h"
#include "Actor.h"
#include "Player.h"
#include "../Mod/Utils/Game.h"
#include "../ModuleManager.h"
#include "NoAttackFriend.h"


HitBox::HitBox() : Module(0, "HitBox", "增大其他玩家的碰撞体积,更容易击中") {

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
				actor->setHitBox(vec2_t(5.0f, 4.0f));
			}
		}
		else {
			actor->resetHitBox();
		}
	}
}