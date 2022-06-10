#include "yaSwapChain.h"

namespace ya
{
	void SwapChain::Initialize(const ImplWin32_Data winData
		, ComPtr<IDXGIFactory> dxgiFactory
		, ComPtr<ID3D12CommandQueue> commandQueue)
	{
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = static_cast<UINT32>(winData.width);
		sd.BufferDesc.Height= static_cast<UINT32>(winData.height);
		sd.BufferDesc.RefreshRate.Numerator = 144;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = 1; //MultiSampling off
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 후면 버퍼 렌더링
		sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
		sd.OutputWindow = winData.hwnd;
		sd.Windowed = winData.windowed;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; 
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		dxgiFactory->CreateSwapChain(commandQueue.Get(), &sd, &swapChain);

		for (size_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
			swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
	}
}
