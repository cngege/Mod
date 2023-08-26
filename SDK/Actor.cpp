#include "Actor.h"
#include "LocalPlayer.h"
#include "ServerPlayer.h"
#include "TextHolder.h"
#include "AttributeInstance.h"
#include "AABB.h"
#include "../Mod/Utils/Utils.h"
#include "../Mod/Utils/Game.h"
#include <math.h>


int Actor::SpeedOffset = 0;		//玩家指针到玩家速度相关信息指针的偏移

int Actor::AABBOffset = 0;
int Actor::LevelOffset = 0;
int Actor::IsRemovedOffset = 0;
int Actor::GetRotationOffset = 0;

int Actor::GetAttributeInstance_HealthFunVT = 0;
//uintptr_t Actor::isSneakingCallptr = 0;
uintptr_t* Actor::setVelocityCallptr = nullptr;
uintptr_t* Actor::getShadowRadiusCallptr = nullptr;
uintptr_t* Actor::isInWaterCallptr = nullptr;

uintptr_t* Actor::getDimensionConstCallptr = 0;

uintptr_t** Actor::vfTables = nullptr;

template <typename TRet, typename... TArgs>
auto Actor::GetVFtableFun(int a)->auto*{
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto Actor::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto Actor::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto Actor::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}

auto Actor::getSpeed()->vec3_t {
	return *(vec3_t*)(*(uintptr_t*)(this + SpeedOffset) + 24);			//+24 是因为前面有两组相同的坐标数据 两个vec3_t
}

auto Actor::setSpeed(vec3_t v) ->void {
	*(vec3_t*)(*(uintptr_t*)(this + SpeedOffset) + 24) = v;
}

auto Actor::getAABB()->AABB* {
	if (AABBOffset == 0) {
		return nullptr;
	}
	return *reinterpret_cast<AABB**>(this + AABBOffset);
}

//获取玩家下边框对角点的位置
auto Actor::getPosEx()->vec3_t {
	auto aabb = this->getAABB();
	if (!aabb) {
		return vec3_t();
	}
	return aabb->min;
}

//获取玩家上边框对角点的位置
auto Actor::getPosEx2()->vec3_t {
	auto aabb = this->getAABB();
	if (!aabb) {
		return vec3_t();
	}
	return aabb->max;
}

//以玩家下对焦点位置为基准设置玩家位置 不建议
auto Actor::setPosEx(vec3_t p)->void {
	auto aabb = this->getAABB();
	if (!aabb) {				//后面的偏移都是根据这个来的，所以只需要判断这一个就可以
		return;
	}
	aabb->min = p;
	aabb->max = vec3_t(p.x + 0.5999756f, p.y + 1.80000305f, p.z + 0.60002518f);
}

auto Actor::getHitBox()->vec2_t {
	auto aabb = this->getAABB();
	if (!aabb) {				//后面的偏移都是根据这个来的，所以只需要判断这一个就可以
		return vec2_t();
	}
	
	return *(vec2_t*)((uintptr_t)aabb + 24);
}

auto Actor::setHitBox(vec2_t hb)->void {
	auto aabb = this->getAABB();
	if (!aabb) {				//后面的偏移都是根据这个来的，所以只需要判断这一个就可以
		return;
	}
	*(float*)((uintptr_t)aabb + 24) = hb.x;
	*(float*)((uintptr_t)aabb + 28) = hb.y;
}

auto Actor::isPlayerEx()->bool {
	if (!this) {
		return false;
	}
	return this->getEntityTypeId() == ActorType::player;
}

//仅玩家
auto Actor::resetHitBox()->void {
	this->setHitBox(vec2_t(0.6000000238f, 1.799999952f));
}


auto Actor::getLevel()->class Level* {
	return *reinterpret_cast<class Level**>((uintptr_t)this + LevelOffset);
}

// ActorCollision没有虚表
auto Actor::getActorCollision() -> class ActorCollision*
{
	return (ActorCollision*)((uintptr_t)this + 8);
}

