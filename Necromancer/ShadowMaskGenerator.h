#pragma once

#include "RenderingResource.h"
#include "CommonUtility.h"

namespace Necromancer{
	class DrawingRequest;
	class Configuration;
	class ShadowMapManager;
	class GBuffer;
	class ShadowMaskGenerator{
		DEF_DATA_MEMBER_AND_GETTER(RWTex2DPtr, shadow_mask_map);
	public:
		ShadowMaskGenerator(Configuration*);
		virtual ~ShadowMaskGenerator();

		virtual void generate_shadow_mask(DrawingRequest*, GBuffer*);
		virtual void enable_shadow_mask();
		virtual void disable_shadow_mask();
	private:
		U32 m_group_x, m_group_y, m_group_z;
		ShadowMapManager* m_shadow_map_manager;
		CSPtr m_shadow_dir_mask_cs;
		CSPtr m_shadow_spot_mask_cs;
		CSPtr m_shadow_point_mask_cs;

		CBPtr m_util_cb;
		CBPtr m_matrix_cb;
		CBPtr m_light_position_cb;

		U32 m_width, m_height;

		static const U32 ks_uav_slot = 0;
		static const U32 ks_srv_slot = 6;
		static const U32 ks_tile_size = 16;

		static const U32 ks_util_cb_slot = 0;
		static const U32 ks_matrix_cb_slot = 1;
		static const U32 ks_shadow_map_matrix_cb_slot = 2;
		static const U32 ks_light_position_cb_slot = 3;
	};
}

