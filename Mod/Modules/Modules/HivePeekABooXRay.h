#pragma once
#include "../Module.h"

#include "imgui.h"

class HivePeekABooXRay : public Module
{
private:
	bool colorFromName = true;
	bool hasLine = true;
	bool lineColorFromPlayer = true;
	ImColor playerBoxColor;
	ImColor playerLineColor;
	ImColor blockBoxColor;
	ImColor blockLineColor;

	
	struct PlayerMapInfo;
	std::unordered_map<Player*, PlayerMapInfo> playerlist;
	std::vector<PlayerMapInfo> removePlyaerList;

public:
	HivePeekABooXRay();

public:
	virtual auto onImGUIRender() -> void override;
	virtual auto onstartLeaveGame(class Level*) -> void override;
	virtual auto onPlayerTick(class Player*) -> void override;		// 远程玩家tick不好使
	virtual auto onDimensionChanged(class ClientInstance*) -> void override;
	virtual auto onLevelTick(Level* level) -> void override;
	virtual auto onloadConfigFile(json& data) -> void override;
	virtual auto onsaveConfigFile(json& data) -> void override;
};