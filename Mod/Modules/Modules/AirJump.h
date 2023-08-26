#pragma once
#include "../Module.h"

class AirJump : public Module {
public:
	AirJump();

public:
	virtual auto onPlayerTick(class Player*) -> void;
	virtual auto onTick(class GameMode*)->void override;
	virtual auto onLocalPlayerTick(class LocalPlayer*)->void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};