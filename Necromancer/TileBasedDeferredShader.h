#pragma once

#include "DeferredShader.h"
#include "CommonUtility.h"
#include "RenderingResource.h"

namespace Necromancer{
	struct TileFrustum{
		Vec4 planes[4];
	};
	struct PerFrameConstants{
		U32 g_buffer_width;
		U32 g_buffer_height;
		U32 group_size_x;
		U32 group_size_y;
		Vec4 camera_params;
		Mat4 projection_matrix;
		Mat4 inverse_view_matrix;
	};
	class TileBasedDeferredShader :public DeferredShader{
	public:
		TileBasedDeferredShader(U32 width, U32 height);

		virtual ~TileBasedDeferredShader();

		virtual void deferred_shading(
			const Light* lights, U32 light_number,
			GBuffer* g_buffer, bool draw_to_frame_buffer,
			CameraPtr camera);
	private:
		void set_state(GBuffer*, bool, CameraPtr camera);
		void deferred_shading(CameraPtr camera);
		void restore_state(GBuffer*, bool, CameraPtr camera);
		void update_frustum(CameraPtr camera);

		void set_light_buffer(const Light* lights, U32 light_number, CameraPtr camera);
		void update_per_frame_constants(CameraPtr camera);
	private:
		const static U32 ks_uav_slot = 0;
		const static U32 ks_max_light_number = 1024;
		const static U32 ks_max_shadow_light_number = 8;
		const static U32 ks_light_number_cb_slot = 1;
		const static U32 ks_per_frame_constants_cb_slot = 0;
		const static U32 ks_light_buffer_slot = 0;
		const static U32 ks_frustum_buffer_slot = 1;

		RWTex2DPtr m_rw_texture;

		RWBufferPtr m_light_buffer;
		RWBufferPtr m_frustum_buffer;

		CSPtr m_compute_shader;
		CBPtr m_light_number_buffer, m_per_frame_constants_buffer;

		U32 m_width, m_height;

		PerFrameConstants m_per_frame_constants;

		U32 m_group_size_x, m_group_size_y;

		static const int ks_tile_size = 16;
	};
}

