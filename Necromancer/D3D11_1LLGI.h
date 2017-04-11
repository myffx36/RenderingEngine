#pragma once

#include "PILLLGI.h"
#include "Configuration.h"
#include "Container.hpp"

namespace Necromancer{
	class D3D11_1LLGI:public PILLLGI{
	public:
		D3D11_1LLGI(Configuration*, HWND);

		virtual ~D3D11_1LLGI();

		virtual void enable_vertex_buffer(const VBPtr&);
		virtual void enable_index_buffer(const IBPtr&);
		virtual void enable_constant_buffer(U32 slot, const CBPtr&, 
			BindFlag flag = DEFAULT_BINDING);
		virtual void disable_vertex_buffer(const VBPtr&);
		virtual void disable_index_buffer(const IBPtr&);
		virtual void disable_constant_buffer(U32 slot, const CBPtr&, 
			BindFlag flag = DEFAULT_BINDING);
		virtual void release_vertex_buffer(const VBPtr&);
		virtual void release_index_buffer(const IBPtr&);
		virtual void release_constant_buffer(const CBPtr&);
		virtual VBPtr create_vertex_buffer(
			U32 size, U32 stride, 
			U32 offset, const String& format, 
			void *data, 
			BufferUsage buffer_usage = NM_BUFFER_USAGE_DEFAULT);
		virtual IBPtr create_index_buffer(
			U32 index_number, void * data,
			BufferUsage buffer_usage = NM_BUFFER_USAGE_DEFAULT);
		virtual CBPtr create_constant_buffer(
			U32 size, const void *data);

		virtual void update_constant_buffer_object(
			const CBPtr&, const void*);
		virtual void* map(const VBPtr&);
		virtual void* map(const IBPtr&);
		virtual void unmap(const VBPtr&);
		virtual void unmap(const IBPtr&);

		virtual void enable_vertex_shader(const VSPtr&);
		virtual void enable_fragment_shader(const FSPtr&);
		virtual void disable_vertex_shader(const VSPtr&);
		virtual void disable_fragment_shader(const FSPtr&);
		virtual void release_vertex_shader(const VSPtr&);
		virtual void release_fragment_shader(const FSPtr&);
		virtual VSPtr create_vertex_shader(
			const char* shader_text,
			const char* file_name);
		virtual FSPtr create_fragment_shader(
			const char* shader_text,
			const char* file_name);
		virtual VSPtr create_vertex_shader_from_binary(
			void* binary_code, U32 binary_code_size,
			const char* file_name);
		virtual FSPtr create_fragment_shader_from_binary(
			void* binary_code, U32 binary_code_size,
			const char* file_name);
		virtual VSPtr create_vertex_shader_from_header(
			const void* data, U32 size,
			const char* file_name);
		virtual FSPtr create_fragment_shader_from_header(
			const void* data, U32 size,
			const char* file_name);

		virtual void enable_geometry_shader(const GSPtr&);
		virtual void disable_geometry_shader(const GSPtr&);
		virtual void release_geometry_shader(const GSPtr&);
		virtual GSPtr create_geometry_shader(
			const char* shader_text,
			const char* file_name);
		virtual GSPtr create_geometry_shader_from_binary(
			void* binary_code, U32 binary_code_size,
			const char* file_name);
		virtual GSPtr create_geometry_shader_from_header(
			const void* data, U32 size,
			const char* file_name);

		virtual void enable_texture(U32 slot, const TexPtr&,
			BindFlag flag = BIND_TO_FS);
		virtual void disable_texture(U32 slot, const TexPtr&,
			BindFlag flag = BIND_TO_FS);
		virtual void release_texture_view(const TexPtr&);
		virtual void release_texture_2D(const Tex2DPtr&);
		virtual void release_texture_cube(const TexCubePtr&);

		virtual Tex2DPtr create_texture_2d(
			U32 width, U32 height, 
			TextureElementType element_type, void* data);
		virtual TexCubePtr create_texture_cube(
			U32 width, U32 height,
			TextureElementType element_type, void** data);
		virtual Tex2DPtr create_texture_2d_with_mip_map(
			U32 mip_map_level, U32 width[], U32 height[],
			TextureElementType elment_type, void** data);

		virtual void update_texture_2d(const Tex2DPtr&, void* data);
		virtual void texture_subimage(const Tex2DPtr&, 
			U32 xmin, U32 xmax, U32 ymin, U32 ymax, void* data);

		virtual void set_render_targets(U32 num, 
			RTPtr[], DSPtr);
		virtual void set_to_default_render_target();
		virtual void release_render_target(const RTPtr&);
		virtual void release_depth_stencil(const DSPtr&);
		virtual void release_window_render_target(const WRTPtr&);

		virtual DSPtr create_depth_stencil(
			U32 width, U32 height);
		virtual DSPtr create_depth_stencil_cube(
			U32 width, U32 height);
		virtual RDSPtr create_readable_depth_stencil(
			U32 width, U32 height);
		virtual RDSPtr create_readable_depth_stencil_cube(
			U32 width, U32 height);
		virtual RTex2DPtr create_renderable_texture_2d(
			U32 width, U32 height, 
			TextureElementType element_type);
		virtual RTexCubePtr create_renderable_texture_cube(
			U32 width, U32 height,
			TextureElementType element_type);

		virtual void enable_graphic_state(const GraphicStatePtr&);
		virtual void disable_graphic_state(const GraphicStatePtr&);
		virtual void release_graphic_state(const GraphicStatePtr&);
		virtual GraphicStatePtr create_graphic_state(SamplerStateDesc,
			RasterizerStateDesc, DepthStencilStateDesc, AlphaStateDesc);

