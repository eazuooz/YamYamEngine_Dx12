#pragma once
#include "YamYamEngine.h"

// [계약서 / 결재]

// CPU [        ]    GPU [        ]
// 한국 [        ]   베트남 [       ]

namespace ya
{
	class RootSignature
	{
	public:
		bool Initialize(ComPtr<ID3D12Device> device);
		void Cleanup();
		
	public:
		ComPtr<ID3D12RootSignature>	GetSignature() { return signature; }

	private:
		ComPtr<ID3D12RootSignature> signature;
	};
}

