﻿#include "ActorMovementProxy.h"

#include "Utils.h"
#include "HMath.h"


uintptr_t** ActorMovementProxy::vfTables = nullptr;

template <typename TRet, typename... TArgs>
auto ActorMovementProxy::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto ActorMovementProxy::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto ActorMovementProxy::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto ActorMovementProxy::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}


uintptr_t** PlayerMovementProxy::vfTables = nullptr;

template <typename TRet, typename... TArgs>
auto PlayerMovementProxy::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto PlayerMovementProxy::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto PlayerMovementProxy::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto PlayerMovementProxy::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}


auto ActorMovementProxy::getActor() -> class Actor*
{
	return *reinterpret_cast<class Actor**>((uintptr_t)this + 16);	//offset 16byte
}

auto ActorMovementProxy::isOnGround() -> bool
{
	return Utils::CallVFunc<39, bool>(this);
}

auto ActorMovementProxy::setOnGround(bool v) -> void
{
	Utils::CallVFunc<40, void, bool>(this,v);
}

auto ActorMovementProxy::getHealth() -> int
{
	return Utils::CallVFunc<43, int>(this);
}

auto ActorMovementProxy::getRotation() -> vec2_t*
{
	return Utils::CallVFunc<51, vec2_t*>(this);				//更新自 1.20.30
}

auto ActorMovementProxy::setRotation(vec2_t* rot) -> void
{
	Utils::CallVFunc<52, void, vec2_t*>(this, rot);	//更新自 1.20.30
}

auto ActorMovementProxy::getDimensionBlockSource() -> class BlockSource*
{
	return Utils::CallVFunc<58, BlockSource*>(this);			//更新自 1.20.30
}
