#include "ItemInstance.h"

uintptr_t** ItemInstance::vfTables = nullptr;

template <typename TRet, typename... TArgs>
auto ItemInstance::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto ItemInstance::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto ItemInstance::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto ItemInstance::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}
