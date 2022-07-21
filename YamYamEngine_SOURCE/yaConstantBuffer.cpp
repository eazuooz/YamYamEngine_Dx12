#include "yaConstantBuffer.h"



namespace ya
{
	ConstantBuffer::ConstantBuffer()
	{

	}

	ConstantBuffer::~ConstantBuffer()
	{
		if (cbvBuffer)
		{
			if (cbvBuffer != nullptr)
				cbvBuffer->Unmap(0, nullptr);

			cbvBuffer = nullptr;
		}
	}

	void ConstantBuffer::Initialize(UINT32 size, UINT32 count)
	{
		//상수 버퍼는 256 바이트 배수로 만들어야 한다.
		// 0 256 512 768
		elementSize = (size + 255) & ~255;
		elementCount = count;

		Create();
	}

	void ConstantBuffer::Clear()
	{
		currentIndex = 0;
	}

	void ConstantBuffer::PushData(INT32 rootParamIndex, void* buffer, UINT32 size)
	{
	}

	D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(UINT32 index)
	{
		return D3D12_GPU_VIRTUAL_ADDRESS();
	}

	void ConstantBuffer::Create()
	{
		UINT32 bufferSize = elementSize * elementCount;
		D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);


	}

}
