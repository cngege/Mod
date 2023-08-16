#pragma once
#include "../Module.h"

class Debug : public Module {
public:
	Debug();
public:
	virtual auto onImGUIRender() -> void override;
	virtual auto onInternalImGUIRender() -> void override;
	virtual auto onloadConfigFile(json& data) -> void override;
	virtual auto onsaveConfigFile(json& data) -> void override;
};