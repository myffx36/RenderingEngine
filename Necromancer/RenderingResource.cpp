#include "stdafx.h"
#include "RenderingResource.h"

namespace Necromancer{
	//RenderingResource
	RenderingResource::RenderingResource(){
	}

	RenderingResource::~RenderingResource(){
	}

	//BufferObject
	BufferObject::BufferObject(){
	}

	BufferObjectPointer::~BufferObjectPointer(){
	}

	BufferObject::BufferObject(
		U32 size, BufferUsage buffer_usage)
		:m_size(size), m_buffer_usage(buffer_usage)
	{
		m_pointer = nullptr;
	}

	BufferObject::~BufferObject(){
		if(m_pointer != nullptr)
			delete m_pointer;
	}

	//VertexBuffer
	VertexBuffer::VertexBuffer(){
	}

	VertexBuffer::VertexBuffer(
		U32 size, 
		U32 stride, U32 offset, 
		const String& format,
		BufferUsage buffer_usage)
		:BufferObject(size, buffer_usage), 
		m_stride(stride), m_offset(offset), m_format(format)
	{
	}

	VertexBuffer::~VertexBuffer(){
	}

	//IndexBuffer
	IndexBuffer::IndexBuffer(){
	}

	IndexBuffer::IndexBuffer(
		U32 index_number, 
		BufferUsage buffer_usage)
		:BufferObject(sizeof(U32) * index_number, 
		buffer_usage), m_index_number(index_number)
	{
	}

	IndexBuffer::~IndexBuffer(){
	}

	//ConstantBuffer
	ConstantBuffer::ConstantBuffer(){
	}

	ConstantBuffer::ConstantBuffer(U32 size)
		:BufferObject(size, NM_BUFFER_USAGE_DEFAULT)
	{
	}

	ConstantBuffer::~ConstantBuffer(){
	}


	//VertexShader
	VertexShaderPointer::~VertexShaderPointer(){
	}

	VertexShader::VertexShader()
		
	{
		m_pointer = nullptr;
	}

	VertexShader::~VertexShader(){
		if(m_pointer) delete m_pointer;
	}

	//FragmentShader
	FragmentShaderPointer::~FragmentShaderPointer(){
	}

	FragmentShader::FragmentShader()
		
	{
		m_pointer = nullptr;
	}

	FragmentShader::~FragmentShader(){
		if(m_pointer) delete m_pointer;
	}

	//Texture
	TexturePointer::~TexturePointer(){
	}

	Texture::Texture(){
	}

	Texture::Texture(
		TextureElementType element_type)
		:m_element_type(element_type)
	{
		m_texture_pointer = nullptr;
	}

	Texture::~Texture(){
		if(m_texture_pointer) delete m_texture_pointer;
	}

	//Texture2D
	Texture2DPointer::~Texture2DPointer(){
	}

	Texture2D::Texture2D(){
	}

	Texture2D::Texture2D(
		TextureElementType element_type,
		U32 width, U32 height,
		U32 mip_map_level)
		:Texture(element_type),
		m_width(width), m_height(height),
		m_mip_map_level(mip_map_level)
	{
		m_texture_2d_pointer = nullptr;
	}

	Texture2D::~Texture2D(){
		if(m_texture_2d_pointer) delete m_texture_2d_pointer;
	}

	//TextureCubes
	TextureCubePointer::~TextureCubePointer(){
	}

	TextureCube::TextureCube(){
	}

	TextureCube::TextureCube(
		TextureElementType element_type,
		U32 width, U32 height)
		:Texture(element_type),
		m_width(width), m_height(height)
	{
		m_texture_cube_pointer = nullptr;
	}

	TextureCube::~TextureCube(){
		if(m_texture_cube_pointer) delete m_texture_cube_pointer;
	}

	//RenderTarget
	RenderTargetPointer::~RenderTargetPointer(){
	}

	RenderTarget::RenderTarget(){
	}

	RenderTarget::RenderTarget(
		U32 rt_width, U32 rt_height)
		:m_rt_width(rt_width), m_rt_height(rt_height)
	{
		m_render_target_pointer = nullptr;
	}

