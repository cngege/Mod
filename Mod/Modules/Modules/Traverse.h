#pragma once
#include "../Module.h"

class Traverse : public Module {
public:
	Traverse();

public:
	virtual auto getBindKeyName()->std::string override;
	virtual auto onMouseUpdate(char mousebutton, char isdown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY)->void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};