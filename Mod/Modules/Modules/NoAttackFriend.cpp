#include "NoAttackFriend.h"
#include "Actor.h"
#include "LocalPlayer.h"
#include "TextHolder.h"
#include "../../Utils/Game.h"
#include "../../Utils/Utils.h"

NoAttackFriend::NoAttackFriend() : Module(VK_F4, "NoAttackFriend", "从名称颜色识别玩家为队友时拦截攻击") {

}

auto NoAttackFriend::IsFriend(Player* p)->bool {
	if (Game::localplayer == nullptr) {
		return false;
	}
	auto name = p->getNameTag()->getText();
	auto myname = Game::localplayer->getNameTag()->getText();								//暂时还不能百分百准确获取自己的名字

	if (*myname == colorbyte[0] && *name == colorbyte[0]) {		//判断自己和对方名字是否是带有颜色 即§开头
		if (*(myname + 1) == *(name + 1)) {						//判断（如果是）颜色（那）是否是一样
			for (int i = 1; i < 17; i++) {										//判断符号后面是否接颜色，长度是定值
				if (colorbyte[i] == *(myname + 1)) {
					return true;
				}
			}
		}
	}
	return false;
}

#include "../../Utils/Logger.h"
auto NoAttackFriend::onAttack(Actor* actor)->bool {
	if (!isEnabled()) {
		return true;
	}
	{
		//暂时搁置
		logF("myptr:%llX,myname:%s", Game::localplayer, Game::localplayer->getNameTag()->getText());
		logF("actorname:%s,actorptr:%llX,isPlayer:%i", actor->getNameTag()->getText(), actor, IsFriend((Player*)actor));
	}
	if (!actor->isPlayerEx()) {
		return true;
	}
	
	return !IsFriend((Player*)actor);
}