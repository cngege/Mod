#pragma once
#include "../Module.h"

class HiveTreasurePos : public Module
{
public:
	HiveTreasurePos();

private:
	float S_TreasureHeight = 0;
public:

	virtual auto onActorTick(class Actor* actor)->void override;
};