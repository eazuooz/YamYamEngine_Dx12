#pragma once
#include "YamYamEngine.h"
#include "yaConstantBuffer.h"

namespace ya
{
	class Mesh
	{
	public:
		bool Initialize(ComPtr<ID3D12Device> device, std::vector<Vertex>& vec);
		void Render(ComPtr<ID3D12GraphicsCommandList> cmdList);

		void SetTransform(const Transform& t) { transform = t; }

		void Cleanup();

	private:
		ComPtr<ID3D12Resource> vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		UINT32 vertexCount;
		Transform transform;

	public:
		std::shared_ptr<ConstantBuffer> cb;
	};
}
