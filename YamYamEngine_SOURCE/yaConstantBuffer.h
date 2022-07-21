#pragma once
#include "YamYamEngine.h"

namespace ya
{
	class ConstantBuffer
	{
	public:
		ConstantBuffer();
		~ConstantBuffer();

		void Initialize(UINT32 size, UINT32 count);

		void Clear();
		void PushData(INT32 rootParamIndex, void* buffer, UINT32 size);

		D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(UINT32 index);

	private:
		void Create();

	private:
		ComPtr<ID3D12Resource> cbvBuffer;
		BYTE* mappedBuffer;
		UINT32 elementSize;
		UINT32 elementCount;
		UINT32 currentIndex;

	};
}
