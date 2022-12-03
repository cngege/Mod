#pragma once
#include "../Module.h"

class GameTimeLock : public Module {
public:
	GameTimeLock();
public:
	int time = 0;
public:
	virtual auto onTick(class GameMode*)->void override;
	virtual auto onLevelTick(class Level*)->void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;

};