#include "HundredTimesMoreDrops.h"

HundredTimesMoreDrops::HundredTimesMoreDrops() : Module(0, "HundredTimesMoreDrops", "百倍掉落物品") {
	AddIntUIValue("掉落倍数", &multiple, 1, 200);
	// DEBUG
	setEnabled(true);
}


auto HundredTimesMoreDrops::onloadConfigFile(json& data)->void {
	multiple = config::readDataFromJson<int>(data, "multiple", 100);
}
auto HundredTimesMoreDrops::onsaveConfigFile(json& data)->void {
	data["multiple"] = multiple;
}