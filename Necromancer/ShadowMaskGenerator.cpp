#include "stdafx.h"
#include "ShadowMaskGenerator.h"

#include "Configuration.h"
#include "PIL.h"
#include "PILLLGI.h"
#include "DrawingRequest.h"
#include "ShadowMapManager.h"
#include "ShadowMaskDirCS.h"
#include "ShadowMaskSpotCS.h"
#include "ShadowMaskPointCS.h"
#include "GBuffer.h"

namespace Necromancer{
	namespace{
		struct MatrixCBData{
			Mat4 inverse_view_matrix;
			Mat4 projection_matrix;
		};
	}

	ShadowMaskGenerator::ShadowMaskGenerator(Configuration* config){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		m_width = config->get_integer_value("Screen", "width");
		m_height = config->get_integer_value("Screen", "height");
		m_shadow_mask_map = llgi->create_rw_texture(
			m_width, m_height, NECROMANCER_TEXTURE_R32_UINT);
		m_shadow_map_manager = new ShadowMapManager(config);
		m_shadow_mask_map = llgi->create_rw_texture(
			m_width, m_height, NECROMANCER_TEXTURE_R32_UINT);
		m_shadow_dir_mask_cs =
			llgi->create_compute_shader_from_header(ShadowMaskDirCS,
			sizeof(ShadowMaskDirCS), "ShadowMaskDirCS.hlsl");
		m_shadow_spot_mask_cs =
			llgi->create_compute_shader_from_header(ShadowMaskSpotCS,
			sizeof(ShadowMaskSpotCS), "ShadowMaskSpotCS.hlsl");
		m_shadow_point_mask_cs =
			llgi->create_compute_shader_from_header(ShadowMaskPointCS,
			sizeof(ShadowMaskPointCS), "ShadowMaskPointCS.hlsl");
		m_group_x = m_width / ks_tile_size;
		m_group_y = m_height / ks_tile_size;
		if (m_group_x * ks_tile_size != m_width) ++m_group_x;
		if (m_group_y * ks_tile_size != m_height) ++m_group_y;
		m_group_z = 1;
		m_util_cb = llgi->create_constant_buffer(sizeof(U32) * 4, nullptr);
		m_matrix_cb = llgi->create_constant_buffer(sizeof(Mat4) * 2, nullptr);
		m_light_position_cb = llgi->create_constant_buffer(sizeof(Vec4), nullptr);
	}

	ShadowMaskGenerator::~ShadowMaskGenerator(){
		delete m_shadow_map_manager;
	}

	void ShadowMaskGenerator::generate_shadow_mask(
		DrawingRequest* request, GBuffer* g_buffer)
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		const auto& lights = request->lights();
		U32 geometry_num = request->opaque_geometry_num();
		const DrawingRequest::GeometryArray& geometry_array =
			request->opaque_geometrys();
		U32 light_num = request->light_num();
		CameraPtr camera = request->camera();
		Mat4 view = camera->view();
		Mat4 inverse_view = inverse_matrix(view);
		Mat4 projection = camera->projection();
		MatrixCBData cb_data;
		cb_data.projection_matrix = projection;
		cb_data.inverse_view_matrix = inverse_view;
		llgi->update_constant_buffer_object(
			m_matrix_cb, &cb_data);
		U32 result_base = 1;
		for (U32 i = 0; i < light_num; ++i){
			m_shadow_map_manager->generate_shadow_map(
				lights[i], geometry_array, geometry_num);
			m_shadow_map_manager->enable_shadow_maps(lights[i]);
			Vec4 light_position = lights[i].position();
			llgi->update_constant_buffer_object(m_light_position_cb, &light_position);
			switch (lights[i].light_type()){
			case NM_LIGHT_POINT:
				llgi->enable_compute_shader(m_shadow_point_mask_cs);
				break;
			case NM_LIGHT_SPOT:
				llgi->enable_compute_shader(m_shadow_spot_mask_cs);
				break;
			case NM_LIGHT_DIRECTIONAL:
				llgi->enable_compute_shader(m_shadow_dir_mask_cs);
				break;
			}
			llgi->bind_rw_texture_to_write(ks_uav_slot, m_shadow_mask_map);
			U32 shadow_map_size = m_shadow_map_manager->shadow_map_size();
			U32 util_data [] = {
				m_width, m_height, result_base, i
			};
			llgi->update_constant_buffer_object(m_util_cb, util_data);
			g_buffer->bind_to_read();
			llgi->enable_constant_buffer(ks_util_cb_slot, m_util_cb, BIND_TO_CS);
			llgi->enable_constant_buffer(ks_matrix_cb_slot, m_matrix_cb, BIND_TO_CS);
			llgi->enable_constant_buffer(ks_light_position_cb_slot, 
				m_light_position_cb, BIND_TO_CS);
			llgi->dispatch(m_group_x, m_group_y, m_group_z);
			g_buffer->unbind_to_read();
			switch (lights[i].light_type()){
			case NM_LIGHT_POINT:
				llgi->disable_compute_shader(m_shadow_point_mask_cs);
				break;
			case NM_LIGHT_SPOT:
				llgi->disable_compute_shader(m_shadow_spot_mask_cs);
				break;
			case NM_LIGHT_DIRECTIONAL:
				llgi->disable_compute_shader(m_shadow_dir_mask_cs);
				break;
			}
			m_shadow_map_manager->disable_shadow_maps(lights[i]);
			result_base = result_base << 1;
		}

		llgi->bind_rw_texture_to_write(ks_uav_slot, nullptr);
	}

	void ShadowMaskGenerator::enable_shadow_mask(){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		llgi->enable_texture(ks_srv_slot, m_shadow_mask_map, BIND_TO_CS);
	}

	void ShadowMaskGenerator::disable_shadow_mask(){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		llgi->disable_texture(ks_srv_slot, m_shadow_mask_map, BIND_TO_CS);
	}
}