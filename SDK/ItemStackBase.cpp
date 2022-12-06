#include "ItemStackBase.h"


uintptr_t** ItemStackBase::vfTables = nullptr;

template <typename TRet, typename... TArgs>
auto ItemStackBase::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto ItemStackBase::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto ItemStackBase::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto ItemStackBase::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}

Item* ItemStackBase::getItemEx()
{
	return *reinterpret_cast<Item**>((uintptr_t)this + 8);
}
