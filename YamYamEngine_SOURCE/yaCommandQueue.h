#pragma once
#include "YamYamEngine.h"


namespace ya
{
	class CommandQueue
	{
	public:
		void Initialize(ComPtr<ID3D12Device> device
						, std::shared_ptr<class SwapChain> swapChain
						, std::shared_ptr<class DescriptorHeap> descriptorHeap);
		void WaitSync();

		void RenderBegin(const D3D12_VIEWPORT viewPort, const D3D12_RECT* rect);
		void RenderEnd();

	public:
		~CommandQueue();

	public:
		ComPtr<ID3D12CommandQueue> GetCommandQueue() { return queue; }

	private:
		// Command Queue dx12 ���� ����
		// ���ָ� ��û �Ҷ�, �ϳ� �� ��û�ϸ� ��ȿ����
		// [���� ���]�� �ϰ��� ���� ���� ����ߴٰ� �ѹ��� ��û�ϴ� ��
		ComPtr<ID3D12CommandQueue> queue;
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		ComPtr<ID3D12GraphicsCommandList> list;

		// fense ��Ÿ��
		// cpu / gpu ����ȭ�� ���� ������ ����
		ComPtr<ID3D12Fence> fence;
		UINT32 fenceValue;
		HANDLE fenceEvent = INVALID_HANDLE_VALUE;

		std::shared_ptr<class SwapChain> swapChain;
		std::shared_ptr<class DescriptorHeap> descriptorHeap;
	};
}



