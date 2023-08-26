#include "ActorMovementProxy.h"

#include "Utils.h"
#include "HMath.h"

auto ActorMovementProxy::isOnGround() -> bool
{
	return Utils::CallVFunc<39, bool>(this);
}

auto ActorMovementProxy::setOnGround(bool v) -> void
{
	Utils::CallVFunc<40, void, bool>(this,v);
}

auto ActorMovementProxy::getHealth() -> int
{
	return Utils::CallVFunc<43, int>(this);
}

auto ActorMovementProxy::getRotation() -> vec2_t*
{
	return Utils::CallVFunc<73, vec2_t*>(this);
}

auto ActorMovementProxy::setRotation(vec2_t* rot) -> void
{
	Utils::CallVFunc<74, void, vec2_t*>(this, rot);
}
