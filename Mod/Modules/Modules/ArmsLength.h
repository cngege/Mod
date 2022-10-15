#pragma once
#include "../Module.h"

class ArmsLength : public Module {
public:
	ArmsLength();

private:
	float* arms;
	float distance = 7;
public:
	virtual auto onEnable()->void override;
	virtual auto onDisable()->void override;
	virtual auto isEnabled()->bool override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};