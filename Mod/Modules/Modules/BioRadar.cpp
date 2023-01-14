#include "BioRadar.h"
#include "imgui.h"
#include "Game.h"
#include <cmath>

#include "LocalPlayer.h"
#include "RemotePlayer.h"
#include "Level.h"
#include "TextHolder.h"

BioRadar::BioRadar() : Module(VK_F6, "BioRadar", "生物雷达-可在雷达显示屏上看到玩家和其他生物位置信息") {
	setcontrolkeysbind({ VK_SHIFT });
	AddFloatUIValue("地图大小", &radarSide, 200, 1000, true);						//雷达地图边长 像素
	AddIntUIValue("地图比例尺", &roomscale, 2, 6, true);					//雷达像素与游戏中距离的比例，比如2表示  雷达中2像素表示游戏中的一格
	AddFloatUIValue("屏幕水平边距", &marginx, 0, 1000, true);
	AddFloatUIValue("屏幕垂直边距", &marginy, 0, 1000, true);

	AddButtonUIEvent("靠左", false, [this]() { this->sideDirectionLeft = true; });
	AddButtonUIEvent("靠右", true, [this]() { this->sideDirectionLeft = false; });
	AddButtonUIEvent("靠上", true, [this]() { this->sideDirectionTop = true; });
	AddButtonUIEvent("靠下", true, [this]() { this->sideDirectionTop = false; });
}

struct BioRadar::PlayerMapInfo{
	float x;							//计算后的 该玩家应该显示在的位置
	float z;
	ImColor color;						//应该显示的颜色
	bool top;							//是否在本地玩家上面
	//int updatetick;
};

auto BioRadar::onImGUIRender() -> void
{
	if (!isEnabled()) {
		return;
	}
	if (!Game::localplayer->isValid()) {
		return;
	}
	RECT rect{};
	if (::GetWindowRect((HWND)ImGui::GetMainViewport()->PlatformHandleRaw, (LPRECT)&rect)) {
		auto drawList = ImGui::GetForegroundDrawList();
		float rectwidth = (float)(rect.right - rect.left);
		float rectheight = (float)(rect.bottom - rect.top);
		//计算 靠向不同边的时候 雷达左上角的不同基础值
		ImVec2 radarLeftTop(0,0);
		if (sideDirectionLeft) {
			radarLeftTop.x = marginx;
			//左上
			if (sideDirectionTop) {
				radarLeftTop.y = marginy;
			}
			//左下
			else {
				radarLeftTop.y = rectheight - marginy - radarSide/*地图宽*/;
			}
		}
		else {
			radarLeftTop.x = rectwidth - marginx - radarSide;
			//右上
			if (sideDirectionTop) {
				radarLeftTop.y = marginy;
			}
			//右下
			else {
				radarLeftTop.y = rectheight - marginy - radarSide/*地图宽*/;
			}
		}
		drawList->AddRectFilled(radarLeftTop, { radarLeftTop.x + radarSide,radarLeftTop.y + radarSide }, ImColor(0, 0, 0, 100));
		drawList->AddLine({ radarLeftTop.x ,radarLeftTop.y + radarSide / 2}, { radarLeftTop.x + radarSide , radarLeftTop.y + radarSide / 2 }, ImColor(102, 153, 255, 200));
		drawList->AddLine({ radarLeftTop.x + radarSide / 2 ,radarLeftTop.y }, { radarLeftTop.x + radarSide / 2 , radarLeftTop.y + radarSide }, ImColor(102, 153, 255, 200));
		for (auto& kv : playerlist) {
			if (kv.first->isValid()) {
				if (!kv.first->isRemovedEx()) {
					//防止玩家点跑出地图外
					float remoteside = radarSide / (2 * roomscale);
					float X = kv.second.x; if (X > remoteside) { X = remoteside; } if (X < -remoteside) { X = -remoteside; }
					float Z = kv.second.z; if (Z > remoteside) { Z = remoteside; } if (Z < -remoteside) { Z = -remoteside; }
					//在远程玩家位置在本地玩家之上时显示空心圆
					if (kv.second.top) {
						drawList->AddCircle({ radarLeftTop.x + (radarSide / 2) + X * roomscale, radarLeftTop.y + (radarSide / 2) + Z * roomscale }, 4, kv.second.color);
					}
					else {
						drawList->AddCircleFilled({ radarLeftTop.x + (radarSide / 2) + X * roomscale, radarLeftTop.y + (radarSide / 2) + Z * roomscale }, 4, kv.second.color);
					}
				}
				else {
					playerlist.erase(kv.first);
					break;
				}
			}
			else {
				playerlist.erase(kv.first);
				break;
			}
		}
	}
}

