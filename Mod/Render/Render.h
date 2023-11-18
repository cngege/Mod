#pragma once
#include "imgui.h"
#include "HMath.h"

#include <memory>
#include <optional>

class Render {
	static vec2_t fov;
	static vec2_t screen;
	static vec3_t origin;
	static std::shared_ptr<glmatrixf> refdef;

public:
	static void Updata();
	/**
	 * @brief 绘制方块Box 返回一个屏幕上的位置
	*/
	static std::optional<vec2_t> RenderBlockBox(vec3_ti bPos,ImColor boxColor = ImColor(241, 196, 15, 255), float linesize = 1.5f);

	static std::optional<vec2_t> RenderPlayerBox2D(class Player player, ImColor boxColor = ImColor(241, 196, 15, 255), float linesize = 1.5f);

	static std::optional<vec2_t> RenderWorldBox2D(vec3_t pos, ImColor boxColor = ImColor(241, 196, 15, 255), float linesize = 1.5f);

	static std::optional<vec2_t> RenderAABB2D(class AABB aabb, ImColor boxColor = ImColor(241, 196, 15, 255), float linesize = 1.5f);
};