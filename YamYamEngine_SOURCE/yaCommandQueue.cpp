#include "yaCommandQueue.h"
#include "yaSwapChain.h"
#include "yaDescriptorHeap.h"

namespace ya
{
	void CommandQueue::Initialize(ComPtr<ID3D12Device> device
									, std::shared_ptr<SwapChain> swapChain
									, std::shared_ptr<class DescriptorHeap> descriptorHeap)
	{
		this->swapChain = swapChain;
		this->descriptorHeap = descriptorHeap;

		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue));

		// D3D12_COMMAND_LIST_TYPE_DIRECT : GPU�� ���� �����ϴ� ��� ���
		device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT
										, IID_PPV_ARGS(&commandAllocator));
		
		// GPU�� �ϳ��� �ý��ۿ����� 0����
		// DIRECT or BUNDLE
		// Allocator
		// �ʱ� ���� (�׸��� ����� nullptr ����)
		device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT
									, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&queue));

		// CommandList�� close / open ���°� �ִµ�
		// open ���¿��� command �� �ִٰ� close�� ���� �����ϴ� ����
		list->Close();

		// CreateFense
		// cpu - gpu ����ȭ ����
		device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	}

	void CommandQueue::WaitSync()
	{
		// Advance the fence value to mark command up to this fence point
		fenceValue++;

		// Add an instruction to the command queue to set a new fence point Because we
		// are on the gpu timeline. the new fense point won't be set until the gpu finishes
		// processing all the commands prior to this signal()
		queue->Signal(fence.Get(), fenceValue);

		// Wait until the GPU has completed commands up to this fence point
		if (fence->GetCompletedValue() < fenceValue)
		{
			// Fire event when GPU hits current fence
			fence->SetEventOnCompletion(fenceValue, fenceEvent);

			// Wait untill the GPU hits current fence event is fired.
			WaitForSingleObject(fenceEvent, INFINITE);
		}
	}

	void CommandQueue::RenderBegin(const D3D12_VIEWPORT viewPort, const D3D12_RECT* rect)
	{
	}

	void CommandQueue::RenderEnd()
	{
	}

	CommandQueue::~CommandQueue()
	{
		CloseHandle(fenceEvent);
	}
}