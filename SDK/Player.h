#pragma once
#include "..\Mod\Utils\HMath.h"

class Player
{
public:
	static Player* LocalPlayer;

public:
	static int SpeedXOffset;
	static int SpeedYOffset;
	static int SpeedZOffset;

	static int PosXOffset1;
	static int PosYOffset1;
	static int PosZOffset1;
	static int PosXOffset2;
	static int PosYOffset2;
	static int PosZOffset2;

	static int XHitBoxOffset;
	static int YHitBoxOffset;

public:
	auto getSpeed()->vec3_t;
	auto setSpeed(vec3_t)->void;

	auto getPos()->vec3_t;
	auto getPos2()->vec3_t;
	auto setPos(vec3_t)->void;

	auto getHitBox()->vec2_t;
	auto setHitBox(vec2_t)->void;
	auto resetHitBox()->void;

public:
	auto onLocalPlayerTick()->void;
};