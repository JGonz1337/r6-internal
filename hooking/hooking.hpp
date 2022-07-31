#include "../types.hpp"
class hooking : public singleton<hooking>
{
public:
	bool init();
 
	WNDPROC wndproc;
	HWND window;
	uintptr_t orig_present;
	uintptr_t orig_buffer;

	static long __stdcall hk_present(IDXGISwapChain* chain, UINT sync_interval, UINT flags);
	static long __stdcall hk_resize_buffer(IDXGISwapChain* chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT format, UINT flags);
};