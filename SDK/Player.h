#pragma once
#include "..\Mod\Utils\HMath.h"
#include "Actor.h"

class Player : Actor
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

	static int YView1;
	static int XView1;
	static int YView2;
	static int XView2;

public:
	auto getSpeed()->vec3_t;
	auto setSpeed(vec3_t)->void;

	auto getPos()->vec3_t;
	auto getPos2()->vec3_t;
	auto setPos(vec3_t)->void;

	auto getHitBox()->vec2_t;
	auto setHitBox(vec2_t)->void;
	auto resetHitBox()->void;

	auto getViewYX()->const vec2_t;
	auto getViewYX2()->const vec2_t;

public:
	auto onLocalPlayerTick()->void;
	auto onAllPlayerTick()->void;
};