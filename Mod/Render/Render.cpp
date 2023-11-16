#include "Render.h"


#include "imgui.h"
#include "Game.h"
#include "ClientInstance.h"

vec2_t Render::fov = vec2_t();
vec2_t Render::screen = vec2_t();
std::shared_ptr<glmatrixf> Render::refdef = std::make_shared<glmatrixf>();

std::optional<vec2_t> Render::RenderBlockBox(vec3_ti bPos, vec3_t lPos, ImColor boxColor, float linesize)
{
	std::optional<vec2_t> ret;
	vec2_t out;
	std::vector<vec3_t> offset = {
		{0,0,0},	//0
		{0,1,0},	//1
		{0,0,1},	//2
		{0,1,1},	//3
		{1,0,0},	//4
		{1,1,0},	//5
		{1,0,1},	//6
		{1,1,1}		//7
	};

	vec2_t point[8]{};
	for (int i = 0; i < 8; i++) {
		if (refdef->OWorldToScreen(lPos, { bPos.x + offset[i].x, bPos.y + offset[i].y, bPos.z + +offset[i].z }, out, fov, screen)) {
			point[i] = out;
		}
		else {
			point[i] = vec2_t();
		}
	}

	auto drawList = ImGui::GetForegroundDrawList();
	if (point[0] != vec2_t()) {
		if (point[1] != vec2_t()) {
			drawList->AddLine({ point[0].x, point[0].y }, { point[1].x,point[1].y }, boxColor, linesize);
		}
		if (point[2] != vec2_t()) {
			drawList->AddLine({ point[0].x, point[0].y }, { point[2].x,point[2].y }, boxColor, linesize);
		}
		if (point[4] != vec2_t()) {
			drawList->AddLine({ point[0].x, point[0].y }, { point[4].x,point[4].y }, boxColor, linesize);
		}
	}

	if (point[1] != vec2_t()) {
		if (point[3] != vec2_t()) {
			drawList->AddLine({ point[1].x, point[1].y }, { point[3].x,point[3].y }, boxColor, linesize);
		}
		if (point[5] != vec2_t()) {
			drawList->AddLine({ point[1].x, point[1].y }, { point[5].x,point[5].y }, boxColor, linesize);
		}
	}

	if (point[2] != vec2_t()) {
		if (point[3] != vec2_t()) {
			drawList->AddLine({ point[2].x, point[2].y }, { point[3].x,point[3].y }, boxColor, linesize);
		}
		if (point[6] != vec2_t()) {
			drawList->AddLine({ point[2].x, point[2].y }, { point[6].x,point[6].y }, boxColor, linesize);
		}
	}

	if (point[3] != vec2_t()) {
		if (point[7] != vec2_t()) {
			drawList->AddLine({ point[3].x, point[3].y }, { point[7].x,point[7].y }, boxColor, linesize);
		}
	}

	if (point[4] != vec2_t()) {
		if (point[5] != vec2_t()) {
			drawList->AddLine({ point[4].x, point[4].y }, { point[5].x,point[5].y }, boxColor, linesize);
		}
		if (point[6] != vec2_t()) {
			drawList->AddLine({ point[4].x, point[4].y }, { point[6].x,point[6].y }, boxColor, linesize);
		}
	}

	if (point[5] != vec2_t()) {
		if (point[7] != vec2_t()) {
			drawList->AddLine({ point[5].x, point[5].y }, { point[7].x,point[7].y }, boxColor, linesize);
		}
	}

	if (point[6] != vec2_t()) {
		if (point[7] != vec2_t()) {
			drawList->AddLine({ point[6].x, point[6].y }, { point[7].x,point[7].y }, boxColor, linesize);
		}
	}

	if (point[0] != vec2_t() && point[7] != vec2_t()) {
		ret = point[0].add(point[7]).div(2.f);
	}

	return ret;
}

void Render::Updata()
{
	// 获取屏幕宽高
	RECT rect{};
	::GetWindowRect((HWND)Game::ChildWindowsHandle, (LPRECT)&rect);
	screen.x = (float)(rect.right - rect.left);
	screen.y = (float)(rect.bottom - rect.top);

	if (Game::Cinstance) {
		fov = Game::Cinstance->getFov();

		refdef = std::shared_ptr<glmatrixf>(Game::Cinstance->getGlmatrixf()->correct());
	}
}
