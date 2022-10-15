#include "FastViewPerspective.h"
#include "../../Utils/Game.h"
#include "../../Utils/Utils.h"

#include "GameMode.h"
#include "LocalPlayer.h"
#pragma execution_character_set("utf-8")

FastViewPerspective::FastViewPerspective() : Module(VK_F9, "FastViewPerspective", "快速预览第二人称视角") {
	//setEnabled(true);
}

auto FastViewPerspective::getBindKeyName()->std::string {
	//ret : CTRL + SHIFT + F
	std::string name = "(R) ";
	name += Utils::getKeybindName(this->getKeybind());
	return name;
}


auto FastViewPerspective::isToggle()->bool {
	return Game::IsKeyDown('R');
}

auto FastViewPerspective::onTick(GameMode* gm)->void {
	if (!isEnabled()) return;
	if (gm->GetLocalPlayer() == nullptr) return;
	return;
	//(遗留故障： 按下R时可移动视角不移动方向，松开R时，会改变方向)
	static vec2_t srot(0,0);
	if (!isToggle()) {
		srot = *gm->GetLocalPlayer()->getRotEx2();
	}
	else {
		//这里表示我按下了R
		//可以尝试做自瞄 和自由视角
		vec2_t nrot(srot.x,srot.y);
		gm->GetLocalPlayer()->setRot(&nrot);
	}
}

auto FastViewPerspective::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", true));
}
auto FastViewPerspective::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}