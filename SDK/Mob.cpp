#include "Mob.h"

uintptr_t** Mob::vTables = nullptr;

auto Mob::GetVtableFun(int a)->uintptr_t* {
	return vTables[a];
}

auto Mob::GetVtables()->uintptr_t** {
	return vTables;
}

auto Mob::SetVtables(uintptr_t** vTable)->void {
	vTables = vTable;
}
