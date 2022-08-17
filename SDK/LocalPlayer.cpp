#include "LocalPlayer.h"

LocalPlayer* LocalPlayer::localPlayer = nullptr;

uintptr_t** LocalPlayer::vTables = nullptr;

auto LocalPlayer::GetVtableFun(int a)->uintptr_t* {
	return vTables[a];
}

auto LocalPlayer::GetVtables()->uintptr_t** {
	return vTables;
}

auto LocalPlayer::SetVtables(uintptr_t** vTable)->void {
	vTables = vTable;
}



auto LocalPlayer::SetLocalPlayer(LocalPlayer* lp) -> void {
	localPlayer = lp;
}

auto LocalPlayer::GetLocalPlayer()->LocalPlayer* {
	return localPlayer;
}

auto LocalPlayer::onLocalPlayerTick()->void {
	localPlayer = this;
}