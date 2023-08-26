#include "Level.h"
#include "Utils.h"

uintptr_t* Level::startLeaveGameCall = nullptr;
//uintptr_t* Level::forEachPlayerCall = nullptr;
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
	return Utils::CallVFunc<112, int>(this);
	//return reinterpret_cast<int(__fastcall*)(Level*)>((*(uintptr_t**)this)[112])(this);
}

auto Level::setTime(int time)->void {
	return Utils::CallVFunc<113, void,int>(this, time);
	//return reinterpret_cast<void(__fastcall*)(Level*,int)>((*(uintptr_t**)this)[113])(this,time);
}

auto Level::getSeed() -> unsigned int
{
	return Utils::CallVFunc<114, unsigned int>(this);
	//return reinterpret_cast<unsigned int(__fastcall*)(Level*)>((*(uintptr_t**)this)[114])(this);
}

// 虚表Hook

auto Level::startLeaveGame() -> void
{
	using Fn = void(__fastcall*)(Level*);
	reinterpret_cast<Fn>(startLeaveGameCall)(this);
}

auto Level::Tick() -> void
{
	using Fn = float(__fastcall*)(Level*);
	reinterpret_cast<Fn>(tickCall)(this);
}

// 207 Level::forEachPlayer(class std::function<bool __cdecl(class Player const & __ptr64)>)const
// 208 Level::forEachPlayer(class std::function<bool __cdecl(class Player const & __ptr64)>)
// 虚表 检查版本 1.20
auto Level::forEachPlayer(std::function<bool(class Player&)> fp)->void {
	return Utils::CallVFunc<207, void, std::function<bool(class Player&)>>(this, fp);
	//return reinterpret_cast<void(__fastcall*)(Level*, std::function<bool(class Player&)>)>((*(uintptr_t**)this)[207])(this, fp);
}

// 虚表 检查版本 1.20.15
auto Level::isClientSide() -> bool
{
	return Utils::CallVFunc<291, bool>(this);
	//return reinterpret_cast<bool(__fastcall*)(Level*)>((*(uintptr_t**)this)[291])(this);
}

auto Level::setSimPaused(bool v) -> void
{
	return Utils::CallVFunc<322, void, bool>(this, v);
}

auto Level::getSimPaused() -> bool
{
	return Utils::CallVFunc<323, bool>(this);
}
