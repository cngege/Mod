#include "AirWater.h"
#include "NoWaterResistence.h"
#include "Game.h"
#include "../ModuleManager.h"

AirWater::AirWater() : Module(0, "AirWater", "像水中般遨游在空气中") {

}

auto AirWater::onEnable()->void
{
	static NoWaterResistence* nwr = Game::GetModuleManager()->GetModule<NoWaterResistence*>();
	if (nwr->isEnabled()) {
		nwr->setEnabled(false);
	}
}


auto AirWater::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", false));
}
auto AirWater::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}