#include "ShowCoordinates.h"
#pragma execution_character_set("utf-8")

ShowCoordinates::ShowCoordinates() : Module(0, "ShowCoordinates", "开启后强制显示左上角坐标") {
	setEnabled(true);
}