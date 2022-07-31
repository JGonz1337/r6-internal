#include "../hooking.hpp"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_impl_dx11.h"
#include "../../features/features.hpp"
#include "../../utils/utils.hpp"

#pragma comment(lib, "d3d11.lib")
#pragma warning( disable : 6387 )

ID3D11DeviceContext*	context = nullptr;
ID3D11RenderTargetView* target_view = nullptr;
ID3D11Texture2D*		back_buffer = NULL;
IDXGISwapChain*			swap_chain = nullptr;
ID3D11Device*			device = nullptr;
WNDPROC					original_wndproc = {};
HWND					siege_window = {};
bool					initialize = true;

__int64 __stdcall wnd_proc(const HWND window, UINT u_message, WPARAM w_param, LPARAM l_param) {
	// if you want input you must use 'ImGui_ImplWin32_WndProcHandler' here.

	if (u_message == WM_KEYDOWN)
		utils::get().key_states[w_param] = true;
	else if (u_message == WM_KEYUP)
		utils::get().key_states[w_param] = false;

	return CallWindowProc(original_wndproc, window, u_message, w_param, l_param);
}

long __stdcall hooking::hk_present(IDXGISwapChain* chain, UINT sync_interval, UINT flags) {
	auto setup = [&]() 
	{
		if (!initialize)
			return 0;
		siege_window = FindWindow(NULL, "Rainbow Six");
		original_wndproc = (WNDPROC)SetWindowLongPtr(siege_window, -4, (LONG_PTR)wnd_proc);
		swap_chain = chain;
		swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device));
		device->GetImmediateContext(&context);
		swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
		device->CreateRenderTargetView(back_buffer, NULL, &target_view);
		back_buffer->Release();
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(siege_window);
		ImGui_ImplDX11_Init(device, context);
		printf("hooking initialized.\n");
		initialize = false;
		return 0;
	} ();

	context->OMSetRenderTargets(1, &target_view, nullptr);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	features::run();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return reinterpret_cast<std::decay_t<decltype(&hk_present)>>(hooking::get().orig_present)(chain, sync_interval, flags);
}

long __stdcall hooking::hk_resize_buffer(IDXGISwapChain* chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT format, UINT flags) {
	if (context)
		context->Release();
	if (target_view)
		target_view->Release();

	HRESULT result = reinterpret_cast<std::decay_t<decltype(&hk_resize_buffer)>>(hooking::get().orig_buffer)(chain, buffer_count, width, height, format, flags);
	initialize = true;
	return result;
}
