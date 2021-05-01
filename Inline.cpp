#include "Inline.h"
using namespace std;

/* InlineHook */
HookInline::HookInline(wstring name, DWORD original_function, DWORD detour_function)
{
	this->name = name;
	this->is_hooking = false;

	this->original_function = original_function;
	this->prologue = NULL;

	this->detour_function = detour_function;
	this->trampoline_function = 0;
	this->trampoline_size = 0;
}

HookInline::~HookInline()
{
	if (is_hooking)
		unhook();

	delete prologue;
}

wstring HookInline::get_name() const { return name; }
DWORD HookInline::get_trampoline_function() const { return trampoline_function; }

void HookInline::hook()
{
	_DecodeResult res;
	_DecodedInst decoded_inst[MAX_INSTRUCTIONS];
	unsigned int decoded_inst_count = 0;
	_OffsetType offset = 0;

	res = distorm_decode(offset,
		(unsigned char*)original_function,
		20,
		Decode32Bits,
		decoded_inst,
		MAX_INSTRUCTIONS,
		&decoded_inst_count);

	if (res == DECRES_INPUTERR)
		return;

	if (!(trampoline_function = (DWORD)VirtualAlloc(NULL, 20, MEM_COMMIT, PAGE_EXECUTE_READWRITE)))
		return;

	DWORD inst_size = 0;
	for (unsigned int x = 0; x < decoded_inst_count; x++)
	{
		if (inst_size >= 5)
			break;

		unsigned char *cur_inst = (unsigned char*)(original_function + inst_size);
		memcpy((unsigned char*)trampoline_function + inst_size, (void*)cur_inst, decoded_inst[x].size);
		inst_size += decoded_inst[x].size;
	}

	prologue = new unsigned char[inst_size];
	memcpy((void*)prologue, (void*)original_function, inst_size);

	// Create Trampoline
	write_jump(trampoline_function + inst_size, original_function + inst_size);

	// Create Detour
	write_jump(original_function, detour_function);

	trampoline_size = inst_size;
	is_hooking = true;
}

void HookInline::unhook()
{
	DWORD old_protect = 0;
	DWORD new_protect = PAGE_EXECUTE_READWRITE;

	VirtualProtect((void*)original_function, trampoline_size, new_protect, &old_protect);

	memcpy((void*)original_function, (void*)prologue, trampoline_size);

	VirtualProtect((void*)original_function, trampoline_size, old_protect, &new_protect);
	VirtualFree((void*)trampoline_function, 0, MEM_RELEASE);

	trampoline_function = 0;
	trampoline_size = 0;
	is_hooking = false;
}

void HookInline::write_jump(DWORD jmp_from, DWORD jmp_to)
{
	DWORD old_protect = 0;
	DWORD new_protect = PAGE_EXECUTE_READWRITE;

	VirtualProtect((void*)jmp_from, 5, new_protect, &old_protect);
	unsigned char *p_cur = (unsigned char*)jmp_from;

	*p_cur = 0xE9; // JMP
	DWORD rel_addr = jmp_to - jmp_from - 5;
	memcpy(++p_cur, &rel_addr, sizeof(DWORD));

	VirtualProtect((void*)jmp_from, 5, old_protect, &new_protect);
}

void HookInline::debug()
{
}