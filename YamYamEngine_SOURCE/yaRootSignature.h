#pragma once
#include "YamYamEngine.h"

// [��༭ / ����]

// CPU [        ]    GPU [        ]
// �ѱ� [        ]   ��Ʈ�� [       ]

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

