﻿#include "Actor.h"
#include "LocalPlayer.h"
#include "../Mod/Utils/Utils.h"

int Actor::SpeedXOffset = 0;
int Actor::SpeedYOffset = 0;
int Actor::SpeedZOffset = 0;

int Actor::PosXOffset1 = 0;
int Actor::PosYOffset1 = 0;
int Actor::PosZOffset1 = 0;
int Actor::PosXOffset2 = 0;
int Actor::PosYOffset2 = 0;
int Actor::PosZOffset2 = 0;

int Actor::XHitBoxOffset = 0;
int Actor::YHitBoxOffset = 0;

uintptr_t* Actor::VTable = nullptr;

auto Actor::getSpeed()->vec3_t {
	if (SpeedXOffset == 0 || SpeedYOffset == 0 || SpeedZOffset == 0) {
		return vec3_t(0.0f, 0.0f, 0.0f);
	}
	float* Xspeed = (float*)(this + SpeedXOffset);
	float* Yspeed = (float*)(this + SpeedYOffset);
	float* Zspeed = (float*)(this + SpeedZOffset);
	return vec3_t(*Xspeed, *Yspeed, *Zspeed);
}

auto Actor::setSpeed(vec3_t v) ->void {
	if (SpeedXOffset == 0 || SpeedYOffset == 0 || SpeedZOffset == 0) {
		return;
	}
	float* Xspeed = (float*)(this + SpeedXOffset);
	float* Yspeed = (float*)(this + SpeedYOffset);
	float* Zspeed = (float*)(this + SpeedZOffset);
	*Xspeed = v.x;
	*Yspeed = v.y;
	*Zspeed = v.z;
}

auto Actor::getPos()->vec3_t {
	if (PosXOffset1 == 0) {
		return vec3_t(0.0f, 0.0f, 0.0f);
	}
	float* Xpos = (float*)(this + PosXOffset1);
	float* Ypos = (float*)(this + PosYOffset1);
	float* Zpos = (float*)(this + PosZOffset1);
	return vec3_t(*Xpos, *Ypos, *Zpos);
}

auto Actor::getPos2()->vec3_t {
	if (PosXOffset2 == 0) {
		return vec3_t(0.0f, 0.0f, 0.0f);
	}
	float* Xpos = (float*)(this + PosXOffset2);
	float* Ypos = (float*)(this + PosYOffset2);
	float* Zpos = (float*)(this + PosZOffset2);
	return vec3_t(*Xpos, *Ypos, *Zpos);
}

auto Actor::setPos(vec3_t p)->void {
	if (PosXOffset1 == 0) {				//后面的偏移都是根据这个来的，所以只需要判断这一个就可以
		return;
	}
	float* Xpos1 = (float*)(this + PosXOffset1);
	float* Ypos1 = (float*)(this + PosYOffset1);
	float* Zpos1 = (float*)(this + PosZOffset1);

	float* Xpos2 = (float*)(this + PosXOffset2);
	float* Ypos2 = (float*)(this + PosYOffset2);
	float* Zpos2 = (float*)(this + PosZOffset2);

	*Xpos1 = p.x;
	*Ypos1 = p.y;
	*Zpos1 = p.z;
	*Xpos2 = p.x + 0.5999756f;
	*Ypos2 = p.y + 1.80000305f;
	*Zpos2 = p.z + 0.60002518f;
}

auto Actor::getHitBox()->vec2_t {
	if (XHitBoxOffset == 0 || YHitBoxOffset == 0) {
		return vec2_t(0.0f, 0.0f);
	}
	auto hbx = *(float*)(this + XHitBoxOffset);
	auto hby = *(float*)(this + YHitBoxOffset);
	return vec2_t(hbx, hby);
}

auto Actor::setHitBox(vec2_t hb)->void {
	if (XHitBoxOffset == 0 || YHitBoxOffset == 0) {
		return;
	}
	*(float*)(this + XHitBoxOffset) = hb.x;
	*(float*)(this + YHitBoxOffset) = hb.y;
}

auto Actor::isPlayer()->bool {
	if (!this || !LocalPlayer::GetLocalPlayer()) {
		return false;
	}
	if (*(void**)this == *(void**)LocalPlayer::GetLocalPlayer()) {
		return true;
	}
	return false;
}


auto Actor::resetHitBox()->void {
	Actor::setHitBox(vec2_t(0.6000000238f, 1.799999952f));
}


auto Actor::onMoveBBs(vec3_t p)->void {

}

auto Actor::onAllActorTick()->void {
	//判断是否是玩家 大写锁定
	//if (!this->isPlayer())
	//	return;
	if (GETKEYSTATE(VK_CAPITAL)) {
		if (this != (Actor*)LocalPlayer::GetLocalPlayer()) {
			this->setHitBox(vec2_t(6.0f, 6.0f));
		}
	}
	else {
		this->resetHitBox();
	}
}