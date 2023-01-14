#pragma once
#include "../Module.h"
#include "MinecraftUIRenderContext.h"

class Render : public Module {
public:
	Render();
public:
	virtual auto onRenderDetour(MinecraftUIRenderContext*)->void override;
	virtual auto onImGUIRender()->void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};