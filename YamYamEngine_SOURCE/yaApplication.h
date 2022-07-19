#pragma once
#include "YamYamEngine.h"
#include "yaGraphicDevice.h"

namespace ya
{
	class Application
	{
	public:
		void Initailize();
		void ResizeWindow(INT32 width, INT32 height);
		void ResizeSwapChainBuffer(LPARAM lParam);
		bool Initialize(const ImplWin32_Data& info);
		void WaitForLastSubmittedFrame();
		void RenderBegin();
		void Render();
		void RenderEnd();
		void Clear();
		void Cleanup();

	public:
		Application();

	public:
		std::shared_ptr<GraphicDevice> GetGraphicDevice() { return graphicDevice; }
		ComPtr<ID3D12Device> Get3DDevice();

	private:
		std::shared_ptr<GraphicDevice> graphicDevice;

		std::shared_ptr<class Shader> shader;
		std::shared_ptr<class Mesh> mesh;
		
		
		ImplWin32_Data window;
		D3D12_VIEWPORT viewport;
		D3D12_RECT scissorRect;
	};
}