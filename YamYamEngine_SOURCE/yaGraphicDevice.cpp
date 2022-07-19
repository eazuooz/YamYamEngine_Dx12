#include "yaGraphicDevice.h"
#include "yaCommandQueue.h"
#include "yaSwapChain.h"
#include "yaRootSignature.h"



namespace ya
{
	bool GraphicDevice::CreateDeviceD3D(const ImplWin32_Data& windowsInfo)
	{
		viewPort = { 0, 0, static_cast<FLOAT>(windowsInfo.width), static_cast<FLOAT>(windowsInfo.height), 0.0f, 1.0f };
		scissorRect = CD3DX12_RECT(0, 0, windowsInfo.width, windowsInfo.height);

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
		if (!rootSignature->Initialize(g_pd3dDevice))
			return false;

		return true;
	}

	void GraphicDevice::Clear()
	{
		ZeroInitialize(viewPort);
		ZeroInitialize(scissorRect);
		g_pd3dDevice = nullptr;
		g_dxgiFactory = nullptr;

		commandQueue->Clear();
		swapChain->Clear();
	}

	void GraphicDevice::CleanupDeviceD3D()
	{
		swapChain->Cleanup(commandQueue);
		commandQueue->Cleanup();
		rootSignature->Cleanup();

		if (g_dxgiFactory) { g_dxgiFactory = nullptr; }
		if (g_pd3dDevice) { g_pd3dDevice = nullptr; }
	}

	void GraphicDevice::MemoryLeakDetector()
	{
#ifdef DX12_ENABLE_DEBUG_LAYER
		IDXGIDebug1* pDebug = NULL;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
		{
			pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
			pDebug->Release();
		}
#endif
	}

	void GraphicDevice::RenderBegin()
	{
		commandQueue->RenderBegin(&viewPort, &scissorRect);
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
		rootSignature = std::make_shared<RootSignature>();

		Clear();
	}
}