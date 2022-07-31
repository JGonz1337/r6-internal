#include <Windows.h>
#include <cstdio>
#include "hooking/hooking.hpp"
#pragma warning( disable : 6031 )
bool WINAPI DllMain(HINSTANCE instance, unsigned long reason, void* reserved) {
	if (reason != DLL_PROCESS_ATTACH)
		return true;

	DisableThreadLibraryCalls(instance);
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	printf("R6 Internal | %s - %s\n", __DATE__, __TIME__);

	hooking::get().init();
	return true;
}