#include "HundredTimesMoreDrops.h"

HundredTimesMoreDrops::HundredTimesMoreDrops() : Module(0, "HundredTimesMoreDrops", "百倍掉落物品") {
	AddIntUIValue("掉落倍数", &multiple, 1, 99999);
}