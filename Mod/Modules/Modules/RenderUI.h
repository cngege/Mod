#pragma once
#include "../Module.h"

class RenderUI : public Module {
public:
	RenderUI();
public:
	virtual auto onEnable() -> void override;
	virtual auto onDisable() -> void override;
	virtual auto onKeyUpdate(int key, bool isDown)-> void override;
	virtual auto onImGUIRender()->void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};