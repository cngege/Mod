#pragma once
#include "../Module.h"

class HundredTimesMoreDrops : public Module {
public:
	HundredTimesMoreDrops();

public:
	int multiple = 100;
	bool onlyLocalPlayer = false;
public:
	virtual auto onloadConfigFile(json& data) -> void override;
	virtual auto onsaveConfigFile(json& data) -> void override;
};