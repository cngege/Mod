#include "Actor.h"
#include "LocalPlayer.h"
#include "ServerPlayer.h"
#include "AttributeInstance.h"
#include "ActorCollision.h"
#include "AABB.h"
#include "../Mod/Utils/Utils.h"
#include "../Mod/Utils/Game.h"
#include <math.h>


int Actor::AABBOffset = 0;
int Actor::LevelOffset = 0;
int Actor::IsRemovedOffset = 0;
int Actor::GetRotationOffset = 0;

int Actor::GetAttributeInstance_HealthFunVT = 0;
//uintptr_t Actor::isSneakingCallptr = 0;
uintptr_t* Actor::setVelocityCallptr = nullptr;
uintptr_t* Actor::getShadowRadiusCallptr = nullptr;
uintptr_t* Actor::isInWaterCallptr = nullptr;
uintptr_t* Actor::isInvisibleCallptr = nullptr;

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

auto Actor::getDimensionBlockSource() -> class BlockSource*
{
	// 来自 getDimensionBlockSource调用者 getDimensionBlockSource来自 BlockLegacy::playerDestroy 这个函数可在Block虚表中找到
	static uintptr_t offset = FindSignature("E8 ? ? ? ? 48 8B CF 48 8B 90 ? ? ? ? 48 8B C3 FF 15 ? ? ? ? 48");
	if (!offset) {
		throw "Actor::getDimensionBlockSource() Error, sig no fond";
	}
	using getDimensionBlockSourceFn = BlockSource * (__fastcall*)(Actor*);
	return Utils::FuncFromSigOffset<getDimensionBlockSourceFn>(offset, 1)(this);
}

// 特征码的获取：
auto Actor::getTypeName() -> std::mcstring*
{
	//sub_142E82E70
	using Fn = void*(__fastcall*)(Actor*);
	//uintptr_t FnPtr = Utils::getBase() + 0x2E82E70;		//Actor::getActorIdentifier
	//return (std::mcstring*)(((Fn)FnPtr)(this) + 17);			//+17(*8) => ActorDefinitionIdentifier::getCanonicalName
	static auto call = FindSignature("E8 ? ? ? ? 48 05 ? ? ? ? 48 83 78");		// 这个特征码要获取两个东西,一个是+1函数call, 另一个是+7的偏移
	if (!call) throw "Actor::getTypeName() 特征码失效";
	int offset = *reinterpret_cast<int*>(call + 7);
	uintptr_t ActorDefinitionIdentifier = (uintptr_t)((Fn)Utils::FuncFromSigOffset(call, 1))(this);
	return (std::mcstring*)(ActorDefinitionIdentifier + offset);
}

//auto Actor::onMoveBBs(vec3_t p)->void {
//
//}

auto Actor::isLocalPlayerEx()->bool {
	return (*(__int64*)this == (__int64)LocalPlayer::GetVFtables());
}

