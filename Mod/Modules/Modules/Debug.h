#pragma once
#include "../Module.h"

class Debug : public Module {
public:
	Debug();
public:
	virtual auto onImGUIRender() -> void override;
};