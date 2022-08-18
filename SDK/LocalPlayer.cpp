#include "LocalPlayer.h"

LocalPlayer* LocalPlayer::localPlayer = nullptr;

//uintptr_t** LocalPlayer::vfTables = nullptr;
//
//template <typename TRet, typename... TArgs>
//auto LocalPlayer::GetVFtableFun(int a)->auto* {
//	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
//}
//
//auto LocalPlayer::GetVFtableFun(int a)->uintptr_t* {
//	return vfTables[a];
//}
//
//auto LocalPlayer::GetVFtables()->uintptr_t** {
//	return vfTables;
//}
//
//auto LocalPlayer::SetVFtables(uintptr_t** vfTable)->void {
//	vfTables = vfTable;
//}



auto LocalPlayer::SetLocalPlayer(LocalPlayer* lp) -> void {
	localPlayer = lp;
}

auto LocalPlayer::GetLocalPlayer()->LocalPlayer* {
	return localPlayer;
}

auto LocalPlayer::onLocalPlayerTick()->void {
	localPlayer = this;
}