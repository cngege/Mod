#include "BlockLegacy.h"
#include "Utils.h"

//v14 = *(_WORD*)(v4 + 422);
//if (v14 < 0x100u)
//    v15 = *(_WORD*)(v4 + 422);
//else
//v15 = 255 - v14;
// 可搜索  < 0x100u )
//或者搜索"minecraft:yellow_flower" 有三处 在大函数内部， 在下面一点的地方有关键代码
unsigned short BlockLegacy::getBlockItemIdEx()
{
    static SignCode sign("BlockLegacy::getBlockItemIdEx");
    sign.AddSign("0F B7 80 ? ? ? ? B9 00 01 00 00", [this](uintptr_t v) { return v + 3; });
    _ASSERT(sign);
    // 关键这个422需要用特征码获取
    unsigned short id = *(unsigned short*)((uintptr_t)this + *(int*)*sign);
    if (id < 0x100u)
        return id;
    else
        return  255 - id;
}

bool BlockLegacy::isAir()
{
    return Utils::CallVFunc<29, bool>(this);            //1.20.41.02
}
