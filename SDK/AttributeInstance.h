#pragma once

enum Attribute : __int64 {
	HEALTH = 25769804032,						//25769804032 30064771328 06(int)
	MOVEMENT_SPEED = 38654705921				//来自 Player第292个虚表函数 Player::getSpeed 38654705921
};

class AttributeInstance {
public:
	static int getCurrentValueoffset;

public:
	auto getCurrentValue()->float;
};
