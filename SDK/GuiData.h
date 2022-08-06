#pragma once
#include "..\Mod\Utils\HMath.h"
#include <cstdint>

class C_GuiData {
public:
private:
	char pad_0x0000[0x18];  //0x0000
public:
	union {
		struct {
			float widthReal;   //0x0018
			float heightReal;  //0x001C
		};
		vec2_t windowSizeReal;  //0x0018
	};

	float widthReal2;   //0x0020
	float heightReal2;  //0x0024
	union {
		struct {
			float widthGame;   //0x0028
			float heightGame;  //0x002C
		};
		vec2_t windowSize;  //0x0028
	};

	float getGuiScale() {
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0x34);
	};

	uint16_t getMouseX() {
		return *reinterpret_cast<uint16_t*>(reinterpret_cast<uintptr_t>(this) + 0x52);
	};

	uint16_t getMouseY() {
		return *reinterpret_cast<uint16_t*>(reinterpret_cast<uintptr_t>(this) + 0x54);
	};
};