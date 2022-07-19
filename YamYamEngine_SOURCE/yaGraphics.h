#pragma once

namespace ya
{
	/// <summary>
	/// YamYamEngine Math 
	/// </summary>
	using Vector2 = XMFLOAT2;
	using Vector3 = XMFLOAT3;
	using Vector4 = XMFLOAT4;
	using Matrix = XMMATRIX;

	/// <summary>
	/// YamYamEngine Graphic 
	/// </summary>
	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
	};
}