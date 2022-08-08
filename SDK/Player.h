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
public:
	auto getSpeed()->vec3_t;
	auto setSpeed(vec3_t)->void;

public:
	auto onLocalPlayerTick()->void;
};