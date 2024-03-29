﻿#include "FastViewPerspective.h"
#include "../../Utils/Game.h"
#include "../../Utils/Utils.h"

#include "imgui.h"

#include "GameMode.h"
#include "LocalPlayer.h"


FastViewPerspective::FastViewPerspective() : Module(VK_F9, "FastViewPerspective", "快速预览第二人称视角") {
	//setEnabled(true);
	AddBoolUIValue("切换人称视角时隐藏自己", &hide);
}

auto FastViewPerspective::getBindKeyName()->std::string {
	//ret : CTRL + SHIFT + F
	std::string name = "(鼠标后退键) ";
	name += Module::getBindKeyName();
	return name;
}

auto FastViewPerspective::onInternalImGUIRender() -> void
{
	ImGui::Text("选择该功能触发时展示的人称视角");
	ImGui::RadioButton("第一人称", &ViewPerspective, 0);
	ImGui::SameLine();
	ImGui::RadioButton("第二人称", &ViewPerspective, 1);
	ImGui::SameLine();
	ImGui::RadioButton("第三人称", &ViewPerspective, 2);

}


auto FastViewPerspective::isToggle()->bool {
	return Game::IsMouseDown(VK_XBUTTON1) && !Utils::isCursorVisible();
}

auto FastViewPerspective::getViewPerspective(int source) -> int
{
	if (isToggle()) {
		return ViewPerspective;
	}
	return source;
}

auto FastViewPerspective::Hide() -> bool
{
	return hide;
}

auto FastViewPerspective::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", true));
	ViewPerspective = config::readDataFromJson<int>(data, "ViewPerspective", 2);
	hide = config::readDataFromJson<bool>(data, "Hide", false);
}
auto FastViewPerspective::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
	data["ViewPerspective"] = ViewPerspective;
	data["Hide"] = hide;
}
