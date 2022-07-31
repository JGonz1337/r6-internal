#include "utils.hpp"
#include <TlHelp32.h>
#include <map>
uintptr_t utils::fetch_module_base(const char* name)
{
	static std::map<const char*, uintptr_t> address_map = {};
	if (address_map.count(name))
		return address_map[name];
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());
	auto address = uintptr_t();

	if (snapshot == INVALID_HANDLE_VALUE)
		return address;
	

	MODULEENTRY32 modEntry;
	modEntry.dwSize = sizeof(modEntry);
	if (Module32First(snapshot, &modEntry))
	{
		do
		{
			if (!strcmp(modEntry.szModule, name))
			{
				address = (uintptr_t)modEntry.modBaseAddr;
				break;
			}
		} while (Module32Next(snapshot, &modEntry));
	}

	CloseHandle(snapshot);

	address_map[name] = address;
	return address;
}

bool utils::is_key_down(int vk)
{
	return key_states[vk];
}
