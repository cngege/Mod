#pragma once
#include "../Module.h"
#include "../../Utils/HMath.h"

class RenderHealth : public Module {
public:
	RenderHealth();

private:
	// UI
	float actorTime = 5.f;
private:
	vec2_t showpos = vec2_t(230.f, 250.f);
	std::optional<double> showtime;

	std::string currentPlayerName;
	std::string currentPlayerTypeName;
	float currentPlayerHealth = 0.f;

public:
	virtual auto onAttackAfter(class GameMode* gm,class Actor*)->void override;
	virtual auto onImGUIRender() -> void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};
