#pragma once
#include "../Module.h"
#include "MinecraftUIRenderContext.h"

class Render : public Module {
public:
	Render();
public:
	virtual auto onRenderDetour(MinecraftUIRenderContext*)->void;

};