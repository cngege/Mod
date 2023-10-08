#include "TPPoint.h"
#include "../../Utils/Game.h"
#include "LocalPlayer.h"
#include "ClientInstance.h"

#include "imgui.h"

TPPoint::TPPoint() : Module(VK_F3, "TPPoint", "传送坐标点,由高到低当心摔落伤害") {
	SetKeyMode(KeyMode::Trigger);
}

auto TPPoint::onTrigger()->void {
	if (!Game::Cinstance) {
		return;
	}
	LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
	if (!lp) {
		return;
	}
	//记录传送点
	if (Game::IsKeyDown(VK_CONTROL)) {
		point = *lp->getPosition();
		return;
	}

	//读取并传送过去
	if (Game::IsKeyDown(VK_SHIFT) && point.has_value()) {
		//Game::localplayer->setPos(point);
		vec3_t tpPos = *point;
		lp->teleportTo(&tpPos,true,0,1);
	}
}

auto TPPoint::getBindKeyName()->std::string {
	//ret : CTRL + SHIFT + F
	std::string name = Utils::getKeybindName(VK_SHIFT);
	name += "/";
	name += Utils::getKeybindName(VK_CONTROL);
	name += "+";
	name += Module::getBindKeyName();
	return name;
}

auto TPPoint::onInternalImGUIRender() -> void
{
	if (point.has_value()) {
		ImGui::Text("快捷键传送点: x:%.3f, y:%.3f, z:%.3f", point->x, point->y, point->z);
	}
	else {
		ImGui::Text("未记录传送点");
	}

	if (ImGui::Button("添加", { -1, 0 })) {
		ImGui::OpenPopup("AddTpPointMenu");
	}
	ImGui::Separator();

	if (ImGui::BeginPopup("AddTpPointMenu")) {
		static char name[30] = { '\0' };
		ImGui::InputTextWithHint("传送点名称", "HOME", name, IM_ARRAYSIZE(name));

		if (ImGui::Button("添加")) {
			auto lp = Game::Cinstance->getCILocalPlayer();
			if (lp) {
				std::string tppointlistname = name;

				TpPointList pointlist{ tppointlistname, {0} , 0};

				tpPoints.push_back(pointlist);

				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}

	for (auto i = tpPoints.begin(); i != tpPoints.end(); i++) {
		auto& list = (*i);
		//ImGui::Text(list.name.c_str());
		ImGui::PushID(list.name.c_str());
		{
			float* pos = list.point;
			ImGui::DragFloat3(list.name.c_str(), pos, 0.1f);
			{
				if (ImGui::Button("读取")) {
					if (!Game::Cinstance) {
						return;
					}
					LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
					if (!lp) {
						return;
					}
					vec3_t lppos = *lp->getPosition();
					pos[0] = lppos.x;
					pos[1] = lppos.y;
					pos[2] = lppos.z;
				}
			}
			ImGui::SameLine();
			{
				if (ImGui::Button("传送")) {
					if (!Game::Cinstance) {
						return;
					}
					LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
					if (!lp) {
						return;
					}
					vec3_t lppos(pos[0], pos[1], pos[2]);
					lp->teleportTo(&lppos, true, 0, 1);
				}
			}
			ImGui::SameLine();
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
				if (ImGui::Button("删除")) {
					i = tpPoints.erase(i);
					if (i == tpPoints.end())
					{
						ImGui::PopStyleColor(3);
						ImGui::PopID();
						break;
					}
				}
				ImGui::PopStyleColor(3);
			}
			ImGui::Separator();
		}
		ImGui::PopID();
	}
}

auto TPPoint::onloadConfigFile(json& data)->void {
	//setEnabled(config::readDataFromJson<bool>(data, "enable", true));
	// 遍历 JSON 数组并输出每个元素
	if(!data["tppoints"].is_array())
		data["tppoints"].array();
	tpPoints.clear();
	for (const auto& item : data["tppoints"]) {
		TpPointList list{};
		list.name = item["name"].get<std::string>();
		list.point[0] = item["position"]["x"].get<float>();
		list.point[1] = item["position"]["y"].get<float>();
		list.point[2] = item["position"]["z"].get<float>();
		list.dim = item["dim"].get<int>();
		tpPoints.push_back(list);
	}
	
}
auto TPPoint::onsaveConfigFile(json& data)->void {
	//data["enable"] = isEnabled();
	//data["tppoints"]
	data["tppoints"].clear();
	for (auto pos : tpPoints) {
		json current_json;
		current_json["name"] = pos.name;
		current_json["position"]["x"] = pos.point[0];
		current_json["position"]["y"] = pos.point[1];
		current_json["position"]["z"] = pos.point[2];
		current_json["dim"] = pos.dim;

		data["tppoints"].push_back(current_json);
	}
}