#pragma once

#include "../Module.h"

class ChangeGameMode : public Module {
public:
	ChangeGameMode();

	virtual auto onKeyUpdate(int key, bool isdown) -> void override;
	virtual auto onloadConfigFile(json& data) -> void override;
	virtual auto onsaveConfigFile(json& data) -> void override;
};