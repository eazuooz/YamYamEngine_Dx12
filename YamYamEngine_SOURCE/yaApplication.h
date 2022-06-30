#pragma once
#include "YamYamEngine.h"
#include "yaGraphicDevice.h"

namespace ya
{
	class Application
	{
	public:
		void Initailize();


	private:
		std::unique_ptr<GraphicDevice> graphicDevice;

	public:
		bool CreateDeviceD3D(const ImplWin32_Data& info);
		
		void CreateRenderTarget();
		void WaitForLastSubmittedFrame();
		FrameContext* WaitForNextFrameResources();
		
		void RenderBegin();
		void Render();
		void RenderEnd();

		void CleanupDeviceD3D();
		void CleanupRenderTarget();

	public:
		void ResizeWindow(INT32 width, INT32 height);

	public:
		ImplWin32_Data window;
		D3D12_VIEWPORT viewport;
		D3D12_RECT scissorRect;

		FrameContext                 g_frameContext[NUM_FRAMES_IN_FLIGHT] = {};
		UINT                         g_frameIndex = 0;

		//int const                    NUM_BACK_BUFFERS = 3;
		ComPtr<ID3D12Device> g_pd3dDevice = NULL;
		ComPtr<ID3D12DescriptorHeap> g_pd3dRtvDescHeap = NULL;
		
		ComPtr<ID3D12DescriptorHeap> g_pd3dSrvDescHeap = NULL;
		//ID3D12DescriptorHeap* g_pd3dSrvDescHeap = NULL;

		ComPtr<ID3D12CommandQueue> g_pd3dCommandQueue = NULL;
		ComPtr<ID3D12GraphicsCommandList> g_pd3dCommandList = NULL;
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