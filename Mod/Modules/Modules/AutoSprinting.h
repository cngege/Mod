#pragma once
#include "../Module.h"
//#include "Mob.h"

class AutoSprinting : public Module {
public:
	AutoSprinting();


public:
	virtual auto onTick(class GameMode* gm)->void override;
};