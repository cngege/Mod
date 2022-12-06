#include "Item.h"
#include "Utils.h"
#include "ItemStackBase.h"

uintptr_t** Item::vfTables = nullptr;

int Item::getIdOffset = 0;

template <typename TRet, typename... TArgs>
auto Item::GetVFtableFun(int a)->auto* {
	return reinterpret_cast<TRet(__fastcall*)(TArgs...)>(vfTables[a]);
}

auto Item::GetVFtableFun(int a)->uintptr_t* {
	return vfTables[a];
}

auto Item::GetVFtables()->uintptr_t** {
	return vfTables;
}

auto Item::SetVFtables(uintptr_t** vfTable)->void {
	vfTables = vfTable;
}

auto Item::isValid() -> bool
{
	return this && *(void**)this;
}

auto Item::getIdEx()->short
{
	return *reinterpret_cast<short*>((uintptr_t)this + getIdOffset);
}



auto Item::use(ItemStack* item, Player* player)->ItemStack* {
	using Fn = ItemStack*(__fastcall*)(Item*, ItemStack*, Player*);
	return reinterpret_cast<Fn>((*(uintptr_t**)this)[84])(this, item, player);
	//return Utils::CallVFunc<84, ItemStack&, Item*, ItemStack&, Player&>(*(void**)this, this, item, player);
}

auto Item::buildDescriptionName(TextHolder& text, ItemStackBase item)->TextHolder& {
	//using Fn = TextHolder& (__fastcall*)(Item*, TextHolder&, ItemStackBase&);
	//return reinterpret_cast<Fn>((*(uintptr_t**)this)[94])(this, text, item);
	return Utils::CallVFunc<94, TextHolder&, Item*, TextHolder&, ItemStackBase&>(*(void**)this, this, text, item);
}