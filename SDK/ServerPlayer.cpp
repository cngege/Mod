#include "ServerPlayer.h"

uintptr_t** ServerPlayer::vfTables = nullptr;

template <typename TRet, typename... TArgs>
auto ServerPlayer::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto ServerPlayer::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto ServerPlayer::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto ServerPlayer::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}