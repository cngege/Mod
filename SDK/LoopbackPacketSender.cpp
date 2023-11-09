#include "LoopbackPacketSender.h"

uintptr_t** LoopbackPacketSender::vfTables = nullptr;


template <typename TRet, typename... TArgs>
auto LoopbackPacketSender::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto LoopbackPacketSender::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto LoopbackPacketSender::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto LoopbackPacketSender::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}

