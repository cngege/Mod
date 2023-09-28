#pragma once
#include <cstdint>
#include <functional>

//LevelForILevel 已确定
class Level {
protected:
	static uintptr_t** vfTables;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int) -> auto*;
	static auto GetVFtableFun(int) -> uintptr_t*;
	static auto GetVFtables() -> uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables) -> void;

public:
	static uintptr_t* startLeaveGameCall;
	//static uintptr_t* forEachPlayerCall;
	static uintptr_t* tickCall;
public:
	auto getAllPlayer()->std::vector<class Player*>;
public:
	//虚表函数
	auto getTime()->int;															/*112*/
	auto setTime(int)->void;														/*113*/
	auto getSeed() -> unsigned int;													/*114*/
	
	//虚表函数Hook
	auto startLeaveGame() -> void;													/*2*/
	auto Tick()->void*;																/*101*/
	// 虚表函数
	auto forEachPlayer(std::function<bool(class Player&)>) -> void;					/*207-208 /-562(不对)- 用特征码定位call非虚表 */
	auto isClientSide() -> bool;													/*291*/  // 1.20.15
	auto setSimPaused(bool) -> void;												/*322*/	 // 1.20.15
	auto getSimPaused() -> bool;													/*323*/	 // 1.20.15
};