#pragma once
#include "Player.h"

class ServerPlayer : public Player
{
protected:
	static uintptr_t** vfTables;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int)->auto*;
	static auto GetVFtableFun(int)->uintptr_t*;
	static auto GetVFtables()->uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables)->void;

	//虚函数
public:
	static uintptr_t* tickWorldCall;							/*332*/
	auto tickWorld(void* Tick)->void*;
};
