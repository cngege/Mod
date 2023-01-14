#include "ShowCoordinates.h"

ShowCoordinates::ShowCoordinates() : Module(0, "ShowCoordinates", "开启后强制显示左上角坐标") {
	//setEnabled(true);
}

auto ShowCoordinates::onloadConfigFile(json& data)->void {
	setEnabled(config::readDataFromJson<bool>(data, "enable", true));
}
auto ShowCoordinates::onsaveConfigFile(json& data)->void {
	data["enable"] = isEnabled();
}