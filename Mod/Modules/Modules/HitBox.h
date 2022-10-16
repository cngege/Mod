#pragma once
#include "../Module.h"

class HitBox : public Module {
public:
	HitBox();
	float width = 5.0f;
	float height = 3.0f;
public:
	virtual auto isEnabled()->bool override;
	virtual auto getBindKeyName()->std::string override;
	virtual auto onActorTick(class Actor*)->void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};