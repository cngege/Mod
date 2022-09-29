#pragma once
#include "../Module.h"

class FastViewPerspective : public Module {
public:
	FastViewPerspective();
private:
	float lpRotVertical = -30.0f;
public:
	auto isToggle()->bool;
	
public:
	virtual auto getBindKeyName()->std::string override;
	virtual auto onTick(class GameMode* gm)->void override;
};