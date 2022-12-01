#include "Player.h"
#include "../Mod/Utils/Utils.h"

int Player::RotPtrOffset = 0;
//int Player::Rot2 = 0;


uintptr_t** Player::vfTables = nullptr;

uintptr_t* Player::tickWorldCallptr = nullptr;
uintptr_t* Player::getShadowRadiusCallptr = nullptr;

template <typename TRet, typename... TArgs>
auto Player::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto Player::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto Player::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto Player::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}

//vec.x表示竖直方向的值且竖直方向的值在前
auto Player::getRotEx1()->vec2_t* {
	return *(vec2_t**)(this + RotPtrOffset);
}

auto Player::getRotEx2()->vec2_t* {
	return *(vec2_t**)(this + RotPtrOffset) + 1;
}


//虚表函数

auto Player::teleportTo(vec3_t* pos, bool a1, unsigned int a2, unsigned int a3)->void {
	GetVFtableFun<void,Player*, vec3_t*,bool, unsigned int, unsigned int>(45)(this,pos,a1,a2,a3);
}

//91
auto Player::displayChatMessage(TextHolder* a1, TextHolder* a2)->__int64{
	return GetVFtableFun<__int64, Player*, TextHolder*, TextHolder*>(91)(this, a1, a2);
}

auto Player::getShadowRadius() -> float
{
	using Fn = float(__fastcall*)(Player*);
	return reinterpret_cast<Fn>(getShadowRadiusCallptr)(this);
}

auto Player::tickWorld(Tick* tick) -> void
{
	using Fn = void(__fastcall*)(Player*, class Tick*);
	reinterpret_cast<Fn>(tickWorldCallptr)(this, tick);
}
