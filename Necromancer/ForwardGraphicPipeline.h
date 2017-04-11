#pragma once

#include "GraphicPipeline.h"
#include "Mesh.h"
#include "WVPMatrices.h"
#include "PILLLGI.h"
#include "Geometry.h"
#include "Camera.h"

namespace Necromancer{
	class Light;
	class RenderContext;
	class ShadowMapManager;
	class Configuration;
	class ForwardGraphicPipeline:public GraphicPipeline{
	public:
		ForwardGraphicPipeline(Configuration*, PILLLGI* llgi);

		virtual ~ForwardGraphicPipeline();

		virtual void process_drawing_request(DrawingRequest*);
	private:
		void draw_geometry(const GeometryPtr&, const CameraPtr& camera);
		void apply_render_context(RenderContext*);
		void set_lights(const Light*, U32 num);
	private:
		static const U32 ks_light_cb_slot = 1;
		static const U32 ks_light_cb_num_slot = 2;
		static const U32 ks_max_light_num = 32;
		CBPtr m_light_cb, m_light_number_cb;
		WVPMatrices m_matrices;
		PILLLGI* m_llgi;
		ShadowMapManager* m_shadow_map_manager;
	};
}

