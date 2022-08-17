#pragma once
#include "../Mod/Utils/Utils.h"


class GameMode {
protected:
	static uintptr_t** vTables;
public:
	static auto GetVtableFun(int)->uintptr_t*;
	static auto SetVtables(uintptr_t** vTables)->void;

// 虚表函数被Hook后 具有原始功能的调用
public:
	static uintptr_t* startDestroyBlockCall;


//调用具有原始功能的被Hook的函数
public:
	auto startDestroyBlock(vec3_ti* Bpos, uint8_t* Face, void* a1, void* a2)->bool;

// 虚表函数
public:

	
	auto destroyBlock(vec3_ti* Bpos, uint8_t* Face)->bool;			/*2*/

};
