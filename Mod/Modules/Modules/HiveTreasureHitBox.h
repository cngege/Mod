#pragma once
#include "../Module.h"

class HiveTreasureHitBox : public Module
{
public:
	HiveTreasureHitBox();

public:

	virtual auto onActorTick(class Actor* actor)->void override;
};