#include "Render.h"
#include "../ModuleManager.h"
#include "../../Utils/Game.h"

Render::Render() : Module(VK_INSERT, "Render", "渲染UI管理器") {
	setEnabled(true);
	SetKeyMode(KeyMode::Switch);
}

auto Render::onRenderDetour(MinecraftUIRenderContext* ctx)->void {
	if (isEnabled()) {
		//画面板 功能列表
		UIColor bgcolor = UIColor(0,0,0,60);
		UIColor textcolor1 = UIColor(255, 255, 255);
		UIColor textcolor2 = UIColor(150, 150, 150);

		float ltY = 15.f;					//绘制文字左上角Y值的位置
		float add = 13.f;					//下一行的偏移量

		auto mcount = Game::GetModuleManager()->GetAllModule().size();

		ctx->Fillshape(vec2_t(20, 10), vec2_t(200.f, mcount*add + ltY + 5.f), bgcolor);
		
		for (auto pMod : Game::GetModuleManager()->GetAllModule()) {
			if (pMod->GetKeyMode() == KeyMode::Switch) {
				// 如果这个模块是有开关状态的切换模式
				std::string showText = pMod->getModuleName() + " ";
				if (pMod->getBindKeyName() != "") {
					showText += "[" + pMod->getBindKeyName() + "]  ";
				}
				showText += pMod->isEnabled() ? "[ON]" : "[OFF]";
				ctx->Drawtext(vec2_t(25.f, ltY), &showText, pMod->isEnabled()? textcolor1 : textcolor2, 1.f);
				ltY += add;
			}
			else if (pMod->GetKeyMode() == KeyMode::Trigger) {
				// 如果这个模块是点击快捷键触发的
				std::string showText = pMod->getModuleName() + " ";
				if (pMod->getBindKeyName() != "") {
					showText += "[" + pMod->getBindKeyName() + "]  ";
				}
				showText += "[Trigger]";
				ctx->Drawtext(vec2_t(25.f, ltY), &showText, textcolor2, 1.f);
				ltY += add;
			}
			else if (pMod->GetKeyMode() == KeyMode::Hold) {
				// 如果这个模块是长按开启，松开关闭模式
				std::string showText = pMod->getModuleName() + " ";
				if (pMod->getBindKeyName() != "") {
					showText += "[" + pMod->getBindKeyName() + "]  ";
				}
				showText += pMod->isEnabled() ? "[HoldON]" : "[HoldOFF]";
				ctx->Drawtext(vec2_t(25.f, ltY), &showText, pMod->isEnabled() ? textcolor1 : textcolor2, 1.f);
				ltY += add;
			}
		}
		ctx->flushText(0);
	}
}
