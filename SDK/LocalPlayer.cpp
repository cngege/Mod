#include "LocalPlayer.h"
#include "ClientInstance.h"
#include "TextHolder.h"

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

auto LocalPlayer::isOnGround()->bool* {
	return (bool*)((uintptr_t)this + onGroundoffset);
}

//auto LocalPlayer::isValid()->bool {
//	return this->isLocalPlayer();
//}


//虚表函数

//auto LocalPlayer::setPos(vec3_t* pos)->__int64 {
//	return GetVFtableFun<__int64, LocalPlayer*, vec3_t*>(13)(this,pos);	//19?
//}

//无法验证虚表
auto LocalPlayer::jumpFromGround()->UINT64 {
	return GetVFtableFun<UINT64, LocalPlayer*>(346)(this);
}

//无法验证虚表
auto LocalPlayer::displayClientMessage(TextHolder* text)->UINT64 {
	return GetVFtableFun<UINT64, LocalPlayer*, TextHolder*>(389)(this, text);
}