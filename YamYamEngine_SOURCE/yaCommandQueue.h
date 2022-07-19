#pragma once


namespace ya
{
	class CommandQueue
	{
	public:
		bool Initailize(ComPtr<ID3D12Device> d3dDevice, std::shared_ptr<class SwapChain> swapChain, std::shared_ptr<class RootSignature> rootSignature);
		void WaitForLastSubmittedFrame();
		FrameContext* WaitForNextFrameResources();

		void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
		void RenderEnd();

		void Clear();
		void Cleanup();

	public:
		CommandQueue();
		~CommandQueue();
		
	public:
		std::shared_ptr<class SwapChain> swapChain;
		std::shared_ptr<class RootSignature> rootSignature;

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



