#include "ChangeGameMode.h"
//程序模块
#include "Game.h"
//SDK
#include "ClientInstance.h"
#include "LocalPlayer.h"

ChangeGameMode::ChangeGameMode() : Module(0, "ChangeGameMode", "切换游戏模式,开启后可使用快捷键(快捷键仅地图游戏界面有效)") {
	//SetKeyMode(KeyMode::Trigger);

	AddButtonUIEvent("生存[Z]", false, []() {
		if (Game::Cinstance) {
			auto lp = Game::Cinstance->getCILocalPlayer();
			if (lp) {
				lp->setPlayerGameType(0);
			}
		}
		});
	AddButtonUIEvent("创造[X]", true, []() {
		if (Game::Cinstance) {
			auto lp = Game::Cinstance->getCILocalPlayer();
			if (lp) {
				lp->setPlayerGameType(1);
			}
		}
		});
	AddButtonUIEvent("冒险[C]", true, []() {
		if (Game::Cinstance) {
			auto lp = Game::Cinstance->getCILocalPlayer();
			if (lp) {
				lp->setPlayerGameType(2);
			}
		}
		});
	AddButtonUIEvent("旁观[V]", true, []() {
		if (Game::Cinstance) {
			auto lp = Game::Cinstance->getCILocalPlayer();
			if (lp) {
				lp->setPlayerGameType(6);
			}
		}
		});
	AddButtonUIEvent("默认[B]", true, []() {
		if (Game::Cinstance) {
			auto lp = Game::Cinstance->getCILocalPlayer();
			if (lp) {
				lp->setPlayerGameType(5);
			}
		}
		});

}

auto ChangeGameMode::onKeyUpdate(int key, bool isdown) -> void
{
	if (isEnabled() && isdown && Game::Cinstance) {
		auto screen = Game::Cinstance->getTopScreenName().to_string();
		if (screen.rfind("hud_screen") != std::string::npos) {
			auto lp = Game::Cinstance->getCILocalPlayer();
			if (lp) {
				if (key == 'Z') {
					// TUDO 生存
					lp->setPlayerGameType(0);
				}
				else if (key == 'X') {
					// TUDO: 
					lp->setPlayerGameType(1);
				}
				else if (key == 'C') {
					// TUDO: 
					lp->setPlayerGameType(2);
				}
				else if (key == 'V') {
					// TUDO: 
					lp->setPlayerGameType(6);
				}
				else if (key == 'B') {
					// TUDO: 
					lp->setPlayerGameType(5);
				}
			}
		}
	}
}
