#include "Debug.h"
#include "imgui.h"
#include "../../imgui/toggle/imgui_toggle.h"
#include "../../imgui/toggle/imgui_toggle_presets.h"

#include "Game.h"
#include "HMath.h"

#define FMT_HEADER_ONLY
#include "fmt/core.h"

#include "ClientInstance.h"
#include "Level.h"
#include "LocalPlayer.h"
#include "ServerPlayer.h"
#include "RemotePlayer.h"
#include "Logger.h"

ImGuiToggleConfig toggerConfig_Debug;
std::shared_ptr<glmatrixf> refdef;

bool ShowDemoForm = false;
bool ShowFontSelectForm = false;
bool renderW2SDebugBox = false;
bool ShowPtrList = false;

vec2_t outFov;

Debug::Debug() : Module(0, "Debug", "开发者调试") {
	toggerConfig_Debug = ImGuiTogglePresets::RectangleStyle();

	AddBoolUIValue("显示Demo窗口", &ShowDemoForm);
	AddBoolUIValue("显示字体选择", &ShowFontSelectForm);
	AddBoolUIValue("尝试方框透视", &renderW2SDebugBox);
	AddBoolUIValue("显示常用指针", &ShowPtrList);

	AddFloatUIValue("FovX", &outFov.x, 0, 10);
	AddFloatUIValue("FovY", &outFov.y, 0, 10);

	AddButtonUIEvent("DebugBtn", false, [&]() {
		LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
		if (lp != nullptr) {
			auto count = lp->getLevel()->getAllPlayer().size();
			logF("all player num: %d", count);
		}
		});

}


void RenderDebugBox() {
	// 尝试世界到屏幕
	LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
	if (lp) {

		vec3_t lpPos = *lp->getPosition();

		// 获取屏幕宽高
		RECT rect{};
		::GetWindowRect((HWND)Game::WindowsHandle, (LPRECT)&rect);
		float rectwidth = (float)(rect.right - rect.left);
		float rectheight = (float)(rect.bottom - rect.top);

		// 画线 屏幕到00 -60 00
		vec2_t out;
		vec2_t fov = Game::Cinstance->getFov();
		refdef = std::shared_ptr<glmatrixf>(Game::Cinstance->getGlmatrixf()->correct());

		vec3_t blockpos = { 5.f,-60.f,-1.f };

		if (refdef->OWorldToScreen(lpPos, { blockpos.x + 0.5f, blockpos.y + 1.f, blockpos.z + 0.5f }, out, fov, { rectwidth,rectheight })) {
			auto drawList = ImGui::GetForegroundDrawList();
			drawList->AddLine({ rectwidth / 2, 0 }, { out.x,out.y }, ImColor(241, 196, 15, 255));


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
				if (refdef->OWorldToScreen(lpPos, { blockpos.x + offset[i].x, blockpos.y + offset[i].y, blockpos.z + +offset[i].z }, out, fov, { rectwidth,rectheight })) {
					point[i] = out;
				}
				else {
					point[i] = vec2_t();
				}
			}

			float linesize = 1.5;

			if (point[0] != vec2_t()) {
				if (point[1] != vec2_t()) {
					drawList->AddLine({ point[0].x, point[0].y }, { point[1].x,point[1].y }, ImColor(30, 132, 73, 255), linesize);
				}
				if (point[2] != vec2_t()) {
					drawList->AddLine({ point[0].x, point[0].y }, { point[2].x,point[2].y }, ImColor(30, 132, 73, 255), linesize);
				}
				if (point[4] != vec2_t()) {
					drawList->AddLine({ point[0].x, point[0].y }, { point[4].x,point[4].y }, ImColor(30, 132, 73, 255), linesize);
				}
			}

			if (point[1] != vec2_t()) {
				if (point[3] != vec2_t()) {
					drawList->AddLine({ point[1].x, point[1].y }, { point[3].x,point[3].y }, ImColor(30, 132, 73, 255), linesize);
				}
				if (point[5] != vec2_t()) {
					drawList->AddLine({ point[1].x, point[1].y }, { point[5].x,point[5].y }, ImColor(30, 132, 73, 255), linesize);
				}
			}

			if (point[2] != vec2_t()) {
				if (point[3] != vec2_t()) {
					drawList->AddLine({ point[2].x, point[2].y }, { point[3].x,point[3].y }, ImColor(30, 132, 73, 255), linesize);
				}
				if (point[6] != vec2_t()) {
					drawList->AddLine({ point[2].x, point[2].y }, { point[6].x,point[6].y }, ImColor(30, 132, 73, 255), linesize);
				}
			}

			if (point[3] != vec2_t()) {
				if (point[7] != vec2_t()) {
					drawList->AddLine({ point[3].x, point[3].y }, { point[7].x,point[7].y }, ImColor(30, 132, 73, 255), linesize);
				}
			}

			if (point[4] != vec2_t()) {
				if (point[5] != vec2_t()) {
					drawList->AddLine({ point[4].x, point[4].y }, { point[5].x,point[5].y }, ImColor(30, 132, 73, 255), linesize);
				}
				if (point[6] != vec2_t()) {
					drawList->AddLine({ point[4].x, point[4].y }, { point[6].x,point[6].y }, ImColor(30, 132, 73, 255), linesize);
				}
			}

			if (point[5] != vec2_t()) {
				if (point[7] != vec2_t()) {
					drawList->AddLine({ point[5].x, point[5].y }, { point[7].x,point[7].y }, ImColor(30, 132, 73, 255), linesize);
				}
			}

			if (point[6] != vec2_t()) {
				if (point[7] != vec2_t()) {
					drawList->AddLine({ point[6].x, point[6].y }, { point[7].x,point[7].y }, ImColor(30, 132, 73, 255), linesize);
				}
			}

		}
	}
}

