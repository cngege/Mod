#pragma once
#include <string>
#include "..\Mod\Utils\HMath.h"

#include "AttributeInstance.h"

#include "../Hook/HookFunction.h"

template <typename A, typename T>
class AutomaticID {
	T id;

public:
	AutomaticID() {
		id = 0;
	}

	AutomaticID(T x) {
		id = x;
	}

	inline operator T() const {
		return id;
	}
};

enum ActorType {
	player = 1,
	iron_golem = 788,
	panda = 9477,
	parrot = 21278,
	villager = 16778099,

	Hive_Treasure = 256
};

enum ActorFlags {
	isSneaking = 1,
	isSprinting = 3,
	//canFly = 21					// 这就是直接从 bds里看来的 Actor::canFly内部( 未检验->不是21
};

class Actor
{
protected:
	static uintptr_t** vfTables;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int)->auto*;
	static auto GetVFtableFun(int)->uintptr_t*;
	static auto GetVFtables()->uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables)->void;

public:

	static int AABBOffset;
	//static int PosXOffset1;
	//static int PosYOffset1;
	//static int PosZOffset1;
	//static int PosXOffset2;
	//static int PosYOffset2;
	//static int PosZOffset2;

	//static int XHitBoxOffset;
	//static int YHitBoxOffset;

	static int LevelOffset;
	static int IsRemovedOffset;
	static int GetRotationOffset;

	static int GetAttributeInstance_HealthFunVT;
	//static uintptr_t isSneakingCallptr;
	static uintptr_t* setVelocityCallptr;
	static uintptr_t* getShadowRadiusCallptr;
	static uintptr_t* isInWaterCallptr;
	static uintptr_t* isInvisibleCallptr;

	// 使用特征码直接找到的函数地址
	static uintptr_t* getDimensionConstCallptr;


public:
	
	auto isPlayerEx()->bool;

public:

	auto getAABB()->class AABB*;
	auto getPosEx()->vec3_t;
	auto getPosEx2()->vec3_t;
	[[deprecated]]
	auto setPosEx(vec3_t)->void;

	auto getHitBox()->vec2_t;
	auto setHitBox(vec2_t)->void;
	auto resetHitBox()->void;
	auto getLevel()->class Level*;
	auto getActorCollision() -> class ActorCollision*;		//this + 8bit
	auto getDimensionBlockSource() -> class BlockSource*;
		//E8 ? ? ? ? 48 8B CF 48 8B 90 ? ? ? ? 48 8B C3 FF 15 ? ? ? ? 48

public:
	//auto onMoveBBs(vec3_t)->void;
	auto isLocalPlayer()->bool;
	auto getHealth()->float;
	auto isSneaking()->bool;
	auto isRemovedEx()->bool;
	auto getRotationEx()->vec2_t*;
	auto isValid()->bool;
public:
	/* 原版函数 通过特征码获取 */
	auto getDimensionConst()->class Dimension*;
	auto setPos(vec3_t*) -> void*;
	auto setPosPrev(vec3_t*) -> void*;
	//E8 ? ? ? ? 4C 8B C0 33 FF 8B DF 48 8B 50 ? 48 85 D2
	auto getMovementProxy() -> class ActorMovementProxy*;
public:
	// Hook的函数
	auto setVelocity(vec3_t*)->void*;												/*46*/
	/**
	 * @brief 是隐形的
	 * @return 
	*/
	auto isInvisible() -> bool;														/*52*/
	auto isInWater()->bool;															/*73*/
	auto getShadowRadius()->float;													/*79*/
public:




	//原生虚表函数
	auto getStatusFlag(ActorFlags) -> bool;											/*0*/
	auto setStatusFlag(ActorFlags,bool)->void;										/*1*/
	
	auto getPosition()->vec3_t*;													/*21*/
	auto getPosPrev()->vec3_t*;														/*22*/
	//设置玩家移动的方向  这个函数在1.19.50.02 版本开始没有了
	//auto setRot(vec2_t*)->void;													/*26*/
	auto teleportTo(vec3_t* pos, bool a1, unsigned int a2, unsigned int a3)->void;	/*43*/
	auto getNameTag()->class TextHolder*;											/*62*/
	auto getNameTagAsHash()->unsigned __int64;										/*63*/
	auto getFormattedNameTag()-> TextHolder;										/*64*/
	auto isPlayer()->bool;															/*67*/  // 因为MC中该函数功能的实现方法是 Player类重写,现在由类地址获取虚表获取该函数地址
	
	//获取玩家的移动方向 该函数在1.19.50.02 版本开始没有了
	//auto getRotation()->vec2_t*;													/*81*/
	auto setSneaking(bool)->void;													/*87*/	// 1.20.15
	auto isOnFire(void) -> bool;													/*91*/
	auto isRemotePlayer() -> bool;													/*97*/	// 1.20.15
	auto getEntityTypeId() -> int;													/*153*/	// 1.20.15 可能是 Player::getEntityTypeId()
	// 需要serverActor权限
	auto changeDimension(AutomaticID<class Dimension, int>)->void;					/*165*/ // 1.20.15
	auto checkFallDamage(float, bool) -> void*;										/*167*/	// 1.20.15
	//auto causeFallDamage(float, float, class ActorDamageSource*)->void*;			/*168*/ // 1.20.15
	auto isClientSide() -> bool;													/*186*/ // 1.20.15
	auto getAttribute(Attribute) -> class AttributeInstance*;						/*188*/
	/*211*/
	// Actor::setAuxValue(int)
	// Actor::resetUserPos(bool) 
	// Actor::animateHurt(void) 
	// Actor::onTame(void) 
	// Actor::reloadHardcoded(enum Actor::InitializationMethod,class VariantParameterList const & __ptr64)
	// Actor::buildDebugInfo(class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > & __ptr64)
	// Actor::setEquippedSlot(enum EquipmentSlot,class ItemStack const & __ptr64)
	// Actor::healEffects(int)
	// Actor::playerTouch(class Player & __ptr64)
	// Actor::stopSpinAttack(void)
	// Actor::onBounceStarted(class BlockPos const & __ptr64,class Block const & __ptr64)
	// Actor::swing(void)
	// ...
	// Actor::setSleeping(bool)
	// Actor::changeDimension(class ChangeDimensionPacket const & __ptr64)
	auto setSize(float,float)->void;												/*212*/
};
