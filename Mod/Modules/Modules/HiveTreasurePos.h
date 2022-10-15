#pragma once
#include "../Module.h"

class HiveTreasurePos : public Module
{
public:
	HiveTreasurePos();

public:

	virtual auto onActorTick(class Actor* actor)->void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};