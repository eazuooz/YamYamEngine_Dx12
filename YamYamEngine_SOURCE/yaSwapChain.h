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
		bool Initialize(const ImplWin32_Data winData
						, ComPtr<IDXGIFactory4> dxgiFactory
						, ComPtr<ID3D12CommandQueue> commandQueue);

		void Render();
		void SwapIndex();

		ComPtr<IDXGISwapChain3> GetSwapChain() { return swapChain; }
		HANDLE GetSwapChainWaitableObject() { return swapChainWaitableObject; }
		void ClearSwapChain() { swapChain.Reset(); }

	private:
		ComPtr<IDXGISwapChain3> swapChain;
		HANDLE swapChainWaitableObject;
	};
}
