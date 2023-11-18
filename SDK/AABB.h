﻿#pragma once
#include "..\Mod\Utils\HMath.h"

class AABB {
public:
	vec3_t min;
	vec3_t max;
public:
	AABB() {};
	AABB(vec3_t _min, vec3_t _max) { min = _min; max = _max; };
	AABB(const AABB& aabb);
	auto getsize()->float;
	auto isValid()->bool;
	auto getCenter()->vec3_t;
	auto getVolume()->float;				//获取体积
	auto getBounds()->vec3_t;				//获取边框
	inline bool operator==(const AABB& rhs) const;
	inline bool operator!=(const AABB& rhs) const;
};