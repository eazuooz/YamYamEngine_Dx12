#include "yaApplication.h"
#include "yaGraphicDevice.h"
#include "yaCommandQueue.h"
#include "yaSwapChain.h"
#include "yaRootSignature.h"

#include "yaShader.h"
#include "yaMesh.h"




namespace ya
{
	void Application::Initailize()
	{
		graphicDevice = std::make_shared<GraphicDevice>();
		shader = std::make_shared<Shader>();
		mesh = std::make_shared<Mesh>();
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

		std::vector<Vertex> vec(3);
		vec[0].pos = Vector3(0.f, 0.5f, 0.5f);
		vec[0].color = Vector4(1.f, 0.f, 0.f, 1.f);
		vec[1].pos = Vector3(0.5f, -0.5f, 0.5f);
		vec[1].color = Vector4(0.f, 1.0f, 0.f, 1.f);
		vec[2].pos = Vector3(-0.5f, -0.5f, 0.5f);
		vec[2].color = Vector4(0.f, 0.f, 1.f, 1.f);

		mesh->Initialize(graphicDevice->Get3DDevice(), vec);
		shader->Initialize(L"..\\Shaders\\default.hlsli", graphicDevice->Get3DDevice(), graphicDevice->GetRootSignature()->GetSignature());

		WaitForLastSubmittedFrame();
		return true;
	}

	void Application::WaitForLastSubmittedFrame()
	{
		graphicDevice->GetCmdQueue()->WaitForLastSubmittedFrame();
	}

	void Application::RenderBegin()
	{
		mesh->cb->Clear();
		graphicDevice->RenderBegin();
	}

	void Application::Render()
	{
		shader->Update(graphicDevice->GetCmdQueue()->g_pd3dCommandList, graphicDevice->GetRootSignature()->GetSignature());

		{
			Transform t;
			t.offset = Vector4(0.75f, 0.f, 0.f, 0.f);
			mesh->SetTransform(t);

			mesh->Render(graphicDevice->GetCmdQueue()->g_pd3dCommandList);
		}
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
		mesh->Cleanup();
		shader->Cleanup();

		graphicDevice->MemoryLeakDetector();
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
