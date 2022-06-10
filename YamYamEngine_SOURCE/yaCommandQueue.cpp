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

		// D3D12_COMMAND_LIST_TYPE_DIRECT : GPU가 직접 실행하는 명령 목록
		device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT
										, IID_PPV_ARGS(&commandAllocator));
		
		// GPU가 하나인 시스템에서는 0으로
		// DIRECT or BUNDLE
		// Allocator
		// 초기 상태 (그리기 명령은 nullptr 지정)
		device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT
									, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&queue));

		// CommandList는 close / open 상태가 있는데
		// open 상태에서 command 를 넣다가 close한 다음 제출하는 개념
		list->Close();

		// CreateFense
		// cpu - gpu 동기화 수단
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