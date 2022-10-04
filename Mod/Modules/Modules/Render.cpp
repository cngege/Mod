#include "Render.h"
#include "../ModuleManager.h"
#include "../../Utils/Game.h"
#include "imgui.h"

Render::Render() : Module(VK_INSERT, "Render", "渲染UI管理器") {
	
	SetKeyMode(KeyMode::Switch);
	setEnabled(true);
}

auto Render::onRenderDetour(MinecraftUIRenderContext* ctx)->void {
	if (isEnabled()) {
		//画面板 功能列表
		UIColor bgcolor = UIColor(0,0,0,80);
		UIColor textcolor1 = UIColor(255, 255, 255);
		UIColor textcolor2 = UIColor(160, 160, 160);

		float ltX = x + 5.f;
		float ltY = y + 5.f;						//绘制文字左上角Y值的位置
		float textlineheight = 8.f * fontsize;		//下一行的偏移量

		auto mcount = Game::GetModuleManager()->GetAllModule().size();

		ctx->Fillshape(vec2_t(x, ltY - 5.f), vec2_t(200.f, mcount* (textlineheight + 5.f) + 5.f), bgcolor);
		
		for (auto pMod : Game::GetModuleManager()->GetAllModule()) {
			if (pMod->GetKeyMode() == KeyMode::Switch) {
				// 如果这个模块是有开关状态的切换模式
				std::string showText = pMod->getModuleName() + " ";
				if (pMod->getBindKeyName() != "") {
					showText += "[" + pMod->getBindKeyName() + "]  ";
				}
				showText += pMod->isEnabled() ? "[ON]" : "[OFF]";
				ctx->Drawtext(vec2_t(ltX, ltY), &showText, pMod->isEnabled()? textcolor1 : textcolor2, fontsize);
				ltY += textlineheight + 5.f;
			}
			else if (pMod->GetKeyMode() == KeyMode::Trigger) {
				// 如果这个模块是点击快捷键触发的
				std::string showText = pMod->getModuleName() + " ";
				if (pMod->getBindKeyName() != "") {
					showText += "[" + pMod->getBindKeyName() + "]  ";
				}
				showText += "[Trigger]";
				ctx->Drawtext(vec2_t(ltX, ltY), &showText, textcolor2, fontsize);
				ltY += textlineheight + 5.f;
			}
			else if (pMod->GetKeyMode() == KeyMode::Hold) {
				// 如果这个模块是长按开启，松开关闭模式
				std::string showText = pMod->getModuleName() + " ";
				if (pMod->getBindKeyName() != "") {
					showText += "[" + pMod->getBindKeyName() + "]  ";
				}
				showText += pMod->isEnabled() ? "[HoldON]" : "[HoldOFF]";
				ctx->Drawtext(vec2_t(ltX, ltY), &showText, pMod->isEnabled() ? textcolor1 : textcolor2, fontsize);
				ltY += textlineheight + 5.f;
			}
		}
		ctx->flushText(0);
	}
}

auto Render::onImGUIRender()->void {
	if (!isEnabled()) {
		return;
	}
	if (ImGui::Begin("Gui")) {
		ImGui::SetWindowSize(ImVec2(360.f, 430.f));
		if (ImGui::CollapsingHeader("Visuals")) {
			ImGui::Spacing();
			if (ImGui::Button("Test")) {
			}
			ImGui::Toggle("Toggle Snow D3D11", &ImGui::doSnow);
			ImGui::ButtonScrollable("Button Scrollable", ImVec2(300.f, 0.f));
			//ImGui::ButtonScrollable("Button Scrollable that fits in button size", ImVec2(350.f, 0.f));
			ImGui::ButtonScrollableEx("Button Scrollable (Right-click only!)", ImVec2(355.f, 0.f), ImGuiButtonFlags_MouseButtonRight);
			ImGui::Spacing();
		}
		if (ImGui::CollapsingHeader(("Aura"))) {
			ImGui::Spacing();
			if (ImGui::Button("Test")) {
			}
			ImGui::Spacing();
		}
		if (ImGui::CollapsingHeader(("Client"))) {
			ImGui::Spacing();
			if (ImGui::Button("Test")) {
			}
			ImGui::Spacing();
		}
		if (ImGui::CollapsingHeader(("Exploits"))) {
			ImGui::Spacing();
			if (ImGui::Button("中文")) {
			}
			ImGui::Spacing();
		}
	}
	ImGui::End();
}