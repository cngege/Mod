#include "AttributeInstance.h"

int AttributeInstance::getCurrentValueoffset = 0;

auto AttributeInstance::getCurrentValue()->float {
	return *((float*)((char*)this + getCurrentValueoffset));			//这个偏移是偏移字节
}