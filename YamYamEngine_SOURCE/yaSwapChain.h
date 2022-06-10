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
		void Initialize(const ImplWin32_Data winData
						, ComPtr<IDXGIFactory> dxgiFactory
						, ComPtr<ID3D12CommandQueue> commandQueue);

		void Render();
		void SwapIndex();



	private:
		ComPtr<IDXGISwapChain> swapChain;
		ComPtr<ID3D12Resource> renderTargets[SWAP_CHAIN_BUFFER_COUNT];
		UINT32 backBufferIndex = 0;
	};
}
