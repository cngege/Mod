#pragma once
#include "../Module.h"
#include "MinecraftUIRenderContext.h"

class Render : public Module {
public:
	Render();

private:
	float fontsize = 1.f;

public:
	float x = 5.f;
	float y = 65.f;
public:
	virtual auto onRenderDetour(MinecraftUIRenderContext*)->void;
	virtual auto onImGUIRender()->void;
};