auto Actor::getHealth()->float {
	//AttributeInstance* AI = this->getAttribute(Attribute::HEALTH);
	//float v = AI->getCurrentValue();
	//return ceil(v);
	return this->getActorCollision()->getHealth();
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

// （函数）特征码的寻找方法见wiki
auto Actor::getMovementProxy() -> class ActorMovementProxy*
{
	static auto offset_fn = FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B dA 33 FF 48 89 5C 24 ? 48 8B D1 48 8B 41 ? 48 8B");
	using Fn = ActorMovementProxy * (__fastcall*)(Actor*);
	return *(ActorMovementProxy**)reinterpret_cast<Fn>(offset_fn)(this);
}

auto Actor::getOrCreateUniqueID() -> int*
{
	const char* memcode_fn = "40 53 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B 41 ? 48 8D";
	const char* memcode_call = "E8 ? ? ? ? 48 8B D0 48 8B 0B E8";
	static auto offset_fn = Utils::getFunFromSigAndCall(memcode_fn, memcode_call, 1);
	using Fn = int* (__fastcall*)(Actor*);
	return reinterpret_cast<Fn>(offset_fn)(this);
}


auto Actor::setVelocity(vec3_t* sp)->void*{
	using Fn = void*(__fastcall*)(Actor*, vec3_t*);
	return reinterpret_cast<Fn>(setVelocityCallptr)(this,sp);
}

auto Actor::isInvisible() -> bool
{
	using Fn = bool (__fastcall*)(Actor*);
	return reinterpret_cast<Fn>(isInvisibleCallptr)(this);
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
	//return GetVFtableFun<vec3_t*, Actor*>(22)(this);	// 1.20.40.01 不在虚表
	// 手动特征码实现
	static uintptr_t offset = FindSignature("E8 ? ? ? ? 48 C7 44 24 ? ? ? ? ? 4C 8B C8 4C 8D 05 ? ? ? ? 48 8B D7"); //+1 //1.20.41.02 : sub_142F19290
	
	_ASSERT(offset);

	static auto offset_fn = Utils::FuncFromSigOffset<vec3_t*(__fastcall*)(Actor*)>(offset, 1);
	return offset_fn(this);
}

auto Actor::getPosPrev()->vec3_t* {
	//return GetVFtableFun<vec3_t*, Actor*>(23)(this);	// 1.20.40.01 不在虚表
	return nullptr;
}

// 新版本中虚表不存在此函数
//auto Actor::setRot(vec2_t* rot)->void {
//	GetVFtableFun<void, Actor*,vec2_t*>(29)(this,rot);
//}

auto Actor::teleportTo(vec3_t* pos, bool a1, unsigned int a2, unsigned int a3)->void {
	GetVFtableFun<void, Actor*, vec3_t*, bool, unsigned int, unsigned int>(26)(this, pos, a1, a2, a3);	//更新自 1.20.41
}

auto Actor::getNameTag()->std::mcstring* {
	//return GetVFtableFun<std::mcstring*, Actor*>(56)(this);			// 1.20.40.01 不在虚表
	static uintptr_t offset_fn = FindSignature("48 83 EC 28 48 8B 81 ? ? ? ? 48 85 C0 74 ? 48 8B 08 BA 04");	// 函数本身 1.20.41.02: sub_142F27330
	_ASSERT(offset_fn);
	return reinterpret_cast<std::mcstring * (__fastcall*)(Actor*)>(offset_fn)(this);
}

// 第19号虚表调用的都是 Player::isPlayer 所以一定返回true > 20 35 62ok 71 77 81 82 84 86ok
// 第40号虚表调用的都是 Actor::isPlayer  所以一定返回false
// 第67->70号虚表 Actor调用的是 Actor::isPlayer, Player调用的是Player::isPlayer,所以可用	//Actor::setNameTag -X- Actor::setScoreTag
auto Actor::isPlayer()->bool {
	return reinterpret_cast<bool(__fastcall*)(Actor*)>((*(uintptr_t**)this)[63])(this);//更新自 1.20.41.02
}

//新版本中虚表不存在该函数
//auto Actor::getRotation()->vec2_t* {
//	return GetVFtableFun<vec2_t*, Actor*>(81)(this);
//}

auto Actor::setSneaking(bool b)->void {
	return GetVFtableFun<void, Actor*, bool>(55)(this, b);	  //更新自 1.20.41
}

// 是否着火
auto Actor::isOnFire() -> bool
{
	return GetVFtableFun<bool, Actor*>(59)(this);				  //更新自 1.20.41（原90）
}

// isLocalPlayer 在 isRemotePlayer上面一个,不确定可以通过DirectPlayerMovementProxy::isLocalPlayer内容确定,其中(_QWORD **)this + 2)就是Actor
auto Actor::isLocalPlayer() -> bool
{
	return Utils::CallVFunc<62, bool>(this);				  //更新自 1.20.41
}

// 函数有个关键字：-1594224897 要满足函数仅一个参数，关键字在函数开头，整个函数只有这一个负数的关键字
auto Actor::isRemotePlayer()->bool {
	//return Utils::CallVFunc<96, bool>(this);				  //更新自 1.20.30
	// 1.20.41.02 ： sub_140A66A20  （this + 8byte）
	const char* offset_fn = "40 53 48 83 EC ? 48 8B 01 48 8B D9 BA FF"; // 本函数
	const char* offset_call = "E8 ? ? ? ? 84 C0 74 ? 88 9D"; // 它的调用处
	static auto offset = Utils::getFunFromSigAndCall(offset_fn, offset_call, 1);
	_ASSERT(offset);
	return reinterpret_cast<bool(__fastcall*)(Actor*)>(offset)((Actor*)((uintptr_t)this + 8));
}

auto Actor::getEntityTypeId()->int {
	return GetVFtableFun<unsigned int, Actor*>(100)(this);	  //更新自 1.20.41
}

auto Actor::changeDimension(AutomaticID<class Dimension, int> dim)->void {
	return GetVFtableFun<void, Actor*, class AutomaticID<class Dimension, int>>(110)(this,dim);	//更新自 1.20.41
}

// 造成跌落伤害 
auto Actor::checkFallDamage(float a, bool b)->void* {
	return GetVFtableFun<void*, Actor*, float, bool>(112)(this,a,b);								//更新自 1.20.41
}

//sub_142F2D8B0 1.20.41.02
auto Actor::isClientSide() -> bool
{
	//return GetVFtableFun<bool, Actor*>(182)(this);			//更新自 1.20.30
	const char* offset_fn = "48 83 EC 28 48 8B 89 ? ? ? ? 48 85 C9 74 ? 48 8B ? 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 84 C0 75";
	const char* offset_call = "E8 ? ? ? ? 49 8B 0F 84 ? 74 ? 49"; //+1
	static auto offset = Utils::getFunFromSigAndCall(offset_fn, offset_call, 1);
	_ASSERT(offset);
	return reinterpret_cast<bool(__fastcall*)(Actor*)>(offset)(this);

	// 搜索 "GameMode::useItemOn (client)" 能找到调用处， 往上找参数为 a1 + 8的即是
	//v27 = sub_142F2D8B0(*(_QWORD*)(a1 + 8));
	//v28 = *v26;
	//if (v27)
	//{
	//	sub_141489A00(v28, a4);
	//	sub_141489AC0(*v26);
	//	*(_QWORD*)&v86 = "GameMode::useItemOn (client)";
}

auto Actor::getAttribute(Attribute attribute)->AttributeInstance* {
	//return GetVFtableFun<AttributeInstance*, Actor*, Attribute*>(184)(this, &attribute);			//更新自 1.20.41
	//调用处可在 Mob::baseTick 中 < 0.0 那一行找到 当前特征码不在 Mob::baseTick 中
	//sub_142FCC100
	const char* offset_call = "E8 ? ? ? ? 0F 57 C0 0F 2F 80";//+1
	static auto offset = FindSignature(offset_call);
	_ASSERT(offset);
	offset = Utils::FuncFromSigOffset(offset, 1);
	return reinterpret_cast<AttributeInstance*(__fastcall*)(Actor*, Attribute)>(offset)(this, attribute);

}

auto Actor::setSize(float width, float height)->void {
	return GetVFtableFun<void, Actor*, float,float >(144)(this, width, height);					//更新自 1.20.41
}