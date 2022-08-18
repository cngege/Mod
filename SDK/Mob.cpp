#include "Mob.h"

uintptr_t** Mob::vfTables = nullptr;

auto Mob::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto Mob::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto Mob::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}
