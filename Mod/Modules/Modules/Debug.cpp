#include "Debug.h"
#include "imgui.h"
#include "../../imgui/toggle/imgui_toggle.h"
#include "../../imgui/toggle/imgui_toggle_presets.h"

#include "Game.h"
#include "HMath.h"
#include "ClientInstance.h"

#include "Level.h"
#include "LocalPlayer.h"
#include "Logger.h"

ImGuiToggleConfig toggerConfig_Debug;

bool ShowDemoForm = false;
bool ShowFontSelectForm = false;

Debug::Debug() : Module(0, "Debug", "开发者调试") {
	toggerConfig_Debug = ImGuiTogglePresets::RectangleStyle();

	AddBoolUIValue("显示Demo窗口", &ShowDemoForm);
	AddBoolUIValue("显示字体选择", &ShowFontSelectForm);


	AddButtonUIEvent("Debug -", false, [&]() {
		LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
		if (lp != nullptr) {
			auto count = lp->getLevel()->getAllPlayer().size();
			logF("all player num: %d", count);
		}
		});

}

auto Debug::onImGUIRender() -> void
{
	if (!isEnabled())
		return;
	/*
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.45f, 0.45f, 0.45f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.65f, 0.65f, 0.65f, 1.0f));
	if (ImGui::Begin("Debug窗口")) {
		ImGui::Toggle("显示Demo窗口", &ShowDemoWindow, toggerConfig_Debug);
		ImGui::Toggle("显示字体选择", &ShowFontSelector, toggerConfig_Debug);
	}
	ImGui::End();
	ImGui::PopStyleColor(2);
	*/
	if (ShowDemoForm) {
		ImGui::ShowDemoWindow();
	}
	if (ShowFontSelectForm) {
		ImGui::ShowFontSelector("字体选择");
	}



	// 尝试世界到屏幕
	LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
	if (lp) {
		// 获取屏幕宽高
		RECT rect{};
		::GetWindowRect((HWND)Game::WindowsHandle, (LPRECT)&rect);
		float rectwidth = (float)(rect.right - rect.left);
		float rectheight = (float)(rect.bottom - rect.top);

		// 画线 屏幕到00 00 00
		vec3_t out = Game::Cinstance->getGlmatrixf()->WorldToScreen({ 0,0,0 }, rectwidth, rectheight);
		auto drawList = ImGui::GetForegroundDrawList();
		drawList->AddLine({ rectwidth / 2, 0 }, { out.x,out.y }, ImColor(30, 132, 73, 255));

	}

}

auto Debug::onloadConfigFile(json& data)->void {
	//setEnabled(config::readDataFromJson<bool>(data, "enable", false));
}

auto Debug::onsaveConfigFile(json& data)->void {
	//data["enable"] = isEnabled();
}
