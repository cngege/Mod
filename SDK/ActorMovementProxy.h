#pragma once


class ActorMovementProxy {

public:

	auto isOnGround() -> bool;								/*39*/	//1.20.15
	auto setOnGround(bool) -> void;							/*40*/	//1.20.15

	auto getHealth() -> int;								/*43*/	//1.20.15
	auto getRotation() -> struct vec2_t*;					/*73*/	//1.20.15
	auto setRotation(struct vec2_t*) ->void;				/*74*/	//1.20.15
};