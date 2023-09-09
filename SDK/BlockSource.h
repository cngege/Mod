#pragma once
#include <cstdint>

struct vec3_ti;


class BlockSource {
public:
	auto getBlock(vec3_ti*, int) -> class Block*;					/*1*/  // 1.20.15
	auto getBlock(vec3_ti*) -> class Block*;						/*2*/  // 1.20.15
	auto getBlock(int, int, int)-> class Block*;					/*3*/  // 1.20.15
	auto getBlockEntity(vec3_ti*) -> class BlockActor*;				/*4*/  // 1.20.15

	auto hasBlock(vec3_ti*) -> bool;								/*7*/  // 1.20.15

	// 获取亮度
	auto getBrightness(vec3_ti*) -> float;							/*19*/ // 1.20.15

	// 是实体阻塞方块
	auto isSolidBlockingBlock(int, int, int) -> bool;				/*36*/ // 1.20.15
	auto isSolidBlockingBlock(vec3_ti*) -> bool;					/*37*/ // 1.20.15
};