#include "HiveTreasureHitBox.h"
#include "Actor.h"

HiveTreasureHitBox::HiveTreasureHitBox() : Module(VK_SHIFT, "HiveTreasureHitBox", "在 HIVE 起床战争中,让小的宝箱碰撞箱变大") {
	SetKeyMode(KeyMode::Hold);
}

#include "../../Utils/Logger.h"
auto HiveTreasureHitBox::onActorTick(Actor* actor)->void {
	if (isEnabled()) {
		if (actor->getEntityTypeId() == ActorType::Hive_Treasure) {
			if (actor->getHitBox().x == 0.800000f) {							//0.800000 两边小宝箱，  2.400000 中间大宝箱  类型ID都是一样的
				actor->setHitBox(vec2_t(7.0f, 7.0f));
			}
		}
	}
	else {
		if (actor->getEntityTypeId() == ActorType::Hive_Treasure) {
			if (actor->getHitBox().x == 7.0f) {
				actor->setHitBox(vec2_t(0.800000f, 0.800000f));
			}
		}
	}
}