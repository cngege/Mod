//ImGui Shit

#include "../Utils/Logger.h"

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


auto GetDllMod(void) -> HMODULE {
	MEMORY_BASIC_INFORMATION info;
	size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)GetDllMod, &info, sizeof(info));
	assert(len == sizeof(info));
	return len ? (HMODULE)info.AllocationBase : NULL;
}

//Index shit
int countnum = -1;

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
	D3D12_CPU_DESCRIPTOR_HANDLE main_render_target_descriptor;
};
uint64_t buffersCounts = -1;
FrameContext* frameContext = nullptr;
ID3D12DescriptorHeap* d3d12DescriptorHeapImGuiRender = nullptr;
ID3D12DescriptorHeap* d3d12DescriptorHeapBackBuffers = nullptr;
ID3D12GraphicsCommandList* d3d12CommandList = nullptr;
ID3D12CommandAllocator* allocator = nullptr;
D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
ID3D12CommandQueue* d3d12CommandQueue = nullptr;
bool initContext = false;
HRESULT hookPresentD3D12(IDXGISwapChain3* ppSwapChain, UINT syncInterval, UINT flags) {
	auto deviceType = ID3D_Device_Type::INVALID_DEVICE_TYPE;
	//auto window = (HWND)FindWindowA(nullptr, (LPCSTR)"Minecraft");
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
		if (!initContext)
			ImGui::CreateContext();
		ID3D11DeviceContext* ppContext = nullptr;
		d3d11Device->GetImmediateContext(&ppContext);
		ID3D11Texture2D* pBackBuffer;
		ppSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		ID3D11RenderTargetView* mainRenderTargetView;
		d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
		pBackBuffer->Release();
		//POINT mouse;
		//RECT rc = { 0 };
		//md::FadeInOut fade;
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(d3d11Device, ppContext);
		if (!initContext) {
			ImGuiIO& io = ImGui::GetIO();
			(void)io;

			std::string font_JNMYT = Utils::WStringToString(Logger::GetRoamingFolderPath()) + std::string("\\Mod\\Assets\\JNMYT.ttf");
			if (_access(font_JNMYT.c_str(), 0 /*F_OK*/) != -1) {
				io.Fonts->AddFontFromFileTTF(font_JNMYT.c_str(), 15.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
			}
			io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 15.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
			// 这里注意值如果不是常亮就要当心其被释放掉
			io.IniFilename = Game::ImConfigIni.c_str();

			initContext = true;
		}
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
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
		}

		Game::GetModuleManager()->onImGUIRender();
		
		ImGui::Render();
		ppContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		mainRenderTargetView->Release();
		d3d11Device->Release();
	}
	else if (deviceType == ID3D_Device_Type::D3D12) {
		if (!initContext)
			ImGui::CreateContext();
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
			return false;
		for (size_t i = 0; i < buffersCounts; i++) {
			frameContext[i].commandAllocator = allocator;
		};
		if (d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, NULL, IID_PPV_ARGS(&d3d12CommandList)) != S_OK ||
			d3d12CommandList->Close() != S_OK)
			return false;
		D3D12_DESCRIPTOR_HEAP_DESC descriptorBackBuffers;
		descriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descriptorBackBuffers.NumDescriptors = buffersCounts;
		descriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		descriptorBackBuffers.NodeMask = 1;
		if (d3d12Device->CreateDescriptorHeap(&descriptorBackBuffers, IID_PPV_ARGS(&d3d12DescriptorHeapBackBuffers)) != S_OK)
			return false;
		const auto rtvDescriptorSize = d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		rtvHandle = d3d12DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();
		for (size_t i = 0; i < buffersCounts; i++) {
			ID3D12Resource* pBackBuffer = nullptr;
			frameContext[i].main_render_target_descriptor = rtvHandle;
			ppSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
			d3d12Device->CreateRenderTargetView(pBackBuffer, nullptr, rtvHandle);
			frameContext[i].main_render_target_resource = pBackBuffer;
			rtvHandle.ptr += rtvDescriptorSize;
			pBackBuffer->Release();
		};
		if (!initContext) {
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX12_Init(d3d12Device, buffersCounts,
				DXGI_FORMAT_R8G8B8A8_UNORM,
				d3d12DescriptorHeapImGuiRender,
				d3d12DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
				d3d12DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());

			ImGuiIO& io = ImGui::GetIO();
			(void)io;

			std::string font_JNMYT = Utils::WStringToString(Logger::GetRoamingFolderPath()) + std::string("\\Mod\\Assets\\JNMYT.ttf");
			if (_access(font_JNMYT.c_str(), 0 /*F_OK*/) != -1) {
				io.Fonts->AddFontFromFileTTF(font_JNMYT.c_str(), 15.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
			}
			io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 15.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
			// 这里注意值如果不是常亮就要当心其被释放掉
			io.IniFilename = Game::ImConfigIni.c_str();

			initContext = true;
		};
		if (d3d12CommandQueue == nullptr)
			goto out;
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
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
		}

		Game::GetModuleManager()->onImGUIRender();


		FrameContext& currentFrameContext = frameContext[ppSwapChain->GetCurrentBackBufferIndex()];
		currentFrameContext.commandAllocator->Reset();
		D3D12_RESOURCE_BARRIER barrier;
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
		delete frameContext;
	};
	goto out;
