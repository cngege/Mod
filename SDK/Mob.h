#pragma once
#include "Actor.h"

class Mob : Actor {
protected:
	static uintptr_t** vfTables;
public:
	static auto GetVFtableFun(int)->uintptr_t*;
	static auto GetVFtables()->uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables)->void;

};