#pragma execution_character_set("utf-8")
#include "Debug.h"
#include "imgui.h"

Debug::Debug() : Module(0, "Debug", "开发者调试") {
	setEnabled(true);
}

auto Debug::onImGUIRender() -> void
{
	if (!isEnabled())
		return;
	if (ImGui::Begin("Debug窗口")) {

	}
	ImGui::End();
	ImGui::ShowDemoWindow();
}
