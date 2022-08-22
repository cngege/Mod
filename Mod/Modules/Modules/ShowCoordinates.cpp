#include "ShowCoordinates.h"

ShowCoordinates::ShowCoordinates() : Module(0, "ShowCoordinates", "开启后强制显示左上角坐标") {
	setEnabled(true);
}