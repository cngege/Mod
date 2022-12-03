#include "Level.h"

uintptr_t* Level::forEachPlayerCall = nullptr;
uintptr_t* Level::tickCall = nullptr;


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


auto Level::getTime()->int {
	return reinterpret_cast<int(__fastcall*)(Level*)>((*(uintptr_t**)this)[114])(this);
}

auto Level::setTime(int time)->void {
	return reinterpret_cast<void(__fastcall*)(Level*,int)>((*(uintptr_t**)this)[115])(this,time);
}

// Ðé±íHook

auto Level::Tick() -> void
{
	using Fn = float(__fastcall*)(Level*);
	reinterpret_cast<Fn>(tickCall)(this);
}

auto Level::forEachPlayer(std::function<bool(class Player&)> fp)->void {
	return reinterpret_cast<void(__fastcall*)(Level*, std::function<bool(class Player&)>)>(forEachPlayerCall)(this, fp);
}
