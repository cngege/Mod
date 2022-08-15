#include "LocalPlayer.h"

LocalPlayer* LocalPlayer::localPlayer = nullptr;

auto LocalPlayer::SetLocalPlayer(LocalPlayer* lp) -> void {
	localPlayer = lp;
}

auto LocalPlayer::GetLocalPlayer()->LocalPlayer* {
	return localPlayer;
}

auto LocalPlayer::onLocalPlayerTick()->void {
	localPlayer = this;
}