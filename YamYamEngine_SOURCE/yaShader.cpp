#include "yaShader.h"


namespace ya
{
	bool Shader::Initialize(const std::wstring& path, ComPtr<ID3D12Device> device, ComPtr<ID3D12RootSignature>	signature)
	{
		if (!CreateVertexShader(path, "VS_Main", "vs_5_0"))
			return false;

		if (!CreatePixelShader(path, "PS_Main", "ps_5_0"))
			return false;

		D3D12_INPUT_ELEMENT_DESC desc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		pipelineDesc.InputLayout = { desc, _countof(desc) };
		pipelineDesc.pRootSignature = signature.Get();

		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pipelineDesc.DepthStencilState.DepthEnable = FALSE;
		pipelineDesc.DepthStencilState.StencilEnable = FALSE;
		pipelineDesc.SampleMask = UINT_MAX;
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineDesc.NumRenderTargets = 1;
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineDesc.SampleDesc.Count = 1;

		if (device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState)) != S_OK)
			return false;

		return true;
	}

	void Shader::Update(ComPtr<ID3D12GraphicsCommandList> cmdList, ComPtr<ID3D12RootSignature>	signature)
	{
		cmdList->SetPipelineState(pipelineState.Get());
	}

	void Shader::Cleanup()
	{
		if (vsBlob) { vsBlob = nullptr; }
		if (psBlob) { psBlob = nullptr; }
		if (errBlob) { errBlob = nullptr; }

		if (pipelineState) { pipelineState = nullptr; }
		ZeroInitialize(pipelineDesc);
	}

	bool Shader::CreateShader(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode)
	{
		UINT32 compileFlag = 0;

#ifdef _DEBUG
		compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif 
		if (FAILED(::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, name.c_str(), version.c_str(), compileFlag, 0, &blob, &errBlob)))
		{
			::MessageBoxA(nullptr, "Shader Create Failed !", nullptr, MB_OK);

			auto error = (char*)errBlob->GetBufferPointer();
			std::cout << error << std::endl;


			return false;
		}
		shaderByteCode = { blob->GetBufferPointer(), blob->GetBufferSize() };

		return true;
	}

	bool Shader::CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version)
	{
		return CreateShader(path, name, version, vsBlob, pipelineDesc.VS);
	}

	bool Shader::CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version)
	{
		return CreateShader(path, name, version, psBlob, pipelineDesc.PS);
	}
}