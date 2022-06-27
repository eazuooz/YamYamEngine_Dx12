#include "YamYamEngine.h"
#include "yaCommandQueue.h"
#include "yaSwapChain.h"
#include "yaDescriptorHeap.h"

namespace ya
{
	bool CommandQueue::Initialize(ComPtr<ID3D12Device> device, std::shared_ptr<class SwapChain> swapChain)
	{
		this->swapChain = swapChain;

		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 1;
		if (device->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue)) != S_OK)
			return false;

		for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
			device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT
											, IID_PPV_ARGS(&frameContext[i].CommandAllocator));

		if (device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT
			, frameContext[0].CommandAllocator, NULL, IID_PPV_ARGS(&commandList)) != S_OK ||
			commandList->Close() != S_OK)
			return false;

		if (device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)) != S_OK)
			return false;

		fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		return true;
	}

	void CommandQueue::WaitForLastSubmittedFrame()
	{
		FrameContext* frameCtx = &frameContext[frameIndex % NUM_FRAMES_IN_FLIGHT];

		UINT64 fenceValue = frameCtx->FenceValue;
		if (fenceValue == 0)
			return; // No fence was signaled

		frameCtx->FenceValue = 0;
		if (fence->GetCompletedValue() >= fenceValue)
			return;

		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	void CommandQueue::Reset()
	{

	}

	void CommandQueue::Cleanup()
	{
		for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
			if (frameContext[i].CommandAllocator)
			{
				frameContext[i].CommandAllocator->Release();
				frameContext[i].CommandAllocator = nullptr;
			}

		if (queue) { queue->Release(); queue = nullptr; }
		if (commandList) { commandList->Release(); commandList = nullptr; }
	}

	void CommandQueue::CleaupFence()
	{
		if (fence) { fence->Release(); fence = nullptr; }
		if (fenceEvent) { CloseHandle(fenceEvent); fenceEvent = nullptr; }
	}

	FrameContext* CommandQueue::WaitForNextFrameResources()
	{
		UINT nextFrameIndex = frameIndex + 1;
		frameIndex = nextFrameIndex;

		HANDLE waitableObjects[] = { swapChain->GetSwapChainWaitableObject(), NULL};
		DWORD numWaitableObjects = 1;

		FrameContext* frameCtx = &frameContext[nextFrameIndex % NUM_FRAMES_IN_FLIGHT];
		UINT64 fenceValue = frameCtx->FenceValue;
		if (fenceValue != 0) // means no fence was signaled
		{
			frameCtx->FenceValue = 0;
			fence->SetEventOnCompletion(fenceValue, fenceEvent);
			waitableObjects[1] = fenceEvent;
			numWaitableObjects = 2;
		}

		WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

		return frameCtx;
	}

	CommandQueue::~CommandQueue()
	{
		CloseHandle(fenceEvent);
	}
}