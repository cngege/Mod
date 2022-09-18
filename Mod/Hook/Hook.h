#pragma once
#include <string>
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
	static auto NoFallDamage_Tick(class Player*,float*)->void*;
	static auto LocalPlayer_getCameraOffset(class LocalPlayer*)->vec2_t*;
	static auto AllActor_Tick(class Actor*, float*, float)->float*;
	static auto Actor_moveBBs(class Actor*, vec3_t*)->void*;
	static auto KeyUpdate(__int64, int)->void*;
	static auto RenderDetour(void*, class MinecraftUIRenderContext*)->void;
	static auto Draw_Text(class MinecraftUIRenderContext*,class BitmapFont*, struct RectangleArea const&, class TextHolder*, struct UIColor const& , float , float , struct TextMeasureData*, uintptr_t*)->void;
	static auto sendMessage(void*, TextHolder*)->__int64;
public:
	//虚表Hook
	static auto GameMode_attack(class GameMode*, class Actor*)->bool;
	static auto GameMode_tick(class GameMode*)->void*;
	static auto GameMode_startDestroyBlock(class GameMode* _this, vec3_ti* a2, uint8_t* face, void* a3, void* a4)->bool;

	static auto ServerPlayer_TickWorld(class ServerPlayer* _this, class struck* tick)->void*;
};