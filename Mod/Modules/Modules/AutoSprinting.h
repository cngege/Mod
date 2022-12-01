#pragma once
#include "../Module.h"
//#include "Mob.h"

class AutoSprinting : public Module {
public:
	AutoSprinting();


public:
	virtual auto onTick(class GameMode* gm)->void override;
	virtual auto onLocalPlayerTick(class LocalPlayer* lp) -> void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};
