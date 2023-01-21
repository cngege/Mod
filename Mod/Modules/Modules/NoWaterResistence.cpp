#include "NoWaterResistence.h"
#include "LocalPlayer.h"

NoWaterResistence::NoWaterResistence() : Module(0, "NoWaterResistence", "无视水的阻力在水下行走") {

}

auto NoWaterResistence::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", false));
}
auto NoWaterResistence::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}
