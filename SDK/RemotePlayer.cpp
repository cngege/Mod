#include "RemotePlayer.h"

uintptr_t** RemotePlayer::vfTables = nullptr;

uintptr_t* RemotePlayer::tickWorldCallptr = nullptr;

template <typename TRet, typename... TArgs>
auto RemotePlayer::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto RemotePlayer::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto RemotePlayer::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto RemotePlayer::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}

auto RemotePlayer::tickWorld()->void*
{
	using Fn = void*(__fastcall*)(RemotePlayer*);
	return reinterpret_cast<Fn>(tickWorldCallptr)(this);
}
