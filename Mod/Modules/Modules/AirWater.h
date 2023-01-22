#pragma once
#include "../Module.h"

class AirWater : public Module {
public:
	AirWater();

public:

	virtual auto onEnable() -> void override;
	virtual auto onloadConfigFile(json& data) -> void override;
	virtual auto onsaveConfigFile(json& data) -> void override;
};