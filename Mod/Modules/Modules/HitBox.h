#pragma once
#include "../Module.h"

class HitBox : public Module {
public:
	HitBox();

public:
	virtual auto onDisable()->void override;
	virtual auto onActorTick(class Actor*)->void override;
};