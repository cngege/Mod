#include "ServerPlayer.h"

uintptr_t** ServerPlayer::vfTables = nullptr;

auto ServerPlayer::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto ServerPlayer::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto ServerPlayer::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}