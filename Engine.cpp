#include "Engine.h"
using namespace std;

HookEngine::HookEngine()
{
	this->hook_inlines = new Queue();
	this->hook_vtables = new Queue();
}

HookEngine::~HookEngine()
{
	Node *n_hi = hook_inlines->get_head();
	while (n_hi != NULL) {

		HookInline *hi = NULL;
		hi = (HookInline*)n_hi->get_data();

		delete hi;
		n_hi = n_hi->get_next();
	}

	Node *n_hv = hook_vtables->get_head();
	while (n_hv != NULL) {

		HookVTable *hv = NULL;
		hv = (HookVTable*)n_hv->get_data();

		delete hv;
		n_hv = n_hv->get_next();
	}

	if (hook_inlines != NULL)
		delete hook_inlines;

	if (hook_vtables != NULL) {
		delete hook_vtables;
	}
}

void HookEngine::add_hookinline(HookInline *hi)
{
	hook_inlines->add(hi);
}

void HookEngine::add_hookvtable(HookVTable *hv)
{
	hook_vtables->add(hv);
}

HookInline* HookEngine::search_hookinline(wstring search_name)
{
	if (hook_inlines == NULL)
		return NULL;

	Node *n = hook_inlines->get_head();

	while (n != NULL) {

		HookInline *hi = NULL;
		hi = (HookInline*)n->get_data();

		if (hi->get_name() == search_name) {
			return hi;
		}

		n = n->get_next();
	}

	return NULL;
}

HookVTableEntry* HookEngine::search_hookvtable(wstring search_name, int entry_index)
{
	if (hook_vtables == NULL)
		return NULL;

	Node *n = hook_vtables->get_head();

	while (n != NULL) {

		HookVTable *hv = NULL;
		hv = (HookVTable*)n->get_data();

		if (hv->get_name() == search_name) {
			return hv->get_vtable_entry(entry_index);
		}

		n = n->get_next();
	}

	return NULL;
}