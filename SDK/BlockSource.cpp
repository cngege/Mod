#include "BlockSource.h"

#include "Utils.h"
#include "HMath.h"

// a = 0, 调用第二个虚函数, a = 1 调用 getExtraBlock， 否则返回空气方块
auto BlockSource::getBlock(vec3_ti* BlockPos, int a) -> class Block*
{
	return Utils::CallVFunc<1, Block*, vec3_ti*, int>(this, BlockPos,a);
}

auto BlockSource::getBlock(vec3_ti* BlockPos) -> class Block*
{
	return Utils::CallVFunc<2, Block*, vec3_ti*>(this, BlockPos);
}

auto BlockSource::getBlock(int x, int y, int z) -> class Block*
{
	return Utils::CallVFunc<3, Block*, int, int, int>(this, x, y, x);
}

auto BlockSource::getBlockEntity(vec3_ti* BlockPos) -> class BlockActor*
{
	return Utils::CallVFunc<4, BlockActor*, vec3_ti*>(this, BlockPos);
}

auto BlockSource::hasBlock(vec3_ti* BlockPos) -> bool
{
	return Utils::CallVFunc<7, bool, vec3_ti*>(this, BlockPos);
}

auto BlockSource::getBrightness(vec3_ti* BlockPos) -> float
{
	return Utils::CallVFunc<19, float, vec3_ti*>(this, BlockPos);
}

auto BlockSource::isSolidBlockingBlock(int x, int y, int z) -> bool
{
	return Utils::CallVFunc<36, bool, int, int, int>(this, x, y, x);
}

auto BlockSource::isSolidBlockingBlock(vec3_ti* BlockPos) -> bool
{
	return Utils::CallVFunc<37, bool, vec3_ti*>(this, BlockPos);
}
