#include "AirJump.h"
#include "LocalPlayer.h"
#include "ServerPlayer.h"
#include "GameMode.h"
#include "ActorCollision.h"

#include "Game.h"
#include "Logger.h"

AirJump::AirJump() : Module(0, "AirJump", "空气跳") {

}

// GameMode::Tick 被优化内联
auto AirJump::onTick(GameMode* gm)->void {
	if (isEnabled()) {
		LocalPlayer* lp = gm->GetLocalPlayer();
		if (lp != nullptr) {
			//lp->getMovementProxy()->setOnGround(true);
		}
	}
}


auto AirJump::onLocalPlayerTick(LocalPlayer* lp)->void
{
	if (isEnabled()) {
		//lp->getMovementProxy()->setOnGround(true);
		lp->getActorCollision()->setOnGround(true);
		auto lsp = Game::GetLocalServerPlayer();
		if (lsp) {
			lsp->getActorCollision()->setOnGround(true);
		}
	}
}

auto AirJump::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", false));
}
auto AirJump::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}