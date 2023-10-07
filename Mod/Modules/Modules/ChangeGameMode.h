#pragma once

#include "../Module.h"

class ChangeGameMode : public Module {
public:
	ChangeGameMode();

	virtual auto onKeyUpdate(int key, bool isdown) -> void override;
};