out:
	return oPresentD3D12(ppSwapChain, syncInterval, flags);
};

//CommandList
typedef void(__thiscall* ExecuteCommandListsD3D12)(ID3D12CommandQueue*, UINT, ID3D12CommandList*);
ExecuteCommandListsD3D12 oExecuteCommandListsD3D12;
void hookExecuteCommandListsD3D12(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists) {
	if (!d3d12CommandQueue)
		d3d12CommandQueue = queue;
	oExecuteCommandListsD3D12(queue, NumCommandLists, ppCommandLists);
};

//Instanced
typedef void(__stdcall* D3D12DrawInstanced)(ID3D12GraphicsCommandList* dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
D3D12DrawInstanced o_D12DrawInstanced = NULL;
void __stdcall hkDrawInstancedD12(ID3D12GraphicsCommandList* dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation) {
	return o_D12DrawInstanced(dCommandList, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

//Indexed
typedef void(__stdcall* D3D12DrawIndexedInstanced)(ID3D12GraphicsCommandList* dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex);
D3D12DrawIndexedInstanced o_D12DrawIndexedInstanced = NULL;
void __stdcall hkDrawIndexedInstancedD12(ID3D12GraphicsCommandList* dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex) {
	//Tests
	 
	//for (int i = -3; i <= 3; i++) {
	//	INT offset = BaseVertex + i * 10;
	//	o_D12DrawIndexedInstanced(dCommandList, IndexCount, InstanceCount, StartIndex, offset); /multiply rendering
	//}

	//IndexCount = 3; // Only draw the first three triangles of the mesh

	//static float time = 0.0f;
	//BaseVertex += static_cast<int>(sin(time) * 10.0f); // Offset the vertices based on a sine wave
	//time += 0.01f;

	//IndexCount += rand() % 10; // Add a random offset to IndexCount
	//StartIndex += rand() % 10; // Add a random offset to StartIndex
	//BaseVertex += rand() % 10; // Add a random offset to BaseVertex

	//static float time = 0.0f;
	//time += 0.1f; // Increase time to make the wave move faster
	//float wave = sin(time) * 10.0f; // Calculate the wave value based on the current time
	//BaseVertex += (INT)wave; // Add the wave value to the BaseVertex parameter

	//Wireframe
	//dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

	//static float time = 0.0f;
	//time += 0.001f; // Increase time to make the wave move faster
	//float wave = sin(time) * 10.0f; // Calculate the wave value based on the current time
	//if (IndexCount == 72 || IndexCount == 180) {
	//	BaseVertex += static_cast<int>(wave); // Add the wave value to the BaseVertex parameter
	//}

	return o_D12DrawIndexedInstanced(dCommandList, IndexCount, InstanceCount, StartIndex, BaseVertex);
}

//Hooks
class ImguiHooks {
public:
	static void InitImgui() {
		if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success)
			logF("Created hook for SwapChain::Present (DX12)!");

		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
			logF("Created hook for SwapChain::Present (DX11)!");

		kiero::bind(54, (void**)&oExecuteCommandListsD3D12, hookExecuteCommandListsD3D12);
		kiero::bind(140, (void**)&oPresentD3D12, hookPresentD3D12);
		kiero::bind(84, (void**)&o_D12DrawInstanced, hkDrawInstancedD12);
		kiero::bind(85, (void**)&o_D12DrawIndexedInstanced, hkDrawIndexedInstancedD12);
	}
};