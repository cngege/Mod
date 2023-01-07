#pragma execution_character_set("utf-8")
#include "AutoWalking.h"

AutoWalking::AutoWalking() : Module(VK_F7, "AutoWalking", "自动前进") {
	setcontrolkeysbind({ VK_CONTROL });
}
