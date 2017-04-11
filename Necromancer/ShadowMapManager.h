#pragma once

#include "CommonUtility.h"
#include "RenderingResource.h"
#include "Light.h"
#include "DrawingRequest.h"

namespace Necromancer{
	class Configuration;
	class ShadowMapManager{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, shadow_map_size);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, shadow_map_num);
	public:
		ShadowMapManager(Configuration*);
		virtual ~ShadowMapManager();

		virtual void generate_shadow_map(const Light& light, 
			const DrawingRequest::GeometryArray& geometry_array,
			U32 geometry_num);
		virtual void enable_shadow_maps(const Light& light);
		virtual void disable_shadow_maps(const Light& light);
	private:
		virtual void generate_point_shadow_map(const Light& light,
			const DrawingRequest::GeometryArray& geometry_array,
			U32 geometry_num);
		virtual void generate_spot_shadow_map(const Light& light,
			const DrawingRequest::GeometryArray& geometry_array,
			U32 geometry_num);
		virtual void generate_directional_shadow_map(const Light& light,
			const DrawingRequest::GeometryArray& geometry_array,
			U32 geometry_num);
	private:
		Mat4 m_shadow_map_matrix;
		RTexCubePtr m_point_shadow_map;
		RTex2DPtr m_spot_directional_shadow_map;
		DSPtr m_point_ds;
		DSPtr m_spot_dir_ds;

		CBPtr m_shadow_map_matrix_cb;
		CBPtr m_light_matrix_cb;
		CBPtr m_light_pos_cb;
		static const U32 ks_shadow_map_matrix_cb_slot = 2;
		static const U32 ks_light_matrix_cb_slot = 0;
		static const U32 ks_light_pos_cb_slot = 1;
		static const U32 ks_shadow_map_slot = 0;

		VSPtr m_point_vs;
		VSPtr m_spot_vs;
		VSPtr m_directional_vs;
		GSPtr m_point_gs;
		FSPtr m_point_fs;
		FSPtr m_spot_fs;
		FSPtr m_directional_fs;
		GraphicStatePtr m_dir_graphic_state;
		GraphicStatePtr m_spot_graphic_state;
		GraphicStatePtr m_point_graphic_state;
	};
}

