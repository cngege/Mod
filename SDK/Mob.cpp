#include "Mob.h"

uintptr_t** Mob::vfTables = nullptr;

template <typename TRet, typename... TArgs>
auto Mob::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto Mob::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto Mob::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto Mob::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}
