#pragma once
#include "YamYamEngine.h"

namespace ya
{
	// [ 외주 과정 ] 
	// - 현재 게임 세상에 있는 상활을 묘사
	// - GPU 가 열심히 계산 ( 외주 )
	// - 결과를 받아서 화면에 그려준다
	// [외주 결과물]을 어디에 받지?
	// - 어떤 종이(Buffer)에 그려서 건내달라고 부탁해보자
	// - 특수 종이를 만들어서 -> 처음에 건내주고 -> 결과물을  해당 종이에 받는다.
	// - 우리 화면에 특수 종이 [외주 결과물] 을 출력해준다
	// - 그런데 화명에 현재 결과물 출력하는 와중에 , 다른 화면도 외주를 맡겨야함
	// 현재 화면 결과물은 이미 화면 출력에 사용중
	// 특수 종이를 2개 만들어서 하나는 화면에 그려주고 하나는 외주 맡기고
	// double buffering
	//현재 화면 [ 1] <-> gpu 작업중 [2] backbuffer

	class SwapChain
	{
	public:
		bool Initialize(const ImplWin32_Data& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory4> dxgiFactory, ComPtr<ID3D12CommandQueue> cmdQueue);

		void CreateRenderTarget(ComPtr<ID3D12Device> device);
		void CleanupRenderTarget(std::shared_ptr<class CommandQueue> cmdQueue);

		void SwapchainBufferResize(LPARAM lParam);

		void Clear();


	private:
		//ComPtr<ID3D12Device> d3dDevice;

	public:
		ComPtr<IDXGISwapChain3>		g_pSwapChain;
		HANDLE						g_hSwapChainWaitableObject;

		ComPtr<ID3D12DescriptorHeap> g_pd3dRtvDescHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[NUM_BACK_BUFFERS];
		ComPtr<ID3D12Resource> g_mainRenderTargetResource[NUM_BACK_BUFFERS];
		ComPtr<ID3D12DescriptorHeap> g_pd3dSrvDescHeap;
	};
}
