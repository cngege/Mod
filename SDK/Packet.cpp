#include "Packet.h"
#include "Utils.h"

auto Packet::isValid() -> bool
{
	return Utils::CallVFunc<6, bool>(this);
}
