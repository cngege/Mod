#include "NoAttackFriend.h"
#include "Actor.h"
#include "LocalPlayer.h"
#include "TextHolder.h"
#include "../../Utils/Game.h"
#include "../../Utils/Utils.h"

NoAttackFriend::NoAttackFriend() : Module(VK_F4, "NoAttackFriend", "根据玩家名称的颜色识别玩家为队友时拦截攻击") {
	setcontrolkeysbind({ VK_SHIFT });
	//colorbyte[0] = Utils::ANSItoUTF8(colorbyte[0].c_str());
}

auto NoAttackFriend::IsFriend(Player* p)->bool {
	if (Game::localplayer == nullptr) {
		return false;
	}
	
	auto name = std::string(p->getNameTag()->getText());
	auto myname = std::string(Game::localplayer->getNameTag()->getText());	//暂时还不能百分百准确获取自己的名字
	
	auto name_first = name.substr(0, 2);
	auto myname_first = myname.substr(0, 2);
	if (name_first == colorbyte[0] && myname_first == colorbyte[0]) {		//判断自己和对方名字是否是带有颜色 即§开头
		auto name_next = name.substr(2, 1);
		auto myname_next = myname.substr(2, 1);
		if (name_next == myname_next) {										//判断（如果是）颜色（那）是否是一样
			for (int i = 1; i < 17; i++) {									//判断符号后面是否接颜色，长度是定值
				if (colorbyte[i] == myname_next) {
					return true;
				}
			}
		}
	}
	return false;
}

//#include "../../Utils/Logger.h"
auto NoAttackFriend::onAttack(Actor* actor)->bool {
	if (!isEnabled()) {
		return true;
	}
	{
		//暂时搁置
		//logF("myptr:%llX,myname:%s", Game::localplayer, Game::localplayer->getNameTag()->getText());
		//logF("actorname:%s,actorptr:%llX,IsFriend:%d", actor->getNameTag()->getText(), actor, IsFriend((Player*)actor));
	}
	if (!actor->isPlayerEx()) {
		return true;
	}
	
	return !IsFriend((Player*)actor);
}

auto NoAttackFriend::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", false));
}
auto NoAttackFriend::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}