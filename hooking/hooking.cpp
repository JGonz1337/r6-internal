#include "hooking.hpp"
#include "../protection/xorstr.hpp"
#include "../utils/utils.hpp"
#pragma warning( disable : 6387 )
bool hooking::init() {
	printf("initializing hooking\n");

	auto protect = DWORD();
	LPVOID* table = reinterpret_cast<LPVOID*>(utils::get().fetch_module_base("dxgi.dll") + O_SWAPTABLE);

	orig_present = reinterpret_cast<uintptr_t>(table[8]);
	orig_buffer = reinterpret_cast<uintptr_t>(table[13]);

	VirtualProtect(table, 8, PAGE_READWRITE, &protect);
	table[8] = reinterpret_cast<LPVOID*>(&hooking::get().hk_present);
	table[13] = reinterpret_cast<LPVOID*>(&hooking::get().hk_resize_buffer);
	VirtualProtect(table, 8, protect, 0);
	return true;
}