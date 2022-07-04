#include "yaApplication.h"
#include "yaGraphicDevice.h"
#include "yaCommandQueue.h"
#include "yaSwapChain.h"
#include "yaDescriptorHeap.h"


namespace ya
{
	void Application::Initailize()
	{
		//Clear();
		graphicDevice = std::make_unique<GraphicDevice>();
		//graphicDevice->CreateDeviceD3D(window);
	}

	void Application::ResizeWindow(LPARAM lParam)
	{
		graphicDevice->WaitForLastSubmittedFrame();
		graphicDevice->CleanupRenderTarget();
		HRESULT result = graphicDevice->SwapchainBufferResize(lParam);
		assert(SUCCEEDED(result) && "Failed to resize swapchain.");
		graphicDevice->CreateRenderTarget();
	}

	bool Application::CreateDeviceD3D(const ImplWin32_Data& windData)
	{
		bool result = false;
		result = graphicDevice->CreateDeviceD3D(windData);

		return result;
	}

	void Application::WaitForLastSubmittedFrame()
	{
		graphicDevice->WaitForLastSubmittedFrame();
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


	void Application::ResizeWindow(INT32 width, INT32 height)
	{
		window.width = width;
		window.height = height;

		RECT rect = { 0, 0, width, height };
		::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		::SetWindowPos(window.hwnd, 0, 100, 100, width, height, 0);
	}

	Application::Application()
	{
		Initailize();
	}

	ID3D12Device* Application::Get3DDevice()
	{
		return graphicDevice->Get3DDevice();
	}

	ID3D12DescriptorHeap* Application::GetSrvDescHeap()
	{
		return graphicDevice->g_pd3dSrvDescHeap.Get();
	}

	ID3D12GraphicsCommandList* Application::GetCommandList()
	{
		return graphicDevice->g_pd3dCommandList.Get();
	}





}
