#pragma once
#include "..\Mod\Utils\HMath.h"

enum ActorType {
	player = 1,
	iron_golem = 788,
	panda = 9477,
	parrot = 21278,
	villager = 16778099
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

public:
	

	/*template <typename TRet, typename... TArgs>
	inline auto* GetVtablecall(int VtCount) {
		using Fn = TRet(__fastcall*)(TArgs...);
		auto Vtaddr = *reinterpret_cast<uintptr_t**>(this);
		return reinterpret_cast<Fn>(Vtaddr + VtCount);
	};


	auto isPlayer() -> bool {
		if (!VTable) {
			VTable = *reinterpret_cast<uintptr_t***>(this);
		}
		using FN = bool(__fastcall*)();
		auto isPlayerCall = reinterpret_cast<FN>(VTable[20]);			//20 - Player::isPlayer() bool
		return isPlayerCall();
	}*/

	auto isPlayerEx() -> bool;

public:
	auto getSpeed()->vec3_t;
	auto setSpeed(vec3_t)->void;

	auto getPos()->vec3_t;
	auto getPos2()->vec3_t;
	auto setPos(vec3_t)->void;

	auto getHitBox()->vec2_t;
	auto setHitBox(vec2_t)->void;
	auto resetHitBox()->void;

public:
	auto onMoveBBs(vec3_t)->void;

	auto onAllActorTick()->void;

public:
	//虚表函数


	auto getPosition()->vec3_t*;						/*22*/
	auto getPosPrev()->vec3_t*;							/*23*/
	auto isPlayer()->bool;								/*99*/
	auto getEntityTypeId()->int;						/*170*/	//可能是 Player::getEntityTypeId()
};