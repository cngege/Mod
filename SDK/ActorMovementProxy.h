#pragma once
#include <cstdint>

//const DirectActorMovementProxy::`vftable'{for `IActorMovementProxy'}
class ActorMovementProxy {
protected:
	static uintptr_t** vfTables;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int) -> auto*;
	static auto GetVFtableFun(int) -> uintptr_t*;
	static auto GetVFtables() -> uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables) -> void;

public:
	/**/
	auto getActor() -> class Actor*;

	/*虚表函数*/
	auto isOnGround() -> bool;								/*39*/	//1.20.15
	auto setOnGround(bool) -> void;							/*40*/	//1.20.15

	auto getHealth() -> int;								/*43*/	//1.20.15
	auto getRotation() -> struct vec2_t*;					/*73*/	//1.20.15
	auto setRotation(struct vec2_t*) ->void;				/*74*/	//1.20.15
	auto getDimensionBlockSource() -> class BlockSource*;	/*81*/	//1.20.15
};

//DirectPlayerMovementProxy::`vftable'{for `IBoatMovementProxy'}
class PlayerMovementProxy {
protected:
	static uintptr_t** vfTables;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int) -> auto*;
	static auto GetVFtableFun(int) -> uintptr_t*;
	static auto GetVFtables() -> uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables) -> void;
};

