#include "Module.h"

Module::Module(int key, std::string name, std::string info) {
	this->keybind = key;
	this->modulename = name;
	this->moduleinfo = info;
}

Module::~Module() {

}

auto Module::getKeybind()->int {
	return this->keybind;
}

auto Module::setKeybind(int key)->void {
	this->keybind = key;
}

auto Module::getModuleName()->std::string {
	return this->modulename;
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

auto Module::onEnable()->void {

}

auto Module::onDisable()->void {

}

auto Module::onAttack(Actor* actor)->bool {
	return true;
}

auto Module::onKnockback(LocalPlayer* lp, vec3_t* v3)->bool {
	return true;
}

auto Module::onStartDestroyBlock(GameMode* gm ,vec3_ti* Bpos, uint8_t* Face)->void {

}