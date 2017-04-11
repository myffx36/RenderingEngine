#pragma once

#include "Container.hpp"
#include "CommonUtility.h"
#include "PoolAllocateObject.h"
#include "BasicMath.h"

namespace Necromancer{
	enum GeometryType{
		NECROMANCER_GEOMETRY_POINTS,
		NECROMANCER_GEOMETRY_LINES,
		NECROMANCER_GEOMETRY_MESH
	};

	enum BindFlag{
		BIND_TO_VS = 1,
		BIND_TO_FS = 2,
		BIND_TO_GS = 4,
		BIND_TO_CS = 8,
		BIND_TO_DS = 16,
		BIND_TO_HS = 32
	};

	class RenderingResource:public PoolAllocateObject{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, u_id);
	public:
		RenderingResource();
		virtual ~RenderingResource();
	};

	enum BufferUsage{
		NM_BUFFER_USAGE_DEFAULT,
		NM_BUFFER_USAGE_IMMUTABLE,
		NM_BUFFER_USAGE_DYNAMIC
	};
	class BufferObjectPointer:public PoolAllocateObject{
	public:
		virtual ~BufferObjectPointer();
	};
	class BufferObject:public RenderingResource{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(BufferUsage, buffer_usage);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, size);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(BufferObjectPointer, pointer);
	public:
		BufferObject();
		BufferObject(U32 size, BufferUsage);
		virtual ~BufferObject();
	};


	class VertexBuffer:public BufferObject{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, stride);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, offset);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, format);
	public:
		VertexBuffer();
		VertexBuffer(U32 size, U32 stride,
			U32 offset, const String& format, 
			BufferUsage buffer_usage);
		virtual ~VertexBuffer();
	};

	class IndexBuffer:public BufferObject{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, index_number);
	public:
		IndexBuffer();
		IndexBuffer(U32 index_number,
			BufferUsage buffer_usage);
		virtual ~IndexBuffer();
	};


	class ConstantBuffer:public BufferObject{
	public:
		ConstantBuffer();
		ConstantBuffer(U32 size);
		virtual ~ConstantBuffer();
	};

	class VertexShaderPointer:public PoolAllocateObject{
	public:
		virtual ~VertexShaderPointer();
	};
	class VertexShader:public RenderingResource{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(VertexShaderPointer, pointer);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, output_format);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, input_format);
	public:
		VertexShader();
		virtual ~VertexShader();
	};

	class FragmentShaderPointer:public PoolAllocateObject{
	public:
		virtual ~FragmentShaderPointer();
	};
	class FragmentShader:public RenderingResource{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(FragmentShaderPointer, pointer);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, output_format);
	public:
		FragmentShader();
		virtual ~FragmentShader();
	};

	enum TextureElementType{
		NECROMANCER_TEXTURE_R8,
		NECROMANCER_TEXTURE_R32,
		NECROMANCER_TEXTURE_R32G32,
		NECROMANCER_TEXTURE_R32G32B32,
		NECROMANCER_TEXTURE_R32G32B32A32,
		NECROMANCER_TEXTURE_D24S8,
		NECROMANCER_TEXTURE_R8G8B8A8,
		NECROMANCER_TEXTURE_R8G8B8A8I,
		NECROMANCER_TEXTURE_R32_UINT
	};
	class TexturePointer:public PoolAllocateObject{
	public:
		virtual ~TexturePointer();
	};
	class Texture:public virtual RenderingResource{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(TextureElementType, element_type);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(TexturePointer, texture_pointer);
	public:
		Texture();
		Texture(TextureElementType element_type);
		virtual ~Texture();
	};


	class Texture2DPointer:public PoolAllocateObject{
	public:
		virtual ~Texture2DPointer();
	};
	class Texture2D:public Texture{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, width);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, height);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, mip_map_level);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(Texture2DPointer, texture_2d_pointer);
	public:
		Texture2D();
		Texture2D(TextureElementType element_type,
			U32 width, U32 height, U32 mip_map_level);
		virtual ~Texture2D();
	};

	class TextureCubePointer:public PoolAllocateObject{
	public:
		virtual ~TextureCubePointer();
	};
	class TextureCube:public Texture{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, width);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, height);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(TextureCubePointer, texture_cube_pointer);
	public:
		TextureCube();
		TextureCube(TextureElementType element_type,
			U32 width, U32 height);
		virtual ~TextureCube();
	};

	class RenderTargetPointer:public PoolAllocateObject{
	public:
		virtual ~RenderTargetPointer();
	};
	class RenderTarget:public virtual RenderingResource{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, rt_width);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, rt_height);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(RenderTargetPointer, render_target_pointer);
	public:
		RenderTarget();
		RenderTarget(U32 rt_width, 
			U32 rt_height);
		virtual ~RenderTarget();
	};

	class RenderableTexture2D:public Texture2D, public RenderTarget{
	public:
		RenderableTexture2D();
		RenderableTexture2D(
			TextureElementType element_type,
			U32 width, U32 height);
		virtual ~RenderableTexture2D();
	};

	class RenderableTextureCube:public TextureCube, public RenderTarget{
	public:
		RenderableTextureCube();
		RenderableTextureCube(
			TextureElementType element_type,
			U32 width, U32 height);
		virtual ~RenderableTextureCube();
	};

	class WindowRenderTargetPointer:public PoolAllocateObject{
	public:
		virtual ~WindowRenderTargetPointer();
	private:
	};
	class WindowRenderTarget:public RenderTarget{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(WindowRenderTargetPointer, wrt_ptr);
	public:
		WindowRenderTarget(U32 rt_width, 
			U32 rt_height);
		virtual ~WindowRenderTarget();
	private:
	};

	class DepthStencilPointer:public PoolAllocateObject{
	public:
		virtual ~DepthStencilPointer();
	};
	class DepthStencil: public virtual RenderingResource{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, ds_width);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, ds_height);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(DepthStencilPointer, depth_stencil_pointer);
	public:
		DepthStencil();
		DepthStencil(U32 ds_width, 
			U32 ds_height);
		virtual ~DepthStencil();
	};

	class ReadableDepthStencilPointer:public PoolAllocateObject{
	public:
		virtual ~ReadableDepthStencilPointer();
	};
	class ReadableDepthStencil:public DepthStencil, public Texture{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ReadableDepthStencilPointer*, readable_pointer);
	public:
		ReadableDepthStencil();
		ReadableDepthStencil(U32 width, 
			U32 height);
		virtual ~ReadableDepthStencil();
	};

	enum BlendOperation{
		NECROMANCER_BLEND_OP_ADD,
		NECROMANCER_BLEND_OP_SUBTRACT,
		NECROMANCER_BLEND_OP_REV_SUBTRACT,
		NECROMANCER_BLEND_OP_MAX,
		NECROMANCER_BLEND_OP_MIN
	};
	enum BlendState{
		NECROMANCER_BLEND_ZERO,
		NECROMANCER_BLEND_ONE,
		NECROMANCER_BLEND_SRC,
		NECROMANCER_BLEND_SRC_COLOR,
		NECROMANCER_BLEND_INV_SRC,
		NECROMANCER_BLEND_INV_SRC_COLOR,
		NECROMANCER_BLEND_DEST,
		NECROMANCER_BLEND_DEST_COLOR,
		NECROMANCER_BLEND_INV_DEST,
		NECROMANCER_BLEND_INV_DEST_COLOR
	};
	enum DepthComparison{
		NECROMANCER_DEPTH_COMPARISON_NEVER,
		NECROMANCER_DEPTH_COMPARISON_LESS,
		NECROMANCER_DEPTH_COMPARISON_EQUAL,
		NECROMANCER_DEPTH_COMPARISON_LESS_EQUAL,
		NECROMANCER_DEPTH_COMPARISON_GREATER,
		NECROMANCER_DEPTH_COMPARISON_NOT_EQUAL,
		NECROMANCER_DEPTH_COMPARISON_GREATER_EQUAL,
		NECROMANCER_DEPTH_COMPARISON_ALWAYS
	};
	enum FillMode{
		NECROMANCER_FILL_WIREFRAME,
		NECROMANCER_FILL_SOLID
	};

	enum CullMode{
		NECROMANCER_CULL_NONE,
		NECROMANCER_CULL_FRONT,
		NECROMANCER_CULL_BACK
	};
	enum TextureAddressMode{
		NECROMANCER_TEXTURE_ADDRESS_WRAP,
		NECROMANCER_TEXTURE_ADDRESS_MIRROR,
		NECROMANCER_TEXTURE_ADDRESS_CLAMP,
		NECROMANCER_TEXTURE_ADDRESS_BORDER
	};

	enum TextureSampleFilter{
		NECROMANCER_TEXTURE_FILTER_POINT,
		NECROMANCER_TEXTURE_FILTER_LINEAR,
		NECROMANCER_TEXTURE_FILTER_ANISOTROPIC
	};

	struct AlphaStateDesc{
		bool enable;
		BlendState src, dest, src_alpha, dest_alpha;
		BlendOperation blend_op, alpha_op;
	};

	struct DepthStencilStateDesc{
		bool depth_testing, depth_write;
		DepthComparison comparison;
		bool stencil_testing;
	};

	struct RasterizerStateDesc{
		FillMode fill_mode;
		CullMode cull_mode;
		bool multisample;
		bool antialiased_line;
	};

	struct SamplerStateDesc{
		TextureAddressMode texture_address_mode;
		TextureSampleFilter sampler_filter;
		U32 max_anisotropic;
	};

	class GraphicStatePointer:public PoolAllocateObject{
	public:
		virtual ~GraphicStatePointer();
	};
	class GraphicState:public RenderingResource{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(SamplerStateDesc, sampler_state_desc);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(RasterizerStateDesc, rasterizer_state_desc);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(DepthStencilStateDesc, depth_stencil_state_desc);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(AlphaStateDesc, alpha_state_desc);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(GraphicStatePointer, pointer);
	public:
		GraphicState();
		virtual ~GraphicState();
	};

	class GeometryShaderPointer :public PoolAllocateObject{
	public:
		virtual ~GeometryShaderPointer();
	};
	class GeometryShader :public RenderingResource{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(GeometryShaderPointer, pointer);
	public:
		GeometryShader();
		virtual ~GeometryShader();
	};


	class ComputeShaderPointer :public PoolAllocateObject{
	public:
		virtual ~ComputeShaderPointer();
	};
	class ComputeShader :public RenderingResource{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ComputeShaderPointer, pointer);
	public:
		ComputeShader();
		virtual ~ComputeShader();
	};

	class RWBufferPointer :public PoolAllocateObject{
	public:
		virtual ~RWBufferPointer();
	private:
	};
	class RWBuffer :public BufferObject{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, element_size);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, element_number);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(RWBufferPointer, rw_pointer);
	public:
		RWBuffer(U32 element_size,
			U32 element_number, BufferUsage buffer_usage);
		virtual ~RWBuffer();
	};

	class RWTexture2DPointer :public PoolAllocateObject{
	public:
		virtual ~RWTexture2DPointer();
	private:
	};
	class RWTexture2D :public Texture2D{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(RWTexture2DPointer, rw_pointer);
	public:
		RWTexture2D(TextureElementType element_type,
			U32 width, U32 height, U32 mip_map_level);
		virtual ~RWTexture2D();
	};

	typedef SmartPtr<ComputeShader>::Type CSPtr;
	typedef SmartPtr<RWTexture2D>::Type RWTex2DPtr;
	typedef SmartPtr<RWBuffer>::Type RWBufferPtr;
	typedef SmartPtr<GeometryShader>::Type GSPtr;
	typedef SmartPtr<RenderingResource>::Type RenderingResourcePtr;
	typedef SmartPtr<BufferObject>::Type BOPtr;
	typedef SmartPtr<VertexBuffer>::Type VBPtr;
	typedef SmartPtr<IndexBuffer>::Type IBPtr;
	typedef SmartPtr<ConstantBuffer>::Type CBPtr;
	typedef SmartPtr<VertexShader>::Type VSPtr;
	typedef SmartPtr<FragmentShader>::Type FSPtr;
	typedef SmartPtr<Texture>::Type TexPtr;
	typedef SmartPtr<Texture2D>::Type Tex2DPtr;
	typedef SmartPtr<TextureCube>::Type TexCubePtr;
	typedef SmartPtr<RenderTarget>::Type RTPtr;
	typedef SmartPtr<DepthStencil>::Type DSPtr;
	typedef SmartPtr<ReadableDepthStencil>::Type RDSPtr;
	typedef SmartPtr<RenderableTexture2D>::Type RTex2DPtr;
	typedef SmartPtr<RenderableTextureCube>::Type RTexCubePtr;
	typedef SmartPtr<GraphicState>::Type GraphicStatePtr;
	typedef SmartPtr<WindowRenderTarget>::Type WRTPtr;
}

