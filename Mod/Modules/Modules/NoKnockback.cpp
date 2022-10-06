#include "NoKnockback.h"
#pragma execution_character_set("utf-8")

NoKnockback::NoKnockback() : Module(VK_CONTROL, "NoKnockback", "按下快捷键后不会被击退") {
	SetKeyMode(KeyMode::Hold);
}

auto NoKnockback::onKnockback(LocalPlayer* lp, vec3_t* v3)->bool {
	if (isEnabled()) {
		return false;
	}
	return true;
}