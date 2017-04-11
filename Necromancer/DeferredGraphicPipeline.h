#pragma once

#include "GraphicPipeline.h"
#include "WVPMatrices.h"
#include "Geometry.h"
#include "Camera.h"

namespace Necromancer{
	class ShadowMaskGenerator;
	class PILLLGI;
	class Configuration;
	class DeferredShader;
	class GBuffer;
	class DeferredGraphicPipeline :public GraphicPipeline{
	public:
		DeferredGraphicPipeline(Configuration*, PILLLGI* llgi);
		virtual ~DeferredGraphicPipeline();

		virtual void process_drawing_request(DrawingRequest*);
	private:
		void render_to_g_buffer(DrawingRequest*);
		void draw_geometry_to_g_buffer(
			const GeometryPtr&, const CameraPtr& camera);
		void apply_render_context(RenderContext*);
		void deferred_shading(DrawingRequest*);
		void generate_shadow_mask_map(DrawingRequest*);
		void draw_all_transparent_objects(DrawingRequest*);
	private:
		GBuffer* m_g_buffer;
		DeferredShader* m_deferred_shader;
		WVPMatrices m_matrices;
		ShadowMaskGenerator* m_shadow_mask_generator;
	};
}

