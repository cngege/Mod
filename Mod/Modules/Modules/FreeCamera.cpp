#include "FreeCamera.h"

#include "../../Hook/Hook.h"
#include "Logger.h"

#include "Game.h"
#include "../ModuleManager.h"

#include "LocalPlayer.h"

struct CameraViewInfo
{
	vec3_t pos;
	bool customizePos;
	BYTE unknowbyte_2[3];
	vec2_t rot;
};

using Fn_LockCameraTick = void* (__fastcall*)(void*, unsigned int); //*(bool*)(ret + 4 byte)
using Fn_setCameraPosTick = void* (__fastcall*)(void*, void*, void*);
using Fn_getCameraPosPtr = CameraViewInfo * (__fastcall*)(void*, DWORD*); // 这不是Hook

Fn_LockCameraTick LockCameraTickcall;
Fn_setCameraPosTick setCameraPosTickcall;
// call 不是Hook后的call
Fn_getCameraPosPtr getCameraPos;

static uintptr_t lockCameraTickoffset = 0;
static uintptr_t setCameraPosTickoffset = 0;

bool FreeCamera::UnlockCamera = false;
float speed = 0.1f;




void* FreeCamera::_LockCameraTick(void* a1, unsigned int a2) {
	auto ret = LockCameraTickcall(a1, a2);

	static auto fc = Game::GetModuleManager()->GetModule<FreeCamera*>();

	if (fc && fc->isEnabled() && UnlockCamera) {
		if (ret) {
			*((bool*)ret + 4) = true;	// 必须运行一次命令 /camera CNGEGE set minecraft:free pos ~ ~ ~ 设置位置 才有效
		}
		
	}
	return ret;
}

void* FreeCamera::_setCameraPosTick(void* a1, void* a2, void* a3) {
	//setCameraPosTickcall
	static auto fc = Game::GetModuleManager()->GetModule<FreeCamera*>();
	if (fc && fc->isEnabled() && UnlockCamera) {
		auto posinfo = getCameraPos(**(void***)a1, (DWORD*)((uintptr_t)a1 + 8));

		static auto lp = Game::Cinstance->getCILocalPlayer();
		if (lp && posinfo) {
			posinfo->rot = *lp->getRotationEx(); // 必须执行一次 /camera CNGEGE set minecraft:free rot 50 50 设置方向的命令才有效 posinfo才不为0

			auto screen = Game::Cinstance->getTopScreenName().to_string();
			if (screen.rfind("hud_screen") != std::string::npos) {
				if (Game::IsKeyDown(VK_UP)) {
					posinfo->pos.x -= speed;
				}
				if (Game::IsKeyDown(VK_DOWN)) {
					posinfo->pos.x += speed;
				}
				if (Game::IsKeyDown(VK_LEFT)) {
					posinfo->pos.z += speed;
				}
				if (Game::IsKeyDown(VK_RIGHT)) {
					posinfo->pos.z -= speed;
				}
				if (Game::IsKeyDown(VK_SPACE)) {
					posinfo->pos.y += speed;
				}
				if (Game::IsKeyDown(VK_SHIFT)) {
					posinfo->pos.y -= speed;
				}
			}
		}

		
	}
	return setCameraPosTickcall(a1, a2, a3);
}

FreeCamera::FreeCamera() : Module(0, "FreeCamera", "自由相机控制")
{
	auto memcodeA1 = "E8 ? ? ? ? 48 85 C0 74 ? 80 78 04 00 74 ? 4C";									// 1.20.32.03
	auto memcodeA2 = "E8 ? ? ? ? 48 8B F8 48 85 C0 0F 84 ? ? ? ? 80 78 ? 00 0F 84 ? ? ? ? 8B";		// 1.20.32.03
	auto memcodeA3 = "E8 ? ? ? ? 48 8B D8 48 85 C0 0F 84 ? ? ? ? 48 89 74 24 ? 4C";					// 1.20.32.03

	// 找第一个Hookcall
	lockCameraTickoffset = FindSignature(memcodeA1);
	if (!lockCameraTickoffset) lockCameraTickoffset = FindSignature(memcodeA2);
	if (!lockCameraTickoffset) lockCameraTickoffset = FindSignature(memcodeA3);
	if (!lockCameraTickoffset) {
		logF("[FreeCamera::FreeCamera] lockCameraTickoffset 特征码失效");
		return;
	}
	lockCameraTickoffset = Utils::FuncFromSigOffset(lockCameraTickoffset, 1);

	// 找第二个Hookcall
	auto memcodeB1 = "48 89 5C 24 ? 57 48 83 EC ? 48 8B 01 48 8B FA 48 8D";							// 1.20.32.03
	setCameraPosTickoffset = FindSignature(memcodeB1);
	if (!setCameraPosTickoffset) {
		logF("[FreeCamera::FreeCamera] setCameraPosTickoffset 特征码失效");
		return;
	}

	// 找第三个call
	auto getCameraPosPtrCall = Utils::FindSignatureRelay(setCameraPosTickoffset, "E8", 100);
	if (!getCameraPosPtrCall) {
		logF("[FreeCamera::FreeCamera] 在setCameraPosTickoffset指针内查找 getCameraPosPtrCall 失败, 100 字节内 没有 E8 汇编码");
		return;
	}
	getCameraPos = Utils::FuncFromSigOffset<Fn_getCameraPosPtr>(getCameraPosPtrCall, 1);
	
	// Hook
	MH_CreateHookEx((LPVOID)lockCameraTickoffset, &FreeCamera::_LockCameraTick, &LockCameraTickcall);
	MH_CreateHookEx((LPVOID)setCameraPosTickoffset, &FreeCamera::_setCameraPosTick, &setCameraPosTickcall);
	

	AddBoolUIValue("解绑相机(Debug)", &UnlockCamera);
	AddFloatUIValue("设置相机移动速度", &speed, 0.01f, 5.f);
}

FreeCamera::~FreeCamera() {
	// 关闭Hook
	if (lockCameraTickoffset) MH_DisableHook((LPVOID)lockCameraTickoffset);
	if (setCameraPosTickoffset) MH_DisableHook((LPVOID)setCameraPosTickoffset);
}