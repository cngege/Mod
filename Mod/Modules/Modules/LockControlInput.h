#pragma once
#include "../Module.h"

class LockControlInput : public Module {
public:
	LockControlInput();

public:
	struct ControlKey
	{
		bool Sneak;							// 潜行
		BYTE __unknowkey1;
		bool ProhibitionOfFloating;			// 禁止上浮(在水中无法上浮和创造模式自动下沉)
		BYTE __unknowkey3;
		BYTE __unknowkey4;
		BYTE __unknowkey5;
		bool Jump;
		bool Sprinting;
		bool WA;							// 向左前移动
		bool WD;							// 向右前移动
		bool W;
		bool S;
		bool A;
		bool D;
		BYTE __unknowkey6[2];
		BYTE __unknowkey7[16];
		BYTE __unknowkey8[8];
		float HorizontalRocker;				// 水平的值（左右 左1  右-1  好像是只读
		float VerticalRocker;				// 垂直的值 (前后 前1  后-1  好像是只读
	};

public:
	virtual auto onDisable() -> void override;
	virtual auto onloadConfigFile(json& data) -> void override;
	virtual auto onsaveConfigFile(json& data) -> void override;

public:
	ControlKey ControlKeyStatus{};
	void ControlTick(ControlKey*);
};