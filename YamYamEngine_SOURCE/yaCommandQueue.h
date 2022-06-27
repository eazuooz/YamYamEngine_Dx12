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
		// Command Queue dx12 ���� ����
		// ���ָ� ��û �Ҷ�, �ϳ� �� ��û�ϸ� ��ȿ����
		// [���� ���]�� �ϰ��� ���� ���� ����ߴٰ� �ѹ��� ��û�ϴ� ��
		ComPtr<ID3D12CommandQueue> queue;
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		ComPtr<ID3D12GraphicsCommandList> commandList;
		
		// fense ��Ÿ��
		// cpu / gpu ����ȭ�� ���� ������ ����
		ComPtr<ID3D12Fence> fence;
		UINT32 fenceValue;
		HANDLE fenceEvent = INVALID_HANDLE_VALUE;

		std::shared_ptr<class SwapChain> swapChain;
		std::shared_ptr<class DescriptorHeap> descriptorHeap;
	};
}



