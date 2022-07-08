#pragma once


namespace ya
{
	class CommandQueue
	{
	public:
		bool Initailize(ComPtr<ID3D12Device> device, std::shared_ptr<class SwapChain> swapChain /*ComPtr<IDXGISwapChain3> swapChain*/);
		void WaitForLastSubmittedFrame();
		FrameContext* WaitForNextFrameResources();
		void RenderBegin();
		void RenderEnd();
		void Clear();

	public:
		ComPtr<ID3D12Device>		device;
		std::shared_ptr<class SwapChain> swapChain;
		ComPtr<ID3D12CommandQueue> g_pd3dCommandQueue;
		ComPtr<ID3D12GraphicsCommandList> g_pd3dCommandList;
		FrameContext  g_frameContext[NUM_FRAMES_IN_FLIGHT];
		FrameContext* frameCtx;
		UINT          g_frameIndex;
		
		ComPtr <ID3D12Fence> g_fence;

		HANDLE g_fenceEvent;
		UINT64 g_fenceLastSignaledValue;

		XMFLOAT4 clear_color;
	};
}



