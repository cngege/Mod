#pragma once
#include "../Module.h"
#include "imgui.h"

class AABB;

class BioRadar : public Module {
public:
	BioRadar();
private:
	struct PlayerMapInfo;
	std::unordered_map<Player*, PlayerMapInfo> playerlist;
	std::vector<PlayerMapInfo> removePlyaerList;

	//雷达的地图的边长 像素
	float radarSide = 200.f;
	//游戏中距离与屏幕像素的缩放关系
	int roomscale = 2;
	//雷达与屏幕水平边缘的距离 具体是左还是右取决于 sideDirectionLeft
	float marginx = 100.f;
	//雷达与屏幕垂直边缘的距离 具体是上还是下取决于 sideDirectionTop
	float marginy = 100.f;
	//是否靠向屏幕左边,false表示靠向屏幕右边
	bool sideDirectionLeft = false;
	//是否靠向屏幕上边,false表示靠向屏幕下边
	bool sideDirectionTop = false;

	// 渲染移除的玩家
	bool renderRemovePlayer = false;
	// 直接屏幕透视
	bool xRay = false;
public:
	virtual auto onImGUIRender() -> void override;
	virtual auto onstartLeaveGame(class Level*) -> void override;
	virtual auto onPlayerTick(class Player*)->void override;		// 远程玩家tick不好使
	virtual auto onDimensionChanged(class ClientInstance*) -> void override;
	virtual auto onLevelTick(Level* level) -> void override;
	virtual auto onloadConfigFile(json& data) -> void override;
	virtual auto onsaveConfigFile(json& data) -> void override;

private:
	vec2_t getMapPosition(vec3_t xdpos, vec2_t lprot);
	//获取字符串中的第n个 单个字符
	auto GetColorbyChar(const std::string&) -> ImColor;
};