#include "GameMode.h"


uintptr_t** GameMode::vTables = nullptr;

auto GameMode::SetVtables(uintptr_t** vTable)->void {
	vTables = vTable;
}