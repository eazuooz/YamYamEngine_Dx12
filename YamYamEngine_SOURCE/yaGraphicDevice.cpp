#include "yaGraphicDevice.h"
#include "yaCommandQueue.h"
#include "yaSwapChain.h"




namespace ya
{
	bool GraphicDevice::CreateDeviceD3D(const ImplWin32_Data& windowsInfo)
	{
#ifdef DX12_ENABLE_DEBUG_LAYER
		ID3D12Debug* pdx12Debug = NULL;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pdx12Debug))))
			pdx12Debug->EnableDebugLayer();
#endif
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_0;
		if (D3D12CreateDevice(NULL, featureLevel, IID_PPV_ARGS(&g_pd3dDevice)) != S_OK)
			return false;

		if (CreateDXGIFactory1(IID_PPV_ARGS(&g_dxgiFactory)) != S_OK)
			return false;

		// [DEBUG] Setup debug interface to break on any warnings/errors
#ifdef DX12_ENABLE_DEBUG_LAYER
		if (pdx12Debug != NULL)
		{
			ID3D12InfoQueue* pInfoQueue = NULL;
			g_pd3dDevice->QueryInterface(IID_PPV_ARGS(&pInfoQueue));
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
			pInfoQueue->Release();
			pdx12Debug->Release();
		}
#endif
		if (!commandQueue->Initailize(g_pd3dDevice, swapChain))
			return false;
		if (!swapChain->Initialize(windowsInfo, g_pd3dDevice, g_dxgiFactory, commandQueue->g_pd3dCommandQueue))
			return false;

		return true;
	}

	void GraphicDevice::Clear()
	{
		commandQueue->Clear();
		swapChain->Clear();
	}

	void GraphicDevice::CleanupDeviceD3D()
	{
		swapChain->CleanupRenderTarget(commandQueue);
		if (swapChain->g_pSwapChain) { swapChain->g_pSwapChain->SetFullscreenState(false, NULL); swapChain->g_pSwapChain = nullptr; }
		if (swapChain->g_hSwapChainWaitableObject != nullptr) { CloseHandle(swapChain->g_hSwapChainWaitableObject); }
		for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
			if (commandQueue->g_frameContext[i].CommandAllocator) { commandQueue->g_frameContext[i].CommandAllocator = nullptr; }
		if (commandQueue->g_pd3dCommandQueue) { commandQueue->g_pd3dCommandQueue = nullptr; }
		if (commandQueue->g_pd3dCommandList) { commandQueue->g_pd3dCommandList = nullptr; }
		//if (g_pd3dRtvDescHeap) { g_pd3dRtvDescHeap->Release(); g_pd3dRtvDescHeap = NULL; }
		//if (g_pd3dSrvDescHeap) { g_pd3dSrvDescHeap->Release(); g_pd3dSrvDescHeap = NULL; }
		//if (g_fence) { g_fence->Release(); g_fence = NULL; }
		if (commandQueue->g_fenceEvent) { CloseHandle(commandQueue->g_fenceEvent); commandQueue->g_fenceEvent = nullptr; }
		//if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }

		//swapChain->CleanupRenderTarget(commandQueue);
		//if (swapChain->g_hSwapChainWaitableObject != nullptr) { CloseHandle(swapChain->g_hSwapChainWaitableObject); }
		//if (commandQueue->g_fenceEvent) { CloseHandle(commandQueue->g_fenceEvent); commandQueue->g_fenceEvent = nullptr; }

		//g_pd3dDevice = nullptr;

#ifdef DX12_ENABLE_DEBUG_LAYER
		IDXGIDebug1* pDebug = NULL;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
		{
				pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
				pDebug->Release();
		}
#endif
	}

	ComPtr<ID3D12Device> GraphicDevice::Get3DDevice()
	{
		return g_pd3dDevice;
	}

	void GraphicDevice::RenderBegin()
	{
		commandQueue->RenderBegin();
	}

	void GraphicDevice::Render()
	{

	}

	void GraphicDevice::RenderEnd()
	{
		commandQueue->RenderEnd();
	}

	GraphicDevice::GraphicDevice()
	{
		swapChain = std::make_shared<SwapChain>();
		commandQueue = std::make_shared<CommandQueue>();

		Clear();
	}
}