#include "Player.h"
#include "../Mod/Utils/Utils.h"

int Player::YView1 = 0;
int Player::XView1 = 0;
int Player::YView2 = 0;
int Player::XView2 = 0;


uintptr_t** Player::vTables = nullptr;

auto Player::GetVtableFun(int a)->uintptr_t* {
	return vTables[a];
}

auto Player::SetVtables(uintptr_t** vTable)->void {
	vTables = vTable;
}


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

