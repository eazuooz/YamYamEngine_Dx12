#pragma once


namespace ya
{
	// [��ȼ�]
	// ���ָ� �ñ� �� �̷� ���� �������� ���� �Ѱ���� �ϴµ�,
	// �ƹ� ���·γ� ��û�ϸ� �� �˾ƸԴ´�
	// - ���� ���ҽ��� � �뵵�� ����ϴ��� �Ĳ��ϰ� ��� �Ѱ��ִ� �뵵
	class DescriptorHeap
	{
	public:
		bool Initialize(ComPtr<ID3D12Device> device);
		void Cleanup();
		
	public:
		D3D12_CPU_DESCRIPTOR_HANDLE		GetRenderTargetView(INT32 idx) { return handle[idx]; }
		ComPtr<ID3D12DescriptorHeap>	GetSrvDescHeap() { return srvDescHeap; }
		ComPtr<ID3D12DescriptorHeap>	GetretvDescHeap() { return retvDescHeap; }

	private:
		ComPtr<ID3D12DescriptorHeap>			retvDescHeap;
		ComPtr<ID3D12DescriptorHeap>			srvDescHeap;
		UINT32									heapSize = 0;
		D3D12_CPU_DESCRIPTOR_HANDLE				handle[NUM_BACK_BUFFERS];

		std::shared_ptr<class SwapChain>		swapChain;
		std::shared_ptr<class CommandQueue>		commandQueue;
	};
}
