#pragma once
#include <cstdint>
#include "..\Mod\Utils\HMath.h"

class ClientInstance {
public:
	char firstPad[0x90];  //0x0008

	//MinecraftGame* minecraftGame;  //0x0098
	__int64* minecraftGame;

	//MinecraftGame* N0000080C;  //0x00A0
	__int64* N0000080C;

	//MinecraftGame* N0000080D;  //0x00A8
	__int64* N0000080D;

	__int64* minecraft;  //0x00B0

	char pad_0x0068[0x8];  //0x00B8

	__int64* levelRenderer;  //0x00C0

	char pad_0x0078[0x8];  //0x00C8

	//LoopbackPacketSender* loopbackPacketSender;  //0x00D0
	__int64* loopbackPacketSender;  //0x00D0

	char pad_0x0088[0x18];  //0x00D8

	//PtrToGameSettings1* ptr;  //0x00F0
	__int64* ptr;

	/*
	inline void grabMouse() {
		using GrabMouse = void* (__thiscall*) (ClientInstance*);
		static auto _GrabMouse = (GrabMouse)nullptr;

		if (_GrabMouse == nullptr) {
			_GrabMouse = (GrabMouse)(Utils::getVtable(this)[271]);
		}

		_GrabMouse(this);
	};
	*/

	/*
	inline void releaseMouse() {
		using ReleaseMouse = void* (__thiscall*) (ClientInstance*);
		static auto _ReleaseMouse = (ReleaseMouse)nullptr;

		if (_ReleaseMouse == nullptr)
			if (_ReleaseMouse == nullptr) {
				_ReleaseMouse = (ReleaseMouse)(Utils::getVtable(this)[272]);
			}

		_ReleaseMouse(this);
	};
	*/

	inline class LocalPlayer* LocalPlayer() {
		if (this != nullptr) {
			__int64 ptr = *reinterpret_cast<__int64*>(reinterpret_cast<__int64>(this) + 0xD0);
			if (ptr > 0x1000000000 && ptr < 0xF00000000000) {
				class LocalPlayer* res = *reinterpret_cast<class LocalPlayer**>(ptr + 0x278);
				if (reinterpret_cast<__int64>(res) < 0xF00000000000) return res;
			}
		}
		return nullptr;
	}

	inline class C_GuiData* getGuiData() {
		return *(class C_GuiData**)((uintptr_t)(this) + 0x4D0);
	}

	inline class MinecraftGame* MinecraftGame() {
		if (this != nullptr) {
			return *(class MinecraftGame**)((uintptr_t)(this) + 0xA8);
		}
		else {
			return nullptr;
		}
	}

	vec2_t* getMousePos() {
		return *(vec2_t**)((uintptr_t)(this) + 0x448);
	}

	//inline GameSettingsInput* getGameSettingsInput() { return this->ptr->ptr->ptr->settingsInput; };
};