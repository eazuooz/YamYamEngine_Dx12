#pragma once
#include "YamYamEngine.h"

namespace ya
{
	class Shader
	{
	public:
		bool Initialize(const std::wstring& path, ComPtr<ID3D12Device> device, ComPtr<ID3D12RootSignature>	signature);
		void Update(ComPtr<ID3D12GraphicsCommandList> cmdList, ComPtr<ID3D12RootSignature>	signature);

		void Cleanup();

	private:
		bool CreateShader(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
		bool CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version);
		bool CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version);

	private:
		ComPtr<ID3DBlob>					vsBlob;
		ComPtr<ID3DBlob>					psBlob;
		ComPtr<ID3DBlob>					errBlob;

		ComPtr<ID3D12PipelineState>			pipelineState;
		D3D12_GRAPHICS_PIPELINE_STATE_DESC  pipelineDesc;
	};
}
