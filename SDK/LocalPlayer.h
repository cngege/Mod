#pragma once
#include "Player.h"

class LocalPlayer : Player {
protected:
	static uintptr_t** vfTables;
public:
	static auto GetVFtableFun(int)->uintptr_t*;
	static auto GetVFtables()->uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables)->void;

private:
	static LocalPlayer* localPlayer;
public:
	static auto GetLocalPlayer()->LocalPlayer*;
	static auto SetLocalPlayer(LocalPlayer*)->void;

public:
	auto onLocalPlayerTick()->void;
};