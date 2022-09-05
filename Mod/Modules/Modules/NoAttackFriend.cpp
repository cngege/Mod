#include "NoAttackFriend.h"
#include "Actor.h"
#include "LocalPlayer.h"
#include "../../Utils/Game.h"

NoAttackFriend::NoAttackFriend() : Module(VK_F4, "NoAttackFriend", "从名称颜色识别玩家为队友时拦截攻击") {

}

auto NoAttackFriend::IsFriend(Player* p)->bool {
	if (Game::localplayer == nullptr) {
		return false;
	}
	auto name = p->getNameTag();
	auto myname = Game::localplayer->getNameTag();								//暂时还不能百分百准确获取自己的名字

	if (*(char*)myname == colorbyte[0] && *(char*)name == colorbyte[0]) {		//判断自己和对方名字是否是带有颜色 即§开头
		if (*(char*)(myname + 1) == *(char*)(name + 1)) {						//判断（如果是）颜色（那）是否是一样
			for (int i = 1; i < 16; i++) {										//判断符号后面是否接颜色，长度是定值
				if (colorbyte[i] == *(char*)(myname + 1)) {
					return true;
				}
			}
		}
	}
	return false;
}

#include "../../Utils/Logger.h"
auto NoAttackFriend::onAttack(Actor* actor)->bool {
	logF("myname:%s, actorname:%s", Game::localplayer->getNameTag(), actor->getNameTag());
	if (!isEnabled() || !actor->isPlayerEx()) {
		return true;
	}
	return !IsFriend((Player*)actor);
}