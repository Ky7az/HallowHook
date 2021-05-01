#ifndef __INLINE_H__
#define __INLINE_H__

/** INCLUDES **/
#include <iostream>
#include <string>
#include <windows.h>
#include "distorm.h"

/** DEFINES **/
#define MAX_INSTRUCTIONS 100

/** CLASSES **/
/* HookInline */
class HookInline
{
public:
	HookInline(std::wstring name, DWORD original_function, DWORD detour_function);
	~HookInline();

	std::wstring get_name() const;
	DWORD get_trampoline_function() const;

	void hook();
	void unhook();
	void write_jump(DWORD jmp_from, DWORD jmp_to);
	void debug();

private:
	std::wstring name;
	bool is_hooking;

	DWORD original_function;
	unsigned char *prologue;

	DWORD detour_function;
	DWORD trampoline_function;
	unsigned int trampoline_size;
};

#endif