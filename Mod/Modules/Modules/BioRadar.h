#pragma once
#pragma execution_character_set("utf-8")
#include "../Module.h"
#include "imgui.h"

class BioRadar : public Module {
public:
	BioRadar();
private:
	struct PlayerMapInfo;
	std::unordered_map<Player*, PlayerMapInfo> playerlist;

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
public:
	virtual auto onImGUIRender() -> void override;
	virtual auto onstartLeaveGame(class Level*) -> void override;
	virtual auto onRemotePlayerTick(class RemotePlayer*)->void override;
	virtual auto onloadConfigFile(json& data) -> void override;
	virtual auto onsaveConfigFile(json& data) -> void override;

private:
	//获取字符串中的第n个 单个字符
	auto GetColorbyChar(const std::string&) -> ImColor;
};