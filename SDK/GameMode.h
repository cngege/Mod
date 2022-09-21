#pragma once
#include "../Mod/Utils/Utils.h"
#include "../Mod/Utils/HMath.h"

class GameMode {

protected:
	static uintptr_t** vfTables;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int)->auto*;
	static auto GetVFtableFun(int)->uintptr_t*;
	static auto GetVFtables()->uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables)->void;

// 虚表函数被Hook后 具有原始功能的调用
public:
	static uintptr_t* startDestroyBlockCall;
	static uintptr_t* attackCall;
	static uintptr_t* tickCall;
	static uintptr_t* useItemCall;
	static uintptr_t* useItemOnCall;

public:
	auto GetLocalPlayer()->class LocalPlayer*;

//调用具有原始功能的被Hook的函数
public:
	
	auto startDestroyBlock(vec3_ti* Bpos, uint8_t* Face, void* a1, void* a2)->bool;					/*1*/
	auto tick()->void*;																				/*8*/
	auto useItem(class ItemStack*)->bool;															/*11*/  //对着空气右键
	auto useItemOn(class ItemStack*, vec3_ti*, uint8_t*,vec3_t*, class Block*)->bool;				/*12*/	//对着方块右键
	auto attack(class Actor*) ->bool;																/*14*/

// 虚表函数
public:

	auto destroyBlock(vec3_ti* Bpos, uint8_t* Face)->bool;											/*2*/
	

};
