#pragma execution_character_set("utf-8")
#include "GameTimeLock.h"
#include "GameMode.h"
#include "LocalPlayer.h"
#include "Level.h"

GameTimeLock::GameTimeLock() : Module(0, "GameTimeLock", "将游戏时间固定到一个设置的时间") {
	AddIntUIValue("时间", &time, 0, 1000000);
}

auto GameTimeLock::onTick(GameMode* gm)->void {
	auto lp = gm->GetLocalPlayer();
	if (lp && lp->getLevel()) {
		if (isEnabled()) {
			lp->getLevel()->setTime(time);
		}
		else {
			time = lp->getLevel()->getTime();
		}
	}
}

auto GameTimeLock::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", false));
	time = config::readDataFromJson<int>(data, "time", 0);
}

auto GameTimeLock::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
	data["time"] = time;
}