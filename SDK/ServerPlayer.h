#pragma once
#include "Player.h"

class ServerPlayer : Player
{
protected:
	static uintptr_t** vTables;
public:
	static auto GetVtableFun(int)->uintptr_t*;
	static auto GetVtables()->uintptr_t**;
	static auto SetVtables(uintptr_t** vTables)->void;

};