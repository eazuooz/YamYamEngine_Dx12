#include "yaApplication.h"
#include "yaGraphicDevice.h"
#include "yaCommandQueue.h"
#include "yaSwapChain.h"
#include "yaDescriptorHeap.h"

namespace ya
{
	bool Application::CreateDeviceD3D(const ImplWin32_Data& windData)
	{
		//windowData = info;
		//ResizeWindow(info.width, info.height);
		//viewport = 
		//{	0, 0, 
		//	static_cast<FLOAT>(info.width), 
		//	static_cast<FLOAT>(info.height), 
		//	0.0f, 1.0f 
		//};
		//scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

		//graphicDevice = std::make_shared<GraphicDevice>();
		//commandQueue = std::make_shared<CommandQueue>();
		//swapChain = std::make_shared<SwapChain>();
		//descriptorHeap = std::make_shared<DescriptorHeap>();

		//graphicDevice->Initialize();
		//commandQueue->Initialize(graphicDevice->GetDevice(), swapChain);
		//swapChain->Initialize(info, graphicDevice->GetDxgiFactory(), commandQueue->GetCommandQueue());
		//descriptorHeap->Initialize(graphicDevice->GetDevice());

		//CreateRenderTarget();


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


		// [DEBUG] Enable debug interface
#ifdef DX12_ENABLE_DEBUG_LAYER
		ID3D12Debug* pdx12Debug = NULL;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pdx12Debug))))
			pdx12Debug->EnableDebugLayer();
#endif

		//ID3D12Device* device = application.Get3DDevice();

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_0;
		if (D3D12CreateDevice(NULL, featureLevel, IID_PPV_ARGS(&g_pd3dDevice)) != S_OK)
			return false;

		//// Create device
		//D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_0;
		//if (D3D12CreateDevice(NULL, featureLevel, IID_PPV_ARGS(&g_pd3dDevice)) != S_OK)
		//    return false;

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

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			desc.NumDescriptors = NUM_BACK_BUFFERS;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			desc.NodeMask = 1;
			if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dRtvDescHeap)) != S_OK)
				return false;

			SIZE_T rtvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
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
			if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dSrvDescHeap)) != S_OK)
				return false;
		}

		{
			D3D12_COMMAND_QUEUE_DESC desc = {};
			desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.NodeMask = 1;
			if (g_pd3dDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_pd3dCommandQueue)) != S_OK)
				return false;
		}

		for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
			if (g_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_frameContext[i].CommandAllocator)) != S_OK)
				return false;

		if (g_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_frameContext[0].CommandAllocator, NULL, IID_PPV_ARGS(&g_pd3dCommandList)) != S_OK ||
			g_pd3dCommandList->Close() != S_OK)
			return false;

		if (g_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence)) != S_OK)
			return false;

		g_fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (g_fenceEvent == NULL)
			return false;

		{
			IDXGIFactory4* dxgiFactory = NULL;
			IDXGISwapChain1* swapChain1 = NULL;
			if (CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)) != S_OK)
				return false;
			if (dxgiFactory->CreateSwapChainForHwnd(g_pd3dCommandQueue, windData.hwnd, &sd, NULL, NULL, &swapChain1) != S_OK)
				return false;
			if (swapChain1->QueryInterface(IID_PPV_ARGS(&g_pSwapChain)) != S_OK)
				return false;
			swapChain1->Release();
			dxgiFactory->Release();
			g_pSwapChain->SetMaximumFrameLatency(NUM_BACK_BUFFERS);
			g_hSwapChainWaitableObject = g_pSwapChain->GetFrameLatencyWaitableObject();
		}

		CreateRenderTarget();
		return true;
	}

	void Application::CleanupDeviceD3D()
	{
		CleanupRenderTarget();
		if (g_pSwapChain) { g_pSwapChain->SetFullscreenState(false, NULL); g_pSwapChain->Release(); g_pSwapChain = NULL; }
		if (g_hSwapChainWaitableObject != NULL) { CloseHandle(g_hSwapChainWaitableObject); }
		for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
			if (g_frameContext[i].CommandAllocator)
			{
				g_frameContext[i].CommandAllocator->Release();
				g_frameContext[i].CommandAllocator = NULL;
			}
		if (g_pd3dCommandQueue) { g_pd3dCommandQueue->Release(); g_pd3dCommandQueue = NULL; }
		if (g_pd3dCommandList) { g_pd3dCommandList->Release(); g_pd3dCommandList = NULL; }
		if (g_pd3dRtvDescHeap) { g_pd3dRtvDescHeap->Release(); g_pd3dRtvDescHeap = NULL; }
		if (g_pd3dSrvDescHeap) { g_pd3dSrvDescHeap->Release(); g_pd3dSrvDescHeap = NULL; }
		if (g_fence) { g_fence->Release(); g_fence = NULL; }
		if (g_fenceEvent) { CloseHandle(g_fenceEvent); g_fenceEvent = NULL; }
		if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }

