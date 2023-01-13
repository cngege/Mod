#pragma execution_character_set("utf-8")
#include "Render.h"
#include <string>
#include "../ModuleManager.h"
#include "../../Utils/Game.h"
#include "../../Loader.h"
#include "../../Utils/Utils.h"
#include "../../Utils/config.h"
#include "imgui.h"
#include "../../imgui/toggle/imgui_toggle.h"
#include "..\..\imgui\toggle\imgui_toggle_presets.h"

ImGuiToggleConfig toggerConfig;


Render::Render() : Module(VK_INSERT, "Render", "渲染UI管理器") {
	SetKeyMode(KeyMode::Switch);
	toggerConfig = ImGuiTogglePresets::RectangleStyle();
}

auto Render::onRenderDetour(MinecraftUIRenderContext* ctx)->void {
	//return;
	//if (isEnabled()) {
	//	//画面板 功能列表
	//	UIColor bgcolor = UIColor(0,0,0,80);
	//	UIColor textcolor1 = UIColor(255, 255, 255);
	//	UIColor textcolor2 = UIColor(160, 160, 160);

	//	float ltX = x + 5.f;
	//	float ltY = y + 5.f;						//绘制文字左上角Y值的位置
	//	float textlineheight = 8.f * fontsize;		//下一行的偏移量

	//	auto mcount = Game::GetModuleManager()->GetAllModule().size();

	//	ctx->Fillshape(vec2_t(x, ltY - 5.f), vec2_t(200.f, mcount* (textlineheight + 5.f) + 5.f), bgcolor);
	//	
	//	for (auto pMod : Game::GetModuleManager()->GetAllModule()) {
	//		if (pMod->GetKeyMode() == KeyMode::Switch) {
	//			// 如果这个模块是有开关状态的切换模式
	//			std::string showText = pMod->getModuleName() + " ";
	//			if (pMod->getBindKeyName() != "") {
	//				showText += "[" + pMod->getBindKeyName() + "]  ";
	//			}
	//			showText += pMod->isEnabled() ? "[ON]" : "[OFF]";
	//			ctx->Drawtext(vec2_t(ltX, ltY), &showText, pMod->isEnabled()? textcolor1 : textcolor2, fontsize);
	//			ltY += textlineheight + 5.f;
	//		}
	//		else if (pMod->GetKeyMode() == KeyMode::Trigger) {
	//			// 如果这个模块是点击快捷键触发的
	//			std::string showText = pMod->getModuleName() + " ";
	//			if (pMod->getBindKeyName() != "") {
	//				showText += "[" + pMod->getBindKeyName() + "]  ";
	//			}
	//			showText += "[Trigger]";
	//			ctx->Drawtext(vec2_t(ltX, ltY), &showText, textcolor2, fontsize);
	//			ltY += textlineheight + 5.f;
	//		}
	//		else if (pMod->GetKeyMode() == KeyMode::Hold) {
	//			// 如果这个模块是长按开启，松开关闭模式
	//			std::string showText = pMod->getModuleName() + " ";
	//			if (pMod->getBindKeyName() != "") {
	//				showText += "[" + pMod->getBindKeyName() + "]  ";
	//			}
	//			showText += pMod->isEnabled() ? "[HoldON]" : "[HoldOFF]";
	//			ctx->Drawtext(vec2_t(ltX, ltY), &showText, pMod->isEnabled() ? textcolor1 : textcolor2, fontsize);
	//			ltY += textlineheight + 5.f;
	//		}
	//	}
	//	ctx->flushText(0);
	//}
}

