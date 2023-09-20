#include "Block.h"


auto Block::getBlockLegacy()->class BlockLegacy* {
	return *(BlockLegacy**)((uintptr_t)this + 48);	
}