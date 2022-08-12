#pragma once
#include "..\Mod\Utils\HMath.h"
#include "Actor.h"

class Player : Actor
{
public:
	static Player* LocalPlayer;

	static int YView1;
	static int XView1;
	static int YView2;
	static int XView2;

public:

	auto getViewYX()->const vec2_t;
	auto getViewYX2()->const vec2_t;

public:
	auto onLocalPlayerTick()->void;
	auto onAllPlayerTick()->void;
};