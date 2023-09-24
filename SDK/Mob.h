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
	//static uintptr_t setSprintingFunAddr;

public:
	//即使放在Tick中执行，也不会被HIVE踢 - 这个应该是 Mob::isSprinting 内部中的某个函数
	//auto setSprintingEx(bool)->char;							
	auto isSprinting()->bool;

	//虚表函数
public:
	auto setSprinting(bool)->void;								/*251*///函数是对的，但在Tick中执行会持续发包（猜测），会被HIVE踢
	auto getSpeed() -> float;									/*254*/
	auto setSpeed(float s) -> void;								/*255*/
	auto lookAt(Actor*, float, float)->void;					/*303*///最后两个参数始终调试不正确
};