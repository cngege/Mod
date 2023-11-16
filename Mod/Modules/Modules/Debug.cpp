#include "Debug.h"
#include "imgui.h"
#include "../../imgui/toggle/imgui_toggle.h"
#include "../../imgui/toggle/imgui_toggle_presets.h"

#include "Game.h"
#include "HMath.h"

#include <format>

#include "ClientInstance.h"
#include "Level.h"
#include "LocalPlayer.h"
#include "ServerPlayer.h"
#include "RemotePlayer.h"
#include "GameMode.h"
#include "ItemStack.h"
#include "BlockLegacy.h"


#include "Logger.h"
#include "../Hook/Hook.h"
#include "../Modules/ModuleManager.h"
#include "Render.h"

ImGuiToggleConfig toggerConfig_Debug;
std::shared_ptr<glmatrixf> refdef;

bool ShowDemoForm = false;
bool ShowFontSelectForm = false;
bool renderW2SDebugBox = false;
bool ShowPtrList = false;


bool KeyUseItem = false;
bool CheckFlyStatus = false;

// 视频矩阵相机位置高度偏移
float y_offset = 0.f;

vec2_t outFov;

vec3_ti renderBlockPos;

int vtNum = 141;
int pack = 85;

#include "ActorMovementProxy.h"
#include "BlockSource.h"
#include "Block.h"
#include "Dimension.h"
Debug::Debug() : Module(0, "Debug", "开发者调试") {
	toggerConfig_Debug = ImGuiTogglePresets::RectangleStyle();
	
	AddBoolUIValue("D3D渲染同步此UI", &SynchronousD3D12Render_ImGui);
	AddBoolUIValue("GameModeAttack中打印信息", &GameMode_attack_Print);
	AddBoolUIValue("GameModeAttack时使用物品", &GameMode_attack_UseItem);
	AddBoolUIValue("显示Demo窗口", &ShowDemoForm);
	AddBoolUIValue("显示字体选择", &ShowFontSelectForm);
	AddBoolUIValue("尝试方框透视", &renderW2SDebugBox);
	AddBoolUIValue("显示常用指针", &ShowPtrList);
	AddBoolUIValue("G健切换飞行状态", &CheckFlyStatus);
	AddBoolUIValue("H健使用手中物品", &KeyUseItem);

	AddFloatUIValue("坐标转换相机高度偏移", &y_offset, -10.f, 10.f);
	AddFloatUIValue("FovX", &outFov.x, 0, 10);
	AddFloatUIValue("FovY", &outFov.y, 0, 10);

	AddIntUIValue("调用虚表数", &vtNum, 0, 500);
	AddIntUIValue("数据包拦截率", &pack, 0, 100);

	AddButtonUIEvent("DebugBtn", false, [&]() {
		LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
		if (lp != nullptr) {
			
			//logF("Run");
			//bool v = lp->isFlying();
			//logF(v ? "isfly" : "nofly");

			//lp->setCanFlyEx(true);
			//BlockSource* bs = lp->getDimensionConst()->getBlockSourceEx();

			//bool v1 = bs->getBlock(0,1,0)->isAirEx();
			//bool v1 = lp->getDimensionBlockSource()->getBlock(0, 0, 0)->isAirEx();
			//bool v2 = lp->getDimensionBlockSource()->getBlock(0, 1, 0)->isAirEx();
			//logF("0 0 0 %s" , (v1 ? "is air" : "not air"));
			//logF("0 1 0 " + v2 ? "is air" : "not air");
			//logF("debug: block: %llX", bs->getBlock(0, 1, 0));
			//logF("debug: BlockSource vt: %llX", *(uintptr_t*)bs);
			auto pos = lp->getPosition();
			if (pos->x < 0) renderBlockPos.x = (int)(pos->x - 1.f); else renderBlockPos.x = pos->x;
			if (pos->y < 0) renderBlockPos.y = (int)(pos->y - 1.f); else renderBlockPos.y = pos->y;
			if (pos->z < 0) renderBlockPos.z = (int)(pos->z - 1.f); else renderBlockPos.z = pos->z;

			renderBlockPos.y -= 2;

			BlockSource* bs = lp->getDimensionConst()->getBlockSourceEx();
			logF("debug: blockid: %u", bs->getBlock(&renderBlockPos)->getBlockLegacy()->getBlockItemIdEx());
		}

		Game::Cinstance->getMinecraftGame();

		// 函数定位码
		// 48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 81 EC 80 00 00 00 0F 29 74 24 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48
		//std::mcstring out;
		//using GetTopScreenName = void(__thiscall*)(ClientInstance*, std::mcstring*);
		//auto _GetTopScreenName = (GetTopScreenName)((*(uintptr_t**)Game::Cinstance)[vtNum] /*->VTable[141]*/);
		//_GetTopScreenName(Game::Cinstance, &out);
		//logF("_GetTopScreenName:%s ,%llX", out.c_str(), (*(uintptr_t**)Game::Cinstance)[vtNum]);
		//logF("_GetTopScreenName:%s", Game::Cinstance->getTopScreenName().c_str());

		});
	AddButtonUIEvent("GetTopScreenName", true, [&]() {
		std::mcstring out;
		using GetTopScreenName = void(__thiscall*)(ClientInstance*, std::mcstring*);
		auto _GetTopScreenName = (GetTopScreenName)((*(uintptr_t**)Game::Cinstance)[vtNum] /*->VTable[141]*/);
		_GetTopScreenName(Game::Cinstance, &out);
		logF("GetTopScreenName:%s ,%llX", out.c_str(), (*(uintptr_t**)Game::Cinstance)[vtNum]);
		});
	AddButtonUIEvent("DebugString", true, [&]() {
		std::string tmp = "AB";
		uintptr_t* intval = (uintptr_t*)&tmp;
		if (intval == (uintptr_t*)tmp.data()) {
			Game::GetModuleManager()->GetModule<Render*>()->setEnabled(false);
		}
		logF("DebugStringA:%s", tmp.c_str());

		});
}

