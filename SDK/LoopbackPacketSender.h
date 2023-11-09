#pragma once
#include <vector>
#include <functional>

class LoopbackPacketSender {
//private:
//	virtual ~LoopbackPacketSender() = 0;
//public:
//	virtual void __cdecl send(class Packet&) = 0;
//	//__int64 __fastcall sub_141F53910(__int64 a1, _BYTE *a2, __int64 a3)
//	virtual void* __cdecl sendToServer(class Packet&) = 0;
//	virtual void __cdecl sendToClient(class NetworkIdentifier const&, class Packet const&, enum SubClientId) = 0;
//	virtual void __cdecl sendToClient(class UserEntityIdentifierComponent const*, class Packet const&) = 0;
//	virtual void __cdecl sendToClients(class std::vector<struct NetworkIdentifierWithSubId, class std::allocator<struct NetworkIdentifierWithSubId> > const&, class Packet const&) = 0;
//	virtual void __cdecl sendBroadcast(class NetworkIdentifier const&, enum SubClientId, class Packet const&) = 0;
//	virtual void __cdecl sendBroadcast(class Packet const&) = 0;
//	virtual void __cdecl flush(class NetworkIdentifier const&, class std::function<void __cdecl(void)>&&) = 0;

protected:
	static uintptr_t** vfTables;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int) -> auto*;
	static auto GetVFtableFun(int) -> uintptr_t*;
	static auto GetVFtables() -> uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables) -> void;


};