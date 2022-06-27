#pragma once
#include "YamYamEngine.h"

namespace ya
{
	class Application
	{
	public:
		virtual bool CreateDeviceD3D(const ImplWin32_Data& info);
		
		void CreateRenderTarget();
		void WaitForLastSubmittedFrame();
		FrameContext* WaitForNextFrameResources();
		
		void RenderBegin();
		void Render();
		void RenderEnd();

		void CleanupDeviceD3D();
		void CleanupRenderTarget();
		
		
		//virtual void Compose(wi::graphics::CommandList cmd);

	public:
		void ResizeWindow(INT32 width, INT32 height);

	public:


	public:
		ImplWin32_Data windowData;
		D3D12_VIEWPORT viewport;
		D3D12_RECT scissorRect;

		FrameContext                 g_frameContext[NUM_FRAMES_IN_FLIGHT] = {};
		UINT                         g_frameIndex = 0;

		//int const                    NUM_BACK_BUFFERS = 3;
		ID3D12Device* g_pd3dDevice = NULL;
		ID3D12DescriptorHeap* g_pd3dRtvDescHeap = NULL;
		ID3D12DescriptorHeap* g_pd3dSrvDescHeap = NULL;
		ID3D12CommandQueue* g_pd3dCommandQueue = NULL;
		ID3D12GraphicsCommandList* g_pd3dCommandList = NULL;
		ID3D12Fence* g_fence = NULL;
		HANDLE                       g_fenceEvent = NULL;
		UINT64                       g_fenceLastSignaledValue = 0;
		IDXGISwapChain3* g_pSwapChain = NULL;
		HANDLE                       g_hSwapChainWaitableObject = NULL;
		ID3D12Resource* g_mainRenderTargetResource[NUM_BACK_BUFFERS] = {};
		D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[NUM_BACK_BUFFERS] = {};
		FrameContext* frameCtx;

		 
	};
}