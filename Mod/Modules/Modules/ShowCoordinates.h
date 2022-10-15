#pragma once
#include "../Module.h"


class ShowCoordinates : public Module {
public:
	ShowCoordinates();

public:
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};