#include "LocalPlayer.h"
#include "ClientInstance.h"

uintptr_t** LocalPlayer::vfTables = nullptr;
int LocalPlayer::toCIoffset = 0;
int LocalPlayer::onGroundoffset = 0;

uintptr_t* LocalPlayer::tickWorldCall = nullptr;

template <typename TRet, typename... TArgs>
auto LocalPlayer::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto LocalPlayer::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto LocalPlayer::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto LocalPlayer::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}

//定义函数
auto LocalPlayer::getClientInstance()->ClientInstance* {
	return *((ClientInstance**)((uintptr_t)this + toCIoffset));			//这个偏移是偏移字节
}

// 废弃
auto LocalPlayer::isOnGround()->bool* {
	return (bool*)((uintptr_t)this + onGroundoffset);
}


//搜索 ) = -1028390912; 找所在函数  参数只有两个的那个函数即是目标函数
auto LocalPlayer::LocalPlayerTurn(vec2_t* viewAngles)->void{
	//A3C640
	static SignCode sign("LocalPlayer::LocalPlayerTurn");
	sign << "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 44 0F 29 A0 ? ? ? ? 44 0F 29 A8 ? ? ? ? 44 0F 29 B0 ? ? ? ?  44 0F 29 B8 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B DA";
	sign.AddSignCall("E8 ? ? ? ? 48 8B 07 0F 28 CE");
	sign.AddSignCall("E8 ? ? ? ? 48 8B 07 0F 28 CF");
	_ASSERT(sign);
	using Turn = void(__thiscall*)(LocalPlayer*, vec2_t*);
	reinterpret_cast<Turn>(*sign)(this, viewAngles);
}

//虚表函数

//auto LocalPlayer::setPos(vec3_t* pos)->__int64 {
//	return GetVFtableFun<__int64, LocalPlayer*, vec3_t*>(13)(this,pos);	//19?
//}

//无法验证虚表
//auto LocalPlayer::jumpFromGround()->void* {
//	return GetVFtableFun<void*, LocalPlayer*>(346)(this);
//}

//无法验证虚表
//auto LocalPlayer::displayClientMessage(std::mcstring* text)->void* {
//	return GetVFtableFun<void*, LocalPlayer*, std::mcstring*>(389)(this, text);
//}