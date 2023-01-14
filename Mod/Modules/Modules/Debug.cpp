#include "Debug.h"
#include "imgui.h"
#include "../../imgui/toggle/imgui_toggle.h"
#include "../../imgui/toggle/imgui_toggle_presets.h"

ImGuiToggleConfig toggerConfig_Debug;


Debug::Debug() : Module(0, "Debug", "开发者调试") {
	toggerConfig_Debug = ImGuiTogglePresets::RectangleStyle();
}

auto Debug::onImGUIRender() -> void
{
	static bool ShowDemoWindow = false;
	static bool ShowFontSelector = false;
	if (!isEnabled())
		return;
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.45f, 0.45f, 0.45f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.65f, 0.65f, 0.65f, 1.0f));
	if (ImGui::Begin("Debug窗口")) {
		ImGui::Toggle("显示Demo窗口", &ShowDemoWindow, toggerConfig_Debug);
		ImGui::Toggle("显示字体选择", &ShowFontSelector, toggerConfig_Debug);
	}
	ImGui::End();
	ImGui::PopStyleColor(2);

	if (ShowDemoWindow) {
		ImGui::ShowDemoWindow();
	}
	if (ShowFontSelector) {
		ImGui::ShowFontSelector("字体选择");
	}
}

auto Debug::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", false));
}

auto Debug::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}
