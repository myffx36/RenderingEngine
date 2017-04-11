#include "stdafx.h"
#include "TileBasedDeferredShader.h"

#include "TBDS_CS_DATA.h"
#include "GBuffer.h"
#include "PIL.h"
#include "PILLLGI.h"

namespace Necromancer{

	namespace{
		struct CSRWLight{
			Vec4 position;
			Vec4 direction;
			Vec4 color;
			Vec4 params;
			Vec4 other_info;
			Vec4 pos_in_view;
		};
	}

	TileBasedDeferredShader::TileBasedDeferredShader(
		U32 width, U32 height)
		:m_width(width), m_height(height)
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();

		m_group_size_x = m_width / ks_tile_size;
		m_group_size_y = m_height / ks_tile_size;
		if (m_group_size_x * ks_tile_size != m_width)
			++m_group_size_x;
		if (m_group_size_y * ks_tile_size != m_height)
			++m_group_size_y;
		U32 frustum_num = m_group_size_x * m_group_size_y;

		m_light_number_buffer =
			llgi->create_constant_buffer(4 * sizeof(U32), nullptr);
		m_light_buffer =
			llgi->create_rw_buffer(sizeof(CSRWLight), ks_max_light_number);
		m_frustum_buffer =
			llgi->create_rw_buffer(sizeof(TileFrustum), frustum_num);
		m_rw_texture =
			llgi->create_rw_texture(
			m_width, m_height, NECROMANCER_TEXTURE_R32G32B32A32);
		m_per_frame_constants_buffer =
			llgi->create_constant_buffer(
			sizeof(PerFrameConstants), nullptr);
		m_compute_shader =
			llgi->create_compute_shader_from_header(
			TBDS_CS_DATA, sizeof(TBDS_CS_DATA), "TileBasedDeferredShader.hlsl");
	}

	TileBasedDeferredShader::~TileBasedDeferredShader(){

	}

	void TileBasedDeferredShader::set_light_buffer(
		const Light* lights, U32 light_number, CameraPtr camera)
	{
		const U32 shadowed_light_number = 2;
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		const U32 max_shadow_light_num = 8;

		CSRWLight* light_buffer_ptr = (CSRWLight*) llgi->map(m_light_buffer);
		auto view_matrix = camera->view();
		for (U32 i = 0; i < light_number; ++i){
			light_buffer_ptr[i].position = lights[i].position();
			light_buffer_ptr[i].color = lights[i].color();
			light_buffer_ptr[i].direction = lights[i].direction();
			light_buffer_ptr[i].params = lights[i].params();
			light_buffer_ptr[i].other_info = lights[i].other_info();
			light_buffer_ptr[i].pos_in_view = Vec4(
				lights[i].position().xyz, 1.0f) * view_matrix;
		}

		llgi->unmap(m_light_buffer);

		U32 tmp [] = {
			light_number, shadowed_light_number, 0, 0
		};
		llgi->update_constant_buffer_object(m_light_number_buffer, tmp);
	}

	void TileBasedDeferredShader::update_per_frame_constants(CameraPtr camera){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		auto view_matrix = camera->view();
		auto inverse_view_matrix = inverse_matrix(view_matrix);

		m_per_frame_constants.g_buffer_width = static_cast<U32>(m_width);
		m_per_frame_constants.g_buffer_height = static_cast<U32>(m_height);
		m_per_frame_constants.group_size_x = m_group_size_x;
		m_per_frame_constants.group_size_y = m_group_size_y;
		m_per_frame_constants.projection_matrix = camera->projection();
		m_per_frame_constants.camera_params.x = camera->znear();
		m_per_frame_constants.camera_params.y = camera->zfar();
		m_per_frame_constants.inverse_view_matrix = inverse_view_matrix;
		llgi->update_constant_buffer_object(m_per_frame_constants_buffer,
			&m_per_frame_constants);
	}

	void TileBasedDeferredShader::set_state(GBuffer* g_buffer,
		bool draw_to_frame_buffer, CameraPtr camera)
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		g_buffer->bind_to_read();

		llgi->enable_compute_shader(m_compute_shader);

		if (draw_to_frame_buffer)
			llgi->bind_rt_to_rw(ks_uav_slot);
		else
			llgi->bind_rw_texture_to_write(ks_uav_slot, m_rw_texture);

		llgi->enable_constant_buffer(
			ks_light_number_cb_slot, m_light_number_buffer,
			BIND_TO_CS);
		llgi->enable_constant_buffer(
			ks_per_frame_constants_cb_slot, m_per_frame_constants_buffer,
			BIND_TO_CS);
		llgi->bind_rw_buffer_to_read(ks_light_buffer_slot,
			m_light_buffer);
		llgi->bind_rw_buffer_to_read(ks_frustum_buffer_slot,
			m_frustum_buffer);
	}

	void TileBasedDeferredShader::deferred_shading(CameraPtr camera){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		llgi->dispatch(m_group_size_x, m_group_size_y, 1);
	}

	void TileBasedDeferredShader::restore_state(GBuffer* g_buffer,
		bool draw_to_frame_buffer, CameraPtr camera)
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		g_buffer->unbind_to_read();

		if (draw_to_frame_buffer)
			llgi->unbind_rt_from_rw(ks_uav_slot);
		else
			llgi->bind_rw_texture_to_write(ks_uav_slot, nullptr);

		llgi->disable_compute_shader(m_compute_shader);
		llgi->disable_constant_buffer(
			ks_light_number_cb_slot, m_light_number_buffer,
			BIND_TO_CS);
		llgi->disable_constant_buffer(
			ks_per_frame_constants_cb_slot, m_per_frame_constants_buffer,
			BIND_TO_CS);
		llgi->bind_rw_buffer_to_read(ks_light_buffer_slot, nullptr);
		llgi->bind_rw_buffer_to_read(ks_frustum_buffer_slot, nullptr);
	}

	void TileBasedDeferredShader::update_frustum(CameraPtr camera){
		static CameraPtr s_camera = nullptr;
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		if (s_camera != camera){
			s_camera = camera;

			TileFrustum* frustums = (TileFrustum*)
				llgi->map(m_frustum_buffer);

			Mat4 projection = camera->projection();
			float sx = (F32) m_group_size_x;
			float sy = (F32) m_group_size_y;
			Mat4 scale = scale_matrix(sx, sy, 1.0f);

			//make frustums
			for (U32 i = 0; i < m_group_size_x; ++i){
				for (U32 j = 0; j < m_group_size_y; ++j){
					TileFrustum frustum;
					float cx = (F32) (ks_tile_size * i + (ks_tile_size / 2));
					float cy = (F32) (ks_tile_size * j + (ks_tile_size / 2));
					cx = (2 * cx / m_width) - 1.0f;
					cy = 1.0f - (2 * cy / m_height);
					Mat4 tran = translate_matrix(-cx, -cy, 0.0f);
					Mat4 matrix = projection * tran * scale;

					Vec4 c1, c2, c3, c4;
					for (int k = 0; k < 4; ++k){
						c1[k] = matrix[k][0];
						c2[k] = matrix[k][1];
						c3[k] = matrix[k][2];
						c4[k] = matrix[k][3];
					}

					frustum.planes[0] = c4 + c1;
					frustum.planes[1] = c4 - c1;
					frustum.planes[2] = c4 + c2;
					frustum.planes[3] = c4 - c2;
					frustum.planes[0] =
						frustum.planes[0] / length(frustum.planes[0].xyz);
					frustum.planes[1] =
						frustum.planes[1] / length(frustum.planes[1].xyz);
					frustum.planes[2] =
						frustum.planes[2] / length(frustum.planes[2].xyz);
					frustum.planes[3] =
						frustum.planes[3] / length(frustum.planes[3].xyz);

					frustums[i * m_group_size_y + j] = frustum;
				}
			}

			llgi->unmap(m_frustum_buffer);
		}
	}

	void TileBasedDeferredShader::deferred_shading(
		const Light* lights, U32 light_number,
		GBuffer* g_buffer, bool draw_to_frame_buffer,
		CameraPtr camera)
	{
		set_light_buffer(lights, light_number, camera);
		update_per_frame_constants(camera);
		update_frustum(camera);
		set_state(g_buffer, draw_to_frame_buffer, camera);
		deferred_shading(camera);
		restore_state(g_buffer, draw_to_frame_buffer, camera);
	}
}