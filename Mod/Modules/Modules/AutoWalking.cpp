#include "AutoWalking.h"

AutoWalking::AutoWalking() : Module(VK_F7, "AutoWalking", "自动前进") {
	setcontrolkeysbind({ VK_CONTROL });
}
