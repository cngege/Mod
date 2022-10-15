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
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};