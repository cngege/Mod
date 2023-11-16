#include "ClientInstance.h"

uintptr_t** ClientInstance::vfTables = nullptr;

int ClientInstance::getMinecraftGameOffset = 0;


template <typename TRet, typename... TArgs>
auto ClientInstance::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto ClientInstance::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto ClientInstance::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto ClientInstance::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}