auto Render::onImGUIRender()->void {
	if (!isEnabled()) {
		return;
	}
	if (ImGui::Begin("模组功能面板  所有Moudle状态")) {
		Game::GetModuleManager()->Moduleforeach([](Module* mod) {
			std::string showText = mod->getModuleName();		//加上Module本来的名称
			bool enable = mod->isEnabled();
			ImGui::PushID(mod->getModuleName().c_str());

			ImGui::Checkbox("##CheckBox", &enable);
			ImGui::SameLine();
			if (mod->getBindKeyName() != "") {
				showText += " [" + mod->getBindKeyName() + "]";
			}
			if (Utils::HelpCollapsingHeader(showText.c_str(), mod->getModuleInfo().c_str())) {
				ImGui::TextColored(ImVec4(0.98f, 0.63f, 0.01f, 1.f), mod->getModuleInfo().c_str());
				bool modIsEnable = mod->isEnabled();
				if (ImGui::Button(modIsEnable ? "点击关闭" : "点击开启")) {
					mod->setEnabled(!modIsEnable);
				}
				//IntUIValue
				for (auto &intval : mod->GetIntUIValue()) {
					if (intval.slider) {
						ImGui::SliderInt(intval.name.c_str(), intval.value, intval.min, intval.max);
					}
					else {
						ImGui::DragInt(intval.name.c_str(), intval.value, intval.speed, intval.min, intval.max);
					}
				}
				for (auto& floatval : mod->GetFloatUIValue()) {
					if (floatval.slider) {
						//如果对精度没有要求则使用滑块控制
						ImGui::SliderFloat(floatval.name.c_str(), floatval.value, floatval.min, floatval.max);
					}
					else {
						ImGui::DragFloat(floatval.name.c_str(), floatval.value, floatval.speed, floatval.min, floatval.max);
					}
				}
				for (auto& boolval : mod->GetBoolUIValue()) {
					ImGui::Toggle(boolval.name.c_str(), boolval.value, toggerConfig);
				}
				for (auto& buttonet : mod->GetButtonUIEvent()) {
					if (buttonet.sameline) {
						ImGui::SameLine();
					}
					if (ImGui::Button(buttonet.name.c_str())) {
						buttonet.et();
					}
				}

			}
			ImGui::PopID();
		});

		if (Utils::HelpCollapsingHeader("设置", "读取保存配置等")) {
			static char text[32] = { '\0' };
			if (strlen(text) == 0) {
				//memcpy(text, config::currentSaveConfigFile.c_str(), sizeof(config::currentSaveConfigFile.c_str()));
				snprintf(text, sizeof(text), config::currentSaveConfigFile.c_str());
			}
			ImGui::InputText("配置文件名", text, IM_ARRAYSIZE(text));
			if (ImGui::Button("加载使用配置")) {
				//调用所有模块 加载配置
				//将输入框中的配置文件名保存到config.json中
				json sysdata = config::loadConfigonRootFromFile("config");
				{
					config::currentSaveConfigFile = text;
					sysdata["CurrentConfigFile"] = config::currentSaveConfigFile;
				}
				config::writeConfigonRootToFile("config", sysdata);
				json configdata = config::loadConfigonRootFromFile(config::currentSaveConfigFile);
				for (auto& mod : Game::GetModuleManager()->GetAllModule()) {
					mod->onloadConfigFile(configdata[mod->getModuleName()]);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("保存覆盖配置")) {
				//调用所有模块保存配置
				//将输入框中的配置文件名保存到config.json中
				json sysdata = config::loadConfigonRootFromFile("config");
				{
					config::currentSaveConfigFile = text;
					sysdata["CurrentConfigFile"] = config::currentSaveConfigFile;
				}
				config::writeConfigonRootToFile("config", sysdata);
				//调用所有模块 加载配置
				json data = config::loadConfigonRootFromFile(config::currentSaveConfigFile);
				for (auto& mod : Game::GetModuleManager()->GetAllModule()) {
					mod->onsaveConfigFile(data[mod->getModuleName()]);
				}
				//写入文件
				config::writeConfigonRootToFile(config::currentSaveConfigFile, data);
			}
			ImGui::Toggle("启用快捷键(CTRL+L)卸载", &Loader::EnableEjectKey, toggerConfig);
			if (ImGui::Button("卸载")) {
				Loader::Eject_Signal = true;
			}
		}
		//if (ImGui::CollapsingHeader(("Aura"))) {
		//	ImGui::Spacing();
		//	if (ImGui::Button("Test")) {
		//	}
		//	//ImGui::Toggle("Toggle Snow D3D11", &ImGui::doSnow);
		
		//	ImGui::ButtonScrollable("Button Scrollable", ImVec2(300.f, 0.f));
		//	//ImGui::ButtonScrollable("Button Scrollable that fits in button size", ImVec2(350.f, 0.f));
		//	ImGui::ButtonScrollableEx("Button Scrollable (Right-click only!)", ImVec2(355.f, 0.f), ImGuiButtonFlags_MouseButtonRight);
		//	ImGui::Spacing();
		//}
		//if (ImGui::CollapsingHeader(("Client"))) {
		//	ImGui::Spacing();
		//	if (ImGui::Button("中国制造汇集全球")) {
		//	}
		//	ImGui::Spacing();
		//}
		//if (ImGui::CollapsingHeader(("Exploits"))) {
		//	ImGui::Spacing();
		//	if (ImGui::Button("中国制造汇集全球")) {
		//	}
		//	ImGui::Spacing();
		//}
	}
	ImGui::End();
}

auto Render::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", true));
}

auto Render::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}