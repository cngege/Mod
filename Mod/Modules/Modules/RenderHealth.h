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

	std::string currentPlayerName;
	std::string currentPlayerTypeName;
	int currentPlayerHealth = 0;

public:
	virtual auto onAttackAfter(class GameMode* gm,class Actor*)->void override;
	virtual auto onImGUIRender() -> void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};
