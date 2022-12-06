#include "FishingHook.h"

uintptr_t** FishingHook::vfTables = nullptr;


template <typename TRet, typename... TArgs>
auto FishingHook::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto FishingHook::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto FishingHook::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto FishingHook::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}
