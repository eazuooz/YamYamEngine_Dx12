#pragma once

namespace ya
{
	class CommandQueue
	{
	public:
		bool Initialize(ComPtr<ID3D12Device> device, std::shared_ptr<class SwapChain> swapChain);
		void WaitForLastSubmittedFrame();
		void Reset();
		void Cleanup();
		void CleaupFence();
		FrameContext* WaitForNextFrameResources();

	public:
		void SetFenceValue(UINT32 value) { fenceValue = value; }
		UINT32 GetFenceValue() { return fenceValue; }
		ComPtr<ID3D12Fence> GetFence() { return fence; }

	public:
		~CommandQueue();

	public:
		ComPtr<ID3D12CommandQueue> GetCommandQueue() { return queue; }
		ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return commandList; }
		HANDLE GetFenceEvent() { return fenceEvent; }

	private:
		FrameContext frameContext[NUM_FRAMES_IN_FLIGHT] = {};
		UINT frameIndex = 0;
		// Command Queue dx12 부터 등장
		// 외주를 요청 할때, 하나 씩 요청하면 비효율적
		// [외주 목록]에 일감을 차곡 차곡 기록했다가 한번에 요청하는 것
		ComPtr<ID3D12CommandQueue> queue;
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		ComPtr<ID3D12GraphicsCommandList> commandList;
		
		// fense 울타리
		// cpu / gpu 동기화를 위한 간단한 도구
		ComPtr<ID3D12Fence> fence;
		UINT32 fenceValue;
		HANDLE fenceEvent = INVALID_HANDLE_VALUE;

		std::shared_ptr<class SwapChain> swapChain;
		std::shared_ptr<class DescriptorHeap> descriptorHeap;
	};
}



