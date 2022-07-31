#include "../types.hpp"
 
 

class utils : public singleton<utils> 
{
public:
	uintptr_t fetch_module_base(const char* name);
	bool is_key_down(int vk);
	int key_states[255];
	template <typename t>
	inline bool is_valid(t address) {
		if ((uintptr_t)address < 0x0000000000010000)
			return false;

		if ((uintptr_t)address > 0x00007FFFFFFFFFFF)
			return false;

		return true;
	}
};