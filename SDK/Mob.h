#pragma once
#include "Actor.h"

class Mob : public Actor {
protected:
	static uintptr_t** vfTables;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int)->auto*;
	static auto GetVFtableFun(int)->uintptr_t*;
	static auto GetVFtables()->uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables)->void;

public:
	static uintptr_t setSprintingFunAddr;

public:
	auto setSprintingEx(bool)->char;

	//虚表函数
public:
	auto setSprinting(bool)->void;						//函数是对的，但是执行没有效果,可能是只对本地玩家没有效果.

};