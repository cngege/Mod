#include "ActorCollision.h"

#include "Utils.h"
#include "Logger.h"

auto ActorCollision::getHealth() -> float
{
    //三个特征码都在一个函数中，所以只要一个有用，就可以补全另外两个
    static auto offsetcall = FindSignature("E8 ? ? ? ? 85 ? 7F ? 48 8B CE E8 ? ? ? ? 84 ? 74 ? 48");    // 1.20.30.02
    if (offsetcall == 0x00) {
        logF_Debug("[ActorCollision.cpp] [ActorCollision::getHealth] [Warn] 第一个特征码失效");
        offsetcall = FindSignature("E8 ? ? ? ? 85 ? 0F 8F ? ? ? ? 48 8D 95 ? ? ? ? 48 8B 4E ? E8");
        if (offsetcall == 0x00) {
            logF_Debug("[ActorCollision.cpp] [ActorCollision::getHealth] [Warn] 第二个特征码失效");
            offsetcall = FindSignature("E8 ? ? ? ? 85 ? 7F ? 48 8B 46 ? 48 8B 08 8B 46 ? 89");
        }
    }
    _ASSERT(offsetcall);
    auto fun = Utils::FuncFromSigOffset(offsetcall, 1);
    using Fn = float(__fastcall*)(ActorCollision*);
    return reinterpret_cast<Fn>(fun)(this);
}

// 确定 ActorCollision::setOnGround
//----- (0000000142F02C20) ----------------------------------------------------
//char __fastcall sub_142F02C20(__int64 a1, char a2)
//{
//    int v2; // ebx
//    __int64* v3; // rcx
//    __int64* v5; // rax
//    int v6; // [rsp+38h] [rbp+10h] BYREF
//
//    v2 = *(_DWORD*)(a1 + 8);
//    v3 = *(__int64**)a1;
//    if (a2)
//    {
//        v6 = v2;
//        return (unsigned __int8)sub_1407D7520(v3, &v6);
//    }
//    else
//    {
//        v5 = (__int64*)sub_1407DA270(*v3, 555691489);
//        v6 = v2;
//        return sub_140330060(v5, &v6);
//    }
//}
// 搜索保存C文件后搜索关键字：555691489); 然后和BDS版，上面的1.20.30版比较, 最后调用验证即可
auto ActorCollision::setOnGround(bool onGround) -> char
{
    // 直接定位call
    static auto funcall = FindSignature("40 53 48 83 EC ? 8B 59 ? 48 8B 09 84 D2 74 ? 48 8D 54 24 ? 89 5C 24 ? E8");
    if (funcall == 0x00) {
        logF_Debug("[ActorCollision.cpp] [ActorCollision::setOnGround] [Warn] 第一个特征码失效");
        // 调用者
        auto offset = FindSignature("E8 ? ? ? ? 48 8D 4F ? E8 ? ? ? ? 48 8B 07 48 8B CF 48 8B 80");
        if (offset == 0x00) {
            logF_Debug("[ActorCollision.cpp] [ActorCollision::setOnGround] [Warn] 第二个特征码失效");
            offset = FindSignature("E8 ? ? ? ? EB ? 0F B6 ? ? 48 8D 4F ? E8 ? ? ? ? 48");
            if (offset == 0x00) {
                logF_Debug("[ActorCollision.cpp] [ActorCollision::setOnGround] [Warn] 第三个特征码失效");
                offset = FindSignature("E8 ? ? ? ? 48 8B 1F 48 8B CF 48 8B 83 ? ? ? ? FF ? ? ? ? ? F3");
                if (offset == 0x00) {
                    // 没了
                }
                else {
                    funcall = Utils::FuncFromSigOffset(offset, 1);
                }
            }
            else {
                funcall = Utils::FuncFromSigOffset(offset, 1);
            }
        }
        else {
            funcall = Utils::FuncFromSigOffset(offset, 1);
        }
    }
    _ASSERT(funcall);
    //auto fun = Utils::FuncFromSigOffset(offsetcall, 1);
    using Fn = char(__fastcall*)(ActorCollision*, bool);
    return reinterpret_cast<Fn>(funcall)(this, onGround);
}
