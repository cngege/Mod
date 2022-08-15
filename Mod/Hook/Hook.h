#pragma once
#include "../../Memory/MinHook.h"

#include "../Utils/HMath.h"
#include "ClientInstance.h"

template <typename T>
inline MH_STATUS MH_CreateHookEx(LPVOID pTarget, LPVOID pDetour, T** ppOriginal)
{
	return MH_CreateHook(pTarget, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}

class Hook
{
public:
	static auto init() ->void;
	static auto exit() ->void;
	static auto PlayerKB(class Player* ,vec3_t*) ->void;
	static auto ClientInstance_Tick(ClientInstance*, void*)->void;
	static auto Is_ShowCoordinates_Tick(void*)->bool;
	static auto GetHungerValAddress_Tick(void*, const char*, void*)->void*;
	static auto DestroyBlocking(void*,void*,int, float)->float;
	static auto NoFallDamage_Tick(void*,float*)->void*;
	static auto Covers_HitBox_Parts(void*, void*, void*)->void*;
	static auto Player_getCameraOffset(class Player*)->vec2_t*;
	static auto AllActor_Tick(class Actor*, float*, float)->float*;
	static auto Actor_moveBBs(class Actor*, vec3_t*)->void*;
};