//auto Actor::onMoveBBs(vec3_t p)->void {
//
//}

auto Actor::isLocalPlayer()->bool {
	if (*(__int64*)this == (__int64)LocalPlayer::GetVFtables())
		return true;
	else
		return false;
}

auto Actor::getHealth()->float {
	AttributeInstance* AI = this->getAttribute(Attribute::HEALTH);
	float v = AI->getCurrentValue();
	return ceil(v);
}


auto Actor::isSneaking()->bool {
	//using Fn = bool(__fastcall*)(Actor*);
	//return reinterpret_cast<Fn>(isSneakingCallptr)(this);
	return getStatusFlag(ActorFlags::isSneaking);
}

auto Actor::isRemovedEx()->bool
{
	return *(BYTE*)((uintptr_t)this + IsRemovedOffset);
}

auto Actor::getRotationEx()->vec2_t*
{
	return *reinterpret_cast<vec2_t**>(this + GetRotationOffset);
}

auto Actor::isValid() -> bool
{
	return (uintptr_t)this && *(uintptr_t*)this;
}

/* 原版函数 通过特征码获取 */
auto Actor::getDimensionConst() -> class Dimension*{
	using Fn = Dimension*(__fastcall*)(Actor*);
	return reinterpret_cast<Fn>(getDimensionConstCallptr)(this);
}

auto Actor::setPos(vec3_t* pos)->void* {
	static uintptr_t setPosPtr = FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B DA 48 8B F9 48 8B CB");
	using Fn = void* (__fastcall*)(Actor*, vec3_t*);
	return reinterpret_cast<Fn>(setPosPtr)(this, pos);
	//return GetVFtableFun<void*, Actor*, vec3_t*>(21)(this,pos);
}

auto Actor::setPosPrev(vec3_t* pos)->void* {
	static uintptr_t setPosPtr = FindSignature("48 83 EC ? 4C 8B 81 ? ? ? ? 4D 85 C0 74 ? 8B 02");
	using Fn = void* (__fastcall*)(Actor*, vec3_t*);
	return reinterpret_cast<Fn>(setPosPtr)(this, pos);
	//return GetVFtableFun<void*, Actor*, vec3_t*>(21)(this,pos);
}

auto Actor::getMovementProxy() -> class ActorMovementProxy*
{
	static uintptr_t sig = 0;
	if (!sig) {
		uintptr_t sigcall = FindSignature("E8 ? ? ? ? 4C 8B C0 33 FF 8B DF 48 8B 50 ? 48 85 D2");	// 来自 Actor::getHeadLookVector 的第一个call
		if (!sigcall) {
			throw "Actor::getMovementProxy() Error, sig no fond";
		}
		sig = Utils::FuncFromSigOffset(sigcall, 1);
	}
	uintptr_t unknow = 0;
	using Fn = ActorMovementProxy * (__fastcall*)(Actor*, uintptr_t*);
	return reinterpret_cast<Fn>(sig)(this, &unknow);
}



auto Actor::setVelocity(vec3_t* sp)->void*{
	using Fn = void*(__fastcall*)(Actor*, vec3_t*);
	return reinterpret_cast<Fn>(setVelocityCallptr)(this,sp);
}

auto Actor::isInWater() -> bool{
	//isInWaterCallptr
	using Fn = bool(__fastcall*)(Actor*);
	return reinterpret_cast<Fn>(isInWaterCallptr)(this);
}


auto Actor::getShadowRadius()->float {
	using Fn = float(__fastcall*)(Actor*);
	return reinterpret_cast<Fn>(getShadowRadiusCallptr)(this);
}

// 虚表函数

auto Actor::getStatusFlag(ActorFlags af)->bool{
	return GetVFtableFun<bool, Actor*, ActorFlags>(0)(this, af);
}

auto Actor::setStatusFlag(ActorFlags af, bool flag)->void {
	GetVFtableFun<void, Actor*, ActorFlags,bool>(1)(this, af, flag);
}




