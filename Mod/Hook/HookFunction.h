#pragma once
#include <functional>

// TODO 设计一个类  暂不可用
// 包含被Hook的函数
// 原函数

template <typename TRet, typename... TArgs>
class HookFunction {
public:
	using FnType = TRet(__fastcall*)(TArgs...);
	using Fn = std::function<TRet(TArgs...)>;
private:
	uintptr_t* Ori_Fun = nullptr;
	Fn Hooked_Fun;

public:


public:
	TRet call(TArgs... args) {
		return reinterpret_cast<FnType>(Ori_Fun)(args);
	}

	void set(Fn lambdafun) {
		Hooked_Fun = lambdafun;
	}

	void* getOriFun() {
		return (void*)&Ori_Fun;
	}

	void* getHookedFun() {
		return &Hooked_Fun;
	}

};




// 定义
// HookFunction<void,Actor*,float> HF_setSpeed;

// 创建被自定义Hook函数;
// 

// 创建Hook
//1 MH_CreatHookEx(xx, &Actor::HF_setSpeed.Hooked_Fun, &Actor::HF_serSpeed.OriFun);
//2 MH_CreatHookEx(xx, &Actor::HF_setSpeed, &Actor::HF_serSpeed.OriFun);