#pragma execution_character_set("utf-8")
#include "Debug.h"
#include "imgui.h"
#include "../../imgui/toggle/imgui_toggle.h"
#include "../../imgui/toggle/imgui_toggle_presets.h"

ImGuiToggleConfig toggerConfig_Debug;


Debug::Debug() : Module(0, "Debug", "开发者调试") {
	setEnabled(true);
	toggerConfig_Debug = ImGuiTogglePresets::RectangleStyle();
}

auto Debug::onImGUIRender() -> void
{
	static bool ShowDemoWindow = false;
	static bool ShowFontSelector = false;
	if (!isEnabled())
		return;
	if (ImGui::Begin("Debug窗口")) {
		ImGui::Toggle("显示Demo窗口", &ShowDemoWindow, toggerConfig_Debug);
		ImGui::Toggle("显示字体选择", &ShowFontSelector, toggerConfig_Debug);
	}
	ImGui::End();

	if (ShowDemoWindow) {
		ImGui::ShowDemoWindow();
	}
	if (ShowFontSelector) {
		ImGui::ShowFontSelector("字体选择");
	}
}
