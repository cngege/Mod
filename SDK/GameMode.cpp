#include "GameMode.h"
#include "Actor.h"

uintptr_t** GameMode::vfTables = nullptr;

uintptr_t* GameMode::startDestroyBlockCall = nullptr;
uintptr_t* GameMode::attackCall = nullptr;
uintptr_t* GameMode::tickCall = nullptr;

template <typename TRet, typename... TArgs>
auto GameMode::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto GameMode::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto GameMode::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto GameMode::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}




//Hook后虚表函数的实现
auto GameMode::startDestroyBlock(vec3_ti* Bpos, uint8_t* Face, void* a1,void* a2)->bool {
	using Fn = bool(__fastcall*)(GameMode*, vec3_ti*, uint8_t*,void*,void*);
	return reinterpret_cast<Fn>(startDestroyBlockCall)(this, Bpos, Face,a1,a2);
}

auto GameMode::tick()->void* {
	using Fn = void*(__fastcall*)(GameMode*);
	return reinterpret_cast<Fn>(tickCall)(this);
}

auto GameMode::attack(Actor* actor) ->bool {
	using Fn = bool(__fastcall*)(GameMode*, Actor*);
	return reinterpret_cast<Fn>(attackCall)(this, actor);
}


//虚表函数实现

//破坏方块
auto GameMode::destroyBlock(vec3_ti* Bpos, uint8_t* Face)->bool {
	return GetVFtableFun<bool, GameMode*, vec3_ti*, uint8_t*>(2)(this, Bpos, Face);
}
