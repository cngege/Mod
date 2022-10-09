#include "RenderHealth.h"
#include "Actor.h"
#include "MinecraftUIRenderContext.h"

#include <string.h>
#pragma execution_character_set("utf-8")

RenderHealth::RenderHealth() : Module(VK_F10, "RenderHealth", "显示被攻击生物的血量和名字") {
	setcontrolkeysbind({ VK_CONTROL });
	setEnabled(true);
}


auto RenderHealth::onAttack(Actor* actor)->bool {
	if (isEnabled()) {
		currentActorVT = *(uintptr_t*)actor;
		currentActor = actor;
		tick = 300.f;
		show = true;
	}
	return true;
}

auto RenderHealth::onRenderDetour(MinecraftUIRenderContext* ctx)->void {
	if (show && isEnabled()) {
		tick--;
		if (tick <= 0) {
			show = false;
			return;
		}
		if (*(uintptr_t*)currentActor == currentActorVT) {				//防止实体移除后，后面还会调用这个实体类中的方法
			currentPlayerHealth = (int)currentActor->getHealth();
			if (currentPlayerHealth != 0) {								//只有在生物血量不为0的时候才更新生物名称
				std::string sname = currentActor->getNameTag()->getText();
				auto find = sname.find("\n");
				if (find == -1) {
					currentPlayerName = currentActor->getNameTag()->getText();
				}
				else {
					currentPlayerName = sname.substr(0, find);
				}
			}
			else {
				currentActorVT = -1;
			}
		}

		UIColor bgcolor = UIColor(0, 0, 0, (tick < 60) ? tick : 60);
		UIColor textcolor = UIColor(255, 255, 255, (tick < 255) ? tick : 255);
		ctx->Fillshape(showpos, vec2_t(130.f,35.f), bgcolor);

		std::string drawName("Name: ");
		drawName += currentPlayerName;
		std::string drawHealth("Health: ");
		drawHealth += std::to_string(currentPlayerHealth);

		ctx->Drawtext(vec2_t(showpos.x + 5.f, showpos.y + 5.f), &drawName, textcolor, 1.f);
		ctx->Drawtext(vec2_t(showpos.x + 5.f, showpos.y + 20.f), &drawHealth, textcolor, 1.f);
		ctx->flushText(0);
	}
}