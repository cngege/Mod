#pragma once
#include "../Module.h"
#include "../../Utils/HMath.h"

class RenderHealth : public Module {
public:
	RenderHealth();

private:
	vec2_t showpos = vec2_t(230.f, 250.f);
	float tick = 0.f;
	bool show = false;
	uintptr_t currentActorVT = 0;
	class Actor* currentActor = nullptr;
	std::string currentPlayerName;
	int currentPlayerHealth = 0;

public:
	virtual auto onAttack(class Actor*)->bool override;
	virtual auto onRenderDetour(class MinecraftUIRenderContext*)->void override;
};
