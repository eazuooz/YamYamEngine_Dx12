#include "yaMesh.h"
#include "yaGraphicDevice.h"
#include "yaCommandQueue.h"

namespace ya
{
    bool Mesh::Initialize(ComPtr<ID3D12Device> device, std::vector<Vertex>& vec)
    {
        vertexCount = static_cast<UINT32>(vec.size());
        UINT32 bufferSize = vertexCount * sizeof(Vertex);

        D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
        
        if (device->CreateCommittedResource(
            &heapProperty,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&vertexBuffer)) != S_OK)
            return false;
        
        // Copy the triangle data to the vertex buffer.
        void* vertexDataBuffer = nullptr;
        CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
        vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
        ::memcpy(vertexDataBuffer, &vec[0], bufferSize);
        vertexBuffer->Unmap(0, nullptr);

        // Initialize the vertex buffer view.
        vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
        vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
        vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기

        return true;
    }

    void Mesh::Render(ComPtr<ID3D12GraphicsCommandList> cmdList)
    {
        //m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

        //m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        //m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
        //m_commandList->DrawInstanced(3, 1, 0, 0);

        cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        cmdList->IASetVertexBuffers(0, 1, &vertexBufferView); // Slot: (0~15)
        cmdList->DrawInstanced(vertexCount, 1, 0, 0);
    }
    void Mesh::Cleanup()
    {
        if (vertexBuffer) { vertexBuffer = nullptr; }
        ZeroInitialize(vertexBufferView);
        vertexCount = 0;
    }
}