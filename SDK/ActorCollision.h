#pragma once
#include <cstdint>

class ActorCollision {

public:
	auto getHealth() -> float;
	auto setOnGround(bool) -> char;
};