#ifndef __VTABLE_H__
#define __VTABLE_H__

/** INCLUDES **/
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

/** DEFINES **/

/** CLASSES **/
class HookVTable;
class HookVTableEntry;

/* HookVTableEntry */
class HookVTableEntry
{
public:
	HookVTableEntry(HookVTable *hook_vtable, DWORD vtable, int index);
	//HookVTableEntry(const HookVTableEntry &hve);
	~HookVTableEntry();

	DWORD get_entry() const;
	DWORD get_original_function() const;

	void hook(std::wstring name, DWORD detour_function);
	void unhook();
	void reset();
	void debug();

private:
	HookVTable *hook_vtable;
	std::wstring name;
	int index;
	DWORD entry;
	DWORD original_function;
	DWORD detour_function;
	bool is_hooking;
};

/* HookVTable */
class HookVTable
{
public:
	HookVTable(std::wstring name, DWORD vtable, unsigned int vtable_len);
	~HookVTable();

	std::wstring get_name() const;
	DWORD get_vtable() const;
	HookVTableEntry* get_vtable_entry(int index) const;

	void hook(std::wstring name, int index, DWORD detour_function);
	void unhook(int index);
	void reset(DWORD vtable);
	void debug();

private:
	std::wstring name;
	DWORD vtable;
	unsigned int vtable_len;
	std::vector<HookVTableEntry*> VTableEntrz;
};

#endif