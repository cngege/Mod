#pragma once
#include "Player.h"
//#include <intsafe.h>

//LocalPlayer::LocalPlayer
//40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ?  48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 89 4D ? 4C 89 45 ? 48 8B DA
class LocalPlayer : public Player {
protected:
	static uintptr_t** vfTables;

public:
	static int toCIoffset;
	static int onGroundoffset;
	
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int)->auto*;
	static auto GetVFtableFun(int)->uintptr_t*;
	static auto GetVFtables()->uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables)->void;
	//LocalPlayer::displayClientMessage(std::string const&)  //388

	// Hook函数回调
public:
	static uintptr_t* tickWorldCall;									/*332*/

public:
	//定义函数
	auto getClientInstance()->class ClientInstance*;
	auto isOnGround()->bool*;
	//auto isValid()->bool;

public:
	//虚表函数
	//auto setPos(vec3_t*)->__int64;									/*14*/
	auto jumpFromGround()-> void*;										/*346*/
	auto displayClientMessage(std::mcstring*)-> void*;					/*389*/
};