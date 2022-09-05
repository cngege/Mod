#pragma once
#include "../Module.h"

class HitBox : public Module {
public:
	HitBox();


public:
	virtual auto getBindKeyName()->std::string override;
	virtual auto onKeyUpdate(int key, bool isDown)->void override;
	virtual auto onDisable()->void override;
	virtual auto onActorTick(class Actor*)->void override;
};