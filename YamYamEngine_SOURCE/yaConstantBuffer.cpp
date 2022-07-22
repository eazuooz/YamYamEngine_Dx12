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

	void ConstantBuffer::Initialize(ComPtr<ID3D12Device> device, UINT32 size, UINT32 count)
	{
		//상수 버퍼는 256 바이트 배수로 만들어야 한다.
		// 0 256 512 768
		elementSize = (size + 255) & ~255;
		elementCount = count;

		Create(device);
	}

	void ConstantBuffer::Clear()
	{
		currentIndex = 0;
	}

	void ConstantBuffer::PushData(ComPtr<ID3D12GraphicsCommandList> commandList, INT32 rootParamIndex, void* buffer, UINT32 size)
	{
		assert(currentIndex < elementSize);

		::memcpy(&mappedBuffer[currentIndex * elementSize], buffer, size);

		D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(currentIndex);
		commandList->SetGraphicsRootConstantBufferView(rootParamIndex, address);
		currentIndex++;
	}

	D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(UINT32 index)
	{
		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = cbvBuffer->GetGPUVirtualAddress();
		objCBAddress += index * elementSize;
		return objCBAddress;
	}

	void ConstantBuffer::Cleanup()
	{
		if (cbvBuffer)
			cbvBuffer = nullptr;
	}

	void ConstantBuffer::Create(ComPtr<ID3D12Device> device)
	{
		UINT32 bufferSize = elementSize * elementCount;
		D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

		device->CreateCommittedResource
		(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&cbvBuffer)
		);

		cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedBuffer));
	}

}
