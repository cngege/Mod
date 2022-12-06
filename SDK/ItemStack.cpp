#include "ItemStack.h"

uintptr_t** ItemStack::vfTables = nullptr;

uintptr_t* ItemStack::useCall = nullptr;

template <typename TRet, typename... TArgs>
auto ItemStack::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto ItemStack::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto ItemStack::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto ItemStack::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}

auto ItemStack::use(Player* player) -> ItemStack*
{
	using Fn = ItemStack * (__fastcall*)(ItemStack*, Player*);
	return reinterpret_cast<Fn>(useCall)(this, player);
}