		virtual void draw();
		virtual void swap_buffer(const WRTPtr&);
		virtual void clear_default_render_target(F32* color);
		virtual void clear_render_target(const RTPtr&, F32*);
		virtual void clear_depth_stencil(const DSPtr&, F32 depth);
		virtual void set_geometry_type(GeometryType);

		//Compute shaders
		virtual void enable_compute_shader(CSPtr);
		virtual void disable_compute_shader(CSPtr);
		virtual void release_compute_shader(CSPtr);
		virtual CSPtr create_compute_shader(
			const char* shader_text,
			const char* file_name);
		virtual CSPtr create_compute_shader_from_binary(
			void* binary_code, U32 binary_code_size,
			const char* file_name);
		virtual CSPtr create_compute_shader_from_header(
			const void* data, U32 size,
			const char* file_name);

		//read-write buffers and textures
		virtual void bind_rw_buffer_to_read(U32 slot, RWBufferPtr);
		virtual void bind_rw_buffer_to_write(U32 slot, RWBufferPtr);
		virtual void bind_rw_texture_to_write(U32 slot, RWTex2DPtr);
		virtual void release_rw_buffer(RWBufferPtr);
		virtual void release_rw_texture(RWTex2DPtr);
		virtual RWBufferPtr create_rw_buffer(
			U32 element_size, U32 element_number,
			BufferUsage buffer_usage = NM_BUFFER_USAGE_DYNAMIC);
		virtual RWTex2DPtr create_rw_texture(
			U32 width, U32 height,
			TextureElementType element_type);
		virtual void bind_rt_to_rw(U32 slot);
		virtual void unbind_rt_from_rw(U32 slot);
		virtual void* map(RWBufferPtr);
		virtual void unmap(RWBufferPtr);
		virtual void dispatch(U32 x, U32 y, U32 z);
	private:
		void set_viewport(U32 width, U32 height);
		void set_vertex_format(const String& format);
		ID3D11InputLayout* load_input_layout(const String& format);
		DXGI_FORMAT to_dxgi_format(TextureElementType);
		U32 get_byte_width_from_element_type(TextureElementType);
		D3D11_BLEND to_d3d11_blend(BlendState);
		D3D11_BLEND_OP to_d3d11_blend_op(BlendOperation);
		D3D11_USAGE to_d3d11_usage(BufferUsage);
		UINT to_d3d11_cpu_access_flag(BufferUsage);

		//获取相应资源的d3d11指针
		//若无法获取则会抛出异常
		ID3D11Buffer* get_d3d11_pointer(BOPtr);
		//获取相应资源的d3d11指针
		//若无法获取则会抛出异常
		ID3D11VertexShader* get_d3d11_pointer(VSPtr);
		//获取相应资源的d3d11指针
		//若无法获取则会抛出异常
		ID3D11PixelShader* get_d3d11_pointer(FSPtr);
		//获取相应资源的d3d11指针
		//若无法获取则会抛出异常
		ID3D11ShaderResourceView* get_d3d11_pointer(TexPtr);
		//获取相应资源的d3d11指针
		//若无法获取则会抛出异常
		ID3D11RenderTargetView* get_d3d11_pointer(RTPtr);
		//获取相应资源的d3d11指针
		//若无法获取则会抛出异常
		ID3D11DepthStencilView* get_d3d11_pointer(DSPtr);
		//获取相应资源的d3d11指针
		//若无法获取则会抛出异常
		ID3D11ComputeShader* get_d3d11_pointer(CSPtr);
		ID3D11Texture2D* get_d3d11_pointer(Tex2DPtr);
		ID3D11Texture2D* get_d3d11_pointer(TexCubePtr);
		ID3D11Texture2D* get_d3d11_texture(DSPtr);
		ID3D11GeometryShader* get_d3d11_pointer(GSPtr);

		ID3D11BlendState* get_d3d11_as(GraphicStatePtr);
		ID3D11DepthStencilState* get_d3d11_ds(GraphicStatePtr);
		ID3D11RasterizerState* get_d3d11_rs(GraphicStatePtr);
		ID3D11SamplerState* get_d3d11_ss(GraphicStatePtr);

		ID3D11ShaderResourceView* get_srv(RWBufferPtr);
		ID3D11UnorderedAccessView* get_uav(RWBufferPtr);
		ID3D11UnorderedAccessView* get_uav(RWTex2DPtr);

		ID3D11BlendState* create_as(AlphaStateDesc);
		ID3D11DepthStencilState* create_ds(DepthStencilStateDesc);
		ID3D11RasterizerState* create_rs(RasterizerStateDesc);
		ID3D11SamplerState* create_ss(SamplerStateDesc);

		String make_format_vs_text(const String& format);
		D3D11_INPUT_ELEMENT_DESC create_input_element_desc(char ch);
		void make_input_element_desc(const String& format, 
			D3D11_INPUT_ELEMENT_DESC*, U32& num);
	private:
		int m_width, m_height;
		D3D_FEATURE_LEVEL m_feature_level;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
		IDXGISwapChain* m_swap_chain;
		ID3D11RenderTargetView* m_default_render_target;
		ID3D11UnorderedAccessView* m_rt_uav;
		ID3D11SamplerState* m_default_sampler_state;
		U32 m_index_number;
		DSPtr m_default_depth_stencil;
		Map<String, ID3D11InputLayout*>::Type m_input_layout_map;
		char* m_vs_target, *m_fs_target, *m_gs_target, *m_cs_target;

	};
}

