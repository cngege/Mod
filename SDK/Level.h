#pragma once
#include <cstdint>
#include <functional>

class Level {
public:
	static uintptr_t* forEachPlayerCall;
	static uintptr_t* tickCall;
public:
	auto getAllPlayer()->std::vector<class Player*>;
public:
	//虚表函数
	auto getTime()->int;															/*111*/
	auto setTime(int)->void;														/*112*/
	
	//虚表函数Hook
	auto Tick()->void;																/*101*/
	auto forEachPlayer(std::function<bool(class Player&)>) -> void;					/*222 / 223 用特征码定位call非虚表 */
};