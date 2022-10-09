#pragma once
#include "../Module.h"

class AirJump : public Module {
public:
	AirJump();

public:
	virtual auto onTick(class GameMode*)->void override;
};