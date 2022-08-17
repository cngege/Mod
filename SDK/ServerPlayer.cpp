#include "ServerPlayer.h"

uintptr_t** ServerPlayer::vTables = nullptr;

auto ServerPlayer::GetVtableFun(int a)->uintptr_t* {
	return vTables[a];
}

auto ServerPlayer::SetVtables(uintptr_t** vTable)->void {
	vTables = vTable;
}