void RenderDebugBox() {
	// 尝试世界到屏幕
	LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
	if (lp) {
		ImColor yellow = ImColor(241, 196, 15, 255);
		ImColor green = ImColor(30, 132, 73, 255);


		vec3_t lpPos = *lp->getPosition();
		lpPos.y += y_offset;

		// 获取屏幕宽高
		RECT rect{};
		::GetWindowRect((HWND)Game::ChildWindowsHandle, (LPRECT)&rect);
		float rectwidth = (float)(rect.right - rect.left);
		float rectheight = (float)(rect.bottom - rect.top);

		// 画线 屏幕到00 -60 00
		vec2_t out;
		vec2_t fov = Game::Cinstance->getFov();
		refdef = std::shared_ptr<glmatrixf>(Game::Cinstance->getGlmatrixf()->correct());

		vec3_t blockpos = renderBlockPos.toFloatVector();

		if (refdef->OWorldToScreen(lpPos, { blockpos.x + 0.5f, blockpos.y + 1.f, blockpos.z + 0.5f }, out, fov, { rectwidth,rectheight })) {
			auto drawList = ImGui::GetForegroundDrawList();
			drawList->AddLine({ rectwidth / 2, rectheight / 2 }, { out.x,out.y }, green);


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
					drawList->AddLine({ point[0].x, point[0].y }, { point[1].x,point[1].y }, yellow, linesize);
				}
				if (point[2] != vec2_t()) {
					drawList->AddLine({ point[0].x, point[0].y }, { point[2].x,point[2].y }, yellow, linesize);
				}
				if (point[4] != vec2_t()) {
					drawList->AddLine({ point[0].x, point[0].y }, { point[4].x,point[4].y }, yellow, linesize);
				}
			}

			if (point[1] != vec2_t()) {
				if (point[3] != vec2_t()) {
					drawList->AddLine({ point[1].x, point[1].y }, { point[3].x,point[3].y }, yellow, linesize);
				}
				if (point[5] != vec2_t()) {
					drawList->AddLine({ point[1].x, point[1].y }, { point[5].x,point[5].y }, yellow, linesize);
				}
			}

			if (point[2] != vec2_t()) {
				if (point[3] != vec2_t()) {
					drawList->AddLine({ point[2].x, point[2].y }, { point[3].x,point[3].y }, yellow, linesize);
				}
				if (point[6] != vec2_t()) {
					drawList->AddLine({ point[2].x, point[2].y }, { point[6].x,point[6].y }, yellow, linesize);
				}
			}

			if (point[3] != vec2_t()) {
				if (point[7] != vec2_t()) {
					drawList->AddLine({ point[3].x, point[3].y }, { point[7].x,point[7].y }, yellow, linesize);
				}
			}

			if (point[4] != vec2_t()) {
				if (point[5] != vec2_t()) {
					drawList->AddLine({ point[4].x, point[4].y }, { point[5].x,point[5].y }, yellow, linesize);
				}
				if (point[6] != vec2_t()) {
					drawList->AddLine({ point[4].x, point[4].y }, { point[6].x,point[6].y }, yellow, linesize);
				}
			}

			if (point[5] != vec2_t()) {
				if (point[7] != vec2_t()) {
					drawList->AddLine({ point[5].x, point[5].y }, { point[7].x,point[7].y }, yellow, linesize);
				}
			}

			if (point[6] != vec2_t()) {
				if (point[7] != vec2_t()) {
					drawList->AddLine({ point[6].x, point[6].y }, { point[7].x,point[7].y }, yellow, linesize);
				}
			}

		}
	}
}

