#include "Mob.h"

uintptr_t** Mob::vfTables = nullptr;

uintptr_t Mob::setSprintingFunAddr = 0x00;

template <typename TRet, typename... TArgs>
auto Mob::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto Mob::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto Mob::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto Mob::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}

//
auto Mob::setSprintingEx(bool v)->char {
	using FunsetSprinting = char(__fastcall*)(Mob*, bool);
	return reinterpret_cast<FunsetSprinting>(setSprintingFunAddr)(this,v);
}

/*
__int64 __fastcall Mob::isSprinting(Mob* this)
{
	return (**(__int64(__fastcall***)(Mob*, __int64))this)(this, 3i64);
}*/
auto Mob::isSprinting()-> bool{
	return GetVFtableFun<bool, Mob*, int>(0)(this, 3);
}


//虚表函数
auto Mob::setSprinting(bool v)->void {
	GetVFtableFun<void, Mob*,bool>(284)(this,v);
}


auto Mob::lookAt(Actor* actor,float f1, float f2)->void {
	GetVFtableFun<void, Mob*, Actor*,float,float>(298)(this, actor,f1,f2);
}