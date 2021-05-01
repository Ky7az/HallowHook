#include "VTable.h"
using namespace std;

/* HookVTableEntry */
HookVTableEntry::HookVTableEntry(HookVTable *hook_vtable, DWORD vtable, int index)
{
	this->hook_vtable = hook_vtable;
	this->name = name;
	this->index = index;
	this->entry = vtable + sizeof(DWORD) * index;
	this->original_function = *((DWORD*)this->entry);
	this->detour_function = 0x00000000;
	this->is_hooking = false;
}

/*HookVTableEntry::HookVTableEntry(const HookVTableEntry &hve)
{
	this->name = hve.name;
	this->entry = hve.entry;
	this->original_function = hve.original_function;
	this->detour_function = hve.detour_function;
	this->is_hooking = hve.is_hooking;
}*/

HookVTableEntry::~HookVTableEntry()
{
	if (is_hooking)
		unhook();
}

DWORD HookVTableEntry::get_entry() const { return entry; }
DWORD HookVTableEntry::get_original_function() const { return original_function; }

void HookVTableEntry::hook(wstring name, DWORD detour_function)
{
	DWORD old_protect = 0;
	DWORD new_protect = PAGE_EXECUTE_READWRITE;

	VirtualProtect((void*)entry, sizeof(void*), new_protect, &old_protect);

	this->name = name;
	*((DWORD*)entry) = detour_function;
	this->detour_function = detour_function;

	VirtualProtect((void*)entry, sizeof(DWORD), old_protect, &new_protect);
	this->is_hooking = true;
}

void HookVTableEntry::unhook()
{
	DWORD old_protect = 0;
	DWORD new_protect = PAGE_EXECUTE_READWRITE;

	VirtualProtect((void*)entry, sizeof(DWORD), new_protect, &old_protect);

	*((DWORD*)entry) = original_function;
	this->detour_function = 0x00000000;

	VirtualProtect((void*)entry, sizeof(DWORD), old_protect, &new_protect);
	this->is_hooking = false;
}

void HookVTableEntry::reset()
{
	this->entry = hook_vtable->get_vtable() + sizeof(DWORD) * index;
}

void HookVTableEntry::debug()
{
}


/* HookVTable */
HookVTable::HookVTable(wstring name, DWORD vtable, unsigned int vtable_len)
{
	this->name = name;
	this->vtable = vtable;
	this->vtable_len = vtable_len;

	for (unsigned int index = 0; index <= vtable_len; index++) {
		HookVTableEntry *hve = new HookVTableEntry(this, vtable, index);
		VTableEntrz.insert(VTableEntrz.begin() + index, hve);
	}
}

HookVTable::~HookVTable()
{
	for (auto &hve : VTableEntrz) {
		delete hve;
	}
}

wstring HookVTable::get_name() const { return name; }
DWORD HookVTable::get_vtable() const { return vtable; }
HookVTableEntry* HookVTable::get_vtable_entry(int index) const { return VTableEntrz[index]; }

void HookVTable::hook(wstring name, int index, DWORD detour_function)
{
	HookVTableEntry *hve = VTableEntrz[index];
	hve->hook(name, detour_function);
}

void HookVTable::unhook(int index)
{
	HookVTableEntry *hve = VTableEntrz[index];
	hve->unhook();
}

void HookVTable::reset(DWORD vtable)
{
	this->vtable = vtable;

	for (auto &hve : VTableEntrz) {
		hve->reset();
	}
}

void HookVTable::debug()
{
}