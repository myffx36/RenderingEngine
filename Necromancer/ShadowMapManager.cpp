#include "stdafx.h"
#include "ShadowMapManager.h"

typedef unsigned char BYTE;

#include "Configuration.h"
#include "PIL.h"
#include "PILLLGI.h"
#include "Geometry.h"
#include "Mesh.h"

#include "sm_point_vs.h"
#include "sm_point_gs.h"
#include "sm_point_fs.h"
#include "sm_spot_vs.h"
#include "sm_spot_fs.h"
#include "sm_dir_vs.h"
#include "sm_dir_fs.h"

namespace Necromancer{
	namespace{
		struct ShadowMapLightMatrix{
			Mat4 world;
			Mat4 view;
			Mat4 projection;
		};

		GraphicStatePtr create_dir_graphic_state(){
			SamplerStateDesc sampler_state_desc;
			sampler_state_desc.max_anisotropic = 0;
			sampler_state_desc.sampler_filter =
				NECROMANCER_TEXTURE_FILTER_LINEAR;
			sampler_state_desc.texture_address_mode =
				NECROMANCER_TEXTURE_ADDRESS_BORDER;
			RasterizerStateDesc rasterizer_state_desc;
			rasterizer_state_desc.antialiased_line = true;
			rasterizer_state_desc.cull_mode =
				NECROMANCER_CULL_NONE;
			rasterizer_state_desc.fill_mode =
				NECROMANCER_FILL_SOLID;
			rasterizer_state_desc.multisample = false;
			DepthStencilStateDesc depth_stencil_state_desc;
			depth_stencil_state_desc.depth_write = true;
			depth_stencil_state_desc.comparison =
				NECROMANCER_DEPTH_COMPARISON_GREATER_EQUAL;
			depth_stencil_state_desc.depth_testing = true;
			depth_stencil_state_desc.stencil_testing = false;
			AlphaStateDesc alpha_state_desc;
			alpha_state_desc.enable = false;

			PIL* pil = PIL::get_instance();
			PILLLGI* llgi = pil->pil_llgi();

			GraphicStatePtr graphic_state = llgi->create_graphic_state(
				sampler_state_desc, rasterizer_state_desc,
				depth_stencil_state_desc, alpha_state_desc);

			return graphic_state;
		}

		GraphicStatePtr create_spot_graphic_state(){
			SamplerStateDesc sampler_state_desc;
			sampler_state_desc.max_anisotropic = 0;
			sampler_state_desc.sampler_filter =
				NECROMANCER_TEXTURE_FILTER_LINEAR;
			sampler_state_desc.texture_address_mode =
				NECROMANCER_TEXTURE_ADDRESS_BORDER;
			RasterizerStateDesc rasterizer_state_desc;
			rasterizer_state_desc.antialiased_line = true;
			rasterizer_state_desc.cull_mode =
				NECROMANCER_CULL_NONE;
			rasterizer_state_desc.fill_mode =
				NECROMANCER_FILL_SOLID;
			rasterizer_state_desc.multisample = false;
			DepthStencilStateDesc depth_stencil_state_desc;
			depth_stencil_state_desc.depth_write = true;
			depth_stencil_state_desc.comparison =
				NECROMANCER_DEPTH_COMPARISON_LESS_EQUAL;
			depth_stencil_state_desc.depth_testing = true;
			depth_stencil_state_desc.stencil_testing = false;
			AlphaStateDesc alpha_state_desc;
			alpha_state_desc.enable = false;

			PIL* pil = PIL::get_instance();
			PILLLGI* llgi = pil->pil_llgi();

			GraphicStatePtr graphic_state = llgi->create_graphic_state(
				sampler_state_desc, rasterizer_state_desc,
				depth_stencil_state_desc, alpha_state_desc);

			return graphic_state;
		}

		GraphicStatePtr create_point_graphic_state(){
			SamplerStateDesc sampler_state_desc;
			sampler_state_desc.max_anisotropic = 0;
			sampler_state_desc.sampler_filter =
				NECROMANCER_TEXTURE_FILTER_LINEAR;
			sampler_state_desc.texture_address_mode =
				NECROMANCER_TEXTURE_ADDRESS_BORDER;
			RasterizerStateDesc rasterizer_state_desc;
			rasterizer_state_desc.antialiased_line = true;
			rasterizer_state_desc.cull_mode =
				NECROMANCER_CULL_NONE;
			rasterizer_state_desc.fill_mode =
				NECROMANCER_FILL_SOLID;
			rasterizer_state_desc.multisample = false;
			DepthStencilStateDesc depth_stencil_state_desc;
			depth_stencil_state_desc.depth_write = true;
			depth_stencil_state_desc.comparison =
				NECROMANCER_DEPTH_COMPARISON_LESS_EQUAL;
			depth_stencil_state_desc.depth_testing = true;
			depth_stencil_state_desc.stencil_testing = false;
			AlphaStateDesc alpha_state_desc;
			alpha_state_desc.enable = false;

			PIL* pil = PIL::get_instance();
			PILLLGI* llgi = pil->pil_llgi();

			GraphicStatePtr graphic_state = llgi->create_graphic_state(
				sampler_state_desc, rasterizer_state_desc,
				depth_stencil_state_desc, alpha_state_desc);

			return graphic_state;
		}
	}

