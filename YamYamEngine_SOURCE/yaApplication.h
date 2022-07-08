#pragma once
#include "YamYamEngine.h"
#include "yaGraphicDevice.h"

namespace ya
{
	class Application
	{
	public:
		void Initailize();

	private:
		std::shared_ptr<GraphicDevice> graphicDevice;
		

	public:
		void ResizeWindow(LPARAM lParam);
		bool Initialize(const ImplWin32_Data& info);
		void WaitForLastSubmittedFrame();
		void RenderBegin();
		void RenderEnd();
		void Clear();
		void Cleanup();

	public:
		void ResizeWindow(INT32 width, INT32 height);

	public:
		Application();

	public:
		std::shared_ptr<GraphicDevice> GetGraphicDevice() { return graphicDevice; }
		ID3D12Device* Get3DDevice();
		//ID3D12DescriptorHeap* GetSrvDescHeap();
		//ID3D12GraphicsCommandList* GetCommandList();

	private:
		ImplWin32_Data window;
		D3D12_VIEWPORT viewport;
		D3D12_RECT scissorRect;
	};
}