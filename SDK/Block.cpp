#include "Block.h"
#include "Utils.h"
#include "BlockLegacy.h"

auto Block::getBlockLegacy()->class BlockLegacy* {
	return *(BlockLegacy**)((uintptr_t)this + 48);	//1.20.41
}

bool Block::isAir()
{
	auto blockLegay = getBlockLegacy();
	_ASSERT(blockLegay);
	return blockLegay->isAir();
}

bool Block::isAirEx()
{
	static SignCode sign("Block::isAirEx");
	sign << "48 8B 48 ? 48 85 C9 0F 84 ? ? ? ? 48 8B ? 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 84 ? 0F 85 ? ? ? ? 0F";
	//                ^                                         ^
	_ASSERT(sign);
	int offsetA = (int)*reinterpret_cast<BYTE*>(*sign + 3);
	int offsetB = *reinterpret_cast<int*>(*sign + 19);

	uintptr_t v1 = *(uintptr_t*)((uintptr_t)this + offsetA);//getBlockLegacy
	return reinterpret_cast<bool(__fastcall*)(uintptr_t)>(*(uintptr_t*)(*(uintptr_t*)v1 + offsetB))(v1);//BlockLegacy::isAir
}
