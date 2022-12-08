#pragma once
#include "Player.h"

class RemotePlayer : public Player {
protected:
	static uintptr_t** vfTables;

public:
	static uintptr_t* tickWorldCallptr;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int) -> auto*;
	static auto GetVFtableFun(int) -> uintptr_t*;
	static auto GetVFtables() -> uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables) -> void;

public:
	//Hook 虚表函数
	auto tickWorld()->void*;
};