void AddOffsetAndPrint(void* baseptr, int* offset, const char* tip) {
	if (ImGui::Button((std::string("计算(X16)##") + std::to_string((uintptr_t)baseptr)).c_str())) {
		ImGui::OpenPopup("Select");
	}
	if (ImGui::BeginPopup("Select"))
	{
		uintptr_t val = *reinterpret_cast<uintptr_t*>((uintptr_t)baseptr + *offset);
		uintptr_t vfFunval = *reinterpret_cast<uintptr_t*>(*(uintptr_t*)baseptr + *offset);

		if (ImGui::Button((std::string("打印(X16)##") + std::to_string(val)).c_str())) {
			logF(tip, val);
		}
		ImGui::SameLine();
		if (ImGui::Button((std::string("复制(X16)##") + std::to_string(val)).c_str())) {
			std::string copy = std::format("{:X}", val);
			ImGui::SetClipboardText(copy.c_str());
		}
		ImGui::SameLine();
		ImGui::Text("thisoffset: 0x%llX", val);

		if (ImGui::Button((std::string("打印(X16)##") + std::to_string((uintptr_t)vfFunval)).c_str())) {
			logF(tip, vfFunval);
		}
		ImGui::SameLine();
		if (ImGui::Button((std::string("复制(X16)##") + std::to_string((uintptr_t)vfFunval)).c_str())) {
			std::string copy = std::format("{:X}", vfFunval);
			ImGui::SetClipboardText(copy.c_str());
		}
		ImGui::SameLine();
		ImGui::Text("vtoffset: 0x%llX", vfFunval);

		const char* rets[] = { "void", "void*", "std::mcstring*", "int", "float" };
		static int selectval = 0;
		if (ImGui::Button((std::string("单参调用/打印返回值###") + std::to_string((uintptr_t)baseptr)).c_str())) {
			if (selectval == 0) {
				using Fn_0 = void(__thiscall*)(void*);
				reinterpret_cast<Fn_0>(vfFunval)(baseptr);
			}
			else if (selectval == 1) {
				using Fn_1 = void* (__thiscall*)(void*);
				auto ret = reinterpret_cast<Fn_1>(vfFunval)(baseptr);
				logF("Ptr:%llX, offset: %d, Ret: %llX", vfFunval, *offset, ret);
			}
			else if (selectval == 2) {
				using Fn_2 = std::mcstring* (__thiscall*)(void*);
				auto ret = *reinterpret_cast<Fn_2>(vfFunval)(baseptr);
				logF("Ptr:%llX, offset: %d, Ret: %s", vfFunval, *offset, ret.c_str());
			}
			else if (selectval == 3) {
				using Fn_3 = int(__thiscall*)(void*);
				auto ret = reinterpret_cast<Fn_3>(vfFunval)(baseptr);
				logF("Ptr:%llX, offset: %d, Ret: %d", vfFunval, *offset, ret);
			}
			else if (selectval == 4) {
				using Fn_4 = float(__thiscall*)(void*);
				auto ret = reinterpret_cast<Fn_4>(vfFunval)(baseptr);
				logF("Ptr:%llX, offset: %d, Ret: %f", vfFunval, *offset, ret);
			}
		}
		ImGui::SameLine();
		ImGui::Combo("选择返回值类型", &selectval, rets, IM_ARRAYSIZE(rets));

		ImGui::EndPopup();
	}

	ImGui::SameLine();
	ImGui::InputInt((std::string("###InputInt") + std::to_string((uintptr_t)baseptr)).c_str(), offset, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);
}


struct PlayerInfo {
	//Ptr: %llX, 本地玩家:{%d},服务玩家:{%d},远程玩家:{%d},Player:{%d}
	bool isLocalPlayer = false;
	bool isServerPlayer = false;
	bool isRemotePlayer = false;
	bool isPlayerVT = false;
};
std::unordered_map<Player*, PlayerInfo> playerlist;


