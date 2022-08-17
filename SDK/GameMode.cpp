#include "GameMode.h"


uintptr_t** GameMode::vTables = nullptr;

uintptr_t* GameMode::startDestroyBlockCall = nullptr;

auto GameMode::GetVtableFun(int a)->uintptr_t* {
	return vTables[a];
}

auto GameMode::SetVtables(uintptr_t** vTable)->void {
	vTables = vTable;
}




//Hook后虚表函数的实现
auto GameMode::startDestroyBlock(vec3_ti* Bpos, uint8_t* Face, void* a1,void* a2)->bool {
	using Fn = bool(__fastcall*)(GameMode*, vec3_ti*, uint8_t*,void*,void*);
	return reinterpret_cast<Fn>(startDestroyBlockCall)(this, Bpos, Face,a1,a2);
}


//虚表函数实现

//破坏方块
auto GameMode::destroyBlock(vec3_ti* Bpos, uint8_t* Face)->bool {
	using Fn = bool(__fastcall*)(GameMode*, vec3_ti*, uint8_t*);
	return reinterpret_cast<Fn>(vTables[2])(this, Bpos, Face);
}
