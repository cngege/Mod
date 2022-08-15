#pragma once
#include "Player.h"

class LocalPlayer : Player {

private:
	static LocalPlayer* localPlayer;
public:
	static auto GetLocalPlayer()->LocalPlayer*;
	static auto SetLocalPlayer(LocalPlayer*)->void;

public:
	auto onLocalPlayerTick()->void;
};