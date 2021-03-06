#include "yaSwapChain.h"
#include "yaCommandQueue.h"

namespace ya
{
	bool SwapChain::Initialize(const ImplWin32_Data& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory4> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
	{
		//this->d3dDevice = device;

		// Setup swap chain
		DXGI_SWAP_CHAIN_DESC1 sd;
		{
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = NUM_BACK_BUFFERS;
			sd.Width = 0;
			sd.Height = 0;
			sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			sd.Scaling = DXGI_SCALING_STRETCH;
			sd.Stereo = FALSE;
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			desc.NumDescriptors = NUM_BACK_BUFFERS;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			desc.NodeMask = 1;

			if (device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dRtvDescHeap)) != S_OK)
				return false;

			SIZE_T rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
			for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
			{
				g_mainRenderTargetDescriptor[i] = rtvHandle;
				rtvHandle.ptr += rtvDescriptorSize;
			}
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			desc.NumDescriptors = 1;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;


			if (device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dSrvDescHeap)) != S_OK)
				return false;
		}

		IDXGISwapChain1* swapChain1 = NULL;

		if (dxgi->CreateSwapChainForHwnd(cmdQueue.Get(), info.hwnd, &sd, NULL, NULL, &swapChain1) != S_OK)
			return false;
		if (swapChain1->QueryInterface(IID_PPV_ARGS(&g_pSwapChain)) != S_OK)
			return false;
		swapChain1->Release();

		g_pSwapChain->SetMaximumFrameLatency(NUM_BACK_BUFFERS);
		g_hSwapChainWaitableObject = g_pSwapChain->GetFrameLatencyWaitableObject();

		CreateRenderTarget(device);

		return true;
	}

	void SwapChain::CreateRenderTarget(ComPtr<ID3D12Device> device)
	{
		for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
		{
			ID3D12Resource* pBackBuffer = NULL;
			g_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
			device->CreateRenderTargetView(pBackBuffer, NULL, g_mainRenderTargetDescriptor[i]);
			g_mainRenderTargetResource[i] = pBackBuffer;
		}
	}

	void SwapChain::CleanupRenderTarget(std::shared_ptr<CommandQueue> cmdQueue)
	{
		cmdQueue->WaitForLastSubmittedFrame();

		for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
			if (g_mainRenderTargetResource[i]) { g_mainRenderTargetResource[i]->Release(); g_mainRenderTargetResource[i] = nullptr; }
	}

	void SwapChain::SwapchainBufferResize(LPARAM lParam)
	{
		//g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT);
		HRESULT result
			= g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT);
		assert(SUCCEEDED(result) && "Failed to resize swapchain.");
	}
	void SwapChain::Clear()
	{
		g_pd3dRtvDescHeap = nullptr;
		g_pd3dSrvDescHeap = nullptr;

		g_pSwapChain = nullptr;
		g_hSwapChainWaitableObject = nullptr;
		ZeroInitialize(g_mainRenderTargetResource);
		ZeroInitialize(g_mainRenderTargetDescriptor);
	}

	void SwapChain::Cleanup(std::shared_ptr<CommandQueue> commandQueue)
	{
		//ComPtr<IDXGISwapChain3>		g_pSwapChain;

		//ComPtr<ID3D12DescriptorHeap> g_pd3dRtvDescHeap;
		//ComPtr<ID3D12Resource> g_mainRenderTargetResource[NUM_BACK_BUFFERS];
		//ComPtr<ID3D12DescriptorHeap> g_pd3dSrvDescHeap;

		CleanupRenderTarget(commandQueue);
		if (g_pSwapChain) { g_pSwapChain->SetFullscreenState(false, NULL); g_pSwapChain = nullptr; }
		if (g_hSwapChainWaitableObject != nullptr) { CloseHandle(g_hSwapChainWaitableObject); }
		if (g_pd3dRtvDescHeap) { g_pd3dRtvDescHeap = nullptr; }
		if (g_pd3dSrvDescHeap) { g_pd3dSrvDescHeap = nullptr; }
	}
}