void AddOffsetAndPrint(void* baseptr, int* offset, const char* tip) {
	if (ImGui::Button((std::string("计算(X16)###") + std::to_string((uintptr_t)baseptr)).c_str())) {
		logF(tip, *reinterpret_cast<uintptr_t*>((uintptr_t)baseptr + *offset));
	}
	ImGui::SameLine();
	ImGui::InputInt((std::string("###InputInt") + std::to_string((uintptr_t)baseptr)).c_str(), offset, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);
}

void ShowPtr() {
	if (ImGui::Begin("指针", &ShowPtrList)) {
		if (Game::Cinstance) {
			if (ImGui::Button("复制###Cinstance")) {
				std::string ci_copy = fmt::format("{:X}", (uintptr_t)Game::Cinstance);
				ImGui::SetClipboardText(ci_copy.c_str());
			}
			ImGui::SameLine();
			ImGui::Text("ClientInstance: %llX", Game::Cinstance);
			static int offs = 0;
			AddOffsetAndPrint((void*)Game::Cinstance, &offs, "ClientInstance + offset: %llX");

			LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
			if (lp) {
				if (ImGui::Button("复制###LocalPlayer")) {
					std::string lp_copy = fmt::format("{:X}", (uintptr_t)(lp));
					ImGui::SetClipboardText(lp_copy.c_str());
				}
				ImGui::SameLine();
				ImGui::Text("LocalPlayer: %llX", lp);
				static int offs_lp = 0;
				AddOffsetAndPrint((void*)lp, &offs_lp, "LocalPlayer + offset: %llX");

				Level* lvl = lp->getLevel();
				if (lvl) {
					if (ImGui::Button("复制###Level")) {
						std::string lvl_copy = fmt::format("{:X}", (uintptr_t)(lvl));
						ImGui::SetClipboardText(lvl_copy.c_str());
					}
					ImGui::SameLine();
					ImGui::Text("Level: %llX", lvl);
					static int offs_lvl = 0;
					AddOffsetAndPrint((void*)lvl, &offs_lvl, "Level + offset: %llX");

					// 显示所有玩家
					ImGui::Text("All Player - Level::forEachPlayer");
					lvl->forEachPlayer([](Player& player) {
						if (ImGui::Button((std::string("复制地址###") + std::to_string((uintptr_t)(&player))).c_str())) {
							std::string lvl_copy = fmt::format("{:X}", (uintptr_t)(&player));
							ImGui::SetClipboardText(lvl_copy.c_str());
						}
						ImGui::SameLine();
						ImGui::Text("Ptr: %llX, 本地玩家:{%d},服务玩家:{%d},远程玩家:{%d}", &player, player.isLocalPlayer(), *(uintptr_t***)&player == ServerPlayer::GetVFtables(), *(uintptr_t***)&player == RemotePlayer::GetVFtables());
						return true;
						});
				}
			}
		}
	}
	ImGui::End();
}


auto Debug::onImGUIRender() -> void
{
	if (!isEnabled())
		return;
	
	if (ShowDemoForm) {
		ImGui::ShowDemoWindow();
	}
	if (ShowFontSelectForm) {
		ImGui::ShowFontSelector("字体选择");
	}
	if (renderW2SDebugBox) {
		RenderDebugBox();
	}
	if (ShowPtrList) {
		ShowPtr();
	}
	
}

auto Debug::onInternalImGUIRender()->void {

	if (ImGui::Button("PrintFov")) {
		if(Game::Cinstance)
			logF(fmt::format("X: {},Y: {}", *(float*)((uintptr_t)Game::Cinstance + 0x6D0), *(float*)((uintptr_t)Game::Cinstance + 0x6E4)).c_str());
	}
}

auto Debug::onloadConfigFile(json& data)->void {
	//setEnabled(config::readDataFromJson<bool>(data, "enable", false));
}

auto Debug::onsaveConfigFile(json& data)->void {
	//data["enable"] = isEnabled();
}
