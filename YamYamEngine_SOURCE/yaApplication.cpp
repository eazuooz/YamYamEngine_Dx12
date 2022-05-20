#include "yaApplication.h"
#include "yaGraphicDevice.h"

namespace ya
{
	void Application::Initialize(const ImplWin32_Data& info)
	{
		windowData = info;
		ResizeWindow(info.width, info.height);
		viewport = 
		{	0, 0, 
			static_cast<FLOAT>(info.width), 
			static_cast<FLOAT>(info.height), 
			0.0f, 1.0f 
		};
		scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

		graphicDevice = std::make_shared<GraphicDevice>();
		commandQueue = std::make_shared<CommandQueue>();
		swapChain = std::make_shared<SwapChain>();
		descHeap = std::make_shared<DescriptorHeap>();

		graphicDevice->Initialize();

	}

	void Application::Update(float dt)
	{
	}

	void Application::FixedUpdate()
	{
	}

	void Application::Render()
	{
	}

	void Application::ResizeWindow(INT32 width, INT32 height)
	{
		windowData.width = width;
		windowData.height = height;

		RECT rect = { 0, 0, width, height };
		::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		::SetWindowPos(windowData.hwnd, 0, 100, 100, width, height, 0);
	}
}
