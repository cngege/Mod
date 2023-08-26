#pragma once
#include "..\Mod\Utils\HMath.h"

// 不是继承而来
class Item {
protected:
	static uintptr_t** vfTables;

public:
	static int getIdOffset;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int) -> auto*;
	static auto GetVFtableFun(int) -> uintptr_t*;
	static auto GetVFtables() -> uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables) -> void;
public:
	auto isValid() -> bool;
	auto getIdEx() -> short; //5498 钓鱼竿id

public:
	// 这个函数的下面就是 ItemStack::use
	class ItemStack* use(class ItemStack*, class Player*);								/*84*/
	class TextHolder& buildDescriptionName(class TextHolder&, class ItemStackBase);		/*94*/
};