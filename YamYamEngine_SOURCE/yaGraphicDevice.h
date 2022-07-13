#pragma once
#include "YamYamEngine.h"

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

namespace ya
{
	class GraphicDevice
	{
	public:
		bool CreateDeviceD3D(const ImplWin32_Data& info);
		void RenderBegin();
		void Render();
		void RenderEnd();
		void Clear();
		void CleanupDeviceD3D();

	public:
		GraphicDevice();

	public:

		std::shared_ptr<class CommandQueue> GetCmdQueue() { return commandQueue; }
		std::shared_ptr<class SwapChain> GetSwapChain() { return swapChain; }
		ComPtr<ID3D12Device> Get3DDevice() { return g_pd3dDevice; };
	
	private:
		std::shared_ptr<class CommandQueue> commandQueue;
		std::shared_ptr<class SwapChain> swapChain;
		std::shared_ptr<class RootSignature> rootSignature;

		ComPtr<ID3D12Device>		g_pd3dDevice;
		ComPtr<IDXGIFactory4>		g_dxgiFactory;

		D3D12_VIEWPORT viewPort;
		D3D12_RECT scissorRect;
	};
}
