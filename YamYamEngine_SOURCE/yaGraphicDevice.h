#pragma once
#include "YamYamEngine.h"

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

namespace ya
{
	class GraphicDevice
	{
	public:
		GraphicDevice(ImplWin32_Data window);
	};
}
