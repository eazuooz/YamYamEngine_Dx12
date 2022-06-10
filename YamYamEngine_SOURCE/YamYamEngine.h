#pragma once
#include "CommonInclude.h"

#include <Windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>

#include "d3dx12.h"
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "d3dcompiler")

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2
};
//ImGui_ImplWin32_Data
struct ImplWin32_Data
{
	HWND	hwnd; // ��� ������
	INT32	width; // �ʺ�
	INT32 height; // ����
	bool	windowed; // â��� or ��üȭ��
};