#ifdef DX12_ENABLE_DEBUG_LAYER
		IDXGIDebug1* pDebug = NULL;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
		{
			pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
			pDebug->Release();
		}
#endif
	}


	void Application::ResizeWindow(INT32 width, INT32 height)
	{
		windowData.width = width;
		windowData.height = height;

		RECT rect = { 0, 0, width, height };
		::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		::SetWindowPos(windowData.hwnd, 0, 100, 100, width, height, 0);
	}


	void Application::CreateRenderTarget()
	{
		for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
		{
			ID3D12Resource* pBackBuffer = NULL;
			g_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
			g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, g_mainRenderTargetDescriptor[i]);
			g_mainRenderTargetResource[i] = pBackBuffer;
		}
	}

	void Application::CleanupRenderTarget()
	{
		WaitForLastSubmittedFrame();

		for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
			if (g_mainRenderTargetResource[i]) { g_mainRenderTargetResource[i]->Release(); g_mainRenderTargetResource[i] = NULL; }
	}



	void Application::WaitForLastSubmittedFrame()
	{
		FrameContext* frameCtx = &g_frameContext[g_frameIndex % NUM_FRAMES_IN_FLIGHT];

		UINT64 fenceValue = frameCtx->FenceValue;
		if (fenceValue == 0)
			return; // No fence was signaled

		frameCtx->FenceValue = 0;
		if (g_fence->GetCompletedValue() >= fenceValue)
			return;

		g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
		WaitForSingleObject(g_fenceEvent, INFINITE);
	}

	FrameContext* Application::WaitForNextFrameResources()
	{
		UINT nextFrameIndex = g_frameIndex + 1;
		g_frameIndex = nextFrameIndex;

		HANDLE waitableObjects[] = { g_hSwapChainWaitableObject, NULL };
		DWORD numWaitableObjects = 1;

		FrameContext* frameCtx = &g_frameContext[nextFrameIndex % NUM_FRAMES_IN_FLIGHT];
		UINT64 fenceValue = frameCtx->FenceValue;
		if (fenceValue != 0) // means no fence was signaled
		{
			frameCtx->FenceValue = 0;
			g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
			waitableObjects[1] = g_fenceEvent;
			numWaitableObjects = 2;
		}

		WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

		return frameCtx;
	}

	void Application::RenderBegin()
	{
		frameCtx = WaitForNextFrameResources();
		UINT backBufferIdx = g_pSwapChain->GetCurrentBackBufferIndex();
		frameCtx->CommandAllocator->Reset();

		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = g_mainRenderTargetResource[backBufferIdx];
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		g_pd3dCommandList->Reset(frameCtx->CommandAllocator, NULL);
		g_pd3dCommandList->ResourceBarrier(1, &barrier);

		XMFLOAT4 clear_color = XMFLOAT4(0.45f, 0.55f, 0.60f, 1.00f);
		// Render Dear ImGui graphics
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		g_pd3dCommandList->ClearRenderTargetView(g_mainRenderTargetDescriptor[backBufferIdx], clear_color_with_alpha, 0, NULL);
		g_pd3dCommandList->OMSetRenderTargets(1, &g_mainRenderTargetDescriptor[backBufferIdx], FALSE, NULL);
		g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dSrvDescHeap);
	}

	void Application::Render()
	{
		

	}

	void Application::RenderEnd()
	{
		UINT backBufferIdx = g_pSwapChain->GetCurrentBackBufferIndex();

		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = g_mainRenderTargetResource[backBufferIdx];
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		g_pd3dCommandList->ResourceBarrier(1, &barrier);
		g_pd3dCommandList->Close();

		g_pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&g_pd3dCommandList);

		g_pSwapChain->Present(1, 0); // Present with vsync
		//g_pSwapChain->Present(0, 0); // Present without vsync

		UINT64 fenceValue = g_fenceLastSignaledValue + 1;
		g_pd3dCommandQueue->Signal(g_fence, fenceValue);
		g_fenceLastSignaledValue = fenceValue;
		frameCtx->FenceValue = fenceValue;
	}

}
