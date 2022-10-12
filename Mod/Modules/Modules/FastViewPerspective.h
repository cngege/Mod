#pragma once
#include "../Module.h"

class FastViewPerspective : public Module {
public:
	FastViewPerspective();
public:
	auto isToggle()->bool;
	
public:
	virtual auto getBindKeyName()->std::string override;
	virtual auto onTick(class GameMode* gm)->void override;
};