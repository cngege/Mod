#include "FastViewPerspective.h"
#include "../../Utils/Game.h"

FastViewPerspective::FastViewPerspective() : Module(VK_F9, "FastViewPerspective", "快速预览第二人称视角") {
	setEnabled(true);
}

auto FastViewPerspective::isToggle()->bool {
	return Game::IsKeyDown('R');
}