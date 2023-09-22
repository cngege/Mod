#include "LockControlInput.h"

bool disableSignal = false;

LockControlInput::LockControlInput() : Module(VK_F12, "LockControlInput", "锁定移动控制键") {
	setcontrolkeysbind({ VK_CONTROL });

	AddBoolUIValue("锁定下蹲", &ControlKeyStatus.Sneak);
	AddBoolUIValue("锁定跳跃", &ControlKeyStatus.Jump);
	AddBoolUIValue("锁定疾跑", &ControlKeyStatus.Sprinting);
	AddBoolUIValue("锁定W", &ControlKeyStatus.W);
	AddBoolUIValue("锁定S", &ControlKeyStatus.S);
	AddBoolUIValue("锁定A", &ControlKeyStatus.A);
	AddBoolUIValue("锁定D", &ControlKeyStatus.D);
}

auto LockControlInput::onDisable() -> void{
	disableSignal = true;
}

auto LockControlInput::onloadConfigFile(json& data)->void {
	//setEnabled(config::readDataFromJson<bool>(data, "enable", true));
	ControlKeyStatus.Sneak = config::readDataFromJson<bool>(data, "ControlKeyStatus_Sneak", false);
	ControlKeyStatus.Jump = config::readDataFromJson<bool>(data, "ControlKeyStatus_Jump", false);
	ControlKeyStatus.Sprinting = config::readDataFromJson<bool>(data, "ControlKeyStatus_Sprinting", false);
	ControlKeyStatus.W = config::readDataFromJson<bool>(data, "ControlKeyStatus_W", false);
	ControlKeyStatus.S = config::readDataFromJson<bool>(data, "ControlKeyStatus_S", false);
	ControlKeyStatus.A = config::readDataFromJson<bool>(data, "ControlKeyStatus_A", false);
	ControlKeyStatus.D = config::readDataFromJson<bool>(data, "ControlKeyStatus_D", false);
}
auto LockControlInput::onsaveConfigFile(json& data)->void {
	//data["enable"] = isEnabled();
	data["ControlKeyStatus_Sneak"] = ControlKeyStatus.Sneak;
	data["ControlKeyStatus_Jump"] = ControlKeyStatus.Jump;
	data["ControlKeyStatus_Sprinting"] = ControlKeyStatus.Sprinting;
	data["ControlKeyStatus_W"] = ControlKeyStatus.W;
	data["ControlKeyStatus_S"] = ControlKeyStatus.S;
	data["ControlKeyStatus_A"] = ControlKeyStatus.A;
	data["ControlKeyStatus_D"] = ControlKeyStatus.D;
}

auto LockControlInput::ControlTick(ControlKey* keyStatus)->void {
	if (isEnabled()) {
		if (ControlKeyStatus.Sneak) keyStatus->Sneak = ControlKeyStatus.Sneak;
		if (ControlKeyStatus.Jump) keyStatus->Jump = ControlKeyStatus.Jump;
		if (ControlKeyStatus.Sprinting) keyStatus->Sprinting = ControlKeyStatus.Sprinting;
		if (ControlKeyStatus.W) keyStatus->W = ControlKeyStatus.W;
		if (ControlKeyStatus.S) keyStatus->S = ControlKeyStatus.S;
		if (ControlKeyStatus.A) keyStatus->A = ControlKeyStatus.A;
		if (ControlKeyStatus.D) keyStatus->D = ControlKeyStatus.D;
	}
	else {
		if (disableSignal) {
			disableSignal = false;
			memset(keyStatus, '\0', 14);
		}
	}
}