#pragma once

#include "mcstring.h"

class Packet {
public:
	virtual ~Packet() = 0;
	virtual auto getId() -> int = 0;
	virtual auto getName() -> std::mcstring* = 0;

	auto isValid() -> bool;
};

