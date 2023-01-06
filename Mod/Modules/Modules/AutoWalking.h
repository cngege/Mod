#pragma once
#include "../Module.h"

class AutoWalking : public Module {
public:
	AutoWalking();
public:
	virtual auto onloadConfigFile(json& data) -> void override;
	virtual auto onsaveConfigFile(json& data) -> void override;
};