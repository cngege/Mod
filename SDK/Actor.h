#pragma once
#include <string>
#include "..\Mod\Utils\HMath.h"

#include "AttributeInstance.h"

enum ActorType {
	player = 1,
	iron_golem = 788,
	panda = 9477,
	parrot = 21278,
	villager = 16778099,

	Hive_Treasure = 256
};


class Actor
{
protected:
	static uintptr_t** vfTables;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int)->auto*;
	static auto GetVFtableFun(int)->uintptr_t*;
	static auto GetVFtables()->uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables)->void;

public:
	static int SpeedXOffset;
	static int SpeedYOffset;
	static int SpeedZOffset;

	static int PosXOffset1;
	static int PosYOffset1;
	static int PosZOffset1;
	static int PosXOffset2;
	static int PosYOffset2;
	static int PosZOffset2;

	static int XHitBoxOffset;
	static int YHitBoxOffset;

	static int GetAttributeInstance_HealthFunVT;

public:
	
	auto isPlayerEx() -> bool;

public:
	auto getSpeed()->vec3_t;
	auto setSpeed(vec3_t)->void;

	auto getPosEx()->vec3_t;
	auto getPosEx2()->vec3_t;
	auto setPosEx(vec3_t)->void;

	auto getHitBox()->vec2_t;
	auto setHitBox(vec2_t)->void;
	auto resetHitBox()->void;

public:
	auto onMoveBBs(vec3_t)->void;
	auto isLocalPlayer()->bool;

	auto getHealth()->float;

public:
	//虚表函数

	auto getAttribute(const __int64)->class AttributeInstance*;

public:

	//原生虚表函数
	auto setPos(vec3_t*)->void*;													/*19*/
	auto getPosition()->vec3_t*;													/*22*/
	auto getPosPrev()->vec3_t*;														/*23*/
	auto teleportTo(vec3_t* pos, bool a1, unsigned int a2, unsigned int a3)->void;	/*44*/
	auto getNameTag()->const char*;													/*63*/
	auto getNameTagAsHash()->unsigned __int64;										/*64*/
	auto getFormattedNameTag()->void*;										/*65*/
	//auto isPlayer()->bool;														/*99*/  // 不能用 因为MC中该函数功能的实现方法是 Player类重写
	auto getEntityTypeId()->int;													/*170*/	//可能是 Player::getEntityTypeId()
	auto causeFallDamage()->void*;													/*186*/
};