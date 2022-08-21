#include "NoKnockback.h"


NoKnockback::NoKnockback() : Module(VK_CONTROL, "NoKnockback", "按下快捷键后不会被击退") {
	SetKeyMode(KeyMode::Hold);
}