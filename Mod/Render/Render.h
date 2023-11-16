#pragma once
#include "imgui.h"
#include "HMath.h"

#include <memory>
#include <optional>

class Render {
	static vec2_t fov;
	static vec2_t screen;
	static std::shared_ptr<glmatrixf> refdef;

public:

	static void Updata();
	/**
	 * @brief 绘制方块Box 返回一个屏幕上的位置
	*/
	static std::optional<vec2_t> RenderBlockBox(vec3_ti bPos, vec3_t lPos, ImColor boxColor = ImColor(241, 196, 15, 255), float linesize = 1.5f);
};