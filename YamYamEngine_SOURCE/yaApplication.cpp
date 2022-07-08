#include "yaApplication.h"
#include "yaGraphicDevice.h"
#include "yaCommandQueue.h"
#include "yaSwapChain.h"



namespace ya
{
	void Application::Initailize()
	{
		graphicDevice = std::make_shared<GraphicDevice>();
	}

	void Application::ResizeWindow(INT32 width, INT32 height)
	{
		window.width = width;
		window.height = height;

		RECT rect = { 0, 0, width, height };
		::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		::SetWindowPos(window.hwnd, 0, 100, 100, width, height, 0);
	}

	void Application::ResizeSwapChainBuffer(LPARAM lParam)
	{
		graphicDevice->GetCmdQueue()->WaitForLastSubmittedFrame();
		graphicDevice->GetSwapChain()->CleanupRenderTarget(graphicDevice->GetCmdQueue());
		graphicDevice->GetSwapChain()->SwapchainBufferResize(lParam);
		graphicDevice->GetSwapChain()->CreateRenderTarget(graphicDevice->Get3DDevice());
	}

	bool Application::Initialize(const ImplWin32_Data& windData)
	{
		if (!graphicDevice->CreateDeviceD3D(windData))
			return false;

		return true;
	}

	void Application::WaitForLastSubmittedFrame()
	{
		graphicDevice->GetCmdQueue()->WaitForLastSubmittedFrame();
	}

	void Application::RenderBegin()
	{
		graphicDevice->RenderBegin();
	}

	void Application::RenderEnd()
	{
		graphicDevice->RenderEnd();
	}

	void Application::Clear()
	{

		graphicDevice->Clear();
	}

	void Application::Cleanup()
	{
		graphicDevice->CleanupDeviceD3D();
	}

	Application::Application()
	{
		//Clear();
		Initailize();
	}

	ComPtr<ID3D12Device> Application::Get3DDevice()
	{
		return graphicDevice->Get3DDevice();
	}
}
