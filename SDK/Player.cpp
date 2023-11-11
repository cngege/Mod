#include "Player.h"

#include "../Mod/Utils/Utils.h"


int Player::RotPtrOffset = 0;
//int Player::Rot2 = 0;


uintptr_t** Player::vfTables = nullptr;

uintptr_t* Player::tickWorldCallptr = nullptr;
uintptr_t* Player::getShadowRadiusCallptr = nullptr;
//uintptr_t* Player::startSwimmingCallptr = nullptr;

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

// 函数的定位可以通过搜索关键字(a1, 218128893);找到
auto Player::getAbilitiesComponent() -> void*
{
	// 首先获取：entt::basic_registry<EntityId,std::allocator<EntityId>>::try_get<AbilitiesComponent> 函数
	const char* memcode_fn = "40 53 48 83 EC ? 48 8B DA BA FD 61";
	const char* memcode_call = "E8 ? ? ? ? 48 85 ? 74 ? 48 8B 5C 24 ? 48 83 C4 ? 5F C3";
	static uintptr_t call = Utils::getFunFromSigAndCall(memcode_fn, memcode_call, 1);
	_ASSERT(call);
	return reinterpret_cast<void*(__fastcall*)(uintptr_t, uintptr_t)>(call)(**(uintptr_t**)((uintptr_t)this + 8), (uintptr_t)this + 16);
}

auto Player::isFlying() -> bool
{
	const char* memcode_call = "E8 ? ? ? ? 84 C0 75 ? F3 0F 59 F7";
	const char* memcode_fn = "48 83 EC ? 48 8B 41 ? 48 8B D1 48 8B 08 8B 42 ? 48 8D 54 24 ? 89 44 24 ? E8 ? ? ? ? 48 85 C0 74 ? 48 8D 88 ? ? ? ? 48 83 C0 ? 48 3B C8 74 ? 48 83";
	static uintptr_t call = Utils::getFunFromSigAndCall(memcode_fn, memcode_call, 1);
	_ASSERT(call);
	return reinterpret_cast<bool(__fastcall*)(Player*)>(call)(this);
}

auto Player::setFlying(bool isfly) -> void
{
	uintptr_t comp = (uintptr_t)getAbilitiesComponent();
	// 0xA canFly  9 flying
	char* result = (char*)(comp + 4 * (3i64 * (char)9u + 58));
	if (*result == 1)
	{
		*result = 2;
		result[4] = 0;
	}
	result[4] = isfly;
}

auto Player::setCanFlyEx(bool canfly) -> void
{
	uintptr_t comp = (uintptr_t)getAbilitiesComponent();
	// 0xA canFly  9 flying
	char* result = (char*)(comp + 4 * (3i64 * (char)0xAu + 58));
	if (*result == 1)
	{
		*result = 2;
		result[4] = 0;
	}
	result[4] = canfly;
}


//虚表函数
// 检查版本 1.20.41
auto Player::teleportTo(vec3_t* pos, bool a1, unsigned int a2, unsigned int a3)->void {
	GetVFtableFun<void,Player*, vec3_t*,bool, unsigned int, unsigned int>(26)(this,pos,a1,a2,a3);
}

/*
// 检查版本 1.20 找不到
auto Player::displayChatMessage(TextHolder* a1)->__int64{
	return GetVFtableFun<__int64, Player*, TextHolder*, TextHolder*>(77)(this, a1, a2);
}
*/

// 检查版本 1.20.30
auto Player::getSelectedItem()->ItemStack*
{
	return GetVFtableFun<ItemStack*, Player*>(94)(this);
}

auto Player::getSpeed() -> float
{
	return GetVFtableFun<float, Player*>(178)(this);
}

auto Player::setSpeed(float s) -> void
{
	return GetVFtableFun<void, Player*, float>(179)(this, s);
}

auto Player::setPlayerGameType(int GameType) -> void
{
	GetVFtableFun<void, Player*, int>(253)(this, GameType);
}

auto Player::getXuid() ->std::mcstring
{
	std::mcstring* ret = GetVFtableFun<std::mcstring*, Player*>(274)(this);
	return *ret;
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
