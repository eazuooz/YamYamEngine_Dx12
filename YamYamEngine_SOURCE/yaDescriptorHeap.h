#pragma once


namespace ya
{
	// [기안서]
	// 외주를 맡길 때 이런 저런 정보들을 같이 넘겨줘야 하는데,
	// 아무 형태로나 요청하면 못 알아먹는다
	// - 각종 리소스를 어떤 용도로 사용하는지 꼼꼼하게 적어서 넘겨주는 용도
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