	ShadowMapManager::ShadowMapManager(Configuration* config){
		m_shadow_map_size = config->get_integer_value("Shadow", "size");
		m_shadow_map_num = config->get_integer_value("Shadow", "num");

		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		m_point_shadow_map = llgi->create_renderable_texture_cube(
			m_shadow_map_size, m_shadow_map_size, NECROMANCER_TEXTURE_R32);
		m_point_ds = llgi->create_depth_stencil_cube(
			m_shadow_map_size, m_shadow_map_size);

		m_spot_directional_shadow_map = llgi->create_renderable_texture_2d(
			m_shadow_map_size, m_shadow_map_size, NECROMANCER_TEXTURE_R32);
		m_spot_dir_ds = llgi->create_depth_stencil(
			m_shadow_map_size, m_shadow_map_size);

		m_shadow_map_matrix_cb = llgi->create_constant_buffer(
			sizeof(m_shadow_map_matrix), nullptr);
		m_light_matrix_cb = llgi->create_constant_buffer(
			sizeof(ShadowMapLightMatrix), nullptr);
		m_light_pos_cb = llgi->create_constant_buffer(
			sizeof(Vec4), nullptr);

		m_point_vs = llgi->create_vertex_shader_from_header(
			sm_point_vs, sizeof(sm_point_vs), "sm_point_vs.hlsl");
		m_point_gs = llgi->create_geometry_shader_from_header(
			sm_point_gs, sizeof(sm_point_gs), "sm_point_gs.hlsl");
		m_point_fs = llgi->create_fragment_shader_from_header(
			sm_point_fs, sizeof(sm_point_fs), "sm_point_fs.hlsl");
		m_spot_vs = llgi->create_vertex_shader_from_header(
			sm_spot_vs, sizeof(sm_spot_vs), "sm_spot_vs.hlsl");
		m_spot_fs = llgi->create_fragment_shader_from_header(
			sm_spot_fs, sizeof(sm_spot_fs), "sm_spot_fs.hlsl");
		m_directional_vs = llgi->create_vertex_shader_from_header(
			sm_dir_vs, sizeof(sm_dir_vs), "sm_dir_vs.hlsl");
		m_directional_fs = llgi->create_fragment_shader_from_header(
			sm_dir_fs, sizeof(sm_dir_fs), "sm_dir_fs.hlsl");

		m_dir_graphic_state = create_dir_graphic_state();
		m_spot_graphic_state = create_spot_graphic_state();
		m_point_graphic_state = create_point_graphic_state();
	}

	ShadowMapManager::~ShadowMapManager(){
	}

	void ShadowMapManager::generate_shadow_map(const Light& light,
		const DrawingRequest::GeometryArray& geometry_array,
		U32 geometry_num)
	{
		switch (light.light_type()){
		case NM_LIGHT_POINT:
			generate_point_shadow_map(light, geometry_array, geometry_num);
			break;
		case NM_LIGHT_DIRECTIONAL:
			generate_directional_shadow_map(light, geometry_array, geometry_num);
			break;
		case NM_LIGHT_SPOT:
			generate_spot_shadow_map(light, geometry_array, geometry_num);
			break;
		}
	}

	void ShadowMapManager::generate_point_shadow_map(const Light& light,
		const DrawingRequest::GeometryArray& geometry_array, U32 geometry_num)
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();

		llgi->enable_vertex_shader(m_point_vs);
		llgi->enable_geometry_shader(m_point_gs);
		llgi->enable_fragment_shader(m_point_fs);
		llgi->enable_constant_buffer(
			ks_light_matrix_cb_slot, m_light_matrix_cb,
			(BindFlag) (BIND_TO_GS | BIND_TO_VS));
		llgi->enable_constant_buffer(
			ks_light_pos_cb_slot, m_light_pos_cb);

		Vec3 light_postion = light.position().xyz;
		Vec3 light_direction = light.direction().xyz;
		Vec4 vec4_light_position = Vec4(light_postion, 1.0f);

		llgi->update_constant_buffer_object(
			m_light_pos_cb, &vec4_light_position);
		llgi->enable_graphic_state(m_point_graphic_state);