auto BioRadar::onstartLeaveGame(Level* _) -> void
{
	playerlist.clear();
}

auto BioRadar::onRemotePlayerTick(RemotePlayer* remotePlayer)->void
{
	if (!isEnabled()) {
		return;
	}
	if (Game::localplayer->isValid() && !remotePlayer->isLocalPlayer()) {
		//获得本地玩家的位置视角相关信息
		vec3_t* lpos = Game::localplayer->getPosition();
		vec2_t* lrot = Game::localplayer->getRotationEx();
		//获得 对方玩家对本地玩家的相对位置 即本地玩家对远程玩家的空间向量
		vec3_t xdpos = remotePlayer->getPosition()->sub(*lpos);
		//获取向量长度 也就是斜边长度
		float vecLength = xdpos.magnitudexz();
		//获取与原版等同的夹角
		float deg = atan2f(-xdpos.x, xdpos.z) * 180 / PI;
		//本地玩家转动视角后 计算远程玩家的视角 得到计算后的视角度数
		float afterdeg = deg - lrot->y;
		if (afterdeg > 180.f) {
			afterdeg = afterdeg - 360.f;				// = -180.f + (afterdeg - 180.f)
		}
		else if (afterdeg < -180.f) {
			afterdeg = afterdeg + 360.f;				// = 180.f + (afterdeg - (-180.f))
		}
		//斜边有了，夹角有了 然后算出新的x z值
		float x = vecLength * sinf(afterdeg * PI / 180.f);
		float z = -vecLength * cosf(afterdeg * PI / 180.f);
		PlayerMapInfo pmi;
		pmi.x = x; pmi.z = z;

		auto name = std::string(remotePlayer->getNameTag()->getText()).substr(0,3);  //章节号占两字节
		pmi.color = GetColorbyChar(name);
		pmi.top = xdpos.y > 0;
		//pmi.updatetick = 0;

		playerlist[remotePlayer] = pmi;
	}
}

auto BioRadar::onloadConfigFile(json& data) -> void
{
	setEnabled(config::readDataFromJson<bool>(data, "enable", false));
	radarSide = config::readDataFromJson<float>(data, "radarSide", 200.f);
	roomscale = config::readDataFromJson<int>(data, "roomscale", 2);
	marginx = config::readDataFromJson<float>(data, "marginx", 100.f);
	marginy = config::readDataFromJson<float>(data, "marginy", 100.f);
	sideDirectionLeft = config::readDataFromJson<bool>(data, "sideDirectionLeft", false);
	sideDirectionTop = config::readDataFromJson<bool>(data, "sideDirectionTop", false);
}

auto BioRadar::onsaveConfigFile(json& data) -> void
{
	data["enable"] = isEnabled();
	data["radarSide"] = radarSide;
	data["roomscale"] = roomscale;
	data["marginx"] = marginx;
	data["marginy"] = marginy;
	data["sideDirectionLeft"] = sideDirectionLeft;
	data["sideDirectionTop"] = sideDirectionTop;
}

auto BioRadar::GetColorbyChar(const std::string& colorchar)-> ImColor
{
	if (colorchar == "§0") {
		return ImColor(0, 0, 0, 255);
	}
	else if (colorchar == "§1") {
		return ImColor(51, 0, 255, 255);
	}
	else if (colorchar == "§2") {
		return ImColor(0, 153, 0, 255);
	}
	else if (colorchar == "§3") {
		return ImColor(102, 153, 153, 255);
	}
	else if (colorchar == "§4") {
		return ImColor(204, 0, 0, 255);
	}
	else if (colorchar == "§5") {
		return ImColor(153, 0, 204, 255);
	}
	else if (colorchar == "§6") {
		return ImColor(255, 127, 0, 255);
	}
	else if (colorchar == "§7") {
		return ImColor(204, 204, 204, 255);
	}
	else if (colorchar == "§8") {
		return ImColor(153, 153, 153, 255);
	}
	else if (colorchar == "§9") {
		return ImColor(102, 102, 255, 255);
	}
	else if (colorchar == "§a") {	//#33FF00
		return ImColor(51, 255, 0, 255);
	}
	else if (colorchar == "§b") {
		return ImColor(0, 255, 255, 255);
	}
	else if (colorchar == "§c") {
		return ImColor(255, 102, 102, 255);
	}
	else if (colorchar == "§d") {
		return ImColor(255, 0, 255, 255);
	}
	else if (colorchar == "§e") {
		return ImColor(255, 255, 0, 255);
	}
	else {
		return ImColor(255, 255, 255, 255);
	}
}
