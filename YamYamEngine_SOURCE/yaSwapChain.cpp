#include "yaSwapChain.h"

namespace ya
{
	bool SwapChain::Initialize(const ImplWin32_Data winData
		, ComPtr<IDXGIFactory4> dxgiFactory
		, ComPtr<ID3D12CommandQueue> commandQueue)
	{
		DXGI_SWAP_CHAIN_DESC1 sd;
		{
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = NUM_BACK_BUFFERS;
			sd.Width = static_cast<UINT32>(winData.width);;
			sd.Height = static_cast<UINT32>(winData.height);
			sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			sd.Scaling = DXGI_SCALING_STRETCH;
			sd.Stereo = FALSE;
		}

		IDXGISwapChain1* swapChain1 = NULL;
		if (dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), winData.hwnd, &sd, NULL, NULL, &swapChain1) != S_OK)
			return false;
		if (swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain)) != S_OK)
			return false;

		swapChain1->Release();
		swapChain->SetMaximumFrameLatency(NUM_BACK_BUFFERS);
		swapChainWaitableObject = swapChain->GetFrameLatencyWaitableObject();

		return true;
	}

	void SwapChain::Render()
	{
		swapChain->Present(0, 0);
	}
	
	void SwapChain::SwapIndex()
	{
		//backBufferIndex = (backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
	}
}
