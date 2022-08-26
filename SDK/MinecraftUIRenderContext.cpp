#include "MinecraftUIRenderContext.h"
#include "..\Mod\Utils\HMath.h"


auto MinecraftUIRenderContext::Fillshape(vec2_t pos, vec2_t size, UIColor color)->void {
	RectangleArea ra = RectangleArea(pos.x, pos.x + size.x, pos.y, pos.y + size.y);
	this->fillRectangle(ra, color, 1);
}

auto MinecraftUIRenderContext::Drawshape(vec2_t pos, vec2_t size, UIColor color, float w)->void {
	RectangleArea ra = RectangleArea(pos.x, pos.x + size.x, pos.y, pos.y + size.y);
	this->drawRectangle(ra, color, 1, w);
}