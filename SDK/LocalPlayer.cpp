#include "LocalPlayer.h"
#include "TextHolder.h"

uintptr_t** LocalPlayer::vfTables = nullptr;

template <typename TRet, typename... TArgs>
auto LocalPlayer::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto LocalPlayer::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto LocalPlayer::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto LocalPlayer::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}

//Ðé±íº¯Êý

//auto LocalPlayer::setPos(vec3_t* pos)->__int64 {
//	return GetVFtableFun<__int64, LocalPlayer*, vec3_t*>(13)(this,pos);	//19?
//}

auto LocalPlayer::jumpFromGround()->UINT64 {
	return GetVFtableFun<UINT64, LocalPlayer*>(346)(this);
}

auto LocalPlayer::displayClientMessage(TextHolder* text)->UINT64 {
	return GetVFtableFun<UINT64, LocalPlayer*, TextHolder*>(389)(this, text);
}