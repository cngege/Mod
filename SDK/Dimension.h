﻿#pragma once
//a1->lpVtbl = (struct DimensionVtbl*)&Dimension::`vftable'{for `IDimension'};
//a1[1].lpVtbl = (struct DimensionVtbl*)&Dimension::`vftable'{for `LevelListener'};
//a1[2].lpVtbl = (struct DimensionVtbl*)&OverworldDimension::`vftable'{for `SavedData'};
//a1[8].lpVtbl = (struct DimensionVtbl*)&OverworldDimension::`vftable'{for `Bedrock::EnableNonOwnerReferences'};
#include "BlockSource.h"

class Dimension {
public:
	BlockSource* getBlockSourceEx() {
		return *(BlockSource**)((uintptr_t)this + 208);
	}
};