		ShadowMapLightMatrix light_matrix;
		light_matrix.projection = perspective_projection_matrix(
			1.0f, 53.0f, 1.0f, 1000.0f);
		light_matrix.view = translate_matrix(-1.0f * light_postion);
		light_matrix.world = Mat4();

		Mat4 shadow_map_matrix = light_matrix.view * light_matrix.projection;
		m_shadow_map_matrix = shadow_map_matrix;


		RTPtr rts [] = { m_point_shadow_map };
		llgi->set_render_targets(1, rts, m_point_ds);

		float white [] = { 0.0f, 0.0f, 0.0f, 1.0f };

		llgi->clear_render_target(m_point_shadow_map, white);
		llgi->clear_depth_stencil(m_point_ds, 1.0f);

		for (U32 i = 0; i < geometry_num; ++i){
			const GeometryPtr& geometry = geometry_array[i];
			MeshPtr mesh = geometry->mesh();
			if (mesh->geometry_type() != NECROMANCER_GEOMETRY_MESH) continue;
			VBPtr vb = mesh->vertex_buffer();
			IBPtr ib = mesh->index_buffer();
			llgi->enable_vertex_buffer(vb);
			llgi->enable_index_buffer(ib);
			llgi->set_geometry_type(mesh->geometry_type());

			light_matrix.world = geometry->world();
			llgi->update_constant_buffer_object(
				m_light_matrix_cb, &light_matrix);
			llgi->draw();
		}

