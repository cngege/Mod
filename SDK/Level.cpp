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
	return Utils::CallVFunc<111, int>(this);											//更新自 1.20.30
}

auto Level::setTime(int time)->void {
	return Utils::CallVFunc<112, void,int>(this, time);						//更新自 1.20.30
}

auto Level::getSeed() -> unsigned int
{
	return Utils::CallVFunc<113, unsigned int>(this);								//更新自 1.20.30
}

// 虚表Hook

auto Level::startLeaveGame() -> void
{
	using Fn = void(__fastcall*)(Level*);
	reinterpret_cast<Fn>(startLeaveGameCall)(this);
}

auto Level::Tick() -> void*
{
	try
	{
		using Fn = void* (__fastcall*)(Level*);
		return reinterpret_cast<Fn>(tickCall)(this);
	}
	catch (const std::exception&)
	{}
	return nullptr;
}

// 207 Level::forEachPlayer(class std::function<bool __cdecl(class Player const & __ptr64)>)const 取这个
// 208 Level::forEachPlayer(class std::function<bool __cdecl(class Player const & __ptr64)>)
// 虚表 检查版本 1.20.30
auto Level::forEachPlayer(std::function<bool(class Player&)> fp)->void {
	return Utils::CallVFunc<203, void, std::function<bool(class Player&)>>(this, fp);
	//return reinterpret_cast<void(__fastcall*)(Level*, std::function<bool(class Player&)>)>((*(uintptr_t**)this)[207])(this, fp);
}

auto Level::isClientSide() -> bool
{
	return Utils::CallVFunc<287, bool>(this);								//更新自 1.20.30
}

auto Level::setSimPaused(bool v) -> void
{
	return Utils::CallVFunc<319, void, bool>(this, v);				//更新自 1.20.30
}

auto Level::getSimPaused() -> bool
{
	return Utils::CallVFunc<320, bool>(this);								//更新自 1.20.30
}
