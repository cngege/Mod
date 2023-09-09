#include "RenderHealth.h"
#include "Actor.h"
#include "MinecraftUIRenderContext.h"
#include "imgui.h"
#include <string.h>

RenderHealth::RenderHealth() : Module(VK_F10, "RenderHealth", "显示被攻击生物的血量和名字") {
	setcontrolkeysbind({ VK_CONTROL });
	setEnabled(true);
}


auto RenderHealth::onAttack(Actor* actor)->bool {
	if (isEnabled()) {
		currentPlayerHealth = (int)actor->getHealth();
		std::string sname = std::string(actor->getNameTag()->getText());
		auto find = sname.find("\n");
		if (find == -1) {
			currentPlayerName = std::string(actor->getNameTag()->getText());
		}
		else {
			currentPlayerName = sname.substr(0, find);
		}

		tick = 400.f;
		show = true;
	}
	return true;
}


auto RenderHealth::onImGUIRender() -> void
{
	if (show && isEnabled()) {
		tick--;
		if (tick <= 0) {
			show = false;
			return;
		}
		RECT rect{};
		if (::GetWindowRect((HWND)Game::WindowsHandle, (LPRECT)&rect))
		{
			auto drawList = ImGui::GetForegroundDrawList();
			float rectwidth = (float)(rect.right - rect.left);
			float rectheight = (float)(rect.bottom - rect.top);

			float bgWidth = 150;
			float bgHeight = 40;

			ImVec2 LTop = { rectwidth * 0.5f - bgWidth * 0.5f, rectheight * 0.75f };

			drawList->AddRectFilled(LTop, { LTop.x + bgWidth,LTop.y + bgHeight }, ImColor(0, 0, 0, 100));
			
			std::string drawName("Name: ");
			drawName += currentPlayerName;
			std::string drawHealth("Health: ");
			drawHealth += std::to_string(currentPlayerHealth);
			
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
}
auto RenderHealth::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}