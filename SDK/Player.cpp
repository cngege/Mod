#include "Player.h"
#include "../Mod/Utils/Utils.h"

Player* Player::LocalPlayer = nullptr;

int Player::YView1 = 0;
int Player::XView1 = 0;
int Player::YView2 = 0;
int Player::XView2 = 0;

auto Player::getViewYX()->const vec2_t {
	if (YView1 == 0 || XView1 == 0) {
		return vec2_t(0.0f, 0.0f);
	}
	auto viewy = *(float*)(this + YView1);
	auto viewx = *(float*)(this + XView1);
	return vec2_t(viewy, viewx);
}

auto Player::getViewYX2()->const vec2_t {
	if (YView2 == 0 || XView2 == 0) {
		return vec2_t(0.0f, 0.0f);
	}
	auto viewy = *(float*)(this + YView2);
	auto viewx = *(float*)(this + XView2);
	return vec2_t(viewy, viewx);
}

auto Player::onLocalPlayerTick()->void {
	LocalPlayer = this;
}
