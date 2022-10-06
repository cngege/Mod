#include "Module.h"
#include "../Utils/Game.h"
#include "../Utils/Utils.h"

Module::Module(int key, std::string name, std::string info) {
	this->keybind = key;
	this->modulename = name;
	this->moduleinfo = info;
}

Module::~Module() {

}

auto Module::SetKeyMode(KeyMode km)->void {
	this->km = km;
}

auto Module::GetKeyMode()->KeyMode {
	return this->km;
}

auto Module::getKeybind()->int {
	return this->keybind;
}

auto Module::setKeybind(int key)->void {
	this->keybind = key;
}

auto Module::getcontrolkeysbind()->std::vector<int> {
	return this->controlkeys;
}

auto Module::setcontrolkeysbind(std::vector<int> keys)->void{
	this->controlkeys = keys;
}

//检查模块要求的功能键是否全都按下
auto Module::checkcontrolkeys()->bool {
	bool alldown = true;
	for (auto key : getcontrolkeysbind()) {
		if (!Game::IsKeyDown(key)) {
			alldown = false;
		}
	}
	return alldown;
}

auto Module::getModuleName()->std::string {
	return this->modulename;
}

auto Module::getModuleInfo()->std::string {
	return this->moduleinfo;
}

auto Module::getBindKeyName()->std::string {
	//ret : CTRL + SHIFT + F
	std::string name = "";
	for (auto key : this->getcontrolkeysbind()) {
		name += KeyNames[key];
		name += " + ";
	}
	if (this->getKeybind() != NULL) {
		name += Utils::getKeybindName(this->getKeybind());
	}
	return name;
}

auto Module::isEnabled()->bool {
	return this->enabled;
}

auto Module::setEnabled(bool enabled)->void {
	if (this->enabled != enabled) {
		this->enabled = enabled;
		if (enabled) {
			onEnable();
		}
		else {
			onDisable();
		}
	}
	
}

auto Module::toggle()->void {
	setEnabled(!this->enabled);
}


auto Module::onTick(GameMode* gm)->void {

}

auto Module::onKeyUpdate(int key, bool isenabled)->void {

}

auto Module::onMouseUpdate(char mousebutton, char isdown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY)->void {

}

auto Module::onTrigger()->void {

}

auto Module::onEnable()->void {

}

auto Module::onDisable()->void {

}

auto Module::onAttack(Actor* actor)->bool {
	return true;
}

auto Module::useItem(GameMode* gm, class ItemStack* item)->bool {
	return true;
}

auto Module::useItemOn(GameMode* gm, class ItemStack* item, vec3_ti* bpos, uint8_t* face, vec3_t* f, class Block* block)->bool {
	return true;
}

auto Module::onKnockback(LocalPlayer* lp, vec3_t* v3)->bool {
	return true;
}

auto Module::onActorTick(Actor* actor)->void {

}

auto Module::onLocalPlayerTick(LocalPlayer* lp)->void {

}

auto Module::onRenderDetour(MinecraftUIRenderContext* ctx)->void {

}

auto Module::onImGUIRender()->void {

}

auto Module::onSendMessage(TextHolder*)->bool {
	return true;
}