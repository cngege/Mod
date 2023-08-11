//ImGui Shit
#include "../Utils/Logger.h"
//#include "../../imgui/animations/fade.hpp"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_dx11.h"
#include <initguid.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3d12.h>
#include <d2d1_1.h>
#include <dxgi1_4.h>
#include <dwrite_1.h>
#include "kiero/kiero.h"

#include "../Utils/Game.h"
#include "../Utils/Utils.h"
#include "../Modules/ModuleManager.h"

#include <io.h>

//auto GetDllMod(void) -> HMODULE {
//	MEMORY_BASIC_INFORMATION info;
//	size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)GetDllMod, &info, sizeof(info));
//	assert(len == sizeof(info));
//	return len ? (HMODULE)info.AllocationBase : NULL;
//}

typedef HRESULT(__thiscall* PresentD3D12)(IDXGISwapChain3*, UINT, UINT);
PresentD3D12 oPresentD3D12;
ID3D11Device* d3d11Device = nullptr;
ID3D12Device* d3d12Device = nullptr;
enum ID3D_Device_Type {
	INVALID_DEVICE_TYPE,
	D3D11,
	D3D12
};
struct FrameContext {
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12Resource* main_render_target_resource = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE main_render_target_descriptor = {};
};
UINT buffersCounts = -1;
FrameContext* frameContext = nullptr;
ID3D12DescriptorHeap* d3d12DescriptorHeapImGuiRender = nullptr;
ID3D12DescriptorHeap* d3d12DescriptorHeapBackBuffers = nullptr;
ID3D12GraphicsCommandList* d3d12CommandList = nullptr;
ID3D12CommandAllocator* allocator = nullptr;
ID3D12CommandQueue* d3d12CommandQueue = nullptr;
bool initContext = false;
HRESULT hookPresentD3D12(IDXGISwapChain3* ppSwapChain, UINT syncInterval, UINT flags) {
	auto deviceType = ID3D_Device_Type::INVALID_DEVICE_TYPE;
	static auto window = (HWND)FindWindowA(nullptr, (LPCSTR)"Minecraft");
	static auto childwindow = (HWND)FindWindowExA(window, NULL, NULL, (LPCSTR)"Minecraft");
	if (window == NULL) {
		goto out;
	};
	if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
		deviceType = ID3D_Device_Type::D3D11;
	}
	else if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {
		deviceType = ID3D_Device_Type::D3D12;
	};
	if (deviceType == ID3D_Device_Type::INVALID_DEVICE_TYPE) {
		goto out;
	};
	if (deviceType == ID3D_Device_Type::D3D11) {
		if (!initContext) {
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			(void)io;

			std::string font_JNMYT = Utils::WStringToString(Logger::GetRoamingFolderPath()) + std::string("\\Mod\\Assets\\JNMYT.ttf");
			if (_access(font_JNMYT.c_str(), 0 /*F_OK*/) != -1) {
				io.Fonts->AddFontFromFileTTF(font_JNMYT.c_str(), 15.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
			}
			io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 15.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
			// 这里注意值如果不是常亮就要当心其被释放掉
			io.IniFilename = Game::ImConfigIni.c_str();

		}
		ID3D11DeviceContext* ppContext = nullptr;
		d3d11Device->GetImmediateContext(&ppContext);
		ID3D11Texture2D* pBackBuffer = nullptr;
		ppSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		ID3D11RenderTargetView* mainRenderTargetView;
		if (!pBackBuffer) goto out;
		d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
		pBackBuffer->Release();

		if (!initContext) {
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(d3d11Device, ppContext);
			// Fonts
			initContext = true;

			{
				ImGuiStyle* style = &ImGui::GetStyle();

				style->WindowPadding = ImVec2(15, 15);
				style->WindowRounding = 10.f;
				style->FramePadding = ImVec2(5, 5);
				style->FrameRounding = 6.f;
				style->ItemSpacing = ImVec2(12, 8);
				style->ItemInnerSpacing = ImVec2(8, 6);
				style->IndentSpacing = 20.0f;
				style->ScrollbarSize = 10.0f;
				style->ScrollbarRounding = 9.0f;
				style->GrabMinSize = 5.0f;
				style->GrabRounding = 3.0f;
				style->WindowTitleAlign = ImVec2(0.5, 0.5);
				/*
				style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
				style->Colors[ImGuiCol_Separator] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
				style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
				style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
				style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
				style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
				style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
				style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
				style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
				style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
				style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
				style->Colors[ImGuiCol_CheckMark] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
				style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
				style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
				style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
				style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
				style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
				style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
				style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
				style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
				style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
				style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
				style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
				style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
				style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
				style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
				style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
				style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
				style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
				style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
				style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
				style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
				style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
				style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
				style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
				style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);*/

			}
		}
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
		Game::GetModuleManager()->onImGUIRender();

		ImGui::Render();
		ppContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		RECT window_rectangle, childwindow_rectangle;													//
		GetWindowRect(window, &window_rectangle);														// 计算出ImGui以其所为窗口Y坐标的基准值
		GetWindowRect(childwindow, &childwindow_rectangle);												// 也就是将标题栏富含进去，默认没有，也就导致鼠标悬浮、点击有Y轴偏移
		ImGui::GetDrawData()->DisplayPos.y = (float)(childwindow_rectangle.top - window_rectangle.top); //
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		mainRenderTargetView->Release();
		d3d11Device->Release();
	}
	else if (deviceType == ID3D_Device_Type::D3D12) {
		if (!initContext)
		{
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			(void)io;

			std::string font_JNMYT = Utils::WStringToString(Logger::GetRoamingFolderPath()) + std::string("\\Mod\\Assets\\JNMYT.ttf");
			if (_access(font_JNMYT.c_str(), 0 /*F_OK*/) != -1) {
				io.Fonts->AddFontFromFileTTF(font_JNMYT.c_str(), 15.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
			}
			io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 15.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
			// 这里注意值如果不是常亮就要当心其被释放掉
			io.IniFilename = Game::ImConfigIni.c_str();
		}
		DXGI_SWAP_CHAIN_DESC sdesc;
		ppSwapChain->GetDesc(&sdesc);
		sdesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sdesc.OutputWindow = window;
		sdesc.Windowed = ((GetWindowLongPtr(window, GWL_STYLE) & WS_POPUP) != 0) ? false : true;
		buffersCounts = sdesc.BufferCount;
		frameContext = new FrameContext[buffersCounts];
		D3D12_DESCRIPTOR_HEAP_DESC descriptorImGuiRender = {};
		descriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descriptorImGuiRender.NumDescriptors = buffersCounts;
		descriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		if (d3d12DescriptorHeapImGuiRender == nullptr)
			if (FAILED(d3d12Device->CreateDescriptorHeap(&descriptorImGuiRender, IID_PPV_ARGS(&d3d12DescriptorHeapImGuiRender))))
				goto out;
		if (d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator)) != S_OK)
			return 0;
		for (size_t i = 0; i < buffersCounts; i++) {
			frameContext[i].commandAllocator = allocator;
		};
		if (d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, NULL, IID_PPV_ARGS(&d3d12CommandList)) != S_OK ||
			d3d12CommandList->Close() != S_OK)
			return 0;
		D3D12_DESCRIPTOR_HEAP_DESC descriptorBackBuffers = {};
		descriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descriptorBackBuffers.NumDescriptors = buffersCounts;
		descriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		descriptorBackBuffers.NodeMask = 1;
		if (d3d12Device->CreateDescriptorHeap(&descriptorBackBuffers, IID_PPV_ARGS(&d3d12DescriptorHeapBackBuffers)) != S_OK)
			return 0;
		const auto rtvDescriptorSize = d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = d3d12DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();
		for (UINT i = 0; i < buffersCounts; i++) {
			ID3D12Resource* pBackBuffer = nullptr;
			frameContext[i].main_render_target_descriptor = rtvHandle;
			ppSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
			if (!pBackBuffer) goto out;
			d3d12Device->CreateRenderTargetView(pBackBuffer, nullptr, rtvHandle);
			frameContext[i].main_render_target_resource = pBackBuffer;
			rtvHandle.ptr += rtvDescriptorSize;
			pBackBuffer->Release();
		};
		//POINT mouse;
		RECT rc = { 0 };
		//md::FadeInOut fade;
		if (!initContext) {
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX12_Init(d3d12Device, buffersCounts,
				DXGI_FORMAT_R8G8B8A8_UNORM,
				d3d12DescriptorHeapImGuiRender,
				d3d12DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
				d3d12DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());

			
			initContext = true;

			ImGuiStyle* style = &ImGui::GetStyle();

			style->WindowPadding = ImVec2(15, 15);
			style->WindowRounding = 10.f;
			style->FramePadding = ImVec2(5, 5);
			style->FrameRounding = 6.f;
			style->ItemSpacing = ImVec2(12, 8);
			style->ItemInnerSpacing = ImVec2(8, 6);
			style->IndentSpacing = 25.0f;
			style->ScrollbarSize = 15.0f;
			style->ScrollbarRounding = 9.0f;
			style->GrabMinSize = 5.0f;
			style->GrabRounding = 3.0f;
			style->WindowTitleAlign = ImVec2(0.5, 0.5);

			//style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
			//style->Colors[ImGuiCol_Separator] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			//style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			//style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			//style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
			//style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
			//style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
			//style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			//style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			//style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			//style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			//style->Colors[ImGuiCol_CheckMark] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			//style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
			//style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
			//style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			//style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			//style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			//style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			//style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			//style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			//style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			//style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			//style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			//style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			//style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			//style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			//style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			//style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			//style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			//style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			//style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			//style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			//style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
			//style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			//style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
			//style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

		};
		if (d3d12CommandQueue == nullptr)
			goto out;
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		Game::GetModuleManager()->onImGUIRender();
		
		FrameContext& currentFrameContext = frameContext[ppSwapChain->GetCurrentBackBufferIndex()];
		currentFrameContext.commandAllocator->Reset();
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = currentFrameContext.main_render_target_resource;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		d3d12CommandList->Reset(currentFrameContext.commandAllocator, nullptr);
		d3d12CommandList->ResourceBarrier(1, &barrier);
		d3d12CommandList->OMSetRenderTargets(1, &currentFrameContext.main_render_target_descriptor, FALSE, nullptr);
		d3d12CommandList->SetDescriptorHeaps(1, &d3d12DescriptorHeapImGuiRender);
		ImGui::EndFrame();
		ImGui::Render();
		RECT window_rectangle, childwindow_rectangle;													//
		GetWindowRect(window, &window_rectangle);														// 计算出ImGui以其所为窗口Y坐标的基准值
		GetWindowRect(childwindow, &childwindow_rectangle);												// 也就是将标题栏富含进去，默认没有，也就导致鼠标悬浮、点击有Y轴偏移
		ImGui::GetDrawData()->DisplayPos.y = (float)(childwindow_rectangle.top - window_rectangle.top); //
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), d3d12CommandList);
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		d3d12CommandList->ResourceBarrier(1, &barrier);
		d3d12CommandList->Close();
		d3d12CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&d3d12CommandList));

		d3d12DescriptorHeapBackBuffers->Release();
		d3d12CommandList->Release();
		allocator->Release();
		currentFrameContext.main_render_target_resource->Release();
		currentFrameContext.commandAllocator->Release();
		d3d12Device->Release();
		delete[] frameContext;
	};
	goto out;
out:
	return oPresentD3D12(ppSwapChain, syncInterval, flags);
};

typedef void(__thiscall* ExecuteCommandListsD3D12)(ID3D12CommandQueue*, UINT, ID3D12CommandList*);
ExecuteCommandListsD3D12 oExecuteCommandListsD3D12;

void hookExecuteCommandListsD3D12(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists) {
	if (!d3d12CommandQueue)
		d3d12CommandQueue = queue;

	oExecuteCommandListsD3D12(queue, NumCommandLists, ppCommandLists);
};
class ImguiHooks {
public:
	static void InitImgui() {
		if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success)
			logF("Created hook for SwapChain::Present (DX12)!");

		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
			logF("Created hook for SwapChain::Present (DX11)!");

		kiero::bind(54, (void**)&oExecuteCommandListsD3D12, hookExecuteCommandListsD3D12);
		kiero::bind(140, (void**)&oPresentD3D12, hookPresentD3D12);
	}
};