void ShowPtr() {
	if (ImGui::Begin("指针", &ShowPtrList)) {
		if (Game::Cinstance) {
			if (ImGui::Button("复制###Cinstance")) {
				std::string ci_copy = std::format("{:X}", (uintptr_t)Game::Cinstance);
				ImGui::SetClipboardText(ci_copy.c_str());
			}
			ImGui::SameLine();
			ImGui::Text("ClientInstance: %llX", Game::Cinstance);
			static int offs = 0;
			AddOffsetAndPrint((void*)Game::Cinstance, &offs, "ClientInstance + offset: %llX");

			LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
			if (lp) {
				if (ImGui::Button("复制###LocalPlayer")) {
					std::string lp_copy = std::format("{:X}", (uintptr_t)(lp));
					ImGui::SetClipboardText(lp_copy.c_str());
				}
				ImGui::SameLine();
				ImGui::Text("LocalPlayer: %llX", lp);
				static int offs_lp = 0;
				AddOffsetAndPrint((void*)lp, &offs_lp, "LocalPlayer + offset: %llX");

				Level* lvl = lp->getLevel();
				if (lvl) {
					if (ImGui::Button("复制###Level")) {
						std::string lvl_copy = std::format("{:X}", (uintptr_t)(lvl));
						ImGui::SetClipboardText(lvl_copy.c_str());
					}
					ImGui::SameLine();
					ImGui::Text("Level: %llX", lvl);
					static int offs_lvl = 0;
					AddOffsetAndPrint((void*)lvl, &offs_lvl, "Level + offset: %llX");

					// 显示所有玩家
					ImGui::Text("All Player - Level::forEachPlayer");
					
					//static std::vector<Player*> players = lvl->getAllPlayer();
					if (ImGui::Button("更新")) {
						playerlist.clear();
						auto players = lvl->getAllPlayer();
						lvl->forEachPlayer([&](Player& player) {
							PlayerInfo info;
							info.isLocalPlayer = player.isLocalPlayer();
							info.isRemotePlayer = player.isRemotePlayer();
							info.isServerPlayer = *(uintptr_t***)&player == ServerPlayer::GetVFtables();
							info.isPlayerVT = *(uintptr_t***)&player == Player::GetVFtables();
							playerlist[&player] = info;
							return true;
							});
					}

					for (auto& kv : playerlist) {
						if (ImGui::Button((std::string("复制地址###") + std::to_string((uintptr_t)(kv.first))).c_str())) {
							std::string lvl_copy = std::format("{:X}", (uintptr_t)(kv.first));
							ImGui::SetClipboardText(lvl_copy.c_str());
						}
						ImGui::SameLine();
						ImGui::Text("Ptr: %llX, 本地玩家:{%d},服务玩家:{%d},远程玩家:{%d},Player:{%d}", kv.first, kv.second.isLocalPlayer, kv.second.isServerPlayer, kv.second.isRemotePlayer, kv.second.isPlayerVT);
					}
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

auto Debug::onKeyUpdate(int key, bool isdown) -> void
{
	if (isEnabled() && isdown) {
		if (key == 'G' && CheckFlyStatus) {
			if (Game::Cinstance && Game::Cinstance->getCILocalPlayer()) {
				Game::Cinstance->getCILocalPlayer()->setFlying(!Game::Cinstance->getCILocalPlayer()->isFlying());

				logF("被调用G");
			}
		}
		if (key == 'H' && KeyUseItem) {
			if (Game::Cinstance && Game::Cinstance->getCILocalPlayer()) {
				//Game::Cinstance->getCILocalPlayer()->getSelectedItem()->use(Game::Cinstance->getCILocalPlayer());

				Game::Cinstance->getCILocalPlayer()->getLevel()->setSimPaused(false);
				logF("被调用H");
			}
		}
	}
}

auto Debug::onSendPackToServer(LoopbackPacketSender*, Packet*) -> bool
{
	auto time = Utils::GetCuttentMillisecond();
	
	int64_t inttime = (int64_t)time;//没秒发送一次，每次发送50ms
	if (CheckFlyStatus) {
		if (Game::Cinstance) {
			LocalPlayer* lp = Game::Cinstance->getCILocalPlayer();
			if (lp) {
				if (!lp->isFlying()) return true;

				//static int64_t sendTime = inttime; // 该发送消息的起始时间
				//每发送50ms后 拦截50ms
				if (inttime % 100 < pack) {
					return false;
				}

			}
		}
	}

	return true;
}

auto Debug::onInternalImGUIRender()->void {

	if (ImGui::Button("PrintFov")) {
		if(Game::Cinstance)
			logF(std::format("X: {},Y: {}", Game::Cinstance->getFov().x, Game::Cinstance->getFov().y).c_str());
	}
}

auto Debug::onloadConfigFile(json& data)->void {
	//setEnabled(config::readDataFromJson<bool>(data, "enable", false));
}

auto Debug::onsaveConfigFile(json& data)->void {
	//data["enable"] = isEnabled();
}
