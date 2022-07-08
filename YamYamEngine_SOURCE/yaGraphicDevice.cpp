#include "yaGraphicDevice.h"
#include "yaCommandQueue.h"
#include "yaSwapChain.h"




namespace ya
{
    GraphicDevice::GraphicDevice()
    {
		swapChain = std::make_shared<SwapChain>();
		commandQueue = std::make_shared<CommandQueue>();

		Clear();
    }

	bool GraphicDevice::CreateDeviceD3D(const ImplWin32_Data& windowsInfo)
	{




		//winData = windowsInfo;

		// Setup swap chain
		//DXGI_SWAP_CHAIN_DESC1 sd;
		//{
		//	ZeroMemory(&sd, sizeof(sd));
		//	sd.BufferCount = NUM_BACK_BUFFERS;
		//	sd.Width = 0;
		//	sd.Height = 0;
		//	sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//	sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
		//	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//	sd.SampleDesc.Count = 1;
		//	sd.SampleDesc.Quality = 0;
		//	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		//	sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		//	sd.Scaling = DXGI_SCALING_STRETCH;
		//	sd.Stereo = FALSE;
		//}


		// [DEBUG] Enable debug interface
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


		////swapchain
		//{
		//	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		//	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		//	desc.NumDescriptors = NUM_BACK_BUFFERS;
		//	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		//	desc.NodeMask = 1;

		//	if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dRtvDescHeap)) != S_OK)
		//		return false;

		//	SIZE_T rtvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		//	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
		//	for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
		//	{
		//		g_mainRenderTargetDescriptor[i] = rtvHandle;
		//		rtvHandle.ptr += rtvDescriptorSize;
		//	}
		//}

		//{
		//	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		//	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		//	desc.NumDescriptors = 1;
		//	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;


		//	if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dSrvDescHeap)) != S_OK)
		//		return false;
		//}

		//commandqueue
		//{
		//	D3D12_COMMAND_QUEUE_DESC desc = {};
		//	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		//	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		//	desc.NodeMask = 1;
		//	if (g_pd3dDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_pd3dCommandQueue)) != S_OK)
		//		return false;
		//}

		//for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
		//	if (g_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_frameContext[i].CommandAllocator)) != S_OK)
		//		return false;

		//if (g_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_frameContext[0].CommandAllocator.Get(), NULL, IID_PPV_ARGS(&g_pd3dCommandList)) != S_OK ||
		//	g_pd3dCommandList->Close() != S_OK)
		//	return false;

		//if (g_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence)) != S_OK)
		//	return false;

		//g_fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		//if (g_fenceEvent == NULL)
		//	return false;

		//{
		//	//g_dxgiFactory
		//	//IDXGIFactory4* dxgiFactory = NULL;
		//	IDXGISwapChain1* swapChain1 = NULL;
		//	if (CreateDXGIFactory1(IID_PPV_ARGS(&g_dxgiFactory)) != S_OK)
		//		return false;
		//	if (g_dxgiFactory->CreateSwapChainForHwnd(g_pd3dCommandQueue.Get(), windowsInfo.hwnd, &sd, NULL, NULL, &swapChain1) != S_OK)
		//		return false;
		//	if (swapChain1->QueryInterface(IID_PPV_ARGS(&g_pSwapChain)) != S_OK)
		//		return false;
		//	swapChain1->Release();
		//	//g_dxgiFactory->Release();
		//	g_pSwapChain->SetMaximumFrameLatency(NUM_BACK_BUFFERS);
		//	g_hSwapChainWaitableObject = g_pSwapChain->GetFrameLatencyWaitableObject();
		//}

		//CreateRenderTarget();
		return true;
	}

	void GraphicDevice::Clear()
	{
		//ZeroInitialize(g_frameContext);
		//frameCtx = nullptr;
		//g_frameIndex = 0;

		//g_pd3dDevice = nullptr;
		//g_pd3dRtvDescHeap = nullptr;
		//g_pd3dSrvDescHeap = nullptr;
		//g_pd3dCommandQueue = nullptr;
		//g_pd3dCommandList = nullptr;

		//g_fence = nullptr;
		//g_fenceEvent = nullptr;
		//g_fenceLastSignaledValue = 0;

		//g_pSwapChain = nullptr;
		//g_hSwapChainWaitableObject = nullptr;
		//ZeroInitialize(g_mainRenderTargetResource);
		//ZeroInitialize(g_mainRenderTargetDescriptor);
		commandQueue->Clear();
		swapChain->Clear();
	}

	void GraphicDevice::CleanupDeviceD3D()
	{
		swapChain->CleanupRenderTarget(commandQueue);
		//CleanupRenderTarget();
		//if (g_pSwapChain) { g_pSwapChain->SetFullscreenState(false, NULL); g_pSwapChain->Release(); g_pSwapChain = nullptr; }
		if (swapChain->g_hSwapChainWaitableObject != nullptr) { CloseHandle(swapChain->g_hSwapChainWaitableObject); }

		//for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
		//	if (g_frameContext[i].CommandAllocator)
		//	{
		//		g_frameContext[i].CommandAllocator->Release();
		//		g_frameContext[i].CommandAllocator = nullptr;
		//	}


		//if (g_pd3dCommandQueue) { g_pd3dCommandQueue->Release(); g_pd3dCommandQueue = NULL; }
		//if (g_pd3dCommandList) { g_pd3dCommandList->Release(); g_pd3dCommandList = NULL; }
		//if (g_pd3dRtvDescHeap) { g_pd3dRtvDescHeap->Release(); g_pd3dRtvDescHeap = NULL; }
		//if (g_pd3dSrvDescHeap) { g_pd3dSrvDescHeap->Release(); g_pd3dSrvDescHeap = NULL; }
		//if (g_fence) { g_fence->Release(); g_fence = NULL; }
		if (commandQueue->g_fenceEvent) { CloseHandle(commandQueue->g_fenceEvent); commandQueue->g_fenceEvent = nullptr; }
		//if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }

#ifdef DX12_ENABLE_DEBUG_LAYER
		IDXGIDebug1* pDebug = NULL;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
		{
			//	pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
			//	//pDebug->Release();
		}
#endif
	}

