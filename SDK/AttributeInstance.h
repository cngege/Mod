#pragma once

enum Attribute : __int64 {
	HEALTH = 25769804032
};

class AttributeInstance {
public:
	static int getCurrentValueoffset;

public:
	auto getCurrentValue()->float;
};