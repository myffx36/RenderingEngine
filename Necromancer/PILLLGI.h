#pragma once

#include "RenderingResource.h"
#include "Container.hpp"

namespace Necromancer{

	const BindFlag DEFAULT_BINDING = (BindFlag)(BIND_TO_VS | BIND_TO_FS);
	class PILWindow;

	class PILLLGI{
	public:
		virtual ~PILLLGI();

		virtual void enable_vertex_buffer(const VBPtr&) = 0;
		virtual void enable_index_buffer(const IBPtr&) = 0;
		virtual void enable_constant_buffer(U32 slot, const CBPtr&, 
			BindFlag flag = DEFAULT_BINDING) = 0;
		virtual void disable_vertex_buffer(const VBPtr&) = 0;
		virtual void disable_index_buffer(const IBPtr&) = 0;
		virtual void disable_constant_buffer(U32 slot, const CBPtr&, 
			BindFlag flag = DEFAULT_BINDING) = 0;
		virtual void release_vertex_buffer(const VBPtr&) = 0;
		virtual void release_index_buffer(const IBPtr&) = 0;
		virtual void release_constant_buffer(const CBPtr&) = 0;
		virtual VBPtr create_vertex_buffer(
			U32 size, U32 stride, 
			U32 offset, const String& format, 
			void *data, 
			BufferUsage buffer_usage = NM_BUFFER_USAGE_DEFAULT) = 0;
		virtual IBPtr create_index_buffer(
			U32 index_number, void * data,
			BufferUsage buffer_usage = NM_BUFFER_USAGE_DEFAULT) = 0;
		virtual CBPtr create_constant_buffer(
			U32 size, const void *data) = 0;

		virtual void update_constant_buffer_object(
			const CBPtr&, const void*) = 0;
		virtual void* map(const VBPtr&) = 0;
		virtual void* map(const IBPtr&) = 0;
		virtual void unmap(const VBPtr&) = 0;
		virtual void unmap(const IBPtr&) = 0;

		virtual void enable_vertex_shader(const VSPtr&) = 0;
		virtual void enable_fragment_shader(const FSPtr&) = 0;
		virtual void disable_vertex_shader(const VSPtr&) = 0;
		virtual void disable_fragment_shader(const FSPtr&) = 0;
		virtual void release_vertex_shader(const VSPtr&) = 0;
		virtual void release_fragment_shader(const FSPtr&) = 0;
		virtual VSPtr create_vertex_shader(
			const char* shader_text,
			const char* file_name) = 0;
		virtual FSPtr create_fragment_shader(
			const char* shader_text,
			const char* file_name) = 0;
		virtual VSPtr create_vertex_shader_from_binary(
			void* binary_code, U32 binary_code_size,
			const char* file_name) = 0;
		virtual FSPtr create_fragment_shader_from_binary(
			void* binary_code, U32 binary_code_size,
			const char* file_name) = 0;
		virtual VSPtr create_vertex_shader_from_header(
			const void* data, U32 size,
			const char* file_name) = 0;
		virtual FSPtr create_fragment_shader_from_header(
			const void* data, U32 size,
			const char* file_name) = 0;
		virtual VSPtr create_vertex_shader_from_text_file(
			const char* file_name);
		virtual FSPtr create_fragment_shader_from_text_file(
			const char* file_name);
		VSPtr create_vertex_shader_from_binary_file(
			const char* file_name);
		FSPtr create_fragment_shader_from_binary_file(
			const char* file_name);

		virtual void enable_geometry_shader(const GSPtr&) = 0;
		virtual void disable_geometry_shader(const GSPtr&) = 0;
		virtual void release_geometry_shader(const GSPtr&) = 0;
		virtual GSPtr create_geometry_shader(
			const char* shader_text,
			const char* file_name) = 0;
		virtual GSPtr create_geometry_shader_from_binary(
			void* binary_code, U32 binary_code_size,
			const char* file_name) = 0;
		virtual GSPtr create_geometry_shader_from_header(
			const void* data, U32 size,
			const char* file_name) = 0;
		virtual GSPtr create_geometry_shader_from_text_file(
			const char* file_name);

		virtual void enable_texture(U32 slot, const TexPtr&,
			BindFlag flag = BIND_TO_FS) = 0;
		virtual void disable_texture(U32 slot, const TexPtr&,
			BindFlag flag = BIND_TO_FS) = 0;
		virtual void release_texture_view(const TexPtr&) = 0;
		virtual void release_texture_2D(const Tex2DPtr&) = 0;
		virtual void release_texture_cube(const TexCubePtr&) = 0;

