#pragma once
#include <cstdint>


class Block {
public:
	class BlockLegacy* getBlockLegacy();

	bool isAir();
	bool isAirEx();
};