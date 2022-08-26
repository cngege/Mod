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
		UIColor color = UIColor();
		RectangleArea ra = RectangleArea(100, 150, 100, 150);
		ctx->fillRectangle(ra, color, 1);
	}
}
