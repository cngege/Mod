#include "Player.h"
#include "../Mod/Utils/Utils.h"

int Player::YView1 = 0;
int Player::XView1 = 0;
int Player::YView2 = 0;
int Player::XView2 = 0;


uintptr_t** Player::vfTables = nullptr;

template <typename TRet, typename... TArgs>
auto Player::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto Player::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto Player::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto Player::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
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

