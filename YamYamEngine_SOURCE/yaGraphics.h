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

	struct Transform
	{
		Vector4 offset;
	};

	enum class Format
	{
		UNKNOWN,

		R32G32B32A32_FLOAT,
		R32G32B32A32_UINT,
		R32G32B32A32_SINT,

		R32G32B32_FLOAT,
		R32G32B32_UINT,
		R32G32B32_SINT,

		R16G16B16A16_FLOAT,
		R16G16B16A16_UNORM,
		R16G16B16A16_UINT,
		R16G16B16A16_SNORM,
		R16G16B16A16_SINT,

		R32G32_FLOAT,
		R32G32_UINT,
		R32G32_SINT,
		R32G8X24_TYPELESS,		// depth (32-bit) + stencil (8-bit) + shader resource (32-bit)
		D32_FLOAT_S8X24_UINT,	// depth (32-bit) + stencil (8-bit)

		R10G10B10A2_UNORM,
		R10G10B10A2_UINT,
		R11G11B10_FLOAT,
		R8G8B8A8_UNORM,
		R8G8B8A8_UNORM_SRGB,
		R8G8B8A8_UINT,
		R8G8B8A8_SNORM,
		R8G8B8A8_SINT,
		B8G8R8A8_UNORM,
		B8G8R8A8_UNORM_SRGB,
		R16G16_FLOAT,
		R16G16_UNORM,
		R16G16_UINT,
		R16G16_SNORM,
		R16G16_SINT,
		R32_TYPELESS,			// depth (32-bit) + shader resource (32-bit)
		D32_FLOAT,				// depth (32-bit)
		R32_FLOAT,
		R32_UINT,
		R32_SINT,
		R24G8_TYPELESS,			// depth (24-bit) + stencil (8-bit) + shader resource (24-bit)
		D24_UNORM_S8_UINT,		// depth (24-bit) + stencil (8-bit)

		R8G8_UNORM,
		R8G8_UINT,
		R8G8_SNORM,
		R8G8_SINT,
		R16_TYPELESS,			// depth (16-bit) + shader resource (16-bit)
		R16_FLOAT,
		D16_UNORM,				// depth (16-bit)
		R16_UNORM,
		R16_UINT,
		R16_SNORM,
		R16_SINT,

		R8_UNORM,
		R8_UINT,
		R8_SNORM,
		R8_SINT,

		BC1_UNORM,
		BC1_UNORM_SRGB,
		BC2_UNORM,
		BC2_UNORM_SRGB,
		BC3_UNORM,
		BC3_UNORM_SRGB,
		BC4_UNORM,
		BC4_SNORM,
		BC5_UNORM,
		BC5_SNORM,
		BC6H_UF16,
		BC6H_SF16,
		BC7_UNORM,
		BC7_UNORM_SRGB
	};

	enum class Usage
	{
		DEFAULT,	// CPU no access, GPU read/write
		UPLOAD,	    // CPU write, GPU read
		READBACK,	// CPU read, GPU write
	};

	enum class BindFlag
	{
		NONE = 0,
		VERTEX_BUFFER = 1 << 0,
		INDEX_BUFFER = 1 << 1,
		CONSTANT_BUFFER = 1 << 2,
		SHADER_RESOURCE = 1 << 3,
		RENDER_TARGET = 1 << 4,
		DEPTH_STENCIL = 1 << 5,
		UNORDERED_ACCESS = 1 << 6,
		SHADING_RATE = 1 << 7,
	};

	enum class ResourceMiscFlag
	{
		NONE = 0,
		TEXTURECUBE = 1 << 0,
		INDIRECT_ARGS = 1 << 1,
		BUFFER_RAW = 1 << 2,
		BUFFER_STRUCTURED = 1 << 3,
		RAY_TRACING = 1 << 4,
		PREDICATION = 1 << 5,
	};

	struct GPUBufferDesc
	{
		uint64_t size = 0;
		Usage usage = Usage::DEFAULT;
		BindFlag bind_flags = BindFlag::NONE;
		ResourceMiscFlag misc_flags = ResourceMiscFlag::NONE;
		uint32_t stride = 0; // needed for typed and structured buffer types!
		Format format = Format::UNKNOWN; // only needed for typed buffer!
	};

	struct GraphicsDeviceChild
	{
		std::shared_ptr<void> internal_state;
		inline bool IsValid() const { return internal_state.get() != nullptr; }
	};

	struct GPUResource : public GraphicsDeviceChild
	{
		enum class Type
		{
			BUFFER,
			TEXTURE,
			RAYTRACING_ACCELERATION_STRUCTURE,
			UNKNOWN_TYPE,
		} type = Type::UNKNOWN_TYPE;
		inline bool IsTexture() const { return type == Type::TEXTURE; }
		inline bool IsBuffer() const { return type == Type::BUFFER; }
		inline bool IsAccelerationStructure() const { return type == Type::RAYTRACING_ACCELERATION_STRUCTURE; }

		void* mapped_data = nullptr;
		uint32_t mapped_rowpitch = 0;
	};

	struct GPUBuffer : public GPUResource
	{
		GPUBufferDesc desc;

		const GPUBufferDesc& GetDesc() const { return desc; }
	};
}