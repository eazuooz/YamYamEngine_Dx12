#pragma once
#include "YamYamEngine.h"

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

namespace ya
{
	class GraphicDevice
	{
	public:
		GraphicDevice();

		bool CreateDeviceD3D(const ImplWin32_Data& info);

		void CreateRenderTarget();
		void WaitForLastSubmittedFrame();
		FrameContext* WaitForNextFrameResources();

		void RenderBegin();
		void Render();
		void RenderEnd();

		void Clear();
		void CleanupDeviceD3D();
		void CleanupRenderTarget();

	public:
		HRESULT SwapchainBufferResize(LPARAM lParam);

	public:
		ID3D12Device* Get3DDevice();

	private:
		//ImplWin32_Data winData;

	public:
		FrameContext  g_frameContext[NUM_FRAMES_IN_FLIGHT];
		FrameContext* frameCtx;
		UINT          g_frameIndex;

		//int const                    NUM_BACK_BUFFERS = 3;
		ComPtr<ID3D12Device>		g_pd3dDevice;
		ComPtr<ID3D12DescriptorHeap> g_pd3dRtvDescHeap;
		ComPtr<ID3D12DescriptorHeap> g_pd3dSrvDescHeap;
		ComPtr<ID3D12CommandQueue> g_pd3dCommandQueue;
		ComPtr<ID3D12GraphicsCommandList> g_pd3dCommandList;

		ComPtr <ID3D12Fence> g_fence;
		HANDLE g_fenceEvent;
		UINT64 g_fenceLastSignaledValue;

		ComPtr<IDXGISwapChain3> g_pSwapChain;
		HANDLE                       g_hSwapChainWaitableObject;
		ID3D12Resource* g_mainRenderTargetResource[NUM_BACK_BUFFERS];
		D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[NUM_BACK_BUFFERS];
	};
}
