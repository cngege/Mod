#include "HiveTreasurePos.h"
#include "../../Utils/Game.h"
#include "Actor.h"
#include "LocalPlayer.h"

HiveTreasurePos::HiveTreasurePos() : Module(VK_F7, "HiveTreasurePos", "在 HIVE 起床战争中,把附近小的宝箱碰撞箱拉过来") {
	SetKeyMode(KeyMode::Switch);
}

#include "../../Utils/Logger.h"
auto HiveTreasurePos::onActorTick(Actor* actor)->void {
	if (Game::localplayer == nullptr) {
		return;
	}

	if (isEnabled() && Game::localplayer->isSneaking()) {
		if (actor->getEntityTypeId() == ActorType::Hive_Treasure) {
			if (actor->getHitBox().x == 0.800000f) {					//0.800000 两边小宝箱，  2.400000 中间大宝箱  类型ID都是一样的
				vec3_t lppos(Game::localplayer->getPosition()->x, Game::localplayer->getPosEx().y, Game::localplayer->getPosition()->z);
				if (lppos.CoordinateDistance(*actor->getPosition()) <= 5.f) {
					actor->setPos(&lppos);
				}
			}
		}
	}
}