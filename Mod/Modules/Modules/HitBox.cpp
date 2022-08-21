#include "HitBox.h"
#include "Actor.h"
#include "../Mod/Utils/Game.h"

HitBox::HitBox() : Module(0, "HitBox", "增大其他玩家的碰撞体积,更容易击中") {

}

auto HitBox::onDisable()->void {
	//获取所有非本地玩家,然后重设HitBox
}

auto HitBox::onActorTick(Actor* actor)->void {
	if (Game::localplayer == nullptr) {
		return;
	}
	if (actor == (Actor*)Game::localplayer) {
		return;
	}
	//判断是否是玩家 大写锁定
	if (actor->isPlayerEx()) {
		if (GETKEYSTATE(VK_CAPITAL)) {
			actor->setHitBox(vec2_t(6.0f, 6.0f));
		}
		else {
			actor->resetHitBox();
		}
	}
}