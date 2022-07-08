#pragma once
#include "YamYamEngine.h"

namespace ya
{
	// [ ���� ���� ] 
	// - ���� ���� ���� �ִ� ��Ȱ�� ����
	// - GPU �� ������ ��� ( ���� )
	// - ����� �޾Ƽ� ȭ�鿡 �׷��ش�
	// [���� �����]�� ��� ����?
	// - � ����(Buffer)�� �׷��� �ǳ��޶�� ��Ź�غ���
	// - Ư�� ���̸� ���� -> ó���� �ǳ��ְ� -> �������  �ش� ���̿� �޴´�.
	// - �츮 ȭ�鿡 Ư�� ���� [���� �����] �� ������ش�
	// - �׷��� ȭ�� ���� ����� ����ϴ� ���߿� , �ٸ� ȭ�鵵 ���ָ� �ðܾ���
	// ���� ȭ�� ������� �̹� ȭ�� ��¿� �����
	// Ư�� ���̸� 2�� ���� �ϳ��� ȭ�鿡 �׷��ְ� �ϳ��� ���� �ñ��
	// double buffering
	//���� ȭ�� [ 1] <-> gpu �۾��� [2] backbuffer

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