	RenderTarget::~RenderTarget(){
		if(m_render_target_pointer) delete m_render_target_pointer;
	}

	//RenderableTexture2D
	RenderableTexture2D::RenderableTexture2D(
		TextureElementType element_type,
		U32 width, U32 height)
		:Texture2D(element_type, width, height, 1),
		RenderTarget(width, height)
	{
	}

	RenderableTexture2D::RenderableTexture2D(){
	}

	RenderableTexture2D::~RenderableTexture2D(){
	}



	//RenderableTextureCube
	RenderableTextureCube::RenderableTextureCube(
		TextureElementType element_type,
		U32 width, U32 height)
		:TextureCube(element_type, width, height),
		RenderTarget(width, height),
		RenderingResource()
	{
	}

	RenderableTextureCube::RenderableTextureCube(){
	}

	RenderableTextureCube::~RenderableTextureCube(){
	}

	//WindowRenderTargetPointer
	WindowRenderTarget::WindowRenderTarget(U32 rt_width, U32 rt_height)
		:RenderTarget(rt_width, rt_height)
	{

	}

	WindowRenderTargetPointer::~WindowRenderTargetPointer(){
	}

	WindowRenderTarget::~WindowRenderTarget(){
		if(m_wrt_ptr) delete m_wrt_ptr;
	}



	//DepthStencil
	DepthStencilPointer::~DepthStencilPointer(){
	}

	DepthStencil::DepthStencil(
		U32 ds_width, U32 ds_height)
		:m_ds_width(ds_width), m_ds_height(ds_height)
	{
		m_depth_stencil_pointer = nullptr;
	}

	DepthStencil::DepthStencil(){
	}

	DepthStencil::~DepthStencil(){
		if(m_depth_stencil_pointer) delete m_depth_stencil_pointer;
	}

	//ReadableDepthStencil
	ReadableDepthStencilPointer::~ReadableDepthStencilPointer(){
	}

	ReadableDepthStencil::ReadableDepthStencil(
		U32 width, U32 height)
		:Texture(NECROMANCER_TEXTURE_R32),
		DepthStencil(width, height)
	{
		m_readable_pointer = nullptr;
	}

	ReadableDepthStencil::ReadableDepthStencil(){
	}

	ReadableDepthStencil::~ReadableDepthStencil(){
		if(m_readable_pointer) delete m_readable_pointer;
	}

	//GraphicState
	GraphicStatePointer::~GraphicStatePointer(){
	}

	GraphicState::GraphicState(){
		m_pointer = nullptr;
	}

	GraphicState::~GraphicState(){
		if(m_pointer) delete m_pointer;
	}

	GeometryShaderPointer::~GeometryShaderPointer(){
	}

	GeometryShader::GeometryShader(){
		m_pointer = nullptr;
	}

	GeometryShader::~GeometryShader(){
		if (m_pointer) delete m_pointer;
	}

	//ComputeShader
	ComputeShaderPointer::~ComputeShaderPointer(){
	}

	ComputeShader::ComputeShader(){
		m_pointer = nullptr;
	}

	ComputeShader::~ComputeShader(){
		if (m_pointer) delete m_pointer;
	}

	//RWResource
	RWBufferPointer::~RWBufferPointer(){
	}

	RWBuffer::RWBuffer(U32 element_size,
		U32 element_number, BufferUsage buffer_usage)
		:BufferObject(element_size * element_number, buffer_usage)
	{
		m_rw_pointer = nullptr;
	}

	RWBuffer::~RWBuffer(){
		if (m_rw_pointer) delete m_rw_pointer;
	}

	RWTexture2DPointer::~RWTexture2DPointer(){
	}

	RWTexture2D::RWTexture2D(
		TextureElementType element_type,
		U32 width, U32 height, U32 mip_map_level)
		:Texture2D(element_type, width, height, mip_map_level)
	{
		m_rw_pointer = nullptr;
	}

	RWTexture2D::~RWTexture2D(){
		if (m_rw_pointer) delete m_rw_pointer;
	}
}