//应该获取的是头部位置，Y值会往上偏两个单位
auto Actor::getPosition()->vec3_t* {
	return GetVFtableFun<vec3_t*, Actor*>(22)(this);	// 更新自 1.20
}

auto Actor::getPosPrev()->vec3_t* {
	return GetVFtableFun<vec3_t*, Actor*>(23)(this);	// 更新自 1.20
}

// 新版本中虚表不存在此函数
//auto Actor::setRot(vec2_t* rot)->void {
//	GetVFtableFun<void, Actor*,vec2_t*>(29)(this,rot);
//}

auto Actor::teleportTo(vec3_t* pos, bool a1, unsigned int a2, unsigned int a3)->void {
	GetVFtableFun<void, Actor*, vec3_t*, bool, unsigned int, unsigned int>(45)(this, pos, a1, a2, a3);
}

auto Actor::getNameTag()->TextHolder* {
	return GetVFtableFun<TextHolder*, Actor*>(57)(this);
}

// 第19号虚表调用的都是 Player::isPlayer 所以一定返回true > 20 35 62ok 71 77 81 82 84 86ok
// 第40号虚表调用的都是 Actor::isPlayer  所以一定返回false
// 第67->70号虚表 Actor调用的是 Actor::isPlayer, Player调用的是Player::isPlayer,所以可用
auto Actor::isPlayer()->bool {
	return reinterpret_cast<bool(__fastcall*)(Actor*)>((*(uintptr_t**)this)[62])(this);
}

auto Actor::getNameTagAsHash()->unsigned __int64 {
	return GetVFtableFun<unsigned __int64, Actor*>(58)(this);
}

/*
_QWORD *__fastcall sub_1419CC9C0(__int64 a1, _QWORD *a2)
{
  (*(void (__fastcall **)(__int64))(*(_QWORD *)a1 + 504i64))(a1);
  *a2 = 0i64;
  a2[2] = 0i64;
  a2[3] = 0i64;
  sub_1400A7970(a2, a1 + 1440);			//std::string::string
  return a2;
}
*/
//这个函数暂存，因为会崩溃
auto Actor::getFormattedNameTag()->TextHolder {
	//void* ret = malloc(8 * 4);
	TextHolder ret;
	GetVFtableFun<TextHolder, Actor*, TextHolder>(59)(this,ret);
	return ret;
}

//新版本中虚表不存在该函数
//auto Actor::getRotation()->vec2_t* {
//	return GetVFtableFun<vec2_t*, Actor*>(81)(this);
//}

auto Actor::setSneaking(bool b)->void {
	return GetVFtableFun<void, Actor*, bool>(87)(this, b);
}

// 是否着火
auto Actor::isOnFire(void) -> bool
{
	return GetVFtableFun<bool, Actor*>(91)(this);
}

auto Actor::isRemotePlayer()->bool {
	return GetVFtableFun<bool, Actor*>(97)(this);
}

auto Actor::getEntityTypeId()->int {
	return GetVFtableFun<unsigned int, Actor*>(153)(this);
}

auto Actor::changeDimension(AutomaticID<class Dimension, int> dim)->void {
	return GetVFtableFun<void, Actor*, class AutomaticID<class Dimension, int>>(165)(this,dim);
}

// 造成跌落伤害 
auto Actor::checkFallDamage(float a, bool b)->void* {
	return GetVFtableFun<void*, Actor*, float, bool>(167)(this,a,b);
}

//auto Actor::causeFallDamage(float a, float b, ActorDamageSource* c)->void* {
//	return GetVFtableFun<void*, Actor*, float, float, ActorDamageSource*>(168)(this,a,b,c);
//}

auto Actor::isClientSide() -> bool
{
	return GetVFtableFun<bool, Actor*>(186)(this);
}

auto Actor::getAttribute(Attribute attribute)->AttributeInstance* {
	return GetVFtableFun<AttributeInstance*, Actor*, Attribute*>(188)(this, &attribute);
}

auto Actor::setSize(float width, float height)->void {
	return GetVFtableFun<void, Actor*, float,float >(212)(this, width, height);
}