	//void GraphicDevice::CreateRenderTarget()
	//{
	//	for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
	//	{
	//		ID3D12Resource* pBackBuffer = NULL;
	//		g_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
	//		g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, g_mainRenderTargetDescriptor[i]);
	//		g_mainRenderTargetResource[i] = pBackBuffer;
	//	}
	//}

	//void GraphicDevice::CleanupRenderTarget()
	//{
	//	WaitForLastSubmittedFrame();

	//	for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
	//		if (g_mainRenderTargetResource[i]) { g_mainRenderTargetResource[i]->Release(); g_mainRenderTargetResource[i] = nullptr; }
	//}

	//void GraphicDevice::SwapchainBufferResize(LPARAM lParam)
	//{
	//	//g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT);
	//	HRESULT result 
	//		= swapChain->g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT);
	//	assert(SUCCEEDED(result) && "Failed to resize swapchain.");
	//}

	ID3D12Device* GraphicDevice::Get3DDevice()
	{
		return g_pd3dDevice.Get();
	}



	//void GraphicDevice::WaitForLastSubmittedFrame()
	//{
	//	FrameContext* frameCtx = &g_frameContext[g_frameIndex % NUM_FRAMES_IN_FLIGHT];

	//	UINT64 fenceValue = frameCtx->FenceValue;
	//	if (fenceValue == 0)
	//		return; // No fence was signaled

	//	frameCtx->FenceValue = 0;
	//	if (g_fence->GetCompletedValue() >= fenceValue)
	//		return;

	//	g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
	//	WaitForSingleObject(g_fenceEvent, INFINITE);
	//}

	//FrameContext* GraphicDevice::WaitForNextFrameResources()
	//{
	//	UINT nextFrameIndex = g_frameIndex + 1;
	//	g_frameIndex = nextFrameIndex;

	//	HANDLE waitableObjects[] = { g_hSwapChainWaitableObject, NULL };
	//	DWORD numWaitableObjects = 1;

	//	FrameContext* frameCtx = &g_frameContext[nextFrameIndex % NUM_FRAMES_IN_FLIGHT];
	//	UINT64 fenceValue = frameCtx->FenceValue;
	//	if (fenceValue != 0) // means no fence was signaled
	//	{
	//		frameCtx->FenceValue = 0;
	//		g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
	//		waitableObjects[1] = g_fenceEvent;
	//		numWaitableObjects = 2;
	//	}

	//	WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

	//	return frameCtx;
	//}

	void GraphicDevice::RenderBegin()
	{
		commandQueue->RenderBegin();

		//UINT backBufferIdx = g_pSwapChain->GetCurrentBackBufferIndex();
		//frameCtx = WaitForNextFrameResources();
		//frameCtx->CommandAllocator->Reset();

		//D3D12_RESOURCE_BARRIER barrier = {};
		//barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		//barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		//barrier.Transition.pResource = g_mainRenderTargetResource[backBufferIdx].Get();
		//barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		//barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		//g_pd3dCommandList->Reset(frameCtx->CommandAllocator.Get(), NULL);
		//g_pd3dCommandList->ResourceBarrier(1, &barrier);

		//XMFLOAT4 clear_color = XMFLOAT4(0.45f, 0.55f, 0.60f, 1.00f);
		//// Render Dear ImGui graphics
		//const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		//g_pd3dCommandList->ClearRenderTargetView(g_mainRenderTargetDescriptor[backBufferIdx], clear_color_with_alpha, 0, NULL);
		//g_pd3dCommandList->OMSetRenderTargets(1, &g_mainRenderTargetDescriptor[backBufferIdx], FALSE, NULL);

		////ID3D12DescriptorHeap
		//ID3D12DescriptorHeap* heaps[1] = {
		//	g_pd3dSrvDescHeap.Get(),
		//	//
		//};

		//g_pd3dCommandList->SetDescriptorHeaps(arraysize(heaps), heaps);
	}

	void GraphicDevice::Render()
	{


	}

	void GraphicDevice::RenderEnd()
	{
		commandQueue->RenderEnd();

		//UINT backBufferIdx = g_pSwapChain->GetCurrentBackBufferIndex();

		//D3D12_RESOURCE_BARRIER barrier = {};
		//barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		//barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		//barrier.Transition.pResource = g_mainRenderTargetResource[backBufferIdx].Get();
		//barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		//barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		//barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		//g_pd3dCommandList->ResourceBarrier(1, &barrier);
		//g_pd3dCommandList->Close();


		//ID3D12CommandList* commandlists[] = {
		//	g_pd3dCommandList.Get()
		//};
		//g_pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&commandlists);

		//g_pSwapChain->Present(1, 0); // Present with vsync
		////g_pSwapChain->Present(0, 0); // Present without vsync

		//UINT64 fenceValue = g_fenceLastSignaledValue + 1;
		//g_pd3dCommandQueue->Signal(g_fence.Get(), fenceValue);
		//g_fenceLastSignaledValue = fenceValue;
		//frameCtx->FenceValue = fenceValue;
	}
}