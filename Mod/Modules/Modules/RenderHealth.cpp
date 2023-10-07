#include "RenderHealth.h"
#include "Actor.h"
//#include "MinecraftUIRenderContext.h"
//#include "mcstring.h"
#include "imgui.h"
#include "Game.h"

#include <string.h>

RenderHealth::RenderHealth() : Module(VK_F10, "RenderHealth", "显示被攻击生物的血量和名字") {
	setcontrolkeysbind({ VK_CONTROL });
	//setEnabled(true);
	AddFloatUIValue("UI显示时长", &actorTime, 1.f, 15.f, true, 0.1f);
	AddBoolUIValue("玩家名字渲染颜色(暂不可用)", &renderColor);
}


auto RenderHealth::onAttackAfter(class GameMode* gm,Actor* actor)->void {
	if (isEnabled()) {
		currentPlayerHealth = actor->getHealth();
		std::string sname = actor->getNameTag()->to_string();
		auto find = sname.find("\n");
		if (find == -1) {
			currentPlayerName = sname;
		}
		else {
			currentPlayerName = sname.substr(0, find);
		}
		if (currentPlayerName.empty()) {
			currentPlayerTypeName = actor->getTypeName()->c_str();
		}
		showtime = Utils::GetCuttentMillisecond() + actorTime * 1000;
		//tick = 400.f;
		//show = true;
	}
	return;
}


auto RenderHealth::onImGUIRender() -> void
{
	if (isEnabled() && showtime) {
		
		float endTime = static_cast<float>(*showtime - Utils::GetCuttentMillisecond());
		if (endTime < 0.f) {
			showtime.reset();
			return;
		}
		
		RECT rect{};
		if (::GetWindowRect((HWND)Game::WindowsHandle, (LPRECT)&rect))
		{
			auto drawList = ImGui::GetForegroundDrawList();
			float rectwidth = (float)(rect.right - rect.left);
			float rectheight = (float)(rect.bottom - rect.top);


			std::string drawName;
			if (currentPlayerName.empty()) {
				drawName += "生物类型: ";
				drawName += currentPlayerTypeName;
			}
			else {
				drawName += "生物名字: ";
				drawName += currentPlayerName;
			}

			std::string drawHealth("生物血量: ");
			drawHealth += std::to_string(currentPlayerHealth);
			//ImGui::CalcTextSize()
			
			
			// 计算宽高
			float bgWidth = max(ImGui::CalcTextSize(drawName.c_str()).x, ImGui::CalcTextSize(drawHealth.c_str()).x) + 10.f;	//150
			float bgHeight = 40;

			ImVec2 LTop = { rectwidth * 0.5f - bgWidth * 0.5f, rectheight * 0.75f };

			// 绘制
			drawList->AddRectFilled(LTop, { LTop.x + bgWidth,LTop.y + bgHeight }, ImColor(0, 0, 0, 100));

			// 计算UI剩余显示百分比
			float uiTimeLengthWidth = endTime / (actorTime * 1000) * bgWidth;
			drawList->AddRectFilled({ LTop.x, LTop.y - 2.f }, { LTop.x + uiTimeLengthWidth, LTop.y}, ImColor(194, 31, 48, 255));

			drawList->AddText({ LTop.x + 5.f, LTop.y + 5.f }, ImColor(255, 255, 255, 255), drawName.c_str());
			drawList->AddText({ LTop.x + 5.f, LTop.y + 20.f }, ImColor(255, 255, 255, 255), drawHealth.c_str());

		}
	}
}

//auto RenderHealth::onRenderDetour(MinecraftUIRenderContext* ctx)->void {
//	if (show && isEnabled()) {
//		tick--;
//		if (tick <= 0) {
//			show = false;
//			return;
//		}
//
//		RECT rect{};
//		vec2_t bg_wh = vec2_t(130.f, 35.f);
//		if (::GetWindowRect((HWND)ImGui::GetMainViewport()->PlatformHandleRaw, (LPRECT)&rect)) {
//			float rectwidth = (float)(rect.right - rect.left);
//			showpos.x = (rectwidth - bg_wh.x) / 4.f;
//			//showpos.x = 230.f;
//		}
//
//		UIColor bgcolor = UIColor(0, 0, 0, (tick < 60) ? tick : 60);
//		UIColor textcolor = UIColor(255, 255, 255, (tick < 255) ? tick : 255);
//		ctx->Fillshape(showpos, bg_wh, bgcolor);
//
//		std::string drawName("Name: ");
//		drawName += currentPlayerName;
//		std::string drawHealth("Health: ");
//		drawHealth += std::to_string(currentPlayerHealth);
//
//		ctx->Drawtext(vec2_t(showpos.x + 5.f, showpos.y + 5.f), &drawName, textcolor, 1.f);
//		ctx->Drawtext(vec2_t(showpos.x + 5.f, showpos.y + 20.f), &drawHealth, textcolor, 1.f);
//		ctx->flushText(0);
//	}
//}


auto RenderHealth::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", true));
	actorTime = config::readDataFromJson<float>(data, "UIactorTime", 5.f);
	renderColor = config::readDataFromJson<bool>(data, "renderColor", true);
}
auto RenderHealth::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
	data["UIactorTime"] = actorTime;
	data["renderColor"] = renderColor;
}