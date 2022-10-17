#include "Level.h"

uintptr_t* Level::forEachPlayerCall = nullptr;


auto Level::getAllPlayer()->std::vector<Player*> {
	try {
		std::vector<Player*> player_list;
		this->forEachPlayer([&](Player& sp) {
			Player* player = &sp;
			player_list.push_back(player);
			return true;
		});
		return player_list;
	}catch (...) {
		return {};
	}
}

auto Level::forEachPlayer(std::function<bool(class Player&)> fp)->void {
	return reinterpret_cast<void(__fastcall*)(Level*, std::function<bool(class Player&)>)>(forEachPlayerCall)(this, fp);
}

auto Level::getTime()->int {
	return reinterpret_cast<int(__fastcall*)(Level*)>((*(uintptr_t**)this)[111])(this);
}

auto Level::setTime(int time)->void {
	return reinterpret_cast<void(__fastcall*)(Level*,int)>((*(uintptr_t**)this)[112])(this,time);
}

