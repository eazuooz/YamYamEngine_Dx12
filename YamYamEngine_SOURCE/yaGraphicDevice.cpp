#include "yaGraphicDevice.h"

void ya::GraphicDevice::Initialize()
{
	// D3D12 ������� Ȱ��ȭ
	// - VC++ ���â�� ���� ����� �޽��� ���
	// - riid : ����̽��� COM ID
	// - ppDevice : ������ ��ġ�� �Ű������� ����
#ifdef _DEBUG
	::D3D12GetDebugInterface(IID_PPV_ARGS(&d3dDebug));
	d3dDebug->EnableDebugLayer();
#endif // _DEBUG

	// DXGI(DirectX Graphics Infrastructure)
	// Direct3D�� �Բ� ���̴� API
	// - ��ü ȭ�� ��� ��ȯ
	// - �����Ǵ� ���÷��� ��� ���� ��
	// CreateDXGIFactory
	// - riid : ����̽��� COM ID
	// - ppDevice : ������ ��ġ�� �Ű������� ����
	::CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	// CreateDevice
	// - ���÷��� �����(�׷��� ī��)�� ��Ÿ���� ��ü
	// - pAdapter : nullptr �����ϸ� �ý��� �⺻ ���÷��� �����
	// - MinimumFeatureLevel : ���� ���α׷��� �䱸�ϴ� �ּ� ��� ���� (���ڴٸ� �ɷ�����)
	// - riid : ����̽��� COM ID
	// - ppDevice : ������ ��ġ�� �Ű������� ����
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device));
}

ComPtr<IDXGIFactory> ya::GraphicDevice::GetDxgiFactory()
{
	return dxgiFactory;
}

ComPtr<ID3D12Device> ya::GraphicDevice::GetDevice()
{
	return device;
}
