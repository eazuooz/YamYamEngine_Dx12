#include "YamYamEngine.h"
#include "yaCommandQueue.h"
#include "yaSwapChain.h"
#include "yaRootSignature.h"


namespace ya
{
    bool CommandQueue::Initailize(ComPtr<ID3D12Device> d3dDevice, std::shared_ptr<SwapChain> swapChain, std::shared_ptr<class RootSignature> rootSignature)
    {
		this->swapChain = swapChain;
		this->rootSignature = rootSignature;
		{
			D3D12_COMMAND_QUEUE_DESC desc = {};
			desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.NodeMask = 1;
			if (d3dDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_pd3dCommandQueue)) != S_OK)
				return false;
		}

		for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
			if (d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_frameContext[i].CommandAllocator)) != S_OK)
				return false;

		if (d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_frameContext[0].CommandAllocator.Get(), NULL, IID_PPV_ARGS(&g_pd3dCommandList)) != S_OK ||
			g_pd3dCommandList->Close() != S_OK)
			return false;

		if (d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence)) != S_OK)
			return false;

		g_fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (g_fenceEvent == NULL)
			return false;

        return true;
    }

	void CommandQueue::WaitForLastSubmittedFrame()
	{
		FrameContext* frameCtx = &g_frameContext[g_frameIndex % NUM_FRAMES_IN_FLIGHT];

		UINT64 fenceValue = frameCtx->FenceValue;
		if (fenceValue == 0)
			return; // No fence was signaled

		frameCtx->FenceValue = 0;
		if (g_fence->GetCompletedValue() >= fenceValue)
			return;

		g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
		WaitForSingleObject(g_fenceEvent, INFINITE);
	}

	FrameContext* CommandQueue::WaitForNextFrameResources()
	{
		UINT nextFrameIndex = g_frameIndex + 1;
		g_frameIndex = nextFrameIndex;

		HANDLE waitableObjects[] = { swapChain->g_hSwapChainWaitableObject, NULL };
		DWORD numWaitableObjects = 1;

		FrameContext* frameCtx = &g_frameContext[nextFrameIndex % NUM_FRAMES_IN_FLIGHT];
		UINT64 fenceValue = frameCtx->FenceValue;
		if (fenceValue != 0) // means no fence was signaled
		{
			frameCtx->FenceValue = 0;
			g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
			waitableObjects[1] = g_fenceEvent;
			numWaitableObjects = 2;
		}

		WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

		return frameCtx;
	}

	void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
	{
		UINT backBufferIdx = swapChain->g_pSwapChain->GetCurrentBackBufferIndex();
		frameCtx = WaitForNextFrameResources();
		frameCtx->CommandAllocator->Reset();

		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition
		(
			swapChain->g_mainRenderTargetResource[backBufferIdx].Get(),
			D3D12_RESOURCE_STATE_PRESENT, // ???? ????
			D3D12_RESOURCE_STATE_RENDER_TARGET
		); // ???? ??????

		g_pd3dCommandList->Reset(frameCtx->CommandAllocator.Get(), NULL);
		g_pd3dCommandList->ResourceBarrier(1, &barrier);

		g_pd3dCommandList->SetGraphicsRootSignature(rootSignature->GetSignature().Get());
		g_pd3dCommandList->RSSetViewports(1, vp);
		g_pd3dCommandList->RSSetScissorRects(1, rect);

		//clear_color = XMFLOAT4(0.45f, 0.55f, 0.60f, 1.00f);
		// Render Dear ImGui graphics
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		g_pd3dCommandList->ClearRenderTargetView(swapChain->g_mainRenderTargetDescriptor[backBufferIdx], clear_color_with_alpha, 0, NULL);
		g_pd3dCommandList->OMSetRenderTargets(1, &swapChain->g_mainRenderTargetDescriptor[backBufferIdx], FALSE, NULL);

		//ID3D12DescriptorHeap
		ID3D12DescriptorHeap* heaps[] = {
			swapChain->g_pd3dSrvDescHeap.Get(),
			//
		};
		g_pd3dCommandList->SetDescriptorHeaps(arraysize(heaps), heaps);
	}

	void CommandQueue::RenderEnd()
	{
		UINT backBufferIdx = swapChain->g_pSwapChain->GetCurrentBackBufferIndex();
		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition
		(
			swapChain->g_mainRenderTargetResource[backBufferIdx].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, // ???? ??????
			D3D12_RESOURCE_STATE_PRESENT
		); // ???? ????

		g_pd3dCommandList->ResourceBarrier(1, &barrier);
		g_pd3dCommandList->Close();

		ID3D12CommandList* commandlists[] = {
			g_pd3dCommandList.Get()
		};
		g_pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&commandlists);

		swapChain->g_pSwapChain->Present(1, 0); // Present with vsync
		//g_pSwapChain->Present(0, 0); // Present without vsync

		UINT64 fenceValue = g_fenceLastSignaledValue + 1;
		g_pd3dCommandQueue->Signal(g_fence.Get(), fenceValue);
		g_fenceLastSignaledValue = fenceValue;
		frameCtx->FenceValue = fenceValue;
	}

	void CommandQueue::Clear()
	{
		ZeroInitialize(g_frameContext);
		frameCtx = nullptr;
		g_frameIndex = 0;

		g_pd3dCommandQueue = nullptr;
		g_pd3dCommandList = nullptr;

		g_fence = nullptr;
		g_fenceEvent = nullptr;
		g_fenceLastSignaledValue = 0;
	}
	void CommandQueue::Cleanup()
	{
		for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
			if (g_frameContext[i].CommandAllocator) { g_frameContext[i].CommandAllocator = nullptr; }

		if (g_pd3dCommandQueue) { g_pd3dCommandQueue = nullptr; }
		if (g_pd3dCommandList) { g_pd3dCommandList = nullptr; }
		if (g_fence) { g_fence = nullptr; }
		if (g_fenceEvent) { CloseHandle(g_fenceEvent); g_fenceEvent = nullptr; }
	}

	CommandQueue::CommandQueue()
	{
		Clear();
	}

	CommandQueue::~CommandQueue()
	{

	}
}