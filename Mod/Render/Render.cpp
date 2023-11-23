#include "Render.h"


#include "imgui.h"
#include "Game.h"
#include "ClientInstance.h"
#include "Player.h"
#include "AABB.h"

vec2_t Render::fov = vec2_t();
vec2_t Render::screen = vec2_t();
vec3_t Render::origin = vec3_t();

std::shared_ptr<glmatrixf> Render::refdef = std::make_shared<glmatrixf>();

std::optional<vec2_t> Render::RenderBlockBox(vec3_ti bPos,ImColor boxColor, float linesize)
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
	//lPos = origin;
	vec2_t point[8]{};
	for (int i = 0; i < 8; i++) {
		if (refdef->OWorldToScreen(origin, { bPos.x + offset[i].x, bPos.y + offset[i].y, bPos.z + +offset[i].z }, out, fov, screen)) {
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

	if (Game::Cinstance && *(uintptr_t***)Game::Cinstance == ClientInstance::GetVFtables()) {
		auto LevelRenderer = Game::Cinstance->getLevelRender();
		if (LevelRenderer) {
			auto LevelRendererPlayer = LevelRenderer->getLevelRendererPlayer();
			if (LevelRendererPlayer) {
				fov = Game::Cinstance->getFov();
				refdef = std::shared_ptr<glmatrixf>(Game::Cinstance->getGlmatrixf()->correct());
				origin = LevelRendererPlayer->getCamPos();
			}
				
		}
	}
}

vec2_t Render::getScreen()
{
	return screen;
}

//TODU 待完成
std::optional<vec2_t> Render::RenderPlayerBox2D(Player player, ImColor boxColor, float linesize) {
	//auto aabb = player.getAABB();

	std::optional<vec2_t> ret;
	vec3_t pos = *player.getPosition();
	vec2_t out;

	if (refdef->OWorldToScreen(origin, { pos.x,pos.y - 1, pos.z}, out, fov, screen)) {
		//drawList->AddLine({ rectwidth / 2, rectheight / 2 }, { out.x,out.y }, boxColor);
		auto drawList = ImGui::GetForegroundDrawList();
		drawList->AddCircle({ out.x, out.y }, 20, boxColor, 0, linesize);
		ret = out;
	}
	return ret;
}

//TODU 待完成
std::optional<vec2_t> Render::RenderWorldBox2D(vec3_t pos, ImColor boxColor, float linesize) {
	//auto aabb = player.getAABB();

	std::optional<vec2_t> ret;
	vec2_t out;

	if (refdef->OWorldToScreen(origin, { pos.x,pos.y - 1, pos.z }, out, fov, screen)) {
		//drawList->AddLine({ rectwidth / 2, rectheight / 2 }, { out.x,out.y }, boxColor);
		auto drawList = ImGui::GetForegroundDrawList();
		drawList->AddCircle({ out.x, out.y }, 20, boxColor, 0, linesize);
		ret = out;
	}
	return ret;
}

std::optional<vec2_t> Render::RenderAABB2D(AABB aabb, ImColor boxColor, float linesize) {
	//auto aabb = player.getAABB();

	std::optional<vec2_t> ret;
	vec2_t outmin;
	vec2_t outmax;

	if (refdef->OWorldToScreen(origin, { aabb.min.x,aabb.min.y, aabb.min.z }, outmin, fov, screen) &&
		refdef->OWorldToScreen(origin, { aabb.max.x,aabb.max.y, aabb.max.z }, outmax, fov, screen)) {
		auto drawList = ImGui::GetForegroundDrawList();

		drawList->AddLine({ outmin.x, outmin.y }, { outmin.x, outmax.y }, boxColor, linesize);
		drawList->AddLine({ outmin.x, outmin.y }, { outmax.x, outmin.y }, boxColor, linesize);
		drawList->AddLine({ outmin.x, outmax.y }, { outmax.x, outmax.y }, boxColor, linesize);
		drawList->AddLine({ outmax.x, outmin.y }, { outmax.x, outmax.y }, boxColor, linesize);
		ret = outmin.add(outmax).div(2);
	}
	return ret;
}

std::optional<vec2_t> Render::RenderAABB(AABB aabb, ImColor boxColor, float linesize) {
	//auto aabb = player.getAABB();

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

	if (refdef->OWorldToScreen(origin, { aabb.min.x,aabb.min.y, aabb.min.z }, out, fov, screen)) {
		point[0] = out;
	}
	if (refdef->OWorldToScreen(origin, { aabb.max.x,aabb.min.y, aabb.min.z }, out, fov, screen)) {
		point[1] = out;
	}
	if (refdef->OWorldToScreen(origin, { aabb.max.x,aabb.min.y, aabb.max.z }, out, fov, screen)) {
		point[2] = out;
	}
	if (refdef->OWorldToScreen(origin, { aabb.min.x,aabb.min.y, aabb.max.z }, out, fov, screen)) {
		point[3] = out;
	}

	if (refdef->OWorldToScreen(origin, { aabb.min.x,aabb.max.y, aabb.min.z }, out, fov, screen)) {
		point[4] = out;
	}
	if (refdef->OWorldToScreen(origin, { aabb.max.x,aabb.max.y, aabb.min.z }, out, fov, screen)) {
		point[5] = out;
	}
	if (refdef->OWorldToScreen(origin, { aabb.max.x,aabb.max.y, aabb.max.z }, out, fov, screen)) {
		point[6] = out;
	}
	if (refdef->OWorldToScreen(origin, { aabb.min.x,aabb.max.y, aabb.max.z }, out, fov, screen)) {
		point[7] = out;
	}

	auto drawList = ImGui::GetForegroundDrawList();
	if (point[0] != vec2_t()) {
		if (point[1] != vec2_t()) {
			drawList->AddLine({ point[0].x, point[0].y}, { point[1].x, point[1].y}, boxColor, linesize);
		}
		if (point[3] != vec2_t()) {
			drawList->AddLine({ point[0].x, point[0].y }, { point[3].x, point[3].y }, boxColor, linesize);
		}
		if (point[4] != vec2_t()) {
			drawList->AddLine({ point[0].x, point[0].y }, { point[4].x, point[4].y }, boxColor, linesize);
		}
	}
	if (point[1] != vec2_t()) {
		if (point[2] != vec2_t()) {
			drawList->AddLine({ point[1].x, point[1].y }, { point[2].x, point[2].y }, boxColor, linesize);
		}
		if (point[5] != vec2_t()) {
			drawList->AddLine({ point[1].x, point[1].y }, { point[5].x, point[5].y }, boxColor, linesize);
		}
	}
	if (point[2] != vec2_t()) {
		if (point[3] != vec2_t()) {
			drawList->AddLine({ point[2].x, point[2].y }, { point[3].x, point[3].y }, boxColor, linesize);
		}
		if (point[6] != vec2_t()) {
			drawList->AddLine({ point[2].x, point[2].y }, { point[6].x, point[6].y }, boxColor, linesize);
		}
	}
	if (point[3] != vec2_t()) {
		if (point[7] != vec2_t()) {
			drawList->AddLine({ point[3].x, point[3].y }, { point[7].x, point[7].y }, boxColor, linesize);
		}
	}
	if (point[4] != vec2_t()) {
		if (point[5] != vec2_t()) {
			drawList->AddLine({ point[4].x, point[4].y }, { point[5].x, point[5].y }, boxColor, linesize);
		}
		if (point[7] != vec2_t()) {
			drawList->AddLine({ point[4].x, point[4].y }, { point[7].x, point[7].y }, boxColor, linesize);
		}
	}
	if (point[6] != vec2_t()) {
		if (point[5] != vec2_t()) {
			drawList->AddLine({ point[6].x, point[6].y }, { point[5].x, point[5].y }, boxColor, linesize);
		}
		if (point[7] != vec2_t()) {
			drawList->AddLine({ point[6].x, point[6].y }, { point[7].x, point[7].y }, boxColor, linesize);
		}
	}
	if (point[0] != vec2_t() && point[6] != vec2_t()) {
		ret = point[0].add(point[6]).div(2);
	}
	return ret;
}
