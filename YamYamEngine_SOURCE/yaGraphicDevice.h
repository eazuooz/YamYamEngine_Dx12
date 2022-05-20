#pragma once
#include "YamYamEngine.h"

namespace ya
{
	class GraphicDevice
	{
	public:
		void Initialize();

	public:
		ComPtr<IDXGIFactory> GetDxgiFactory();
		ComPtr<ID3D12Device> GetDevice();

	private:
		ComPtr<ID3D12Debug> d3dDebug;
		ComPtr<IDXGIFactory> dxgiFactory;
		ComPtr<ID3D12Device> device;
		//ComPtr<ID
	};
}
