#include "ClientInstance.h"

uintptr_t** ClientInstance::vfTables = nullptr;

int ClientInstance::getMinecraftGameOffset = 0;


template <typename TRet, typename... TArgs>
auto ClientInstance::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto ClientInstance::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto ClientInstance::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto ClientInstance::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}

// 偏移28*8 看Tick  其实Hook的Tick是frameUpdate, 其中调用了 LevelRenderer::frameUpdate， 在获取 LevelRenderer 中能看到偏移
// 进行 Tick 函数，对比源码找
//sub_14031AA40(a2, (__int64)a1);
//v14 = a1[28];
//if (v14)
//sub_1413A0200(v14, (__int64)&v19);//应该就是这个
//v15 = (*(__int64(__fastcall**)(_QWORD*))(*a1 + 1616i64))(a1);
//if (v15)
//sub_1413E6090(v15, a2);//LightTexture::frameUpdate
//v16 = a1[31];
//if (v16)
//(*(void(__fastcall**)(__int64, __int64))(*(_QWORD*)v16 + 776i64))(v16, a2);
//if (*((_QWORD*)&v20 + 1))
//sub_1400D0770(*((volatile signed __int32**)&v20 + 1));

//源码：
//v8 = *((_QWORD*)this + 19);
//if (v8)
//LevelRenderer::frameUpdate(v8, (FrameUpdateContext*)v12);
//v9 = (LightTexture*)(*(__int64(__fastcall**)(ClientInstance*))(*(_QWORD*)this + 1360LL))(this);
//if (v9)
//LightTexture::frameUpdate(v9, a2);
//v10 = *((_QWORD*)this + 22);
//if (v10)
//(*(void(__fastcall**)(__int64, FrameUpdateContext*))(*(_QWORD*)v10 + 784LL))(v10, a2);
//return __readfsqword(0x28u);

// 其中 LightTexture::frameUpdate 中有明显的字符串可以定位到 "Brightness Texture Update"

//第二个方法 首先通过 "A LevelRenderer group" 定位LevelRenderer构造函数，拿到虚表地址,(注意出现的第二个才是虚表地址)
//然后看CI结构，在E0 左右看谁的指针虚表符合即可得到偏移
LevelRenderer* ClientInstance::getLevelRender() {
	static SignCode sign("ClientInstance::getLevelRender"); //特征码获取偏移
	sign.AddSign("48 8B 8F ? ? ? ? 48 85 C9 74 ? 48 8D 54", [](uintptr_t v) { return v + 3; });
	_ASSERT(sign);
	return *(LevelRenderer**)((uintptr_t)this + *reinterpret_cast<int*>(*sign));
}