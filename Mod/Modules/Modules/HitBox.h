#pragma once
#include "../Module.h"

class HitBox : public Module {
private:
	std::unordered_map<Player*, vec2_t> playerlist;
public:
	HitBox();
	float width = 5.0f;
	float height = 3.0f;
public:
	virtual auto isEnabled()->bool override;
	virtual auto getBindKeyName()->std::string override;
	virtual auto onPlayerSightTick(class Player*)->void override;
	virtual auto onstartLeaveGame(class Level*) -> void override;
	virtual auto onDimensionChanged(class ClientInstance*) -> void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};