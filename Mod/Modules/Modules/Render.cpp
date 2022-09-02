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
		UIColor color = UIColor(255,255,255);
		ctx->Fillshape(vec2_t(20,10),vec2_t(100,50), color);

		UIColor color2 = UIColor(0, 0, 0);
		std::string str = "ABD";
		std::string str2 = "中文";
		ctx->Drawtext(vec2_t(25, 20), &str, color2, 1.f, 1.f);
		ctx->Drawtext(vec2_t(25, 30), &str2, color2, 1.f, 1.f);
		ctx->flushText(0);

	}

}
