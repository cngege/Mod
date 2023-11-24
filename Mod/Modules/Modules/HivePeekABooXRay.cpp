#include "HivePeekABooXRay.h"
#include <mutex>

#include "AABB.h"
#include "ClientInstance.h"
#include "LocalPlayer.h"
#include "BlockSource.h"
#include "Dimension.h"
#include "Block.h"
#include "BlockLegacy.h"
#include "Game.h"
#include "../Render/Render.h"

std::mutex removePlayer_mutex2;

HivePeekABooXRay::HivePeekABooXRay() : Module(VK_F6, "HivePeekABooXRay", "TheHive躲猫猫专用透视") {
	setcontrolkeysbind({ VK_CONTROL });
	AddBoolUIValue("从玩家名称中计算颜色", &colorFromName);
	AddBoolUIValue("启用中心线", &hasLine);
	AddBoolUIValue("中心线的颜色跟随玩家", &lineColorFromPlayer);
	//Color
	AddColorUIValue("玩家的方块颜色", &playerBoxColor);
	AddColorUIValue("玩家的中心线颜色", &playerLineColor);
	AddColorUIValue("玩家变成的方块颜色", &blockBoxColor);
	AddColorUIValue("方块的中心线颜色", &blockLineColor);

	AddButtonUIEvent("清除", false, [this]() { playerlist.clear(); std::lock_guard<std::mutex> guard(removePlayer_mutex2); removePlyaerList.clear(); });
}


struct HivePeekABooXRay::PlayerMapInfo {
	ImColor color;						//应该显示的颜色
	vec3_t pos;
	vec3_ti footPos;
	AABB aabb;
};

auto HivePeekABooXRay::onImGUIRender() -> void
{
	if (!isEnabled()) return;
	auto drawList = ImGui::GetForegroundDrawList();
	float rectwidth = Render::getScreen().x;
	float rectheight = Render::getScreen().y;

	for (auto& kv : playerlist) {

		if (kv.first->isValid()) {
			if (!kv.first->isRemovedEx()) {
				ImColor boxColor = kv.second.color;
				if (!colorFromName) {
					boxColor = playerBoxColor;
				}
				auto centerPos = Render::RenderAABB(kv.second.aabb, boxColor);
				if (centerPos && hasLine) {
					ImColor lineColor = kv.second.color;
					if (!lineColorFromPlayer) {
						lineColor = playerLineColor;
					}
					drawList->AddLine({ rectwidth / 2, rectheight / 2 }, { centerPos->x,centerPos->y }, lineColor);
				}
			}
			else {
				std::lock_guard<std::mutex> guard(removePlayer_mutex2);
				PlayerMapInfo removePlayer = kv.second;
				removePlyaerList.push_back(removePlayer);
				playerlist.erase(kv.first);
				break;
			}
		}
		else {
			playerlist.erase(kv.first);
			break;
		}
	}

	// 绘制变成方块的玩家
	std::lock_guard<std::mutex> guard(removePlayer_mutex2);
	for (auto iter = removePlyaerList.begin(); iter != removePlyaerList.end(); ++iter) {
		std::optional<vec2_t> centerPos = Render::RenderBlockBox((*iter).footPos, blockBoxColor);
		if (centerPos && hasLine) {
			ImColor lineColor = blockLineColor;
			if (lineColorFromPlayer) {
				lineColor = blockBoxColor;
			}
			drawList->AddLine({ rectwidth / 2, rectheight / 2 }, { centerPos->x,centerPos->y }, blockLineColor, 1.5f);
		}
	}
}

auto HivePeekABooXRay::onstartLeaveGame(Level*) -> void
{
	playerlist.clear();

	std::lock_guard<std::mutex> guard(removePlayer_mutex2);
	removePlyaerList.clear();
}

auto HivePeekABooXRay::onPlayerTick(Player* player) -> void
{
	if (!isEnabled()) {
		return;
	}
	if (!Game::Cinstance) {
		return;
	}
	if (player->isLocalPlayer()) {
		return;
	}
	LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();

	if (lp && lp->isValid()) {
		//获得本地玩家的位置视角相关信息
		//vec3_t* lpos = lp->getPosition();
		//vec2_t* lrot = lp->getRotationEx();
		vec3_t* pos = player->getPosition();
		//获得 对方玩家对本地玩家的相对位置 即本地玩家对远程玩家的空间向量
		//vec3_t xdpos = pos->sub(*lpos);

		PlayerMapInfo pmi;
		pmi.pos = *pos;
		pmi.footPos = player->getFootPos();
		pmi.aabb = *player->getAABB();
		//pmi.x = mappos.x; pmi.z = mappos.y;

		auto name = player->getNameTag()->to_string().substr(0, 3);  //章节号占两字节
		pmi.color = Utils::GetColorbyChar(name);

		playerlist[player] = pmi;
	}
}

auto HivePeekABooXRay::onDimensionChanged(ClientInstance* ci) -> void
{
	playerlist.clear();
	std::lock_guard<std::mutex> guard(removePlayer_mutex2);
	removePlyaerList.clear();
}

auto HivePeekABooXRay::onLevelTick(Level* level) -> void
{
	if (!isEnabled()) return;
	if (!Game::Cinstance) return;
	auto lp = Game::Cinstance->getCILocalPlayer();
	if (!lp) return;

	for (auto iter = removePlyaerList.begin(); iter != removePlyaerList.end(); ++iter)
	{
		auto dim = lp->getDimensionConst();
		if (!dim) continue;
		BlockSource* bs = dim->getBlockSourceEx();
		if (!bs) continue;
		vec3_ti playerBlock = (*iter).footPos;
		if (bs->getBlock(&playerBlock)->isAir()) {
			std::lock_guard<std::mutex> guard(removePlayer_mutex2);
			removePlyaerList.erase(iter);
			break;
		}
	}
}

auto HivePeekABooXRay::onloadConfigFile(json& data) -> void
{
	setEnabled(config::readDataFromJson<bool>(data, "enable", false));
}

auto HivePeekABooXRay::onsaveConfigFile(json& data) -> void
{
	data["enable"] = isEnabled();
}

