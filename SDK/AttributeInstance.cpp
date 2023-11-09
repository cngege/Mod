#include "AttributeInstance.h"

int AttributeInstance::getCurrentValueoffset = 33;

auto AttributeInstance::getCurrentValue()->float {
	return *((float*)((char*)this + getCurrentValueoffset));			//这个偏移是偏移字节
}