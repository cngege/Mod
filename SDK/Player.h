#pragma once
#include "..\Mod\Utils\HMath.h"
#include "Actor.h"
#include "Mob.h"


class Player : public Mob
{
protected:
	static uintptr_t** vfTables;

public:
	static uintptr_t* tickWorldCallptr;
	static uintptr_t* getShadowRadiusCallptr;
	//static uintptr_t* startSwimmingCallptr;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int)->auto*;
	static auto GetVFtableFun(int)->uintptr_t*;
	static auto GetVFtables()->uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables)->void;

public:
	
	static int RotPtrOffset;	//偏移指向 vec2_t 指针
	//static int Rot2;			//偏移指向 vec2_t 指针

public:
	//ret->x 表示竖直方向的值
	auto getRotEx1()->vec2_t*;
	//ret->x 表示竖直方向的值
	auto getRotEx2()->vec2_t*;

	auto getFootPos() -> vec3_ti;
	auto getFootBlockPos() -> vec3_ti;

	auto getAbilitiesComponent() -> void*;
	auto isFlying() -> bool;
	auto setFlying(bool) -> void;
	auto setCanFlyEx(bool) -> void;

public:
	//虚表函数
	//44
	auto teleportTo(vec3_t*, bool, unsigned int, unsigned int)->void;
	//auto displayChatMessage(std::mcstring*)->__int64;
	auto getSelectedItem() -> class ItemStack*;												/*164*/

	auto getSpeed() -> float;																/*254*/
	auto setSpeed(float s) -> void;															/*255*/
	auto setPlayerGameType(int GameType) -> void;											/*368*/
	auto getXuid() -> std::mcstring;														/*396*/

	//Hook虚表函数
	auto getShadowRadius()->float;															/*79*/
	//auto startSwimming()->void;																/*202*/
	
	auto tickWorld(class Tick*) -> void;													/*371*/
};