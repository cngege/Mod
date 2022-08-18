#pragma once
#include "..\Mod\Utils\HMath.h"
#include "Actor.h"
#include "Mob.h"

class Player : public Mob
{
//protected:
//	static uintptr_t** vfTables;
//public:
//	template <typename TRet, typename... TArgs>
//	static auto GetVFtableFun(int)->auto*;
//	static auto GetVFtableFun(int)->uintptr_t*;
//	static auto GetVFtables()->uintptr_t**;
//	static auto SetVFtables(uintptr_t** vTables)->void;

public:
	
	static int YView1;
	static int XView1;
	static int YView2;
	static int XView2;

public:

	auto getViewYX()->const vec2_t;
	auto getViewYX2()->const vec2_t;

};