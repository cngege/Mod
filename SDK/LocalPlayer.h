#pragma once
#include "Player.h"

class LocalPlayer : Player {
protected:
	static uintptr_t** vTables;
public:
	static auto GetVtableFun(int)->uintptr_t*;
	static auto SetVtables(uintptr_t** vTables)->void;

private:
	static LocalPlayer* localPlayer;
public:
	static auto GetLocalPlayer()->LocalPlayer*;
	static auto SetLocalPlayer(LocalPlayer*)->void;

public:
	auto onLocalPlayerTick()->void;
};