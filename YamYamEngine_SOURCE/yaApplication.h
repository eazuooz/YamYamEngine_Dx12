#pragma once
#include "YamYamEngine.h"

namespace ya
{
	class Application
	{
	public:
		virtual void Initialize(const ImplWin32_Data& info);
		virtual void Update(float dt);
		virtual void FixedUpdate();
		virtual void Render();
		//virtual void Compose(wi::graphics::CommandList cmd);

	public:
		void ResizeWindow(INT32 width, INT32 height);

	private:
		ImplWin32_Data windowData;
		D3D12_VIEWPORT viewport;
		D3D12_RECT scissorRect;
		
		std::shared_ptr<class GraphicDevice> graphicDevice;
		std::shared_ptr<class CommandQueue> commandQueue;
		std::shared_ptr<class SwapChain> swapChain;
		std::shared_ptr<class DescriptorHeap> descHeap;
	};
}