		llgi->set_to_default_render_target();
		llgi->disable_geometry_shader(m_point_gs);
		llgi->disable_constant_buffer(
			ks_light_matrix_cb_slot, m_light_matrix_cb,
			(BindFlag) (BIND_TO_GS | BIND_TO_VS));
		llgi->disable_graphic_state(m_point_graphic_state);
	}

	void ShadowMapManager::generate_spot_shadow_map(const Light& light,
		const DrawingRequest::GeometryArray& geometry_array, U32 geometry_num)
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();

		llgi->enable_vertex_shader(m_spot_vs);
		llgi->enable_fragment_shader(m_spot_fs);
		llgi->enable_constant_buffer(
			ks_light_matrix_cb_slot, m_light_matrix_cb);
		llgi->enable_constant_buffer(
			ks_light_pos_cb_slot, m_light_pos_cb);

		Vec3 light_postion = light.position().xyz;
		Vec3 light_direction = light.direction().xyz;
		Vec4 vec4_light_position = Vec4(light_postion, 1.0f);
		Vec4 default_direction(0.0f, 0.0f, -1.0f, 0.0f);

		llgi->update_constant_buffer_object(
			m_light_pos_cb, &vec4_light_position);
		llgi->enable_graphic_state(m_spot_graphic_state);

		ShadowMapLightMatrix light_matrix;
		light_matrix.projection = perspective_projection_matrix(
			1.0f, light.theta(), 1.0f, light.params()[3]);
		light_matrix.view = translate_matrix(-1.0f * light_postion) *
			rotate_matrix_vec_to_vec(
			normalize(light_direction), default_direction.xyz);
		light_matrix.world = Mat4();

		Mat4 shadow_map_matrix = light_matrix.view * light_matrix.projection;
		m_shadow_map_matrix = shadow_map_matrix;

		RTPtr rts [] = { m_spot_directional_shadow_map };
		llgi->set_render_targets(1, rts, m_spot_dir_ds);

		float white [] = { 1.0f, 1.0f, 1.0f, 1.0f };

		llgi->clear_render_target(m_spot_directional_shadow_map, white);
		llgi->clear_depth_stencil(m_spot_dir_ds, 1.0f);

		for (U32 i = 0; i < geometry_num; ++i){
			const GeometryPtr& geometry = geometry_array[i];
			MeshPtr mesh = geometry->mesh();
			if (mesh->geometry_type() != NECROMANCER_GEOMETRY_MESH) continue;
			VBPtr vb = mesh->vertex_buffer();
			IBPtr ib = mesh->index_buffer();
			llgi->enable_vertex_buffer(vb);
			llgi->enable_index_buffer(ib);
			llgi->set_geometry_type(mesh->geometry_type());

			light_matrix.world = geometry->world();
			llgi->update_constant_buffer_object(
				m_light_matrix_cb, &light_matrix);
			llgi->draw();
		}

		llgi->set_to_default_render_target();
		llgi->disable_constant_buffer(
			ks_light_matrix_cb_slot, m_light_matrix_cb);
		llgi->disable_graphic_state(m_spot_graphic_state);
	}

	void ShadowMapManager::generate_directional_shadow_map(const Light& light,
		const DrawingRequest::GeometryArray& geometry_array, U32 geometry_num)
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();

		llgi->enable_vertex_shader(m_directional_vs);
		llgi->enable_fragment_shader(m_directional_fs);
		llgi->enable_constant_buffer(
			ks_light_matrix_cb_slot, m_light_matrix_cb);
		llgi->enable_constant_buffer(
			ks_light_pos_cb_slot, m_light_pos_cb);

		Vec3 light_postion = light.position().xyz;
		Vec3 light_direction = light.direction().xyz;
		Vec4 vec4_light_position = Vec4(light_postion, 1.0f);
		Vec4 default_direction(0.0f, 0.0f, -1.0f, 0.0f);

		llgi->update_constant_buffer_object(
			m_light_pos_cb, &vec4_light_position);
		llgi->enable_graphic_state(m_dir_graphic_state);

		ShadowMapLightMatrix light_matrix;
		F32 max_num = 300.0f;
		light_matrix.projection = orthographic_projection_matrix(
			-max_num, max_num, -max_num, max_num, -max_num, max_num);
		light_matrix.view = rotate_matrix_vec_to_vec(
			normalize(light_direction), default_direction.xyz);
		light_matrix.world = Mat4();

		Mat4 shadow_map_matrix = light_matrix.view * light_matrix.projection;
		m_shadow_map_matrix = shadow_map_matrix;

		RTPtr rts [] = { m_spot_directional_shadow_map };
		llgi->set_render_targets(1, rts, m_spot_dir_ds);

		float black [] = { 0.0f, 0.0f, 0.0f, 1.0f };

		llgi->clear_render_target(m_spot_directional_shadow_map, black);
		llgi->clear_depth_stencil(m_spot_dir_ds, 0.0f);

		for (U32 i = 0; i < geometry_num; ++i){
			const GeometryPtr& geometry = geometry_array[i];
			MeshPtr mesh = geometry->mesh();
			if (mesh->geometry_type() != NECROMANCER_GEOMETRY_MESH) continue;
			VBPtr vb = mesh->vertex_buffer();
			IBPtr ib = mesh->index_buffer();
			llgi->enable_vertex_buffer(vb);
			llgi->enable_index_buffer(ib);
			llgi->set_geometry_type(mesh->geometry_type());

			light_matrix.world = geometry->world();
			llgi->update_constant_buffer_object(
				m_light_matrix_cb, &light_matrix);
			llgi->draw();
		}

		llgi->set_to_default_render_target();
		llgi->disable_constant_buffer(
			ks_light_matrix_cb_slot, m_light_matrix_cb);
		llgi->disable_graphic_state(m_dir_graphic_state);
	}

	void ShadowMapManager::enable_shadow_maps(const Light& light){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();

		llgi->update_constant_buffer_object(
			m_shadow_map_matrix_cb, &m_shadow_map_matrix);
		llgi->enable_constant_buffer(ks_shadow_map_matrix_cb_slot,
			m_shadow_map_matrix_cb,
			(BindFlag) (BIND_TO_CS | BIND_TO_FS));

		switch (light.light_type()){
		case NM_LIGHT_POINT:
			llgi->enable_texture(ks_shadow_map_slot, m_point_shadow_map,
				(BindFlag) (BIND_TO_CS | BIND_TO_FS));
			break;
		case NM_LIGHT_DIRECTIONAL:
			llgi->enable_texture(ks_shadow_map_slot, m_spot_directional_shadow_map,
				(BindFlag) (BIND_TO_CS | BIND_TO_FS));
			break;
		case NM_LIGHT_SPOT:
			llgi->enable_texture(ks_shadow_map_slot, m_spot_directional_shadow_map,
				(BindFlag) (BIND_TO_CS | BIND_TO_FS));
			break;
		}
	}

	void ShadowMapManager::disable_shadow_maps(const Light& light){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();

		llgi->disable_constant_buffer(ks_shadow_map_matrix_cb_slot,
			m_shadow_map_matrix_cb,
			(BindFlag) (BIND_TO_CS | BIND_TO_FS));

		switch (light.light_type()){
		case NM_LIGHT_POINT:
			llgi->disable_texture(ks_shadow_map_slot, m_point_shadow_map,
				(BindFlag) (BIND_TO_CS | BIND_TO_FS));
			break;
		case NM_LIGHT_DIRECTIONAL:
			llgi->disable_texture(ks_shadow_map_slot, m_spot_directional_shadow_map,
				(BindFlag) (BIND_TO_CS | BIND_TO_FS));
			break;
		case NM_LIGHT_SPOT:
			llgi->disable_texture(ks_shadow_map_slot, m_spot_directional_shadow_map,
				(BindFlag) (BIND_TO_CS | BIND_TO_FS));
			break;
		}
	}
}