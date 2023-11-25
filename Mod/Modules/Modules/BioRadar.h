#pragma once
#include "../Module.h"


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
	virtual auto onPlayerTick(class Player*)->void override;		// 远程玩家tick不好使
	virtual auto onDimensionChanged(class ClientInstance*) -> void override;
	virtual auto onloadConfigFile(json& data) -> void override;
	virtual auto onsaveConfigFile(json& data) -> void override;

private:

	void renderMapBg(struct ImVec2 radarLeftTop);

	/**
	 * @brief 处理列表中的玩家绘制,或移除
	 * @param lpPos 本地玩家位置
	 * @param lpRot 本地玩家视角
	 * @param radarLeftTop 地图的左上角位置
	 * @return 
	*/
	void drawListPlayer(vec3_t lpPos, vec2_t lpRot, struct ImVec2 radarLeftTop);

	struct ImVec2 getRadarLeftTop(vec2_t screen);

	/**
	 * @brief 根据玩家向量和玩家视角计算该玩家应该在地图上显示的位置
	 * @param xdpos 本地玩家到目标玩家的向量
	 * @param lprot 本地玩家的视角
	 * @return 应该在小地图上显示的位置
	*/
	vec2_t getMapPosition(vec3_t xdpos, vec2_t lprot);
};