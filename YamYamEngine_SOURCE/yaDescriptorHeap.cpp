#include "YamYamEngine.h"
#include "yaDescriptorHeap.h"
#include "yaSwapChain.h"
#include "yaCommandQueue.h"


namespace ya
{
	bool DescriptorHeap::Initialize(ComPtr<ID3D12Device> device)
	{
		// Descriptor (DX12) = View (~DX11)
		// [서술자 힙]으로 RTV 생성
		// DX11의 RTV(RenderTargetView), DSV(DepthStencilView), 
		// CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)를 전부!

		
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			desc.NumDescriptors = NUM_BACK_BUFFERS;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			desc.NodeMask = 1;

			if (device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&retvDescHeap)) != S_OK)
				return false;

			//D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
			//rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			//rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
			//rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			//rtvDesc.NodeMask = 0;

			//// 같은 종류의 데이터끼리 배열로 관리
			//// RTV 목록 : [ ] [ ]
			//device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&heap));
			heapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = retvDescHeap->GetCPUDescriptorHandleForHeapStart();
			for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
			{
				handle[i] = rtvHandle;
				rtvHandle.ptr += heapSize;
			}

			{
				D3D12_DESCRIPTOR_HEAP_DESC desc = {};
				desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				desc.NumDescriptors = 1;
				desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				if (device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvDescHeap)) != S_OK)
					return false;
			}

		}

		return true;
	}

	void DescriptorHeap::Cleanup()
	{
		if (retvDescHeap) { retvDescHeap->Release(); retvDescHeap = nullptr; }
		if (srvDescHeap) { srvDescHeap->Release(); srvDescHeap = nullptr; }
	}




}