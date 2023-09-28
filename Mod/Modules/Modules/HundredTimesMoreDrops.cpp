#include "HundredTimesMoreDrops.h"

HundredTimesMoreDrops::HundredTimesMoreDrops() : Module(0, "HundredTimesMoreDrops", "百倍掉落物品") {
	AddIntUIValue("掉落倍数", &multiple, 1, 200);
	AddBoolUIValue("仅本地玩家", &onlyLocalPlayer);
}


auto HundredTimesMoreDrops::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", false));
	multiple = config::readDataFromJson<int>(data, "multiple", 100);
	onlyLocalPlayer = config::readDataFromJson<bool>(data, "onlyLocalPlayer", false);
}
auto HundredTimesMoreDrops::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
	data["multiple"] = multiple;
	data["onlyLocalPlayer"] = onlyLocalPlayer;
}