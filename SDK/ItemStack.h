#pragma once
#include "ItemStackBase.h"

class ItemStack : public ItemStackBase {
protected:
	static uintptr_t** vfTables;

public:
	static uintptr_t* useCall;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int) -> auto*;
	static auto GetVFtableFun(int) -> uintptr_t*;
	static auto GetVFtables() -> uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables) -> void;

public:
	auto use(class Player*) -> ItemStack*;							//特征码定位Call

};