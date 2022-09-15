#pragma once
#include "../Module.h"
#include "MinecraftUIRenderContext.h"

class Render : public Module {
public:
	Render();

private:
	float fontsize = 1.f;

public:
	virtual auto onRenderDetour(MinecraftUIRenderContext*)->void;

};