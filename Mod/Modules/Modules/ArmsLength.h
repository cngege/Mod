#pragma once
#include "../Module.h"

class ArmsLength : public Module {
public:
	ArmsLength();

private:
	//float* arms = nullptr;
	//float distance = 7;


	BYTE sJumpaddr = 0;							// 未作修改是jmp地址的值，以用作还原
	BYTE* jumpaddr = nullptr;
public:
	virtual auto onEnable()->void override;
	virtual auto onDisable()->void override;
	virtual auto isEnabled()->bool override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};