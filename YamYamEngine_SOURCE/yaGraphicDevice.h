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
		bool Initialize();
		void Cleanup();

	public:
		ComPtr<IDXGIFactory4> GetDxgiFactory();
		ComPtr<ID3D12Device> GetDevice();

	private:
		ComPtr<ID3D12Debug> d3dDebug;
		ComPtr<IDXGIFactory4> dxgiFactory;
		ComPtr<ID3D12Device> device;
		//ComPtr<ID
	};
}