		virtual Tex2DPtr create_texture_2d(
			U32 width, U32 height, 
			TextureElementType element_type, void* data) = 0;
		virtual TexCubePtr create_texture_cube(
			U32 width, U32 height,
			TextureElementType element_type, void** data) = 0;
		virtual Tex2DPtr create_texture_2d_with_mip_map(
			U32 mip_map_level, U32 width[], U32 height[],
			TextureElementType elment_type, void** data) = 0;

		virtual void update_texture_2d(const Tex2DPtr&, void* data) = 0;
		virtual void texture_subimage(const Tex2DPtr&, 
			U32 xmin, U32 xmax, U32 ymin, U32 ymax, void* data) = 0;

		virtual void set_render_targets(U32 num, 
			RTPtr[], DSPtr) = 0;
		virtual void set_to_default_render_target() = 0;
		virtual void release_render_target(const RTPtr&) = 0;
		virtual void release_depth_stencil(const DSPtr&) = 0;
		virtual void release_window_render_target(const WRTPtr&) = 0;

		virtual DSPtr create_depth_stencil(
			U32 width, U32 height) = 0;
		virtual DSPtr create_depth_stencil_cube(
			U32 width, U32 height) = 0;
		virtual RDSPtr create_readable_depth_stencil(
			U32 width, U32 height) = 0;
		virtual RDSPtr create_readable_depth_stencil_cube(
			U32 width, U32 height) = 0;
		virtual RTex2DPtr create_renderable_texture_2d(
			U32 width, U32 height, 
			TextureElementType element_type) = 0;
		virtual RTexCubePtr create_renderable_texture_cube(
			U32 width, U32 height,
			TextureElementType element_type) = 0;

		virtual void enable_graphic_state(const GraphicStatePtr&) = 0;
		virtual void disable_graphic_state(const GraphicStatePtr&) = 0;
		virtual void release_graphic_state(const GraphicStatePtr&) = 0;
		virtual GraphicStatePtr create_graphic_state(SamplerStateDesc,
			RasterizerStateDesc, DepthStencilStateDesc, AlphaStateDesc) = 0;

		virtual void draw() = 0;
		virtual void swap_buffer(const WRTPtr& = WRTPtr((WindowRenderTarget*)nullptr)) = 0;
		virtual void clear_default_render_target(F32* color) = 0;
		virtual void clear_render_target(const RTPtr&, F32*) = 0;
		virtual void clear_depth_stencil(const DSPtr&, F32 depth = 1.0f) = 0;
		virtual void set_geometry_type(GeometryType) = 0;

		//Compute shaders
		virtual void enable_compute_shader(CSPtr) = 0;
		virtual void disable_compute_shader(CSPtr) = 0;
		virtual void release_compute_shader(CSPtr) = 0;
		virtual CSPtr create_compute_shader(
			const char* shader_text,
			const char* file_name) = 0;
		virtual CSPtr create_compute_shader_from_binary(
			void* binary_code, U32 binary_code_size,
			const char* file_name) = 0;
		virtual CSPtr create_compute_shader_from_header(
			const void* data, U32 size,
			const char* file_name) = 0;
		virtual CSPtr create_compute_shader_from_text_file(
			const char* file_name);

		//read-write buffers and textures
		virtual void bind_rw_buffer_to_read(U32 slot, RWBufferPtr) = 0;
		virtual void bind_rw_buffer_to_write(U32 slot, RWBufferPtr) = 0;
		virtual void bind_rw_texture_to_write(U32 slot, RWTex2DPtr) = 0;
		virtual void release_rw_buffer(RWBufferPtr) = 0;
		virtual void release_rw_texture(RWTex2DPtr) = 0;
		virtual RWBufferPtr create_rw_buffer(
			U32 element_size, U32 element_number,
			BufferUsage buffer_usage = NM_BUFFER_USAGE_DYNAMIC) = 0;
		virtual RWTex2DPtr create_rw_texture(
			U32 width, U32 height,
			TextureElementType element_type) = 0;
		virtual void bind_rt_to_rw(U32 slot) = 0;
		virtual void unbind_rt_from_rw(U32 slot) = 0;
		virtual void* map(RWBufferPtr) = 0;
		virtual void unmap(RWBufferPtr) = 0;
		virtual void dispatch(U32 x, U32 y, U32 z) = 0;
	private:
	};
}

