﻿#include "InstantDestroy.h"
#include "../Mod/Utils/Game.h"
#include "../SDK/GameMode.h"

InstantDestroy::InstantDestroy() : Module(0, "InstantDestroy", "按下 CTRL + SHIFT 后破坏方块时瞬间破坏") {
	//setEnabled(true);
}

InstantDestroy::~InstantDestroy() {
	setEnabled(false);
}

auto InstantDestroy::getBindKeyName()->std::string {
	//ret : CTRL + SHIFT + F
	std::string name = Utils::getKeybindName(VK_SHIFT);
	name += "+";
	name += Utils::getKeybindName(VK_CONTROL);
	return name;
}


auto InstantDestroy::onStartDestroyBlock(GameMode* gm, vec3_ti* Bpos, uint8_t* Face)->void {
	if (isEnabled() && Game::IsKeyDown(VK_SHIFT) && Game::IsKeyDown(VK_CONTROL)) {
		gm->destroyBlock(Bpos, Face);
	}
}

auto InstantDestroy::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", true));
}
auto InstantDestroy::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}