#ifndef __ENGINE_H__
#define __ENGINE_H__

/** INCLUDES **/
#include <iostream>
#include <string>
#include "Inline.h"
#include "VTable.h"
#include "..\HallowLib\Queue.h"

/** DEFINES **/
//#define MAX_HOOKS 700

/** CLASSES **/
/* HookEngine */
class HookEngine
{
public:
	HookEngine();
	~HookEngine();

	void add_hookinline(HookInline *hi);
	void add_hookvtable(HookVTable *hv);
	HookInline* search_hookinline(std::wstring search_name);
	HookVTableEntry* search_hookvtable(std::wstring search_name, int entry_index);

private:
	Queue *hook_inlines;
	Queue *hook_vtables;
};

#endif
